/**
 * build.js — Guide4PC build script
 * Incremental by default: only rebuilds changed files (detected by mtime).
 * Use --force for a full rebuild.
 *
 * Usage: node js/build.js [--force]
 * (called from ./update.sh)
 */

const crypto = require('crypto');
const fs = require('fs');
const path = require('path');

const ROOT = path.resolve(__dirname, '..');
const SRC = path.join(ROOT, 'sourcecode');
const DIST = path.join(ROOT, 'docs');
const CACHE_FILE = path.join(ROOT, '.build-cache.json');

// Bump whenever key extraction / metadata logic changes. When the cached
// version differs, the next `./update.sh` performs a full rebuild by itself —
// no need to remember `--force`, and stale keys can never mix with new ones.
const CACHE_VERSION = 3;

// ─── Build version ──────────────────────────────────────────────────────────

/**
 * Content-derived build version (10 hex chars).
 * - Same sources → same version: no pointless reloads for visitors.
 * - Any change → new version: cache-busts assets and lets a running page
 *   detect that a newer deployment exists.
 * Covers everything that ends up in dist/: page template, stylesheet, app
 * script, markdown, images, downloads and favicons.
 */
let _buildVersion = null;
function getBuildVersion() {
  if (_buildVersion) return _buildVersion;

  const hash = crypto.createHash('sha1');
  const add = (file) => {
    hash.update(path.relative(ROOT, file));
    hash.update('\0');
    hash.update(fs.readFileSync(file));
    hash.update('\0');
  };
  const walk = (dir) => {
    if (!fs.existsSync(dir)) return;
    const entries = fs.readdirSync(dir, { withFileTypes: true })
      .sort((a, b) => a.name.localeCompare(b.name));
    for (const ent of entries) {
      const full = path.join(dir, ent.name);
      if (ent.isDirectory()) walk(full);
      else if (ent.isFile()) add(full);
    }
  };

  for (const f of ['index.html', 'css/main.css', 'js/main.js', 'js/build.js']) {
    const full = path.join(ROOT, f);
    if (fs.existsSync(full)) add(full);
  }
  for (const d of ['chs', 'en', 'img', 'dl', 'favicon']) {
    walk(path.join(SRC, d));
  }
  for (const f of [
    'playground/pg.js',
    'playground/pg-worker.js',
    'playground/pg-lint-worker.js',
    'playground/pg.css',
    'playground/scene.js',
    'playground/scene.css',
    'playground/vendor/editor.bundle.js',
    'playground/vendor/clang-wasm-toolchain.global.js',
    'diagnostics/messages.json',
  ]) {
    const full = path.join(ROOT, f);
    if (fs.existsSync(full)) add(full);
  }
  for (const d of ['exercises', 'test']) {
    const full = path.join(ROOT, d);
    if (fs.existsSync(full)) walk(full);
  }

  _buildVersion = hash.digest('hex').slice(0, 10);
  return _buildVersion;
}

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
 * - [dlX] prefix is stripped before extracting the key (it's metadata)
 * - Numeric keys keep decimal parts and letter suffixes so documents stay
 *   distinct, sortable and pairable (letter suffix is uppercased, so
 *   "8a" and "8A" produce the same key and pair across languages):
 *     Section7_作业1.md        → "7"
 *     Section7.10_数据来源.md   → "7.10"
 *     Section8A_多文件与导入.md → "8A"
 * - Letter-only names after a known section prefix keep the letter:
 *     SectionA_前言.md → "A"   AppendixB_… → "B"
 * - The key is namespaced by directory to prevent collisions:
 *   - Top-level "附录…" or "Appendix…" → "appendix/<key>"
 *   - Top-level other files → "root/<key>"
 *   - Subdirectory files → "<normalized-dir>/<key>"
 * - If no key can be derived, use the title as key
 */
function extractKey(filename, relPath) {
  let name = path.basename(filename);

  // Strip [dlX] metadata prefix (e.g. "[dl1]Section0_代码" → "Section0_代码")
  name = name.replace(/^\[dl\d+\]/, '');

  // Draft pages ("[test]…") are scratch content: keep them out of the
  // cross-language pairing keys so they never shadow a real section.
  if (/^\[test\]/.test(name)) {
    const draftDir = path.dirname(relPath);
    const draftNs = (draftDir === '.' || draftDir === '') ? 'root' : normalizeDir(draftDir);
    return draftNs + '/test/' + extractTitle(name);
  }

  if (/^0_/.test(name)) return 'home';

  // Numeric key: digits (with optional decimal parts) plus optional letters.
  const numMatch = name.match(/(\d+(?:\.\d+)*)([A-Za-z]+)?/);
  const numKey = numMatch
    ? numMatch[1] + (numMatch[2] ? numMatch[2].toUpperCase() : '')
    : null;

  // Letter-only key, e.g. "SectionA_前言.md" or "AppendixB_…" (no digits).
  const letterMatch = numKey ? null
    : name.match(/^(?:Section|Chapter|Part|附录|Appendix)([A-Za-z]+)(?=[._\s-]|$)/);

  const key = numKey || (letterMatch ? letterMatch[1].toUpperCase() : extractTitle(name));

  const dir = path.dirname(relPath);

  if (dir === '.' || dir === '') {
    // Top-level files: namespace by content type
    if (/^(附录|Appendix)/.test(name)) {
      return 'appendix/' + key;
    }
    return 'root/' + key;
  }

  // Subdirectory files: namespace by normalized directory path
  const ns = normalizeDir(dir);
  return ns + '/' + key;
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

/**
 * Regenerate dist/index.html from the root template on every build, replacing
 * every GUIDE4PC_BUILD_VER placeholder with the current build version. The
 * template itself carries the placeholders on the css/js URLs and in
 * window.GUIDE4PC_VERSION, so here we only substitute.
 * Also writes js/version.json (tiny no-cache endpoint the app polls to detect
 * new deployments). Both files are only written when their content changed.
 */
function injectVersion() {
  const srcIndex = path.join(ROOT, 'index.html');
  const distIndex = path.join(DIST, 'index.html');
  const version = getBuildVersion();

  if (fs.existsSync(srcIndex)) {
    const html = fs.readFileSync(srcIndex, 'utf-8')
      .replace(/GUIDE4PC_BUILD_VER/g, version);
    const previous = fs.existsSync(distIndex) ? fs.readFileSync(distIndex, 'utf-8') : null;
    if (previous !== html) {
      ensureDir(DIST);
      fs.writeFileSync(distIndex, html, 'utf-8');
    }
  }

  // Tiny endpoint used by the running app to detect new deployments.
  const versionFile = path.join(DIST, 'js', 'version.json');
  const versionJson = JSON.stringify({ version });
  const previousVersion = fs.existsSync(versionFile) ? fs.readFileSync(versionFile, 'utf-8') : null;
  if (previousVersion !== versionJson) {
    ensureDir(path.join(DIST, 'js'));
    fs.writeFileSync(versionFile, versionJson, 'utf-8');
  }
}

/**
 * Keep the version field of the existing dist manifest in sync with the
 * current content version. Needed on incremental runs where only static
 * files (main.js/main.css/images/build.js) changed and the markdown-derived
 * manifest was not rebuilt.
 */
function syncManifestVersion() {
  const manifestPath = path.join(DIST, 'js', 'manifest.json');
  if (!fs.existsSync(manifestPath)) return;
  try {
    const manifest = JSON.parse(fs.readFileSync(manifestPath, 'utf-8'));
    const version = getBuildVersion();
    if (manifest.version === version) return;
    manifest.version = version;
    fs.writeFileSync(manifestPath, JSON.stringify(manifest, null, 2), 'utf-8');
    console.log(`  ✓ manifest version → ${version}`);
  } catch (e) {
    // Unreadable manifest — the next full build will recreate it
  }
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
      if ((cache.version || 0) !== CACHE_VERSION) {
        console.log(`  ⚠  Cache format v${cache.version || 0} → v${CACHE_VERSION}: full rebuild required.`);
        return { version: CACHE_VERSION, mdFiles: {}, statics: {} };
      }
      return {
        version: CACHE_VERSION,
        mdFiles: cache.mdFiles || {},
        statics: cache.statics || {},
      };
    } catch (e) {
      console.log('  ⚠  Cache corrupted, will do full build.');
    }
  }
  return { version: CACHE_VERSION, mdFiles: {}, statics: {} };
}

function writeCache(cache) {
  cache.version = CACHE_VERSION;
  fs.writeFileSync(CACHE_FILE, JSON.stringify(cache, null, 2), 'utf-8');
}

// ─── Tree building ──────────────────────────────────────────────────────────

/**
 * Parse the last segment of a key for natural sorting.
 * Numeric keys ("7", "7.9", "8A") rank before letter-only keys ("A"),
 * which rank before title-based keys (null is returned for those).
 */
function parseSortKey(key) {
  const last = key.split('/').pop();

  const numMatch = last.match(/^(\d+(?:\.\d+)*)([A-Za-z]*)$/);
  if (numMatch) {
    return {
      rank: 0,
      parts: numMatch[1].split('.').map(Number),
      suffix: numMatch[2].toUpperCase(),
    };
  }

  if (/^[A-Za-z]+$/.test(last)) {
    return { rank: 1, str: last.toUpperCase() };
  }

  return null;
}

/**
 * Natural key order: 7 < 7.9 < 7.10 < 8 < 8A < 8B < 10 < A < B < titles.
 * Returns 0 when both keys are equal / not comparable, so the caller can
 * fall back to comparing display names.
 */
function compareKeys(a, b) {
  const ka = parseSortKey(a);
  const kb = parseSortKey(b);
  if (ka && kb) {
    if (ka.rank !== kb.rank) return ka.rank - kb.rank;
    if (ka.rank === 0) {
      const len = Math.max(ka.parts.length, kb.parts.length);
      for (let i = 0; i < len; i++) {
        // Missing decimal levels sort before explicit ones (7 < 7.0)
        const pa = ka.parts[i] === undefined ? -1 : ka.parts[i];
        const pb = kb.parts[i] === undefined ? -1 : kb.parts[i];
        if (pa !== pb) return pa - pb;
      }
      if (ka.suffix !== kb.suffix) return ka.suffix < kb.suffix ? -1 : 1;
    } else if (ka.str !== kb.str) {
      return ka.str < kb.str ? -1 : 1;
    }
  } else if (ka || kb) {
    // Keyed files come before title-based keys
    return ka ? -1 : 1;
  }
  return 0;
}

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
      // Composite keys like "appendix/1" or "main/Chapter3_Python/8A":
      // compare natural key order first, then fall back to display name.
      const cmp = compareKeys(a.key, b.key);
      return cmp !== 0 ? cmp : a.name.localeCompare(b.name, 'zh-Hans-CN');
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

  // css/fonts/** (web fonts for code blocks and the playground)
  const fontsDir = path.join(cssDir, 'fonts');
  if (fs.existsSync(fontsDir)) {
    changed.push(...syncDir(
      fontsDir,
      path.join(DIST, 'css', 'fonts'),
      'css/fonts', cache
    ));
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

// ─── Playground runtime ─────────────────────────────────────────────────────

/**
 * Copy the in-browser playground runtime, exercise data and diagnostics into
 * dist/:
 *   playground/{pg.js,pg-worker.js,pg.css,vendor/*} → docs/pg/
 *   exercises/<id>/{spec.json,*.cpp}               → docs/ex/<id>/
 *   diagnostics/messages.json                      → docs/diag/
 */
function syncPlayground() {
  let copied = 0;
  const copyFile = (from, to) => {
    ensureDir(path.dirname(to));
    fs.copyFileSync(from, to);
    copied++;
  };

  const pgSrc = path.join(ROOT, 'playground');
  for (const f of ['pg.js', 'pg-worker.js', 'pg-lint-worker.js', 'pg.css', 'scene.js', 'scene.css']) {
    const from = path.join(pgSrc, f);
    if (fs.existsSync(from)) copyFile(from, path.join(DIST, 'pg', f));
  }
  const vendorDir = path.join(pgSrc, 'vendor');
  if (fs.existsSync(vendorDir)) {
    for (const f of fs.readdirSync(vendorDir)) {
      const from = path.join(vendorDir, f);
      if (fs.statSync(from).isFile()) copyFile(from, path.join(DIST, 'pg', 'vendor', f));
    }
  }

  const exSrc = path.join(ROOT, 'exercises');
  if (fs.existsSync(exSrc)) {
    for (const id of fs.readdirSync(exSrc)) {
      const dir = path.join(exSrc, id);
      if (!fs.statSync(dir).isDirectory()) continue;
      for (const f of fs.readdirSync(dir)) {
        if (f === 'spec.json' || f.endsWith('.cpp')) {
          copyFile(path.join(dir, f), path.join(DIST, 'ex', id, f));
        }
      }
    }
  }

  const diag = path.join(ROOT, 'diagnostics', 'messages.json');
  if (fs.existsSync(diag)) copyFile(diag, path.join(DIST, 'diag', 'messages.json'));

  // 练习索引：测试台与未来的题目总览共用。
  if (fs.existsSync(exSrc)) {
    const index = [];
    for (const id of fs.readdirSync(exSrc)) {
      const specFile = path.join(exSrc, id, 'spec.json');
      if (!fs.existsSync(specFile)) continue;
      try {
        const spec = JSON.parse(fs.readFileSync(specFile, 'utf8'));
        index.push({
          id: spec.id || id,
          title: spec.title || null,
          lang: spec.lang || 'cpp',
          category: spec.category || null,
          rank: typeof spec.rank === 'number' ? spec.rank : null,
          checks: Array.isArray(spec.checks) ? spec.checks.length : 0,
          harness: !!spec.harness,
        });
      } catch (err) {
        console.warn(`  ! exercises/${id}/spec.json: ${err.message}`);
      }
    }
    ensureDir(path.join(DIST, 'ex'));
    fs.writeFileSync(path.join(DIST, 'ex', 'index.json'), JSON.stringify(index, null, 2));
  }

  // 测试子站：/test/ 独立页面，构建时整体复制。
  const testSrc = path.join(ROOT, 'test');
  if (fs.existsSync(testSrc)) {
    const walkCopy = (from, to) => {
      ensureDir(to);
      for (const ent of fs.readdirSync(from, { withFileTypes: true })) {
        const src = path.join(from, ent.name);
        const dst = path.join(to, ent.name);
        if (ent.isDirectory()) walkCopy(src, dst);
        else if (ent.isFile()) copyFile(src, dst);
      }
    };
    walkCopy(testSrc, path.join(DIST, 'test'));
  }

  return copied;
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
    const title = (key === 'home') ? (lang === 'chs' ? '开屏' : 'Start') : extractTitle(basename);
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

  // Files at the dist root that this build does not generate (site-verification
  // tokens, CNAME, robots.txt, 404.html …) must survive a full rebuild, which
  // otherwise wipes dist/ completely. Read them before the wipe, restore later.
  const preservedRootFiles = [];
  if (fs.existsSync(DIST)) {
    const generatedRoot = new Set(['.nojekyll', 'index.html']);
    const favDir = path.join(SRC, 'favicon');
    if (fs.existsSync(favDir)) {
      for (const f of fs.readdirSync(favDir)) generatedRoot.add(f);
    }
    for (const ent of fs.readdirSync(DIST, { withFileTypes: true })) {
      if (!ent.isFile() || generatedRoot.has(ent.name)) continue;
      const full = path.join(DIST, ent.name);
      if (fs.statSync(full).size > 32 * 1024 * 1024) {
        console.warn(`  ! Skipping oversized non-build file in dist/: ${ent.name}`);
        continue;
      }
      preservedRootFiles.push({ name: ent.name, data: fs.readFileSync(full) });
    }
  }

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
  const manifest = { tree, pairs, version: getBuildVersion() };
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

  // Restore preserved dist-root files (see snapshot above)
  for (const { name, data } of preservedRootFiles) {
    fs.writeFileSync(path.join(DIST, name), data);
  }

  // Record static mtimes in cache
  syncAllStatics(cache);
  syncPlayground();

  // Write cache
  writeCache(cache);

  console.log(`\n✅ Full build complete! Output in ${DIST}/`);
  console.log(`   manifest.json: ${tree.chs.length + tree.en.length} tree nodes`);
  console.log(`   search_index.json: ${searchIndex.docs.length} docs, ${Object.keys(searchIndex.idx).length} unique terms`);
  console.log(`   pairs: ${Object.keys(pairs).length} pair groups`);
  console.log(`   version: ${getBuildVersion()}`);
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
    syncPlayground();
    if (staticChanged.length > 0) {
      console.log(`\n  Static files changed: ${staticChanged.length}`);
      for (const f of staticChanged) {
        console.log(`  ✓ ${f}`);
      }
    } else {
      console.log('  Nothing changed. Build skipped.');
    }
    // A static file or build.js may have changed the content version even
    // though no markdown was rebuilt — keep the manifest in sync.
    syncManifestVersion();
    writeCache(cache);
    return;
  }

  // 3. Process new .md files
  for (const f of newFiles) {
    const fullPath = path.join(SRC, f.lang, f.relPath);
    const basename = path.basename(f.relPath);
    const key = extractKey(basename, f.relPath);
    const title = (key === 'home') ? (f.lang === 'chs' ? '开屏' : 'Start') : extractTitle(basename);
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
    const title = (key === 'home') ? (f.lang === 'chs' ? '开屏' : 'Start') : extractTitle(basename);
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

  // 10. Write manifest & search index (search index is compact to save bandwidth)
  const manifest = { tree, pairs, version: getBuildVersion() };
  fs.writeFileSync(path.join(DIST, 'js', 'manifest.json'), JSON.stringify(manifest, null, 2), 'utf-8');
  fs.writeFileSync(path.join(DIST, 'js', 'search_index.json'), JSON.stringify(searchIndex), 'utf-8');

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
  syncPlayground();
  if (staticChanged.length > 0) {
    for (const f of staticChanged) {
      console.log(`  ✓ ${f}`);
    }
  }

  // 14. Write updated cache (index.html is regenerated in build())
  writeCache(cache);

  console.log(`\n✅ Incremental build complete!`);
  console.log(`   manifest.json: ${tree.chs.length + tree.en.length} tree nodes`);
  console.log(`   search_index.json: ${searchIndex.docs.length} docs, ${Object.keys(searchIndex.idx).length} unique terms`);
  console.log(`   pairs: ${Object.keys(pairs).length} pair groups`);
  console.log(`   version: ${getBuildVersion()}`);
}

// ─── Entry ──────────────────────────────────────────────────────────────────

function build() {
  const forceFull = process.argv.includes('--force');
  const cache = readCache();

  // Full build when: --force flag, empty/invalidated cache, or dist/ missing
  if (forceFull || Object.keys(cache.mdFiles).length === 0 || !fs.existsSync(DIST)) {
    if (forceFull) console.log('  (--force: full rebuild)');
    if (!fs.existsSync(DIST)) console.log('  (dist/ missing: full rebuild)');
    fullBuild(cache);
  } else {
    incrementalBuild(cache);
  }

  // Regenerate index.html from the template with the current content version.
  // Runs on every build (including "nothing changed") so the deployed version
  // and the cache-busting query strings always reflect the sources.
  injectVersion();
}

// Only run when invoked directly (`node js/build.js`), so the internals can
// also be required for quick checks in `node -e`.
if (require.main === module) {
  build();
}

module.exports = { extractKey, parseSortKey, compareKeys, buildTree };
