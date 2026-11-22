/* Guide4PC 练习站 —— 独立小应用。
 *
 * 数据来自构建产物：
 *   ../exercise/index.json          题目列表
 *   ../exercise/papers/index.json   预组卷子
 *   ../exercise/<dir>/<id>/problem.<lang>.md
 *   ../ex/<id>/spec.json            变体声明（播放器读）
 *
 * 挂载卡片复用 playground/pg.js：先设 window.G4PC_BASE = '../'，再加载 pg.js。
 */
(function () {
  'use strict';

  var ROOT = '../';
  var savedLang = '';
  try { savedLang = localStorage.getItem('guide4pc_lang') || ''; } catch (e) { savedLang = ''; }
  var qsLang = '';
  try { qsLang = new URLSearchParams(location.search).get('lang') || ''; } catch (e) { qsLang = ''; }
  var initialLang = (qsLang || savedLang || document.documentElement.lang || 'zh').toLowerCase();
  var LOCALE = initialLang.indexOf('en') === 0 ? 'en' : 'chs';
  try { document.documentElement.lang = LOCALE === 'chs' ? 'zh-CN' : 'en'; } catch (e) { /* ignore */ }
  var CATEGORY = {
    syntax: ['语法', 'Syntax'],
    semantics: ['语义', 'Semantics'],
    types: ['类型', 'Types'],
    stdlib: ['标准库', 'Standard library'],
    io: ['输入输出', 'I/O'],
    algorithm: ['算法', 'Algorithms'],
    data: ['数据结构', 'Data structures'],
    memory: ['内存与资源', 'Memory & resources'],
    oop: ['面向对象', 'OOP'],
    templates: ['模板与泛型', 'Templates'],
    engineering: ['工程综合', 'Engineering'],
    debugging: ['调试排错', 'Debugging'],
  };
  var LANGS = [['cpp', 'C++'], ['c', 'C'], ['python', 'Python']];

  var app = document.getElementById('app');
  var state = {
    lang: 'cpp',
    cats: {},
    ranks: {},
    q: '',
    composing: false,
    picked: {},
    items: [],
    papers: [],
    specs: {},
    progress: {},
    page: 1,
  };

  // ── helpers ────────────────────────────────────────────────────────────────
  function t(chs, en) { return LOCALE === 'chs' ? chs : en; }
  function esc(s) {
    return String(s == null ? '' : s).replace(/[&<>"']/g, function (c) {
      return { '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' }[c];
    });
  }
  function h(tag, attrs, kids) {
    var el = document.createElement(tag);
    if (attrs) Object.keys(attrs).forEach(function (k) {
      if (k === 'class') el.className = attrs[k];
      else if (k === 'html') el.innerHTML = attrs[k];
      else if (k === 'text') el.textContent = attrs[k];
      else if (k.slice(0, 2) === 'on') el.addEventListener(k.slice(2), attrs[k]);
      else if (attrs[k] == null || attrs[k] === false) { /* 跳过空属性 */ }
      else el.setAttribute(k, attrs[k] === true ? '' : attrs[k]);
    });
    (kids || []).forEach(function (k) { if (k) el.appendChild(k); });
    return el;
  }
  function fetchJSON(path) {
    return fetch(path, { cache: 'no-cache' }).then(function (r) { return r.ok ? r.json() : null; }).catch(function () { return null; });
  }
  function titleOf(item) { return item.title ? (item.title[LOCALE] || item.title.chs || item.title.en || '') : item.id; }
  function catLabel(c) { var v = CATEGORY[c]; return v ? (LOCALE === 'chs' ? v[0] : v[1]) : (c || ''); }
  function rankBox(rank) {
    return h('span', { class: 'es-chip es-rank', text: t('Rank ', 'Rank ') + rank });
  }
  function catBox(cat) { return h('span', { class: 'es-chip', text: catLabel(cat) }); }
  function langBox(lang) {
    return h('span', { class: 'es-chip es-lang-chip', text: (lang || 'cpp').toUpperCase() });
  }
  function dayIndex(n) {
    var now = new Date();
    var seed = now.getFullYear() * 10000 + (now.getMonth() + 1) * 100 + now.getDate();
    return n > 0 ? seed % n : 0;
  }

  // ── theme 与站点一致 ───────────────────────────────────────────────────────
  function applyTheme() {
    try {
      var saved = JSON.parse(localStorage.getItem('guide4pc_settings') || '{}');
      if (saved.theme) document.documentElement.setAttribute('data-theme', saved.theme);
    } catch (e) { /* 默认亮色 */ }
  }

  // ── 数据 ───────────────────────────────────────────────────────────────────
  // 做题进度与草稿存在 IndexedDB（g4pc/drafts，key = lang/id），这里只读状态。
  function loadProgress() {
    return new Promise(function (resolve) {
      try {
        var req = indexedDB.open('g4pc', 1);
        req.onupgradeneeded = function () {
          var db = req.result;
          if (!db.objectStoreNames.contains('drafts')) db.createObjectStore('drafts', { keyPath: 'key' });
        };
        req.onerror = function () { resolve({}); };
        req.onsuccess = function () {
          try {
            var r = req.result.transaction('drafts', 'readonly').objectStore('drafts').getAll();
            r.onsuccess = function () {
              var map = {};
              (r.result || []).forEach(function (rec) { if (rec && rec.key) map[rec.key] = rec.status || 'attempted'; });
              resolve(map);
            };
            r.onerror = function () { resolve({}); };
          } catch (e) { resolve({}); }
        };
      } catch (e) { resolve({}); }
    });
  }

  function loadData() {
    return Promise.all([
      fetchJSON(ROOT + 'exercise/index.json'),
      fetchJSON(ROOT + 'exercise/papers/index.json'),
    ]).then(function (res) {
      state.items = (res[0] && res[0].items) || [];
      state.papers = res[1] || [];
      try {
        var picked = JSON.parse(localStorage.getItem('g4pc_paper') || '[]');
        picked.forEach(function (id) { state.picked[id] = true; });
      } catch (e) { /* ignore */ }
      return loadProgress().then(function (p) { state.progress = p || {}; });
    });
  }

  function itemsFor(lang) {
    return state.items.filter(function (it) { return (it.dir || it.lang) === lang; });
  }

  function progressBox(it) {
    var st = state.progress[(it.lang || 'cpp') + '/' + it.id];
    if (!st) return null;
    var solved = st === 'solved';
    return h('span', {
      class: 'es-chip es-progress ' + (solved ? 'es-progress-solved' : 'es-progress-open'),
      text: solved ? t('已通过', 'Solved') : t('编辑中', 'In progress'),
    });
  }

  // ── 页面：列表 ─────────────────────────────────────────────────────────────
  function renderList() {
    var all = itemsFor(state.lang);
    var fsig = state.lang + '|' + state.q + '|' + JSON.stringify(state.cats) + '|' + JSON.stringify(state.ranks);
    if (state._fsig !== fsig) { state._fsig = fsig; state.page = 1; }
    var shown = all.filter(function (it) {
      if (state.q) {
        var hay = (titleOf(it) + ' ' + it.id + ' ' + catLabel(it.category)).toLowerCase();
        if (hay.indexOf(state.q.toLowerCase()) < 0) return false;
      }
      if (Object.keys(state.cats).length && !state.cats[it.category]) return false;
      if (Object.keys(state.ranks).length && !state.ranks[String(it.rank)]) return false;
      return true;
    });

    app.innerHTML = '';

    // 语言 tab
    var tabs = h('div', { class: 'es-tabs' });
    LANGS.forEach(function (pair) {
      var enabled = pair[0] === 'cpp' || itemsFor(pair[0]).length > 0;
      var b = h('button', {
        class: 'es-tab' + (pair[0] === state.lang ? ' is-active' : ''),
        text: pair[1],
        disabled: enabled ? null : 'disabled',
        onclick: function () { state.lang = pair[0]; renderList(); },
      });
      tabs.appendChild(b);
    });
    app.appendChild(tabs);

    if (state.lang !== 'cpp' && all.length === 0) {
      app.appendChild(h('div', { class: 'es-empty', text: t('这个语言的题目还在整理中，先看 C++ 吧。', 'Exercises for this language are still being put together. Try C++ for now.') }));
      return;
    }

    // 每日一题
    if (all.length) {
      var daily = all[dayIndex(all.length)];
      app.appendChild(h('section', { class: 'es-daily' }, [
        h('div', { class: 'es-daily-kicker', text: t('每日一题', 'Daily problem') }),
        h('a', { class: 'es-daily-title', href: '#/' + state.lang + '/' + daily.id, text: titleOf(daily) }),
        h('div', { class: 'es-meta' }, [langBox(daily.lang), catBox(daily.category), rankBox(daily.rank)]),
      ]));
    }

    // 筛选
    var cats = {};
    all.forEach(function (it) { if (it.category) cats[it.category] = true; });
    var catRow = h('div', { class: 'es-filters' }, [
      h('span', { class: 'es-filter-label', text: t('分类', 'Category') }),
    ]);
    Object.keys(cats).forEach(function (cat) {
      catRow.appendChild(h('button', {
        class: 'es-chip es-filter' + (state.cats[cat] ? ' is-on' : ''),
        text: catLabel(cat),
        onclick: function () { if (state.cats[cat]) delete state.cats[cat]; else state.cats[cat] = true; renderList(); },
      }));
    });
    app.appendChild(catRow);

    var rankRow = h('div', { class: 'es-filters' }, [h('span', { class: 'es-filter-label', text: t('难度', 'Rank') })]);
    [1, 2, 3, 4, 5].forEach(function (r) {
      rankRow.appendChild(h('button', {
        class: 'es-chip es-filter' + (state.ranks[String(r)] ? ' is-on' : ''),
        text: 'Rank ' + r,
        onclick: function () { if (state.ranks[String(r)]) delete state.ranks[String(r)]; else state.ranks[String(r)] = true; renderList(); },
      }));
    });
    app.appendChild(rankRow);

    var search = h('input', {
      class: 'es-search', type: 'search', placeholder: t('搜索题目、分类或 id', 'Search title, category or id'),
      value: state.q,
      oninput: function (e) {
        state.q = e.target.value;
        if (e.isComposing) return;
        var pos = e.target.selectionStart;
        renderList();
        var el = document.querySelector('.es-search');
        if (el) { el.focus(); el.setSelectionRange(pos, pos); }
      },
      oncompositionend: function (e) {
        state.q = e.target.value;
        renderList();
        var el = document.querySelector('.es-search');
        if (el) { el.focus(); el.setSelectionRange(el.value.length, el.value.length); }
      },
    });
    app.appendChild(h('div', { class: 'es-search-row' }, [search]));

    // 结果计数
    var totalCount = all.length;
    var countText = shown.length === totalCount
      ? t('共 ' + totalCount + ' 题', totalCount + ' exercises')
      : t('筛出 ' + shown.length + ' / ' + totalCount + ' 题', shown.length + ' of ' + totalCount + ' exercises');
    app.appendChild(h('div', { class: 'es-result-count', text: countText }));

    // 分页
    var PAGE_SIZE = 30;
    var totalPages = Math.max(1, Math.ceil(shown.length / PAGE_SIZE));
    if (!state.page || state.page < 1) state.page = 1;
    if (state.page > totalPages) state.page = totalPages;
    var pageItems = shown.slice((state.page - 1) * PAGE_SIZE, state.page * PAGE_SIZE);

    // 列表
    var list = h('div', { class: 'es-list' });
    pageItems.forEach(function (it) {
      var row = h('div', { class: 'es-item' });
      if (state.composing) {
        var cb = h('input', { type: 'checkbox', class: 'es-pick', 'aria-label': t('加入卷子', 'Add to paper') });
        cb.checked = !!state.picked[it.id];
        cb.addEventListener('change', function () {
          if (cb.checked) state.picked[it.id] = true; else delete state.picked[it.id];
          renderList();
        });
        row.appendChild(cb);
      }
      row.appendChild(h('a', { class: 'es-item-main', href: '#/' + state.lang + '/' + it.id }, [
        h('div', { class: 'es-item-title', text: titleOf(it) }),
        h('div', { class: 'es-meta' }, [langBox(it.lang), catBox(it.category), rankBox(it.rank),
          h('span', { class: 'es-chip es-muted', text: t(it.checks + ' 项测试', it.checks + ' checks') }),
          progressBox(it),
        ]),
      ]));
      list.appendChild(row);
    });
    if (!shown.length) list.appendChild(h('div', { class: 'es-empty', text: t('没有符合条件的题目。', 'No exercises match these filters.') }));
    app.appendChild(list);

    // 分页条
    if (totalPages > 1) {
      app.appendChild(h('div', { class: 'es-pager' }, [
        h('button', { class: 'es-btn', text: t('上一页', 'Prev'), disabled: state.page <= 1 ? 'disabled' : null,
          onclick: function () { state.page -= 1; renderList(); window.scrollTo(0, 0); } }),
        h('span', { class: 'es-count', text: t('第 ' + state.page + ' / ' + totalPages + ' 页', 'Page ' + state.page + ' of ' + totalPages) }),
        h('button', { class: 'es-btn', text: t('下一页', 'Next'), disabled: state.page >= totalPages ? 'disabled' : null,
          onclick: function () { state.page += 1; renderList(); window.scrollTo(0, 0); } }),
      ]));
    }

    // 组卷
    var pickedCount = Object.keys(state.picked).length;
    var bar = h('div', { class: 'es-compose' }, [
      h('button', {
        class: 'es-btn' + (state.composing ? ' is-on' : ''),
        text: state.composing ? t('退出组卷', 'Stop composing') : t('组卷', 'Compose a paper'),
        onclick: function () { state.composing = !state.composing; renderList(); },
      }),
      state.composing && h('span', { class: 'es-count', text: t('已选 ' + pickedCount + ' 题', pickedCount + ' selected') }),
      state.composing && h('button', {
        class: 'es-btn es-btn-primary', text: t('保存卷子', 'Save paper'),
        onclick: function () { localStorage.setItem('g4pc_paper', JSON.stringify(Object.keys(state.picked))); renderList(); },
      }),
      state.composing && h('button', {
        class: 'es-btn', text: t('清空', 'Clear'),
        onclick: function () { state.picked = {}; localStorage.removeItem('g4pc_paper'); renderList(); },
      }),
    ]);
    app.appendChild(bar);

    // 我的卷子 / 预组卷子
    var myIds = Object.keys(state.picked);
    if (myIds.length) {
      var mine = h('section', { class: 'es-paper' }, [h('h3', { text: t('我的卷子', 'My paper') })]);
      var ordered = state.items.filter(function (it) { return state.picked[it.id]; });
      mine.appendChild(paperList(ordered));
      app.appendChild(mine);
    }
    var langPapers = state.papers.filter(function (p) { return paperLang(p) === state.lang; });
    if (state.papers.length && !langPapers.length) {
      app.appendChild(h('section', { class: 'es-paper' }, [
        h('h3', { text: t('预组卷子', 'Ready-made papers') }),
        h('p', { class: 'es-empty', text: t('这个语言还没有预组卷子。', 'No ready-made papers for this language yet.') }),
      ]));
    }
    if (langPapers.length) {
      var preset = h('section', { class: 'es-paper' }, [h('h3', { text: t('预组卷子', 'Ready-made papers') })]);
      langPapers.forEach(function (p) {
        preset.appendChild(h('a', { class: 'es-paper-row', href: '#/paper/' + p.id }, [
          h('span', { class: 'es-paper-title', text: paperTitle(p) }),
          h('span', { class: 'es-paper-count', text: t(p.count + ' 题', p.count + ' items') }),
        ]));
      });
      app.appendChild(preset);
    }
  }

  function paperLang(paper) {
    if (paper && paper.lang) return paper.lang;
    var ids = (paper && paper.items) || [];
    for (var i = 0; i < ids.length; i++) {
      for (var j = 0; j < state.items.length; j++) {
        if (state.items[j].id === ids[i]) return state.items[j].lang || 'cpp';
      }
    }
    return 'cpp';
  }

  function paperTitle(paper) {
    var title = paper.title || {};
    return title[LOCALE] || title.chs || title.en || paper.id;
  }

  function renderPaper(paperId) {
    app.innerHTML = '';
    app.appendChild(h('a', { class: 'es-back', href: '#/', text: t('← 返回题目列表', '← Back to exercises') }));
    var paper = state.papers.filter(function (p) { return p.id === paperId; })[0];
    if (!paper) {
      app.appendChild(h('div', { class: 'es-empty', text: t('没有找到这份卷子。', 'Paper not found.') }));
      return;
    }
    state.lang = paperLang(paper);
    var ids = paper.items || [];
    app.appendChild(h('header', { class: 'es-problem-head' }, [
      h('h1', { class: 'es-problem-title', text: paperTitle(paper) }),
      h('div', { class: 'es-meta' }, [
        langBox(paperLang(paper)),
        h('span', { class: 'es-chip', text: t(ids.length + ' 题', ids.length + ' items') }),
      ]),
    ]));
    var list = h('ol', { class: 'es-paper-items es-paper-detail' });
    var missing = [];
    var first = null;
    ids.forEach(function (id) {
      var it = state.items.filter(function (x) { return x.id === id; })[0];
      if (!it) { missing.push(id); return; }
      if (!first) first = it;
      list.appendChild(h('li', {}, [
        h('a', { href: itemHref(it.dir || 'cpp', it.id, paperId), text: titleOf(it) }),
        h('span', { class: 'es-meta' }, [catBox(it.category), rankBox(it.rank)]),
      ]));
    });
    app.appendChild(list);
    if (missing.length) {
      app.appendChild(h('p', { class: 'es-empty', text: t('有 ' + missing.length + ' 道题暂不可用：', missing.length + ' missing: ') + missing.join(', ') }));
    }
    if (first) {
      app.appendChild(h('a', { class: 'es-btn es-btn-primary es-btn-lg', href: itemHref(first.dir || 'cpp', first.id, paperId), text: t('开始练习', 'Start') }));
    }
  }

  function paperList(items) {
    var wrap = h('ol', { class: 'es-paper-items' });
    items.forEach(function (it) {
      wrap.appendChild(h('li', {}, [h('a', { href: '#/' + (it.dir || 'cpp') + '/' + it.id, text: titleOf(it) })]));
    });
    return wrap;
  }

  // ── 页面：题目 ─────────────────────────────────────────────────────────────
  function renderDetail(lang, id, paperId) {
    app.innerHTML = '';
    var item = state.items.filter(function (it) { return it.id === id && (it.dir || it.lang) === lang; })[0];
    if (!item) {
      app.appendChild(h('div', { class: 'es-empty', text: t('没有找到这道题。', 'Exercise not found.') }));
      return;
    }
    var paper = paperId ? state.papers.filter(function (p) { return p.id === paperId; })[0] : null;
    var paperIds = paper ? (paper.items || []).filter(function (pid) {
      return state.items.some(function (x) { return x.id === pid; });
    }) : null;
    var paperPos = paperIds ? paperIds.indexOf(id) : -1;
    app.appendChild(h('a', { class: 'es-back', href: paper ? '#/paper/' + paper.id : '#/', text: paper ? t('← 返回卷子', '← Back to paper') : t('← 返回题目列表', '← Back to exercises') }));
    app.appendChild(h('header', { class: 'es-problem-head' }, [
      h('h1', { class: 'es-problem-title', text: titleOf(item) }),
      h('div', { class: 'es-meta' }, [
        paper ? h('span', { class: 'es-chip es-paper-ctx', text: paperTitle(paper) + ' \u00b7 ' + (paperPos + 1) + '/' + paperIds.length }) : null,
        langBox(item.lang), catBox(item.category), rankBox(item.rank),
      ].filter(Boolean)),
    ]));

    // 上一题 / 下一题：卷内按卷子顺序，否则按同分类的 rank 顺序
    var navIds = paperIds || state.items.filter(function (x) {
      return (x.dir || x.lang) === lang && x.category === item.category;
    }).sort(function (a, b) {
      return (a.rank || 0) - (b.rank || 0) || (a.id < b.id ? -1 : 1);
    }).map(function (x) { return x.id; });
    var navPos = navIds.indexOf(id);
    var prevId = navPos > 0 ? navIds[navPos - 1] : null;
    var nextId = navPos >= 0 && navPos < navIds.length - 1 ? navIds[navPos + 1] : null;
    app.appendChild(h('div', { class: 'es-item-nav' }, [
      prevId ? h('a', { class: 'es-btn', href: itemHref(lang, prevId, paper ? paper.id : null), text: t('← 上一题', '← Prev') }) : h('span', { class: 'es-btn is-disabled', text: t('← 上一题', '← Prev') }),
      h('span', { class: 'es-item-pos', text: navIds.length > 1 ? t('第 ' + (navPos + 1) + ' / ' + navIds.length + ' 题', 'Problem ' + (navPos + 1) + ' of ' + navIds.length) : '' }),
      nextId ? h('a', { class: 'es-btn es-btn-next', href: itemHref(lang, nextId, paper ? paper.id : null), text: t('下一题 →', 'Next →') }) : h('span', { class: 'es-btn is-disabled', text: t('下一题 →', 'Next →') }),
    ]));

    var prose = h('div', { class: 'es-prose' }, [h('p', { class: 'es-empty', text: t('正在载入题面…', 'Loading problem…') })]);
    app.appendChild(prose);

    var holder = h('div', { class: 'es-mount-holder' });
    var variantRow = h('div', { class: 'es-variants' });
    app.appendChild(variantRow);
    app.appendChild(holder);

    var mdFile = ROOT + 'exercise/' + lang + '/' + id + '/problem.' + LOCALE + '.md';
    fetch(mdFile, { cache: 'no-cache' })
      .then(function (r) { if (!r.ok) throw new Error(r.status + ' ' + mdFile); return r.text(); })
      .then(function (md) {
        return ensureMarkdownIt().then(function () {
          prose.innerHTML = window.markdownit
            ? window.markdownit({ html: false, linkify: true, typographer: false }).render(md)
            : '<pre>' + esc(md) + '</pre>';
        });
      })
      .catch(function () { prose.innerHTML = '<p class="es-empty">' + esc(t('题面载入失败。', 'Failed to load the problem statement.')) + '</p>'; });

    ensurePlayground().then(function () {
      function show(variant) {
        holder.innerHTML = '';
        var mount = h('div', { class: 'ex-mount', 'data-ex': id });
        if (variant) mount.setAttribute('data-variant', variant);
        holder.appendChild(mount);
        if (window.G4PCPlayground) window.G4PCPlayground.mountAll();
        Array.prototype.forEach.call(variantRow.children, function (b) {
          b.classList.toggle('is-on', (b.getAttribute('data-v') || '') === (variant || ''));
        });
      }
      show('');

      fetch(ROOT + 'ex/' + id + '/spec.json', { cache: 'no-cache' }).then(function (r) { return r.ok ? r.json() : null; }).then(function (spec) {
        var variants = (spec && spec.variants) || [];
        if (!variants.length) return;
        variantRow.appendChild(h('span', { class: 'es-filter-label', text: t('变体', 'Variant') }));
        variantRow.appendChild(h('button', { class: 'es-chip es-filter is-on', 'data-v': '', text: t('标准', 'Standard'), onclick: function () { show(''); } }));
        variants.forEach(function (v) {
          variantRow.appendChild(h('button', {
            class: 'es-chip es-filter', 'data-v': v.id,
            text: v.label ? (v.label[LOCALE] || v.label.chs || v.id) : v.id,
            onclick: function () { show(v.id); },
          }));
        });
      });
    }).catch(function (err) {
      holder.appendChild(h('div', { class: 'es-error', text: t('播放器加载失败：', 'Playground failed to load: ') + (err && err.message ? err.message : err) }));
    });
  }

  // ── 播放器与 markdown-it 按需加载 ──────────────────────────────────────────
  var scriptCache = {};
  function loadScript(src) {
    if (scriptCache[src]) return scriptCache[src];
    scriptCache[src] = new Promise(function (resolve, reject) {
      var s = document.createElement('script');
      s.src = src; s.async = true;
      s.onload = function () { resolve(); };
      s.onerror = function () { reject(new Error('script ' + src)); };
      document.head.appendChild(s);
    });
    return scriptCache[src];
  }
  function loadCSS(href) {
    if (document.querySelector('link[href="' + href + '"]')) return;
    var l = document.createElement('link');
    l.rel = 'stylesheet'; l.href = href;
    document.head.appendChild(l);
  }
  function ensureMarkdownIt() {
    if (window.markdownit) return Promise.resolve();
    return loadScript('https://cdn.jsdelivr.net/npm/markdown-it@14.0.0/dist/markdown-it.min.js').catch(function () { /* 离线时退回 <pre> */ });
  }
  function playerVer() {
    var v = window.GUIDE4PC_VERSION;
    return v && v !== '1370990e2b' ? '?v=' + encodeURIComponent(v) : '';
  }
  function ensurePlayground() {
    window.G4PC_BASE = ROOT;
    loadCSS(ROOT + 'pg/pg.css' + playerVer());
    if (window.G4PCPlayground) return Promise.resolve();
    return loadScript(ROOT + 'pg/pg.js' + playerVer()).then(function () {
      if (!window.G4PCPlayground) throw new Error('G4PCPlayground missing');
    });
  }

  // ── 路由 ───────────────────────────────────────────────────────────────────
  function itemHref(lang, id, paperId) {
    return '#/' + lang + '/' + id + (paperId ? '?paper=' + encodeURIComponent(paperId) : '');
  }

  function route() {
    var raw = (location.hash || '').replace(/^#\/?/, '');
    var qi = raw.indexOf('?');
    var query = '';
    if (qi >= 0) { query = raw.slice(qi + 1); raw = raw.slice(0, qi); }
    var parts = raw.split('/').filter(Boolean);
    var paperId = new URLSearchParams(query).get('paper');
    if (parts[0] === 'paper' && parts[1]) renderPaper(parts[1]);
    else if (parts.length >= 2) renderDetail(parts[0], parts.slice(1).join('/'), paperId);
    else { if (!state.lang) state.lang = 'cpp'; renderList(); }
    window.scrollTo(0, 0);
  }

  // ── 启动 ───────────────────────────────────────────────────────────────────
  function applyChrome() {
    document.title = LOCALE === 'chs' ? '练习站 · Guide4PC' : 'Exercises · Guide4PC';
    var title = document.getElementById('es-title');
    if (title) title.textContent = t('练习站', 'Exercises');
    var home = document.getElementById('es-home');
    if (home) home.textContent = t('返回主站', 'Back to site');
    var btn = document.getElementById('es-lang');
    if (btn) {
      btn.textContent = LOCALE === 'chs' ? 'EN' : '中文';
      btn.setAttribute('aria-label', LOCALE === 'chs' ? 'Switch to English' : '切换到中文');
      btn.addEventListener('click', function () {
        var next = LOCALE === 'chs' ? 'en' : 'chs';
        try { localStorage.setItem('guide4pc_lang', next); } catch (e) { /* ignore */ }
        location.reload();
      });
    }
  }
  applyChrome();
  applyTheme();
  loadData().then(function () {
    if (!window.markdownit) {
      loadScript('https://cdn.jsdelivr.net/npm/markdown-it@14.0.0/dist/markdown-it.min.js').catch(function () {});
    }
    route();
  });
  window.addEventListener('hashchange', route);
})();
