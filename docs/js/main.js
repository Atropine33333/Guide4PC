/**
 * main.js — Guide4PC frontend logic
 * Routing, sidebar, search, language switching, reading progress, copy buttons.
 */

(function () {
  'use strict';

  // ─── State ──────────────────────────────────────────────────────────────
  let manifest = null;       // manifest.json data
  let searchIndex = null;    // search_index.json data
  let currentLang = 'en';    // current language
  let currentPath = null;    // current document relative path (within lang dir)
  let currentKey = null;     // current document pairing key
  let isSearching = false;   // whether a search term is active
  let searchTerm = '';       // current search term
  let _switchSeq = 0;        // sequence counter to prevent stale language switches

  // ─── DOM refs ───────────────────────────────────────────────────────────
  const $sidebar    = document.getElementById('sidebar');
  const $backdrop   = document.getElementById('sidebar-backdrop');
  const $navTree    = document.getElementById('nav-tree');
  const $searchInput = document.getElementById('search-input');
  const $searchClear = document.getElementById('search-clear');
  const $sidebarClose = document.getElementById('sidebar-close');
  const $sidebarToggle = document.getElementById('sidebar-toggle');
  const $langSwitch = document.getElementById('lang-switch');
  const $langSwitchText = document.getElementById('lang-switch-text');
  const $content    = document.getElementById('content');
  const $topbar     = document.querySelector('.topbar');
  const $toastContainer = document.getElementById('toast-container');
  const $settingsToggle = document.getElementById('settings-toggle');
  const $settingsPanel = document.getElementById('settings-panel');
  const $settingsClose = document.getElementById('settings-close');
  const $fontScale = document.getElementById('font-scale');
  const $fontScaleVal = document.getElementById('font-scale-val');
  const $contentWidth = document.getElementById('content-width');
  const $contentWidthVal = document.getElementById('content-width-val');
  const $sidebarWidth = document.getElementById('sidebar-width');
  const $sidebarWidthVal = document.getElementById('sidebar-width-val');
  const $themeOptions = document.getElementById('theme-options');

  // ─── Constants ──────────────────────────────────────────────────────────
  const PROGRESS_KEY_PREFIX = 'guide4pc_progress_';
  const SETTINGS_KEY = 'guide4pc_settings';
  const ALLOWED_HTML_TAGS = /^<\/?(ins|mark|kbd|br|strong|em|span|a|code|sub|sup|small|abbr|cite)(\s[^>]*)?\/?>$/i;

  // ─── Markdown-it setup ──────────────────────────────────────────────────
  const md = window.markdownit({
    html: true,
    linkify: true,
    typographer: true,
    breaks: false,
  });

  // Plugins
  md.use(window.markdownitFootnote);
  md.use(window.markdownitDeflist);

  // Custom HTML filter: allow only safe inline tags
  const defaultHtmlBlock = md.renderer.rules.html_block;
  const defaultHtmlInline = md.renderer.rules.html_inline;

  md.renderer.rules.html_block = function (tokens, idx, options, env, self) {
    const token = tokens[idx];
    if (ALLOWED_HTML_TAGS.test(token.content)) {
      return token.content;
    }
    // Escape disallowed HTML
    return md.utils.escapeHtml(token.content);
  };

  md.renderer.rules.html_inline = function (tokens, idx, options, env, self) {
    const token = tokens[idx];
    if (ALLOWED_HTML_TAGS.test(token.content)) {
      return token.content;
    }
    return md.utils.escapeHtml(token.content);
  };

  // Open all links in new tab
  const defaultLinkOpen = md.renderer.rules.link_open || function (tokens, idx, options, env, self) {
    return self.renderToken(tokens, idx, options);
  };
  md.renderer.rules.link_open = function (tokens, idx, options, env, self) {
    const token = tokens[idx];
    const hrefIndex = token.attrIndex('href');
    if (hrefIndex >= 0) {
      token.attrSet('target', '_blank');
      token.attrSet('rel', 'noopener noreferrer');
    }
    return defaultLinkOpen(tokens, idx, options, env, self);
  };

  // ─── Fetch with error handling ──────────────────────────────────────────
  async function fetchJSON(url) {
    const resp = await fetch(url);
    if (!resp.ok) throw new Error(`Failed to load ${url}: ${resp.status}`);
    return resp.json();
  }

  async function fetchMD(url) {
    const resp = await fetch(url);
    if (!resp.ok) throw new Error(`Failed to load ${url}: ${resp.status}`);
    return resp.text();
  }

  // ─── Toast ──────────────────────────────────────────────────────────────
  function showToast(message) {
    const el = document.createElement('div');
    el.className = 'toast';
    el.textContent = message;
    $toastContainer.appendChild(el);

    // Auto-remove after animation (2s visible + transition)
    setTimeout(() => {
      if (el.parentNode) el.parentNode.removeChild(el);
    }, 2200);
  }

  // ─── Settings ───────────────────────────────────────────────────────────
  function loadSettings() {
    try {
      const raw = localStorage.getItem(SETTINGS_KEY);
      if (raw) return JSON.parse(raw);
    } catch (e) { /* ignore */ }
    return { fontScale: 1, contentWidth: 75, sidebarWidth: 280, theme: 'light' };
  }

  function saveSettings(s) {
    try {
      localStorage.setItem(SETTINGS_KEY, JSON.stringify(s));
    } catch (e) { /* ignore */ }
  }

  function applySettings(s) {
    document.documentElement.style.setProperty('--font-scale', s.fontScale);
    document.documentElement.style.setProperty('--content-width', s.contentWidth + '%');
    document.documentElement.style.setProperty('--sidebar-width-dyn', s.sidebarWidth + 'px');
  }

  function syncSliders(s) {
    $fontScale.value = s.fontScale;
    $fontScaleVal.textContent = Number(s.fontScale).toFixed(2) + 'x';
    $contentWidth.value = s.contentWidth;
    $contentWidthVal.textContent = s.contentWidth + '%';
    $sidebarWidth.value = s.sidebarWidth;
    $sidebarWidthVal.textContent = s.sidebarWidth + 'px';
  }

  function applyTheme(theme) {
    document.documentElement.dataset.theme = theme;
    // Toggle highlight.js theme: dark for 暗夜, light for others
    const hljsLight = document.getElementById('hljs-light');
    const hljsDark = document.getElementById('hljs-dark');
    if (hljsLight && hljsDark) {
      if (theme === 'dark') {
        hljsLight.disabled = true;
        hljsDark.disabled = false;
      } else {
        hljsLight.disabled = false;
        hljsDark.disabled = true;
      }
    }
  }

  function syncThemeButtons(theme) {
    if (!$themeOptions) return;
    $themeOptions.querySelectorAll('.theme-btn').forEach(btn => {
      btn.classList.toggle('active', btn.dataset.theme === theme);
    });
  }

  const settings = loadSettings();
  applySettings(settings);
  syncSliders(settings);
  applyTheme(settings.theme);
  syncThemeButtons(settings.theme);

  // Settings panel toggle
  $settingsToggle.addEventListener('click', () => {
    $settingsPanel.classList.toggle('hidden');
    syncSliders(loadSettings());
  });

  $settingsClose.addEventListener('click', () => {
    $settingsPanel.classList.add('hidden');
  });

  // Close panel on outside click
  document.addEventListener('click', (e) => {
    if (!$settingsPanel.classList.contains('hidden') &&
        !$settingsPanel.contains(e.target) &&
        e.target !== $settingsToggle &&
        !$settingsToggle.contains(e.target)) {
      $settingsPanel.classList.add('hidden');
    }
  });

  // Slider handlers
  $fontScale.addEventListener('input', () => {
    const s = loadSettings();
    s.fontScale = parseFloat($fontScale.value);
    saveSettings(s);
    applySettings(s);
    syncSliders(s);
  });

  $contentWidth.addEventListener('input', () => {
    const s = loadSettings();
    s.contentWidth = parseInt($contentWidth.value);
    saveSettings(s);
    applySettings(s);
    syncSliders(s);
  });

  $sidebarWidth.addEventListener('input', () => {
    const s = loadSettings();
    s.sidebarWidth = parseInt($sidebarWidth.value);
    saveSettings(s);
    applySettings(s);
    syncSliders(s);
  });

  // Theme button handler
  if ($themeOptions) {
    $themeOptions.addEventListener('click', (e) => {
      const btn = e.target.closest('.theme-btn');
      if (!btn) return;
      const theme = btn.dataset.theme;
      const s = loadSettings();
      s.theme = theme;
      saveSettings(s);
      applyTheme(theme);
      syncThemeButtons(theme);
    });
  }

  // ─── Hash parsing ───────────────────────────────────────────────────────
  function parseHash() {
    const hash = window.location.hash.slice(1); // remove #
    if (!hash) return { lang: 'en', path: null };

    const slashIdx = hash.indexOf('/');
    if (slashIdx === -1) {
      // Just a language? default to home
      return { lang: hash, path: null };
    }

    const lang = hash.slice(0, slashIdx);
    const path = decodeURIComponent(hash.slice(slashIdx + 1));
    return { lang, path };
  }

  function setHash(lang, docPath) {
    // Use replaceState to avoid triggering hashchange event,
    // which would corrupt currentKey/currentPath with URL-encoded paths.
    if (docPath) {
      history.replaceState(null, '', `#${lang}/${docPath}`);
    } else {
      history.replaceState(null, '', `#${lang}`);
    }
  }

  // ─── Pairing helpers ────────────────────────────────────────────────────
  function getPairKey(lang, docPath) {
    if (!manifest || !docPath) return null;
    for (const [key, pair] of Object.entries(manifest.pairs)) {
      if (pair[lang] === docPath) return key;
    }
    return null;
  }

  function getPairedDoc(key, targetLang) {
    if (!manifest || !key) return null;
    const pair = manifest.pairs[key];
    if (!pair) return null;
    return pair[targetLang] || null;
  }

  // ─── Reading progress ───────────────────────────────────────────────────
  function getProgressKey(lang, docPath) {
    return PROGRESS_KEY_PREFIX + lang + '/' + docPath;
  }

  function saveProgress(lang, docPath, headingText) {
    if (!docPath) return;
    const key = getProgressKey(lang, docPath);
    try {
      localStorage.setItem(key, headingText);
    } catch (e) {
      // localStorage full or unavailable — ignore
    }
  }

  function loadProgress(lang, docPath) {
    if (!docPath) return null;
    const key = getProgressKey(lang, docPath);
    try {
      return localStorage.getItem(key);
    } catch (e) {
      return null;
    }
  }

  // ─── Scroll to heading ──────────────────────────────────────────────────
  function scrollToHeading(headingText) {
    if (!headingText) return;
    // Find the <h2> or <h3> with matching text
    const headings = $content.querySelectorAll('h2, h3');
    for (const h of headings) {
      if (h.textContent.trim() === headingText.trim()) {
        h.scrollIntoView({ behavior: 'smooth', block: 'start' });
        return;
      }
    }
  }

  // ─── Get current visible heading ────────────────────────────────────────
  function getCurrentHeading() {
    const headings = $content.querySelectorAll('h2, h3');
    if (headings.length === 0) return null;

    let closest = null;
    let closestTop = Infinity;

    for (const h of headings) {
      const rect = h.getBoundingClientRect();
      // Heading is above or at the viewport top
      if (rect.top <= 80) {
        const dist = 80 - rect.top;
        if (dist < closestTop) {
          closestTop = dist;
          closest = h.textContent.trim();
        }
      }
    }

    return closest;
  }

  // ─── Highlight search term in content ───────────────────────────────────
  function highlightInContent(term) {
    if (!term) return;

    // Walk text nodes and wrap matches
    const walker = document.createTreeWalker(
      $content,
      NodeFilter.SHOW_TEXT,
      {
        acceptNode: function (node) {
          // Skip nodes inside code blocks, scripts, and already-marked nodes
          if (node.parentElement.closest('pre, code, script, style, .copy-btn, mark.search-mark')) {
            return NodeFilter.FILTER_REJECT;
          }
          return NodeFilter.FILTER_ACCEPT;
        }
      }
    );

    const textNodes = [];
    while (walker.nextNode()) {
      textNodes.push(walker.currentNode);
    }

    const escapedTerm = term.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
    const regex = new RegExp(`(${escapedTerm})`, 'gi');
    let firstMatch = null;

    for (const node of textNodes) {
      const text = node.textContent;
      if (!regex.test(text)) continue;
      regex.lastIndex = 0; // reset

      const frag = document.createDocumentFragment();
      let lastIdx = 0;
      let match;

      while ((match = regex.exec(text)) !== null) {
        // Text before match
        if (match.index > lastIdx) {
          frag.appendChild(document.createTextNode(text.slice(lastIdx, match.index)));
        }
        // Highlighted match
        const mark = document.createElement('mark');
        mark.className = 'search-mark';
        mark.textContent = match[0];
        frag.appendChild(mark);

        if (!firstMatch) {
          firstMatch = mark;
        }

        lastIdx = regex.lastIndex;
        if (match[0].length === 0) regex.lastIndex++; // avoid infinite loop on zero-width
      }

      // Remaining text
      if (lastIdx < text.length) {
        frag.appendChild(document.createTextNode(text.slice(lastIdx)));
      }

      node.parentNode.replaceChild(frag, node);
    }

    // Scroll to first match
    if (firstMatch) {
      setTimeout(() => {
        firstMatch.scrollIntoView({ behavior: 'smooth', block: 'center' });
      }, 100);
    }

    // Remove highlights after 3 seconds
    setTimeout(() => {
      const marks = $content.querySelectorAll('mark.search-mark');
      marks.forEach(m => {
        m.classList.add('fading');
        // Replace mark with its text content after transition
        setTimeout(() => {
          if (m.parentNode) {
            m.parentNode.replaceChild(document.createTextNode(m.textContent), m);
          }
        }, 3000);
      });
    }, 3000);
  }

  // ─── KaTeX lazy-loader ─────────────────────────────────────────────────
  let kaTeXLoaded = false;
  let kaTeXLoading = null; // Promise to avoid double-load

  function loadKaTeX() {
    if (kaTeXLoaded) return Promise.resolve();
    if (kaTeXLoading) return kaTeXLoading;

    kaTeXLoading = Promise.all([
      loadCSS('https://lib.baomitu.com/KaTeX/0.16.9/katex.min.css'),
      loadScript('https://lib.baomitu.com/KaTeX/0.16.9/katex.min.js'),
    ]).then(() => {
      return loadScript('https://lib.baomitu.com/KaTeX/0.16.9/contrib/auto-render.min.js');
    }).then(() => {
      kaTeXLoaded = true;
    });

    return kaTeXLoading;
  }

  function loadScript(src) {
    return new Promise((resolve, reject) => {
      const s = document.createElement('script');
      s.src = src;
      s.onload = resolve;
      s.onerror = reject;
      document.head.appendChild(s);
    });
  }

  function loadCSS(href) {
    return new Promise((resolve, reject) => {
      const l = document.createElement('link');
      l.rel = 'stylesheet';
      l.href = href;
      l.onload = resolve;
      l.onerror = reject;
      document.head.appendChild(l);
    });
  }

  // ─── Copy button ────────────────────────────────────────────────────────
  function addCopyButtons() {
    const pres = $content.querySelectorAll('pre');
    pres.forEach(pre => {
      // Avoid duplicate buttons
      if (pre.querySelector('.copy-btn')) return;

      const btn = document.createElement('button');
      btn.className = 'copy-btn';
      btn.appendChild(svgIcon(ICONS.copy));
      btn.setAttribute('aria-label', '复制代码');
      btn.addEventListener('click', async () => {
        const code = pre.querySelector('code');
        const text = code ? code.textContent : pre.textContent;
        try {
          await navigator.clipboard.writeText(text);
          btn.textContent = '';
          btn.appendChild(svgIcon(ICONS.check));
          showToast('复制成功！');
          setTimeout(() => {
            btn.textContent = '';
            btn.appendChild(svgIcon(ICONS.copy));
          }, 2000);
        } catch (err) {
          showToast('复制失败');
        }
      });

      pre.appendChild(btn);
    });
  }

  // ─── Render document ────────────────────────────────────────────────────
  async function renderDocument(lang, docPath, seq) {
    if (!docPath) return;

    // If a newer render started, don't even show loading for this stale call
    if (seq !== undefined && seq !== _switchSeq) return;

    $content.innerHTML = '<div class="loading"><div class="spinner"></div><p>加载中…</p></div>';

    try {
      const url = `${lang}/${docPath}`;
      const mdText = await fetchMD(url);

      // If a newer render started while we were fetching, discard this result
      if (seq !== undefined && seq !== _switchSeq) return;

      let html = md.render(mdText);

      $content.innerHTML = html;

      // Add home-page class for welcome/home documents
      currentKey = getPairKey(lang, docPath);
      if (currentKey === 'home') {
        $content.classList.add('home-page');
      } else {
        $content.classList.remove('home-page');
      }

      // Lazy-load KaTeX only when math delimiters detected
      if (/\$\$|\$|\\\[|\\\(/.test(html)) {
        await loadKaTeX();
        renderMathInElement($content, {
          delimiters: [
            { left: '$$', right: '$$', display: true },
            { left: '$', right: '$', display: false },
            { left: '\\[', right: '\\]', display: true },
            { left: '\\(', right: '\\)', display: false },
          ],
          throwOnError: false,
        });
      }

      // Highlight code blocks
      $content.querySelectorAll('pre code').forEach(block => {
        if (typeof hljs !== 'undefined') {
          hljs.highlightElement(block);
        }
      });

      // Add copy buttons
      addCopyButtons();

      // Restore reading progress
      const saved = loadProgress(lang, docPath);
      if (saved) {
        setTimeout(() => scrollToHeading(saved), 200);
      }

      // Apply search highlight if searching
      if (isSearching && searchTerm) {
        setTimeout(() => highlightInContent(searchTerm), 300);
      }

      // Update sidebar active state
      updateSidebarActive(lang, docPath);

    } catch (err) {
      console.error(err);
      $content.innerHTML = `<div class="loading"><p>加载文档失败：${err.message}</p></div>`;
    }
  }

  // ─── Navigate to document ───────────────────────────────────────────────
  function navigateTo(lang, docPath) {
    currentLang = lang;
    currentPath = docPath;
    currentKey = getPairKey(lang, docPath);
    document.documentElement.lang = (lang === 'chs') ? 'zh-Hans' : 'en';

    // Update hash silently
    if (docPath) {
      history.replaceState(null, '', `#${lang}/${docPath}`);
    } else {
      history.replaceState(null, '', `#${lang}`);
    }

    // Update language switch button
    updateLangSwitchButton();

    // Render
    renderDocument(lang, docPath);

    // Close mobile sidebar
    closeSidebar();
  }

  // ─── Language switch button ─────────────────────────────────────────────
  function updateLangSwitchButton() {
    if (currentLang === 'chs') {
      $langSwitchText.textContent = 'English';
    } else {
      $langSwitchText.textContent = '中文';
    }
  }

  // ─── Sidebar tree building ──────────────────────────────────────────────
  function buildNavTree(treeNodes, lang, level) {
    level = level || 0;
    const frag = document.createDocumentFragment();

    for (const node of treeNodes) {
      if (node.type === 'directory') {
        // Directory node
        const dirEl = document.createElement('div');
        dirEl.className = 'nav-dir';
        dirEl.dataset.dirPath = node.path;

        const header = document.createElement('div');
        header.className = 'nav-dir-header';
        header.style.paddingLeft = (1 + level * 1) + 'rem';
        const chevIcon = svgIcon(ICONS.chevronDown);
        chevIcon.classList.add('dir-icon');
        header.appendChild(chevIcon);
        header.appendChild(document.createTextNode(node.name));

        header.addEventListener('click', () => {
          dirEl.classList.toggle('collapsed');
        });

        const children = document.createElement('div');
        children.className = 'nav-dir-children';
        children.appendChild(buildNavTree(node.children, lang, level + 1));

        dirEl.appendChild(header);
        dirEl.appendChild(children);
        frag.appendChild(dirEl);

      } else {
        // File node
        const fileEl = document.createElement('div');
        fileEl.className = 'nav-file';
        fileEl.style.paddingLeft = (1 + level * 1) + 'rem';
        fileEl.dataset.path = node.path;
        fileEl.dataset.lang = lang;
        fileEl.dataset.key = node.key;
        const fileIcon = svgIcon(ICONS.file);
        fileIcon.classList.add('file-icon');
        fileEl.appendChild(fileIcon);
        const span = document.createElement('span');
        span.textContent = node.name;
        fileEl.appendChild(span);

        fileEl.addEventListener('click', () => {
          navigateTo(lang, node.path);
        });

        frag.appendChild(fileEl);
      }
    }

    return frag;
  }

  function populateSidebar(lang) {
    if (!manifest || !manifest.tree) return;

    const treeData = manifest.tree[lang];
    if (!treeData) return;

    $navTree.innerHTML = '';
    $navTree.appendChild(buildNavTree(treeData, lang, 0));
  }

  function updateSidebarActive(lang, docPath) {
    // Remove all active states
    $navTree.querySelectorAll('.nav-file.active').forEach(el => el.classList.remove('active'));

    // Find and activate current doc
    const fileEl = $navTree.querySelector(`.nav-file[data-lang="${lang}"][data-path="${escapeAttr(docPath)}"]`);
    if (fileEl) {
      fileEl.classList.add('active');
      // Expand parent directories
      let parent = fileEl.parentElement;
      while (parent) {
        if (parent.classList.contains('nav-dir')) {
          parent.classList.remove('collapsed');
        }
        parent = parent.parentElement;
      }
    }
  }

  // ─── Search ─────────────────────────────────────────────────────────────
  function performSearch(term) {
    searchTerm = term;
    isSearching = !!term;

    if (!term) {
      // Clear search: show all
      $navTree.querySelectorAll('.nav-file, .nav-dir').forEach(el => {
        el.style.display = '';
      });
      $searchClear.classList.add('hidden');

      // Re-populate sidebar with current language
      populateSidebar(currentLang);
      updateSidebarActive(currentLang, currentPath);
      return;
    }

    $searchClear.classList.remove('hidden');
    const lowerTerm = term.toLowerCase();

    // Search in search_index.json
    const matchedPaths = new Set();
    if (searchIndex) {
      for (const entry of searchIndex) {
        const haystack = (entry.title + ' ' + entry.content).toLowerCase();
        if (haystack.includes(lowerTerm)) {
          matchedPaths.add(entry.path); // format: "chs/path.md" or "en/path.md"
        }
      }
    }

    // Filter sidebar tree
    // First, show all files from all languages
    // Rebuild tree with both languages
    rebuildSearchTree(lowerTerm, matchedPaths);
  }

  function rebuildSearchTree(lowerTerm, matchedPaths) {
    $navTree.innerHTML = '';

    if (!manifest || !manifest.tree) return;

    const frag = document.createDocumentFragment();

    // Show files from both languages that match
    for (const lang of ['chs', 'en']) {
      const treeData = manifest.tree[lang];
      if (!treeData) continue;

      const langLabel = document.createElement('div');
      langLabel.className = 'nav-dir-header';
      langLabel.style.cssText = 'cursor:default;font-weight:600;';
      langLabel.textContent = lang === 'chs' ? '中文' : 'English';

      // Don't add label yet — we'll only add if there are matches
      let langHasMatches = false;
      const langFrag = document.createDocumentFragment();

      function filterNodes(nodes, level) {
        const resultFrag = document.createDocumentFragment();

        for (const node of nodes) {
          if (node.type === 'directory') {
            const childFrag = filterNodes(node.children, level + 1);
            if (childFrag.children.length > 0 || hasMatchingChild(node, matchedPaths, lang)) {
              // Keep this directory
              const dirEl = document.createElement('div');
              dirEl.className = 'nav-dir';

              const header = document.createElement('div');
              header.className = 'nav-dir-header';
              header.style.paddingLeft = (1 + level * 1) + 'rem';
              const chevIcon = svgIcon(ICONS.chevronDown);
        chevIcon.classList.add('dir-icon');
        header.appendChild(chevIcon);
        header.appendChild(document.createTextNode(node.name));
              header.addEventListener('click', () => {
                dirEl.classList.toggle('collapsed');
              });

              const children = document.createElement('div');
              children.className = 'nav-dir-children';
              children.appendChild(childFrag);

              dirEl.appendChild(header);
              dirEl.appendChild(children);
              resultFrag.appendChild(dirEl);
              langHasMatches = true;
            }
          } else {
            // File node
            const fullPath = lang + '/' + node.path;
            if (matchedPaths.has(fullPath)) {
              const fileEl = document.createElement('div');
              fileEl.className = 'nav-file';
              fileEl.style.paddingLeft = (1 + level * 1) + 'rem';
              fileEl.dataset.path = node.path;
              fileEl.dataset.lang = lang;
              fileEl.dataset.key = node.key;

              // Highlight matching part in name
              const nameLower = node.name.toLowerCase();
              const matchIdx = nameLower.indexOf(lowerTerm);
              let displayName;
              if (matchIdx !== -1) {
                displayName = escapeHTML(node.name.slice(0, matchIdx)) +
                  '<mark>' + escapeHTML(node.name.slice(matchIdx, matchIdx + lowerTerm.length)) + '</mark>' +
                  escapeHTML(node.name.slice(matchIdx + lowerTerm.length));
              } else {
                displayName = escapeHTML(node.name);
              }

              const fi = svgIcon(ICONS.file);
              fi.classList.add('file-icon');
              fileEl.appendChild(fi);
              const sp = document.createElement('span');
              sp.innerHTML = displayName;
              fileEl.appendChild(sp);

              fileEl.addEventListener('click', () => {
                navigateTo(lang, node.path);
                // After navigation, apply search highlight
                setTimeout(() => highlightInContent(searchTerm), 300);
              });

              resultFrag.appendChild(fileEl);
              langHasMatches = true;
            }
          }
        }

        return resultFrag;
      }

      function hasMatchingChild(node, matchedPaths, lang) {
        if (node.type === 'file') {
          return matchedPaths.has(lang + '/' + node.path);
        }
        if (node.children) {
          return node.children.some(child => hasMatchingChild(child, matchedPaths, lang));
        }
        return false;
      }

      const filtered = filterNodes(treeData, 0);
      if (filtered.children.length > 0) {
        frag.appendChild(langLabel);
        frag.appendChild(filtered);
      }
    }

    if (frag.children.length === 0) {
      const empty = document.createElement('div');
      empty.className = 'nav-file';
      empty.style.cssText = 'cursor:default;color:var(--color-text-muted);';
      empty.textContent = '无匹配结果';
      frag.appendChild(empty);
    }

    $navTree.appendChild(frag);
  }

  // ─── Language switching ─────────────────────────────────────────────────
  async function switchLanguage() {
    const seq = ++_switchSeq;
    const targetLang = currentLang === 'chs' ? 'en' : 'chs';

    // Find paired document
    let targetPath = null;

    if (currentKey) {
      targetPath = getPairedDoc(currentKey, targetLang);
    }

    if (!targetPath) {
      // No translation available — fall back to home
      const homePath = getPairedDoc('home', targetLang);
      if (targetLang === 'chs') {
        showToast('该文档暂无中文版');
      } else {
        showToast('No English version');
      }
      // Navigate to home if no match at all
      if (homePath) {
        targetPath = homePath;
      } else {
        return; // nowhere to go
      }
    }

    // Save current heading for position restoration
    const currentHeading = getCurrentHeading();

    currentLang = targetLang;
    currentPath = targetPath;
    currentKey = getPairKey(targetLang, targetPath);
    setHash(targetLang, targetPath);
    updateLangSwitchButton();

    // Rebuild sidebar for new language (if not searching)
    if (!isSearching) {
      populateSidebar(targetLang);
      updateSidebarActive(targetLang, targetPath);
    }

    // Render the document (pass seq to guard against stale renders)
    await renderDocument(targetLang, targetPath, seq);

    // If a newer switch started while we were rendering, discard this result
    if (seq !== _switchSeq) return;

    // After render, try to scroll to matching heading
    if (currentHeading) {
      setTimeout(() => scrollToHeading(currentHeading), 300);
    }
  }

  // ─── Mobile sidebar ─────────────────────────────────────────────────────
  function openSidebar() {
    $sidebar.classList.add('open');
    $backdrop.classList.remove('hidden');
    document.body.style.overflow = 'hidden';
  }

  function closeSidebar() {
    $sidebar.classList.remove('open');
    $backdrop.classList.add('hidden');
    document.body.style.overflow = '';
  }

  // ─── Event listeners ────────────────────────────────────────────────────
  $sidebarToggle.addEventListener('click', openSidebar);
  $sidebarClose.addEventListener('click', closeSidebar);
  $backdrop.addEventListener('click', closeSidebar);

  // Close sidebar on Escape
  document.addEventListener('keydown', (e) => {
    if (e.key === 'Escape' && $sidebar.classList.contains('open')) {
      closeSidebar();
    }
  });

  // Language switch
  $langSwitch.addEventListener('click', switchLanguage);

  // Search
  $searchInput.addEventListener('input', () => {
    const term = $searchInput.value.trim();
    performSearch(term);
  });

  $searchClear.addEventListener('click', () => {
    $searchInput.value = '';
    performSearch('');
    $searchInput.focus();
  });

  // Also handle Escape in search
  $searchInput.addEventListener('keydown', (e) => {
    if (e.key === 'Escape') {
      $searchInput.value = '';
      performSearch('');
      $searchInput.blur();
    }
  });

  // Scroll tracking for reading progress (debounced)
  let scrollTimer = null;
  document.addEventListener('scroll', () => {
    if (scrollTimer) clearTimeout(scrollTimer);
    scrollTimer = setTimeout(() => {
      if (!currentPath) return;
      const heading = getCurrentHeading();
      if (heading) {
        saveProgress(currentLang, currentPath, heading);
      }
    }, 500);
  }, { passive: true });

  // Hash change (browser back/forward)
  window.addEventListener('hashchange', () => {
    const { lang, path } = parseHash();
    const targetLang = (lang === 'chs' || lang === 'en') ? lang : 'en';
    let targetPath = path;

    // Validate lang
    if (targetLang !== currentLang && !isSearching) {
      populateSidebar(targetLang);
    }

    currentLang = targetLang;
    currentPath = targetPath;
    currentKey = getPairKey(targetLang, targetPath);
    updateLangSwitchButton();

    if (targetPath) {
      renderDocument(targetLang, targetPath);
    }
  });

  // ─── Topbar auto-hide on scroll (mobile only) ───────────────────────────
  let _lastScrollY = 0;
  window.addEventListener('scroll', () => {
    if (!$topbar || window.innerWidth > 768) return;
    const sy = window.scrollY;
    if (sy > _lastScrollY + 8 && sy > 120) {
      $topbar.classList.add('hidden');
    } else if (sy < _lastScrollY - 8) {
      $topbar.classList.remove('hidden');
    }
    _lastScrollY = sy;
  }, { passive: true });

  // ─── Disable swipe gestures on mobile ───────────────────────────────────
  document.addEventListener('touchstart', function (e) {
    // If the touch starts near the left edge and moves right, prevent it
    // Only block horizontal swipes
    const touch = e.touches[0];
    if (touch) {
      document._touchStartX = touch.clientX;
    }
  }, { passive: true });

  document.addEventListener('touchmove', function (e) {
    const touch = e.touches[0];
    if (touch && document._touchStartX !== undefined) {
      const deltaX = Math.abs(touch.clientX - document._touchStartX);
      const deltaY = Math.abs(touch.clientY - (document._touchStartY || 0));
      // If horizontal swipe is dominant, prevent default
      if (deltaX > deltaY && deltaX > 10) {
        e.preventDefault();
      }
    }
  }, { passive: false });

  document.addEventListener('touchstart', function (e) {
    const touch = e.touches[0];
    if (touch) {
      document._touchStartY = touch.clientY;
    }
  }, { passive: true });

  // ─── SVG icon helpers ──────────────────────────────────────────────────
  function svgIcon(pathD, viewBox) {
    const svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
    svg.setAttribute('viewBox', viewBox || '0 0 512 512');
    svg.setAttribute('class', 'icon');
    const path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
    path.setAttribute('d', pathD);
    svg.appendChild(path);
    return svg;
  }

  const ICONS = {
    file: 'M64 0C28.7 0 0 28.7 0 64V352c0 35.3 28.7 64 64 64h96v80c0 8.8 7.2 16 16 16s16-7.2 16-16V384H384c35.3 0 64-28.7 64-64V64c0-35.3-28.7-64-64-64H64zM384 96v256H64V96H384zM176 176c0-8.8 7.2-16 16-16h96c8.8 0 16 7.2 16 16s-7.2 16-16 16H192c-8.8 0-16-7.2-16-16zm0 64c0-8.8 7.2-16 16-16h96c8.8 0 16 7.2 16 16s-7.2 16-16 16H192c-8.8 0-16-7.2-16-16zm0 64c0-8.8 7.2-16 16-16h64c8.8 0 16 7.2 16 16s-7.2 16-16 16H192c-8.8 0-16-7.2-16-16z',
    chevronDown: 'M233.4 406.6c12.5 12.5 32.8 12.5 45.3 0l192-192c12.5-12.5 12.5-32.8 0-45.3s-32.8-12.5-45.3 0L256 338.7 86.6 169.4c-12.5-12.5-32.8-12.5-45.3 0s-12.5 32.8 0 45.3l192 192z',
    copy: 'M384 336H192c-8.8 0-16-7.2-16-16V64c0-8.8 7.2-16 16-16H352l64 64V320c0 8.8-7.2 16-16 16zm-192 48H384c35.3 0 64-28.7 64-64V109.3c0-4.2-1.7-8.3-4.7-11.3l-61.3-61.3c-3-3-7.1-4.7-11.3-4.7H192c-35.3 0-64 28.7-64 64V320c0 8.8 7.2 16 16 16zM160 384H320v48c0 35.3-28.7 64-64 64H64c-35.3 0-64-28.7-64-64V192c0-35.3 28.7-64 64-64h48V384z',
    check: 'M438.6 105.4c12.5 12.5 12.5 32.8 0 45.3l-256 256c-12.5 12.5-32.8 12.5-45.3 0l-128-128c-12.5-12.5-12.5-32.8 0-45.3s32.8-12.5 45.3 0L160 338.7 393.4 105.4c12.5-12.5 32.8-12.5 45.3 0z',
  };

  // ─── HTML escape helpers ────────────────────────────────────────────────
  function escapeHTML(str) {
    const div = document.createElement('div');
    div.textContent = str;
    return div.innerHTML;
  }

  function escapeAttr(str) {
    return str.replace(/"/g, '\\"');
  }

  // ─── Initialize ─────────────────────────────────────────────────────────
  async function init() {
    try {
      // Load manifest and search index
      [manifest, searchIndex] = await Promise.all([
        fetchJSON('js/manifest.json'),
        fetchJSON('js/search_index.json'),
      ]);

      // Parse initial hash
      const { lang, path } = parseHash();
      currentLang = (lang === 'chs' || lang === 'en') ? lang : 'en';
      document.documentElement.lang = (currentLang === 'chs') ? 'zh-Hans' : 'en';
      currentPath = path;
      currentKey = currentPath ? getPairKey(currentLang, currentPath) : null;

      updateLangSwitchButton();

      // Build sidebar
      populateSidebar(currentLang);

      // Load initial document or default to home
      if (!currentPath) {
        // Default to English welcome
        currentPath = getPairedDoc('home', 'en') || '0_WelcomeLetter.md';
        currentLang = 'en';
        currentKey = 'home';
        setHash(currentLang, currentPath);
        updateLangSwitchButton();
      }

      updateSidebarActive(currentLang, currentPath);
      await renderDocument(currentLang, currentPath);

    } catch (err) {
      console.error('Initialization error:', err);
      $content.innerHTML = `<div class="loading"><p>初始化失败：${err.message}</p><p>请确保已运行构建脚本 (./update.sh)</p></div>`;
    }
  }

  // Start when DOM is ready
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }

})();
