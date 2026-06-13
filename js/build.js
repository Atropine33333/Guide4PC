/**
 * build.js — Guide4PC build script
 * Scans sourcecode/chs/ and sourcecode/en/ for Markdown files,
 * generates manifest.json and search_index.json,
 * and copies all files into dist/ for GitHub Pages deployment.
 *
 * Usage: node js/build.js
 * (called from ./update.sh)
 */

const fs = require('fs');
const path = require('path');

const ROOT = path.resolve(__dirname, '..');
const SRC = path.join(ROOT, 'sourcecode');
const DIST = path.join(ROOT, 'docs');

// ─── Helpers ────────────────────────────────────────────────────────────────

/** Recursively list all files under dir, returning relative paths (forward-slash). */
function walkDir(dir, base) {
  const results = [];
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
  // If there's an underscore, take everything after the first underscore
  const underscoreIdx = name.indexOf('_');
  if (underscoreIdx !== -1) {
    return name.slice(underscoreIdx + 1);
  }
  return name;
}

/**
 * Extract pairing key from filename.
 * - Files starting with "0_" → key "home"
 * - Others: extract the first consecutive digits from the filename → that number as string
 * - If no digits found, use the title as key
 */
function extractKey(filename) {
  const name = path.basename(filename);
  // Check for 0_ prefix (home)
  if (/^0_/.test(name)) return 'home';
  // Extract first number
  const match = name.match(/(\d+)/);
  if (match) return match[1];
  // Fallback: use the title
  return extractTitle(name);
}

/** Strip Markdown formatting to get plain text. */
function stripMarkdown(md) {
  let text = md;
  // Remove YAML frontmatter if present
  text = text.replace(/^---[\s\S]*?---\n?/, '');
  // Remove HTML comments
  text = text.replace(/<!--[\s\S]*?-->/g, '');
  // Remove code blocks (``` ... ```)
  text = text.replace(/```[\s\S]*?```/g, ' ');
  // Remove inline code
  text = text.replace(/`[^`]+`/g, ' ');
  // Remove images: ![alt](url)
  text = text.replace(/!\[[^\]]*\]\([^)]+\)/g, ' ');
  // Remove links but keep text: [text](url)
  text = text.replace(/\[([^\]]*)\]\([^)]+\)/g, '$1');
  // Remove headings markers
  text = text.replace(/^#{1,6}\s+/gm, '');
  // Remove blockquote markers
  text = text.replace(/^>\s?/gm, '');
  // Remove horizontal rules
  text = text.replace(/^[-*_]{3,}\s*$/gm, ' ');
  // Remove list markers
  text = text.replace(/^[\s]*[-*+]\s+/gm, ' ');
  text = text.replace(/^[\s]*\d+\.\s+/gm, ' ');
  // Remove bold/italic markers
  text = text.replace(/\*{1,3}([^*]+)\*{1,3}/g, '$1');
  text = text.replace(/_{1,3}([^_]+)_{1,3}/g, '$1');
  // Remove strikethrough
  text = text.replace(/~~([^~]+)~~/g, '$1');
  // Remove HTML tags
  text = text.replace(/<[^>]+>/g, ' ');
  // Collapse whitespace
  text = text.replace(/\s+/g, ' ').trim();
  return text;
}

// ─── Build ──────────────────────────────────────────────────────────────────

function build() {
  console.log('🔨 Building Guide4PC...\n');

  // 1. Scan source files
  const chsFiles = walkDir(path.join(SRC, 'chs'), path.join(SRC, 'chs')).sort();
  const enFiles = walkDir(path.join(SRC, 'en'), path.join(SRC, 'en')).sort();

  console.log(`  Found ${chsFiles.length} Chinese files, ${enFiles.length} English files.`);

  // 2. Build pairing map and tree nodes
  /** @type {Map<string, {chs?: string, en?: string}>} */
  const pairMap = new Map();

  function processFile(relPath, lang) {
    const basename = path.basename(relPath);
    const key = extractKey(basename);
    const title = extractTitle(basename);

    if (!pairMap.has(key)) {
      pairMap.set(key, {});
    }
    pairMap.get(key)[lang] = relPath;

    return {
      type: 'file',
      name: (key === 'home') ? (lang === 'chs' ? '欢迎信' : 'Welcome Letter') : title,
      path: relPath,
      key: key,
      lang: lang,
    };
  }

  const chsNodes = chsFiles.map(f => processFile(f, 'chs'));
  const enNodes = enFiles.map(f => processFile(f, 'en'));

  // 3. Build tree structure (supporting subdirectories)
  function buildTree(nodes, lang) {
    // First pass: group by directory
    const rootNodes = [];
    const dirMap = new Map(); // dirPath -> {type:'directory', name, path, children:[]}

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

    // Sort children in each directory
    function sortNodes(arr) {
      arr.sort((a, b) => {
        // Home always first
        if (a.key === 'home') return -1;
        if (b.key === 'home') return 1;
        // Compare numeric keys
        const aNum = parseInt(a.key, 10);
        const bNum = parseInt(b.key, 10);
        if (!isNaN(aNum) && !isNaN(bNum)) {
          if (aNum !== bNum) return aNum - bNum;
        }
        // Fallback: alphabetical by name
        return a.name.localeCompare(b.name, 'zh-Hans-CN');
      });
    }

    sortNodes(rootNodes);
    for (const [, dirNode] of dirMap) {
      sortNodes(dirNode.children);
    }

    // Combine: root nodes first, then directories
    // Directories sorted by name
    const dirs = [...dirMap.values()].sort((a, b) => a.name.localeCompare(b.name, 'zh-Hans-CN'));
    return [...rootNodes, ...dirs];
  }

  const tree = {
    chs: buildTree(chsNodes, 'chs'),
    en: buildTree(enNodes, 'en'),
  };

  // 4. Build pairs object
  const pairs = {};
  for (const [key, entry] of pairMap) {
    pairs[key] = { chs: entry.chs || null, en: entry.en || null };
  }

  // 5. Build manifest
  const manifest = { tree, pairs };

  // 6. Build search index
  const searchIndex = [];

  function addToSearchIndex(relPath, lang) {
    const fullPath = path.join(SRC, lang, relPath);
    const md = fs.readFileSync(fullPath, 'utf-8');
    const basename = path.basename(relPath);
    const key = extractKey(basename);
    const title = (key === 'home') ? (lang === 'chs' ? '欢迎信' : 'Welcome Letter') : extractTitle(basename);

    searchIndex.push({
      path: `${lang}/${relPath}`,
      title: title,
      lang: lang,
      key: key,
      content: stripMarkdown(md),
    });
  }

  for (const f of chsFiles) addToSearchIndex(f, 'chs');
  for (const f of enFiles) addToSearchIndex(f, 'en');

  // 7. Write dist/
  console.log('  Writing dist/...');

  // Ensure dist directories
  const distJs = path.join(DIST, 'js');
  const distCss = path.join(DIST, 'css');
  const distChs = path.join(DIST, 'chs');
  const distEn = path.join(DIST, 'en');
  const distImg = path.join(DIST, 'img');

  // Clean dist (but keep .git if present)
  if (fs.existsSync(DIST)) {
    const keepGit = path.join(DIST, '.git');
    const hasGit = fs.existsSync(keepGit);
    fs.rmSync(DIST, { recursive: true, force: true });
    fs.mkdirSync(DIST, { recursive: true });
    // We won't restore .git since this is the deploy directory
  }

  fs.mkdirSync(distJs, { recursive: true });
  fs.mkdirSync(distCss, { recursive: true });
  fs.mkdirSync(distChs, { recursive: true });
  fs.mkdirSync(distEn, { recursive: true });

  // .nojekyll — tells GitHub Pages to skip Jekyll processing
  fs.writeFileSync(path.join(DIST, '.nojekyll'), '', 'utf-8');

  // Write JSON manifests
  fs.writeFileSync(path.join(distJs, 'manifest.json'), JSON.stringify(manifest, null, 2), 'utf-8');
  fs.writeFileSync(path.join(distJs, 'search_index.json'), JSON.stringify(searchIndex, null, 2), 'utf-8');

  // Copy index.html
  const srcIndex = path.join(ROOT, 'index.html');
  if (fs.existsSync(srcIndex)) {
    fs.copyFileSync(srcIndex, path.join(DIST, 'index.html'));
  }

  // Copy css/
  const srcCss = path.join(ROOT, 'css');
  if (fs.existsSync(srcCss)) {
    copyDir(srcCss, distCss);
  }

  // Copy js/ (frontend JS only, not build.js)
  const srcJs = path.join(ROOT, 'js');
  if (fs.existsSync(srcJs)) {
    const jsFiles = fs.readdirSync(srcJs).filter(f => f !== 'build.js' && f.endsWith('.js'));
    for (const f of jsFiles) {
      fs.copyFileSync(path.join(srcJs, f), path.join(distJs, f));
    }
  }

  // Copy markdown files preserving directory structure
  copyMarkdownFiles(path.join(SRC, 'chs'), distChs);
  copyMarkdownFiles(path.join(SRC, 'en'), distEn);

  // Copy img/
  const srcImg = path.join(SRC, 'img');
  if (fs.existsSync(srcImg)) {
    copyDir(srcImg, distImg);
  }

  // Copy favicon/ to dist root (referenced as /favicon.ico etc.)
  const srcFavicon = path.join(SRC, 'favicon');
  if (fs.existsSync(srcFavicon)) {
    const favFiles = fs.readdirSync(srcFavicon);
    for (const f of favFiles) {
      fs.copyFileSync(path.join(srcFavicon, f), path.join(DIST, f));
    }
  }

  console.log(`\n✅ Build complete! Output in ${DIST}/`);
  console.log(`   manifest.json: ${manifest.tree.chs.length + manifest.tree.en.length} tree nodes`);
  console.log(`   search_index.json: ${searchIndex.length} documents indexed`);
  console.log(`   pairs: ${Object.keys(pairs).length} pair groups`);
}

/** Recursively copy a directory. */
function copyDir(src, dest) {
  fs.mkdirSync(dest, { recursive: true });
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

/** Copy markdown files from src to dest preserving structure. */
function copyMarkdownFiles(src, dest) {
  if (!fs.existsSync(src)) return;
  fs.mkdirSync(dest, { recursive: true });
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

// ─── Run ────────────────────────────────────────────────────────────────────

build();
