/**
 * build.js — Guide4PC build script
 * Incremental by default: only rebuilds changed files (detected by mtime).
 * Use --force for a full rebuild.
 *
 * Usage: node js/build.js [--force]
 * (called from ./update.sh)
 */

const fs = require('fs');
const path = require('path');

const ROOT = path.resolve(__dirname, '..');
const SRC = path.join(ROOT, 'sourcecode');
const DIST = path.join(ROOT, 'docs');
const CACHE_FILE = path.join(ROOT, '.build-cache.json');
const BUILD_VERSION = Date.now().toString(36);

// ─── Stop-word lists for search-index tokenization ──────────────────────────

// English function words — filtered out to keep the index lean
const EN_STOP_WORDS = new Set([
  'the','a','an','is','are','was','were','be','been','being',
  'have','has','had','do','does','did','will','would','shall',
  'should','may','might','must','can','could',
  'i','you','he','she','it','we','they',
  'me','him','her','us','them',
  'my','your','his','its','our','their',
  'this','that','these','those',
  'in','on','at','to','for','of','with','by','from','up',
  'about','into','through','during','before','after','above','below',
  'between','and','but','or','nor','not','so','yet',
  'both','either','neither','each','every','all','any',
  'few','more','most','other','some','such','no','than',
  'too','very','just','also','if','then','else',
  'when','where','why','how','which','who','whom','what',
  'here','there','now','only','own','same','so',
  'because','as','until','over','while','again',
  'into','onto','upon','within','without',
  'one','two','three','first','second','third',
  'am','been','being','had','having','does','did','doing',
]);

// Chinese function characters — filtered from single-char terms (bigrams
// that contain a stop char are kept when the other char is contentful)
const ZH_STOP_CHARS = new Set([
  '的','了','在','是','我','有','和','就','不','人',
  '都','一','上','也','很','到','说','要','去','你',
  '会','着','没','看','好','自','己','这','他','她',
  '它','们','那','些','所','为','以','因','但','而',
  '如','果','虽','然','可','个','什','么','怎','哪',
  '吗','啊','吧','呢','哦','嗯','与','之','其','中',
  '等','被','把','让','将','从','对','向','往','当',
  '时','来','去','能','做','得','过','已','还','又',
  '再','才','刚','便','即','只','非','常','更','最',
  '较','多','少','大','小','新','旧','前','后','里',
  '外','下','内','年','月','日','点','种','次','用',
  '或','且','及','于','则','呵','哈','呀','哇','啦',
  '嘛','呗','咯','嘿','噢','哎','唉','喂','嗯','吧',
]);

// ─── Helpers ────────────────────────────────────────────────────────────────

/** Recursively list all .md files under dir, returning relative paths (forward-slash). */
function walkDir(dir, base) {
  const results = [];
  if (!fs.existsSync(dir)) return results;
  const entries = fs.readdirSync(dir, { withFileTypes: true });
  for (const ent of entries) {
    const full = path.join(dir, ent.name);
    const rel = path.relative(base, full).replace(/\\/g, '/');
    if (ent.isDirectory()) {
      results.push(...walkDir(full, base));
    } else if (ent.isFile() && ent.name.endsWith('.md')) {
      results.push(rel);
    }
  }
  return results;
}

/** Extract display title from filename: strip prefix_ and .md extension. */
function extractTitle(filename) {
  const name = filename.replace(/\.md$/, '');
  const underscoreIdx = name.indexOf('_');
  if (underscoreIdx !== -1) {
    return name.slice(underscoreIdx + 1);
  }
  return name;
}

/**
 * Normalize a directory path for cross-language pairing.
 * Chinese Chapter2 dir is "main/Chapter2_Cpp_PDE", English is "main/Chapter2".
 * We normalize both to "main/Chapter2" so pairs match across languages.
 */
function normalizeDir(dirPath) {
  return dirPath.replace(/Chapter2_Cpp_PDE/, 'Chapter2');
}

/**
 * Extract pairing key from filename.
 * - Files starting with "0_" → key "home"
 * - [dlX] prefix is stripped before extracting digits (it's metadata)
 * - The key is namespaced by directory to prevent collisions:
 *   - Top-level "附录N" or "AppendixN" → "appendix/N"
 *   - Top-level other files → "root/N"
 *   - Subdirectory files → "<normalized-dir>/N"
 * - If no digits found, use the title as key
 */
function extractKey(filename, relPath) {
  let name = path.basename(filename);

  // Strip [dlX] metadata prefix (e.g. "[dl1]Section0_代码" → "Section0_代码")
  name = name.replace(/^\[dl\d+\]/, '');

  if (/^0_/.test(name)) return 'home';

  const match = name.match(/(\d+)/);
  const numKey = match ? match[1] : extractTitle(name);

  const dir = path.dirname(relPath);

  if (dir === '.' || dir === '') {
    // Top-level files: namespace by content type
    if (/^(附录|Appendix)\d+/.test(name)) {
      return 'appendix/' + numKey;
    }
    return 'root/' + numKey;
  }

  // Subdirectory files: namespace by normalized directory path
  const ns = normalizeDir(dir);
  return ns + '/' + numKey;
}

/** Strip Markdown formatting to get plain text for search index. */
function stripMarkdown(md) {
  let text = md;
  text = text.replace(/^---[\s\S]*?---\n?/, '');
  text = text.replace(/<!--[\s\S]*?-->/g, '');
  text = text.replace(/```[\s\S]*?```/g, ' ');
  text = text.replace(/`[^`]+`/g, ' ');
  text = text.replace(/!\[[^\]]*\]\([^)]+\)/g, ' ');
  text = text.replace(/\[([^\]]*)\]\([^)]+\)/g, '$1');
  text = text.replace(/^#{1,6}\s+/gm, '');
  text = text.replace(/^>\s?/gm, '');
  text = text.replace(/^[-*_]{3,}\s*$/gm, ' ');
  text = text.replace(/^[\s]*[-*+]\s+/gm, ' ');
  text = text.replace(/^[\s]*\d+\.\s+/gm, ' ');
  text = text.replace(/\*{1,3}([^*]+)\*{1,3}/g, '$1');
  text = text.replace(/_{1,3}([^_]+)_{1,3}/g, '$1');
  text = text.replace(/~~([^~]+)~~/g, '$1');
  text = text.replace(/<[^>]+>/g, ' ');
  text = text.replace(/\s+/g, ' ').trim();
  return text;
}

/** Extract headings from raw markdown (before stripping). Returns array of heading strings. */
function extractHeadings(md) {
  const headings = [];
  const lines = md.split('\n');
  for (const line of lines) {
    const m = line.match(/^#{1,6}\s+(.+)$/);
    if (!m) continue;
    let text = m[1];
    // Strip inline markdown but keep the text
    text = text.replace(/`([^`]+)`/g, '$1');
    text = text.replace(/!\[[^\]]*\]\([^)]+\)/g, '');
    text = text.replace(/\[([^\]]*)\]\([^)]+\)/g, '$1');
    text = text.replace(/\*{1,3}([^*]+)\*{1,3}/g, '$1');
    text = text.replace(/_{1,3}([^_]+)_{1,3}/g, '$1');
    text = text.replace(/~~([^~]+)~~/g, '$1');
    text = text.replace(/<[^>]+>/g, '');
    text = text.trim();
    if (text) headings.push(text);
  }
  return headings;
}

/**
 * Tokenize plain text into a term → frequency map.
 * - CJK chars (U+4E00–9FFF) → character bigrams only (single chars are too noisy)
 * - Everything else → lowercase words ≥ 2 chars, split on [^a-z0-9]
 * - Purely numeric terms are skipped
 * - Stop words / stop chars are filtered
 */
function tokenize(text) {
  const terms = new Map();

  // Normalize common technical abbreviations that would be mangled by
  // punctuation stripping (e.g. "C++" → just "c" which is too short)
  text = text.replace(/\bC\+\+(?![a-z0-9])/gi, 'cpp');
  text = text.replace(/\bC#\b/g, 'csharp');
  text = text.replace(/\.NET\b/gi, 'dotnet');
  // Normalize US → UK spellings so both variants hit the same index term
  text = text.replace(/\boptimization\b/gi, 'optimisation');
  text = text.replace(/\bnormalization\b/gi, 'normalisation');
  text = text.replace(/\bvisualization\b/gi, 'visualisation');
  text = text.replace(/\bcharacterization\b/gi, 'characterisation');

  // Split so CJK runs and non-CJK runs are separate segments
  const segments = text.split(/([一-鿿㐀-䶿]+)/);

  for (const seg of segments) {
    if (!seg) continue;

    if (/^[一-鿿㐀-䶿]+$/.test(seg)) {
      // ── CJK segment ──
      // Bigrams only — single characters are too common to be useful
      for (let i = 0; i < seg.length - 1; i++) {
        const bigram = seg.slice(i, i + 2);
        // Skip bigrams where BOTH characters are stop chars
        if (ZH_STOP_CHARS.has(bigram[0]) && ZH_STOP_CHARS.has(bigram[1])) continue;
        terms.set(bigram, (terms.get(bigram) || 0) + 1);
      }
    } else {
      // ── Non-CJK segment ──
      const words = seg.toLowerCase().split(/[^a-z0-9]+/);
      for (const word of words) {
        if (word.length < 2) continue;
        if (EN_STOP_WORDS.has(word)) continue;
        // Skip purely numeric terms (version numbers, list indices, etc.)
        if (/^\d+$/.test(word)) continue;
        terms.set(word, (terms.get(word) || 0) + 1);
      }
    }
  }

  return terms;
}

/**
 * Build an inverted-index search structure from document metadata.
 *
 * Output format (compact keys for small payload):
 *   { docs: [{ p, t, l, k, h }], idx: { term: [[docIdx, weight], ...] } }
 *
 * Weighting:
 *   Title terms  × 10
 *   Heading terms × 5
 *   Body terms    × 1
 */
function buildInvertedIndex(allMeta) {
  const docs = [];
  const idx = Object.create(null); // no prototype — avoids collisions with 'constructor' etc.

  for (const m of allMeta) {
    const docIdx = docs.length;

    // Tokenize each zone separately so we can weight them
    const titleTerms = tokenize(m.title);
    const headingText = (m.headings || []).join(' ');
    const headingTerms = tokenize(headingText);
    const bodyTerms = tokenize(m.content);

    // Merge weighted terms
    const merged = new Map(); // term → weight
    for (const [t, f] of titleTerms)  merged.set(t, (merged.get(t) || 0) + f * 10);
    for (const [t, f] of headingTerms) merged.set(t, (merged.get(t) || 0) + f * 5);
    for (const [t, f] of bodyTerms)    merged.set(t, (merged.get(t) || 0) + f);

    // Doc record (no content — just metadata)
    docs.push({
      p: `${m.lang}/${m.relPath}`,
      t: m.title,
      l: m.lang,
      k: m.key,
      h: m.headings || [],
    });

    // Populate inverted index
    for (const [term, weight] of merged) {
      if (!idx[term]) idx[term] = [];
      idx[term].push([docIdx, weight]);
    }
  }

  // ── Post-filter: drop terms with poor signal-to-noise ratio ──
  const maxDocFreq = Math.floor(docs.length * 0.5);
  for (const term of Object.keys(idx)) {
    const df = idx[term].length;
    // Drop terms that appear in >50% of docs (no discriminatory power)
    if (df > maxDocFreq) { delete idx[term]; continue; }
    // Drop single-document terms with negligible weight (pure noise)
    if (df === 1 && idx[term][0][1] < 2) { delete idx[term]; continue; }
  }

  return { docs, idx };
}

/** Get file mtime as ISO string, or null if file doesn't exist. */
function getMtime(filePath) {
  try {
    return fs.statSync(filePath).mtime.toISOString();
  } catch {
    return null;
  }
}

/** Ensure a directory exists (mkdir -p). */
function ensureDir(dirPath) {
  if (!fs.existsSync(dirPath)) {
    fs.mkdirSync(dirPath, { recursive: true });
  }
}

/** Recursively copy a directory (used in full build). */
function copyDir(src, dest) {
  ensureDir(dest);
  const entries = fs.readdirSync(src, { withFileTypes: true });
  for (const ent of entries) {
    const srcPath = path.join(src, ent.name);
    const destPath = path.join(dest, ent.name);
    if (ent.isDirectory()) {
      copyDir(srcPath, destPath);
    } else {
      fs.copyFileSync(srcPath, destPath);
    }
  }
}

/** Inject build version into the dist index.html (replaces GUIDE4PC_BUILD_VER placeholder). */
function injectVersion() {
  const indexPath = path.join(DIST, 'index.html');
  if (!fs.existsSync(indexPath)) return;
  let html = fs.readFileSync(indexPath, 'utf-8');
  html = html.replace(/GUIDE4PC_BUILD_VER/g, BUILD_VERSION);
  fs.writeFileSync(indexPath, html, 'utf-8');
}

/** Generate dl/files.json manifest from sourcecode/dl/ directory structure. */
function buildDlManifest(srcDlDir) {
  const manifest = {};
  if (!fs.existsSync(srcDlDir)) return manifest;

  const entries = fs.readdirSync(srcDlDir, { withFileTypes: true });
  for (const ent of entries) {
    if (ent.isDirectory()) {
      const folderName = ent.name;
      const folderPath = path.join(srcDlDir, folderName);
      const files = fs.readdirSync(folderPath)
        .filter(f => fs.statSync(path.join(folderPath, f)).isFile())
        .sort();
      if (files.length > 0) {
        manifest[folderName] = files;
      }
    }
  }
  return manifest;
}

/** Copy markdown files preserving structure (used in full build). */
function copyMarkdownFiles(src, dest) {
  if (!fs.existsSync(src)) return;
  ensureDir(dest);
  const entries = fs.readdirSync(src, { withFileTypes: true });
  for (const ent of entries) {
    const srcPath = path.join(src, ent.name);
    const destPath = path.join(dest, ent.name);
    if (ent.isDirectory()) {
      copyMarkdownFiles(srcPath, destPath);
    } else if (ent.isFile() && ent.name.endsWith('.md')) {
      fs.copyFileSync(srcPath, destPath);
    }
  }
}

// ─── Cache ──────────────────────────────────────────────────────────────────

function readCache() {
  if (fs.existsSync(CACHE_FILE)) {
    try {
      const raw = fs.readFileSync(CACHE_FILE, 'utf-8');
      const cache = JSON.parse(raw);
      return {
        mdFiles: cache.mdFiles || {},
        statics: cache.statics || {},
      };
    } catch (e) {
      console.log('  ⚠  Cache corrupted, will do full build.');
    }
  }
  return { mdFiles: {}, statics: {} };
}

function writeCache(cache) {
  fs.writeFileSync(CACHE_FILE, JSON.stringify(cache, null, 2), 'utf-8');
}

// ─── Tree building ──────────────────────────────────────────────────────────

function buildTree(nodes, lang) {
  const rootNodes = [];
  const dirMap = new Map();

  for (const node of nodes) {
    const dir = path.dirname(node.path);
    if (dir === '.') {
      rootNodes.push(node);
    } else {
      if (!dirMap.has(dir)) {
        dirMap.set(dir, {
          type: 'directory',
          name: dir.split('/').pop(),
          path: dir,
          children: [],
        });
      }
      dirMap.get(dir).children.push(node);
    }
  }

  function sortNodes(arr) {
    arr.sort((a, b) => {
      if (a.key === 'home') return -1;
      if (b.key === 'home') return 1;
      // For composite keys like "appendix/1" or "main/Chapter2/3",
      // extract the numeric portion from the last segment
      const aLast = a.key.split('/').pop();
      const bLast = b.key.split('/').pop();
      const aNum = parseInt(aLast, 10);
      const bNum = parseInt(bLast, 10);
      if (!isNaN(aNum) && !isNaN(bNum)) {
        if (aNum !== bNum) return aNum - bNum;
      }
      return a.name.localeCompare(b.name, 'zh-Hans-CN');
    });
  }

  sortNodes(rootNodes);
  for (const [, dirNode] of dirMap) {
    sortNodes(dirNode.children);
  }

  const dirs = [...dirMap.values()].sort((a, b) => a.name.localeCompare(b.name, 'zh-Hans-CN'));
  return [...rootNodes, ...dirs];
}

// ─── Static file syncing ────────────────────────────────────────────────────

/**
 * Sync a single static file: copy to dest if changed, record mtime in cache.
 * Returns the cache key if the file was copied, null otherwise.
 */
function syncOne(srcPath, destPath, cacheKey, cache) {
  const mtime = getMtime(srcPath);
  if (!mtime) {
    // Source deleted – remove from cache but leave dest as-is
    delete cache.statics[cacheKey];
    return null;
  }

  const cached = cache.statics[cacheKey];
  if (cached && cached.mtime === mtime) {
    return null; // unchanged
  }

  ensureDir(path.dirname(destPath));
  fs.copyFileSync(srcPath, destPath);
  cache.statics[cacheKey] = { mtime };
  return cacheKey;
}

/** Sync a directory tree of static files. Returns array of changed cache keys. */
function syncDir(srcDir, destDir, prefix, cache) {
  const changed = [];
  if (!fs.existsSync(srcDir)) return changed;

  const entries = fs.readdirSync(srcDir, { withFileTypes: true });
  for (const ent of entries) {
    const srcPath = path.join(srcDir, ent.name);
    const destPath = path.join(destDir, ent.name);
    const cacheKey = `${prefix}/${ent.name}`;

    if (ent.isDirectory()) {
      changed.push(...syncDir(srcPath, destPath, cacheKey, cache));
    } else {
      const result = syncOne(srcPath, destPath, cacheKey, cache);
      if (result) changed.push(result);
    }
  }

  return changed;
}

/** Sync all static assets. Returns array of changed cache keys. */
function syncAllStatics(cache) {
  const changed = [];

  // index.html
  {
    const r = syncOne(
      path.join(ROOT, 'index.html'),
      path.join(DIST, 'index.html'),
      'index.html', cache
    );
    if (r) changed.push(r);
  }

  // css/*.css
  const cssDir = path.join(ROOT, 'css');
  if (fs.existsSync(cssDir)) {
    for (const f of fs.readdirSync(cssDir)) {
      if (f.endsWith('.css')) {
        const r = syncOne(
          path.join(cssDir, f),
          path.join(DIST, 'css', f),
          `css/${f}`, cache
        );
        if (r) changed.push(r);
      }
    }
  }

  // js/*.js (frontend only, skip build.js)
  const jsDir = path.join(ROOT, 'js');
  if (fs.existsSync(jsDir)) {
    for (const f of fs.readdirSync(jsDir)) {
      if (f !== 'build.js' && f.endsWith('.js')) {
        const r = syncOne(
          path.join(jsDir, f),
          path.join(DIST, 'js', f),
          `js/${f}`, cache
        );
        if (r) changed.push(r);
      }
    }
  }

  // sourcecode/img/**
  changed.push(...syncDir(
    path.join(SRC, 'img'),
    path.join(DIST, 'img'),
    'sourcecode/img', cache
  ));

  // sourcecode/favicon/** → dist root (not dist/favicon/)
  const favDir = path.join(SRC, 'favicon');
  if (fs.existsSync(favDir)) {
    for (const f of fs.readdirSync(favDir)) {
      const r = syncOne(
        path.join(favDir, f),
        path.join(DIST, f),
        `sourcecode/favicon/${f}`, cache
      );
      if (r) changed.push(r);
    }
  }

  // sourcecode/dl/** → dist/dl/ (downloadable files)
  changed.push(...syncDir(
    path.join(SRC, 'dl'),
    path.join(DIST, 'dl'),
    'sourcecode/dl', cache
  ));
  // Always regenerate dl/files.json (it reflects current state of sourcecode/dl/)
  const srcDlDir = path.join(SRC, 'dl');
  if (fs.existsSync(srcDlDir)) {
    const dlManifest = buildDlManifest(srcDlDir);
    ensureDir(path.join(DIST, 'dl'));
    fs.writeFileSync(
      path.join(DIST, 'dl', 'files.json'),
      JSON.stringify(dlManifest, null, 2),
      'utf-8'
    );
  }

  return changed;
}

// ─── Full build ─────────────────────────────────────────────────────────────

function fullBuild(cache) {
  console.log('🔨 Building Guide4PC (full)...\n');

  // 1. Scan source files
  const chsFiles = walkDir(path.join(SRC, 'chs'), path.join(SRC, 'chs')).sort();
  const enFiles = walkDir(path.join(SRC, 'en'), path.join(SRC, 'en')).sort();

  console.log(`  Found ${chsFiles.length} Chinese files, ${enFiles.length} English files.`);

  // Reset caches
  cache.mdFiles = {};
  cache.statics = {};

  // 2. Process all .md files
  const allMeta = [];

  function processFile(relPath, lang) {
    const fullPath = path.join(SRC, lang, relPath);
    const basename = path.basename(relPath);
    const key = extractKey(basename, relPath);
    const title = (key === 'home') ? (lang === 'chs' ? '欢迎信' : 'Welcome Letter') : extractTitle(basename);
    const md = fs.readFileSync(fullPath, 'utf-8');
    const headings = extractHeadings(md);
    const content = stripMarkdown(md);
    const mtime = getMtime(fullPath);

    const cacheKey = `${lang}/${relPath}`;
    cache.mdFiles[cacheKey] = { mtime, key, title, headings, content };

    allMeta.push({ relPath, lang, key, title, headings, content });
  }

  for (const f of chsFiles) processFile(f, 'chs');
  for (const f of enFiles) processFile(f, 'en');

  // 3. Build tree nodes from metadata
  function makeNodes(lang) {
    return allMeta
      .filter(m => m.lang === lang)
      .map(m => ({
        type: 'file',
        name: m.title,
        path: m.relPath,
        key: m.key,
        lang: m.lang,
      }));
  }

  const tree = {
    chs: buildTree(makeNodes('chs'), 'chs'),
    en: buildTree(makeNodes('en'), 'en'),
  };

  // 4. Build pairs
  const pairs = {};
  for (const m of allMeta) {
    if (!pairs[m.key]) pairs[m.key] = { chs: null, en: null };
    pairs[m.key][m.lang] = m.relPath;
  }

  // 5. Build search index (inverted index — compact & fast)
  const searchIndex = buildInvertedIndex(allMeta);

  // 6. Write dist/
  console.log('\n  Writing dist/...');

  // Clean dist/ fully
  if (fs.existsSync(DIST)) {
    fs.rmSync(DIST, { recursive: true, force: true });
  }
  ensureDir(DIST);

  // Create subdirectories
  ensureDir(path.join(DIST, 'js'));
  ensureDir(path.join(DIST, 'css'));
  ensureDir(path.join(DIST, 'chs'));
  ensureDir(path.join(DIST, 'en'));

  // .nojekyll
  fs.writeFileSync(path.join(DIST, '.nojekyll'), '', 'utf-8');

  // Write manifests (search index uses compact JSON to save bandwidth)
  const manifest = { tree, pairs, version: BUILD_VERSION };
  fs.writeFileSync(path.join(DIST, 'js', 'manifest.json'), JSON.stringify(manifest, null, 2), 'utf-8');
  fs.writeFileSync(path.join(DIST, 'js', 'search_index.json'), JSON.stringify(searchIndex), 'utf-8');

  // Copy index.html
  const srcIndex = path.join(ROOT, 'index.html');
  if (fs.existsSync(srcIndex)) {
    fs.copyFileSync(srcIndex, path.join(DIST, 'index.html'));
  }

  // Copy css/
  const srcCss = path.join(ROOT, 'css');
  if (fs.existsSync(srcCss)) {
    copyDir(srcCss, path.join(DIST, 'css'));
  }

  // Copy js/ (frontend only, not build.js)
  const srcJs = path.join(ROOT, 'js');
  if (fs.existsSync(srcJs)) {
    const jsFiles = fs.readdirSync(srcJs).filter(f => f !== 'build.js' && f.endsWith('.js'));
    for (const f of jsFiles) {
      fs.copyFileSync(path.join(srcJs, f), path.join(DIST, 'js', f));
    }
  }

  // Copy markdown files
  copyMarkdownFiles(path.join(SRC, 'chs'), path.join(DIST, 'chs'));
  copyMarkdownFiles(path.join(SRC, 'en'), path.join(DIST, 'en'));

  // Copy img/
  const srcImg = path.join(SRC, 'img');
  if (fs.existsSync(srcImg)) {
    copyDir(srcImg, path.join(DIST, 'img'));
  }

  // Copy favicon/ → dist root
  const srcFav = path.join(SRC, 'favicon');
  if (fs.existsSync(srcFav)) {
    for (const f of fs.readdirSync(srcFav)) {
      fs.copyFileSync(path.join(srcFav, f), path.join(DIST, f));
    }
  }

  // Copy dl/ → dist/dl/ (downloadable files)
  const srcDl = path.join(SRC, 'dl');
  if (fs.existsSync(srcDl)) {
    copyDir(srcDl, path.join(DIST, 'dl'));
    // Generate dl files manifest
    const dlManifest = buildDlManifest(srcDl);
    fs.writeFileSync(
      path.join(DIST, 'dl', 'files.json'),
      JSON.stringify(dlManifest, null, 2),
      'utf-8'
    );
  }

  // Record static mtimes in cache
  syncAllStatics(cache);

  // Inject build version into index.html
  injectVersion();

  // Write cache
  writeCache(cache);

  console.log(`\n✅ Full build complete! Output in ${DIST}/`);
  console.log(`   manifest.json: ${tree.chs.length + tree.en.length} tree nodes`);
  console.log(`   search_index.json: ${searchIndex.docs.length} docs, ${Object.keys(searchIndex.idx).length} unique terms`);
  console.log(`   pairs: ${Object.keys(pairs).length} pair groups`);
  console.log(`   version: ${BUILD_VERSION}`);
}

// ─── Incremental build ──────────────────────────────────────────────────────

function incrementalBuild(cache) {
  console.log('🔄 Building Guide4PC (incremental)...\n');

  // Ensure dist/ structure exists
  ensureDir(DIST);
  ensureDir(path.join(DIST, 'js'));
  ensureDir(path.join(DIST, 'css'));
  ensureDir(path.join(DIST, 'chs'));
  ensureDir(path.join(DIST, 'en'));
  fs.writeFileSync(path.join(DIST, '.nojekyll'), '', 'utf-8');

  // 1. Scan current .md files
  const chsFiles = walkDir(path.join(SRC, 'chs'), path.join(SRC, 'chs')).sort();
  const enFiles = walkDir(path.join(SRC, 'en'), path.join(SRC, 'en')).sort();

  const allCurrent = [
    ...chsFiles.map(f => ({ relPath: f, lang: 'chs' })),
    ...enFiles.map(f => ({ relPath: f, lang: 'en' })),
  ];

  // 2. Classify each file vs cache
  const newFiles = [];
  const modifiedFiles = [];
  const unchangedCount = { value: 0 };
  const currentCacheKeys = new Set();

  for (const { relPath, lang } of allCurrent) {
    const cacheKey = `${lang}/${relPath}`;
    currentCacheKeys.add(cacheKey);
    const fullPath = path.join(SRC, lang, relPath);
    const currentMtime = getMtime(fullPath);
    const cached = cache.mdFiles[cacheKey];

    if (!cached) {
      newFiles.push({ relPath, lang, cacheKey, mtime: currentMtime });
    } else if (cached.mtime !== currentMtime) {
      modifiedFiles.push({ relPath, lang, cacheKey, mtime: currentMtime });
    } else {
      unchangedCount.value++;
    }
  }

  // Find deleted files (in cache but not on disk)
  const deletedKeys = Object.keys(cache.mdFiles).filter(k => !currentCacheKeys.has(k));

  const totalChanges = newFiles.length + modifiedFiles.length + deletedKeys.length;
  console.log(`  Scanned: ${allCurrent.length} .md files`);
  console.log(`  New: ${newFiles.length}  |  Modified: ${modifiedFiles.length}  |  Deleted: ${deletedKeys.length}  |  Unchanged: ${unchangedCount.value}`);

  if (totalChanges === 0) {
    // Only sync static files (they might have changed)
    const staticChanged = syncAllStatics(cache);
    if (staticChanged.length > 0) {
      console.log(`\n  Static files changed: ${staticChanged.length}`);
      for (const f of staticChanged) {
        console.log(`  ✓ ${f}`);
      }
    } else {
      console.log('  Nothing changed. Build skipped.');
    }
    writeCache(cache);
    return;
  }

  // 3. Process new .md files
  for (const f of newFiles) {
    const fullPath = path.join(SRC, f.lang, f.relPath);
    const basename = path.basename(f.relPath);
    const key = extractKey(basename, f.relPath);
    const title = (key === 'home') ? (f.lang === 'chs' ? '欢迎信' : 'Welcome Letter') : extractTitle(basename);
    const md = fs.readFileSync(fullPath, 'utf-8');
    const headings = extractHeadings(md);
    const content = stripMarkdown(md);

    cache.mdFiles[f.cacheKey] = { mtime: f.mtime, key, title, headings, content };
    console.log(`  + ${f.cacheKey}`);
  }

  // 4. Process modified .md files
  for (const f of modifiedFiles) {
    const fullPath = path.join(SRC, f.lang, f.relPath);
    const basename = path.basename(f.relPath);
    const key = extractKey(basename, f.relPath);
    const title = (key === 'home') ? (f.lang === 'chs' ? '欢迎信' : 'Welcome Letter') : extractTitle(basename);
    const md = fs.readFileSync(fullPath, 'utf-8');
    const headings = extractHeadings(md);
    const content = stripMarkdown(md);

    cache.mdFiles[f.cacheKey] = { mtime: f.mtime, key, title, headings, content };
    console.log(`  ~ ${f.cacheKey}`);
  }

  // 5. Remove deleted files from cache
  for (const k of deletedKeys) {
    delete cache.mdFiles[k];
    console.log(`  - ${k}`);
  }

  // 6. Assemble full metadata from cache (all current files)
  const allMeta = [];
  for (const [cacheKey, entry] of Object.entries(cache.mdFiles)) {
    const slashIdx = cacheKey.indexOf('/');
    const lang = cacheKey.slice(0, slashIdx);
    const relPath = cacheKey.slice(slashIdx + 1);
    allMeta.push({
      relPath,
      lang,
      key: entry.key,
      title: entry.title,
      headings: entry.headings || [],
      content: entry.content,
    });
  }

  // 7. Rebuild tree
  function makeNodes(lang) {
    return allMeta
      .filter(m => m.lang === lang)
      .map(m => ({
        type: 'file',
        name: m.title,
        path: m.relPath,
        key: m.key,
        lang: m.lang,
      }));
  }

  const tree = {
    chs: buildTree(makeNodes('chs'), 'chs'),
    en: buildTree(makeNodes('en'), 'en'),
  };

  // 8. Rebuild pairs
  const pairs = {};
  for (const m of allMeta) {
    if (!pairs[m.key]) pairs[m.key] = { chs: null, en: null };
    pairs[m.key][m.lang] = m.relPath;
  }

  // 9. Rebuild search index (inverted index)
  const searchIndex = buildInvertedIndex(allMeta);

  // 10. Write manifest & search index
  const manifest = { tree, pairs, version: BUILD_VERSION };
  fs.writeFileSync(path.join(DIST, 'js', 'manifest.json'), JSON.stringify(manifest, null, 2), 'utf-8');
  fs.writeFileSync(path.join(DIST, 'js', 'search_index.json'), JSON.stringify(searchIndex, null, 2), 'utf-8');

  // 11. Copy new & modified .md files to dist
  const toCopy = [...newFiles, ...modifiedFiles];
  for (const f of toCopy) {
    const srcPath = path.join(SRC, f.lang, f.relPath);
    const destPath = path.join(DIST, f.lang, f.relPath);
    ensureDir(path.dirname(destPath));
    fs.copyFileSync(srcPath, destPath);
  }

  // 12. Delete removed .md files from dist
  for (const k of deletedKeys) {
    const destPath = path.join(DIST, k);
    if (fs.existsSync(destPath)) {
      fs.unlinkSync(destPath);
    }
    // Clean up empty parent directories in dist
    let dir = path.dirname(destPath);
    while (dir !== DIST && dir.startsWith(DIST)) {
      try {
        const remaining = fs.readdirSync(dir);
        if (remaining.length === 0) {
          fs.rmdirSync(dir);
          dir = path.dirname(dir);
        } else {
          break;
        }
      } catch {
        break;
      }
    }
  }

  // 13. Sync static files (compare mtimes, copy only changed)
  console.log('');
  const staticChanged = syncAllStatics(cache);
  if (staticChanged.length > 0) {
    for (const f of staticChanged) {
      console.log(`  ✓ ${f}`);
    }
  }

  // 14. Inject build version into index.html (always, even if no content changes)
  injectVersion();

  // 15. Write updated cache
  writeCache(cache);

  console.log(`\n✅ Incremental build complete!`);
  console.log(`   manifest.json: ${tree.chs.length + tree.en.length} tree nodes`);
  console.log(`   search_index.json: ${searchIndex.docs.length} docs, ${Object.keys(searchIndex.idx).length} unique terms`);
  console.log(`   pairs: ${Object.keys(pairs).length} pair groups`);
  console.log(`   version: ${BUILD_VERSION}`);
}

// ─── Entry ──────────────────────────────────────────────────────────────────

function build() {
  const forceFull = process.argv.includes('--force');
  const cache = readCache();

  // Full build when: --force flag, no cache yet, or dist/ missing
  if (forceFull || Object.keys(cache.mdFiles).length === 0 || !fs.existsSync(DIST)) {
    if (forceFull) console.log('  (--force: full rebuild)');
    if (!fs.existsSync(DIST)) console.log('  (dist/ missing: full rebuild)');
    fullBuild(cache);
  } else {
    incrementalBuild(cache);
  }
}

build();
