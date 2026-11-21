/*
 * Guide4PC 在线练习 —— 页面侧运行器
 *
 * 把 md 里 :::exercise 生成的 .ex-mount 占位卡升级成：编辑器 + 运行 + 检查清单。
 * 经典脚本，不打包；CodeMirror 6 由 pg/vendor/editor.bundle.js 提供（esbuild 产物）。
 *
 * 依赖（构建时从 playground/ 复制到 docs/pg/）：
 *   pg/vendor/editor.bundle.js
 *   pg/vendor/clang-wasm.global.js
 *   pg/pg-worker.js
 *   exterior: ex/<id>/{spec.json,starter*.cpp,harness.cpp}、diag/messages.json
 */
(function () {
  'use strict';

  var ZH = /^zh/i.test(document.documentElement.lang || '');
  var TEXT = {
    badge: ZH ? '在线练习' : 'Playground',
    run: ZH ? '运行' : 'Run',
    reset: ZH ? '重置' : 'Reset',
    running: ZH ? '运行中…' : 'Running…',
    loading: ZH ? '正在准备编译器…' : 'Preparing the compiler…',
    downloading: ZH ? '正在下载编译器' : 'Downloading compiler',
    connecting: ZH ? '正在连接编译器资源…' : 'Connecting to the compiler assets…',
    unpacking: ZH ? '正在解压并启动编译器…' : 'Unpacking and starting the compiler…',
    compiling: ZH ? '编译中…' : 'Compiling…',
    runTest: ZH ? '运行测试' : 'Running checks',
    ready: ZH ? '就绪' : 'Ready',
    preparing: ZH ? '准备中…' : 'Preparing…',
    warmError: ZH ? '编译器准备失败' : 'Compiler setup failed',
    tests: ZH ? '项测试' : 'tests',
    checks: ZH ? '检查' : 'Checks',
    passed: ZH ? '通过' : 'passed',
    compileFailed: ZH ? '编译失败' : 'Compilation failed',
    timeout: ZH ? '运行超时，已强制停止。检查一下死循环，或换个输入再试。' : 'Timed out — stopped. Check for infinite loops or try another input.',
    workerError: ZH ? '运行器出错了' : 'The runner crashed',
    noSpec: ZH ? '没有找到这道题的题面数据。' : 'Exercise data missing.',
    forbidden: ZH ? '这个变体不允许使用下面这些东西：' : 'This variant forbids:',
    stdin: ZH ? '输入' : 'Input',
    expected: ZH ? '期望输出' : 'Expected',
    actual: ZH ? '实际输出' : 'Actual',
    output: ZH ? '程序输出' : 'Program output',
    hint: ZH ? '小提示' : 'Hint',
    loadFail: ZH ? '题目数据加载失败' : 'Failed to load exercise data',
    retry: ZH ? '重试' : 'Retry',
  };

  var CATEGORY_LABELS = {
    syntax: ZH ? '语法' : 'Syntax',
    semantics: ZH ? '语义' : 'Semantics',
    types: ZH ? '类型' : 'Types',
    stdlib: ZH ? '标准库' : 'Stdlib',
    memory: ZH ? '内存与资源' : 'Memory',
    algorithm: ZH ? '算法' : 'Algorithms',
    engineering: ZH ? '工程综合' : 'Engineering',
    debugging: ZH ? '调试排错' : 'Debugging',
  };

  var worker = null;
  var activeRun = null;

  // 编译诊断（红波浪线）：单独的 worker，只编译不运行，卡住也不影响练习卡。
  var lintWorker = null;
  var lintSeq = 0;
  var lintPending = Object.create(null);
  var specs = new Map();
  var starters = new Map();
  var diagRules = null;
  var diagLoading = null;

  // 编译器预加载：页面一出现练习卡片就开始下载，进度广播给所有卡片。
  var warmup = {
    kicked: false,
    state: 'idle',
    value: 0,
    loaded: 0,
    total: 0,
    error: null,
    listeners: new Set(),
  };

  function formatMb(bytes) {
    return (Math.max(0, bytes || 0) / 1000000).toFixed(1);
  }

  function downloadText(loaded, total) {
    if (!total) return TEXT.downloading + '…（' + formatMb(loaded) + ' MB）';
    return TEXT.downloading + ' ' + formatMb(loaded) + ' / ' + formatMb(total) + ' MB';
  }

  var allViews = [];

  function broadcastWarmup(msg) {
    warmup.listeners.forEach(function (fn) {
      try { fn(msg); } catch (e) { /* 单个卡片出错不影响别的 */ }
    });
  }

  function url(path) {
    var href = new URL((window.G4PC_BASE || '') + path, document.baseURI).href;
    var ver = window.GUIDE4PC_VERSION;
    if (ver && ver !== 'GUIDE4PC_BUILD_VER') {
      href += (href.indexOf('?') < 0 ? '?' : '&') + 'v=' + encodeURIComponent(ver);
    }
    return href;
  }

  function h(tag, attrs, children) {
    var el = document.createElement(tag);
    if (attrs) {
      Object.keys(attrs).forEach(function (key) {
        var value = attrs[key];
        if (value == null) return;
        if (key === 'class') el.className = value;
        else if (key === 'text') el.textContent = value;
        else el.setAttribute(key, value);
      });
    }
    (children || []).forEach(function (child) {
      if (child == null) return;
      el.appendChild(typeof child === 'string' ? document.createTextNode(child) : child);
    });
    return el;
  }

  function pickLabel(label) {
    if (!label) return '';
    return (ZH ? label.chs : label.en) || label.chs || label.en || '';
  }

  function asLines(value) {
    if (value == null) return '';
    if (Array.isArray(value)) return value.join('\n');
    return String(value);
  }

  var scriptLoads = {};

  function loadScript(src, id) {
    if (id && scriptLoads[id]) return scriptLoads[id];
    var promise = new Promise(function (resolve, reject) {
      if (id && document.getElementById(id)) return resolve();
      var el = document.createElement('script');
      el.src = src;
      if (id) el.id = id;
      el.onload = function () { resolve(); };
      el.onerror = function () { reject(new Error('script failed: ' + src)); };
      document.head.appendChild(el);
    });
    if (id) scriptLoads[id] = promise;
    return promise;
  }

  function ensureEditorBundle() {
    if (window.G4PCEditor) return Promise.resolve();
    return loadScript(url('pg/vendor/editor.bundle.js'), 'g4pc-editor-bundle');
  }

  function loadDiagnostics() {
    if (diagRules) return Promise.resolve(diagRules);
    if (diagLoading) return diagLoading;
    diagLoading = fetch(url('diag/messages.json'), { cache: 'no-cache' })
      .then(function (res) { return res.ok ? res.json() : null; })
      .then(function (json) { diagRules = json && json.rules ? json.rules : []; return diagRules; })
      .catch(function () { diagRules = []; return diagRules; });
    return diagLoading;
  }

  function stripAnsi(text) {
    return String(text).replace(/\x1b\[[0-9;]*m/g, '');
  }

  /**
   * clang 的 errors 是逐行的：诊断首行、源码摘录、插入符、note、统计行。
   * 这里把首行折成一条诊断，其余收进 notes，避免把源码摘录也当成错误翻译。
   */
  function parseDiagnostics(lines) {
    var out = [];
    (lines || []).forEach(function (rawLine) {
      var line = stripAnsi(rawLine);
      var m = /^(.*?):(\d+):(\d+): (error|warning|note): (.*)$/.exec(line);
      if (m) {
        out.push({ file: m[1], line: +m[2], col: +m[3], severity: m[4], message: m[5], notes: [] });
        return;
      }
      var l = /^([\w.-]+): (error|warning): (.*)$/.exec(line);
      if (l) {
        out.push({ file: '', severity: l[2], message: l[3], notes: [] });
        return;
      }
      if (/^\d+ errors? generated\.$/.test(line)) return;
      if (/^Error: process exited with code \d+\.$/.test(line)) return;
      if (/^\s*\d* \|/.test(line) || /^\s*\|/.test(line)) return;
      if (/^\s*\^[~^ ]*$/.test(line)) return;
      if (out.length) out[out.length - 1].notes.push(line.trim());
    });
    return out;
  }

  function matchRule(message) {
    for (var i = 0; i < diagRules.length; ++i) {
      try {
        if (new RegExp(diagRules[i].match, 'i').test(message)) return diagRules[i];
      } catch (e) { /* 规则写错时跳过，不影响运行 */ }
    }
    return null;
  }

  function fetchText(path) {
    return fetch(url(path), { cache: 'no-cache' }).then(function (res) {
      if (!res.ok) throw new Error(res.status + ' ' + path);
      return res.text();
    });
  }

  function fetchJSON(path) {
    return fetchText(path).then(JSON.parse);
  }

  function loadSpec(id) {
    if (!specs.has(id)) {
      specs.set(id, fetchJSON('ex/' + id + '/spec.json').catch(function (err) {
        specs.delete(id);
        throw err;
      }));
    }
    return specs.get(id);
  }

  function loadStarter(id, fileName) {
    var key = id + '/' + fileName;
    if (!starters.has(key)) {
      starters.set(key, fetchText('ex/' + id + '/' + fileName).catch(function (err) {
        starters.delete(key);
        throw err;
      }));
    }
    return starters.get(key);
  }

  // ---------------------------------------------------------------- worker

  function ensureWorker() {
    if (worker) return worker;
    worker = new Worker(url('pg/pg-worker.js'));
    worker.onmessage = function (event) {
      var msg = event.data || {};
      if (msg.type === 'progress') {
        warmup.state = 'loading';
        warmup.value = msg.value || 0;
        broadcastWarmup(msg);
      } else if (msg.type === 'download-start') {
        warmup.state = 'loading';
        warmup.loaded = 0;
        warmup.total = msg.total || 0;
        broadcastWarmup(msg);
      } else if (msg.type === 'download') {
        warmup.state = 'loading';
        warmup.loaded = msg.loaded || 0;
        warmup.total = msg.total || 0;
        broadcastWarmup(msg);
      } else if (msg.type === 'download-error') {
        warmup.lastDownloadError = msg.message || '';
        broadcastWarmup(msg);
      } else if (msg.type === 'phase' && msg.phase === 'install') {
        warmup.state = 'loading';
        warmup.value = 1;
        broadcastWarmup(msg);
      } else if (msg.type === 'ready') {
        warmup.state = 'ready';
        broadcastWarmup(msg);
      } else if (msg.type === 'warmup-error') {
        warmup.state = 'error';
        warmup.error = msg.message || 'compiler setup failed';
        broadcastWarmup(msg);
      }
      if (!activeRun || !activeRun.onMessage) return;
      activeRun.onMessage(msg);
    };
    worker.onerror = function (event) {
      if (!activeRun || !activeRun.onError) return;
      activeRun.onError((event && event.message) || 'worker error');
    };
    return worker;
  }

  function killWorker() {
    if (worker) {
      worker.terminate();
      worker = null;
    }
    if (warmup.state !== 'ready') {
      warmup.kicked = false;
      warmup.state = 'idle';
    }
  }

  // 页面一出现练习就预先下载编译器；用户点“运行”时通常已经就绪。
  function kickWarmup() {
    if (warmup.kicked) return;
    warmup.kicked = true;
    warmup.state = 'loading';
    ensureWorker().postMessage({ type: 'warmup' });
  }

  function runInWorker(payload, handlers) {
    var w = ensureWorker();
    return new Promise(function (resolve, reject) {
      var settled = false;
      var timer = null;

      function finish(fn, value) {
        if (settled) return;
        settled = true;
        if (timer) clearTimeout(timer);
        activeRun = null;
        fn(value);
      }

      function armTimeout(ms) {
        if (timer) return;
        timer = setTimeout(function () {
          killWorker();
          finish(reject, new Error('timeout'));
        }, ms);
      }

      activeRun = {
        onMessage: function (msg) {
          if (msg.type === 'done') finish(resolve, msg.result);
          else if (msg.type === 'error') finish(reject, new Error(msg.message || 'worker error'));
          else if (handlers && handlers.onMessage) {
            if (msg.type === 'phase' && (msg.phase === 'compile' || msg.phase === 'run')) {
              armTimeout(payload.timeoutMs || 5000);
            }
            handlers.onMessage(msg);
          }
        },
        onError: function (message) {
          killWorker();
          finish(reject, new Error(message));
        },
      };

      w.postMessage({ type: 'run', payload: payload });
    });
  }

  // ------------------------------------------------------------------ DOM

  function ensureLintWorker() {
    if (lintWorker) return lintWorker;
    lintWorker = new Worker(url('pg/pg-lint-worker.js'));
    lintWorker.onmessage = function (evt) {
      var msg = evt.data || {};
      var done = lintPending[msg.id];
      if (!done) return;
      delete lintPending[msg.id];
      done(msg);
    };
    lintWorker.onerror = function () {
      // 诊断是锦上添花：起不来就放弃这一轮，不打断练习。
      lintWorker = null;
      var ids = Object.keys(lintPending);
      for (var i = 0; i < ids.length; ++i) {
        var done = lintPending[ids[i]];
        delete lintPending[ids[i]];
        done(null);
      }
    };
    return lintWorker;
  }

  function lintCode(code, timeoutMs) {
    return new Promise(function (resolve) {
      var w = ensureLintWorker();
      var id = ++lintSeq;
      var timer = setTimeout(function () {
        if (!lintPending[id]) return;
        delete lintPending[id];
        resolve(null);
      }, timeoutMs || 25000);
      lintPending[id] = function (msg) {
        clearTimeout(timer);
        resolve(msg);
      };
      try {
        w.postMessage({ type: 'lint', id: id, code: code });
      } catch (err) {
        clearTimeout(timer);
        delete lintPending[id];
        resolve(null);
      }
    });
  }

  function mountCard(mount, id, variant, spec, starter) {
    var variantSpec = null;
    (spec.variants || []).forEach(function (v) { if (v.id === variant) variantSpec = v; });
    var title = (variantSpec && pickLabel(variantSpec.label)) || pickLabel(spec.title) || id;
    var checks = spec.checks || [];

    var editorHost = h('div', { class: 'ex-editor' });
    var status = h('span', { class: 'ex-status', text: TEXT.preparing, 'data-tone': 'muted' });
    var runBtn = h('button', { class: 'ex-run', type: 'button', text: TEXT.run });
    var resetBtn = h('button', { class: 'ex-reset', type: 'button', text: TEXT.reset });
    var checkList = h('ol', { class: 'ex-checks' });
    var outputBox = h('pre', { class: 'ex-output', hidden: '' });
    var diagBox = h('div', { class: 'ex-diagnostics' });
    var resultLine = h('div', { class: 'ex-result', hidden: '' });
    var score = h('span', { class: 'ex-score', text: '—/' + checks.length });
    var progressFill = h('i', { class: 'ex-progress-fill' });
    var progressText = h('span', { class: 'ex-progress-text', text: '' });
    var progress = h('div', { class: 'ex-progress', hidden: '' }, [
      h('div', { class: 'ex-progress-track' }, [progressFill]),
      progressText,
    ]);

    var rowByCheck = {};
    checks.forEach(function (check) {
      var mark = h('span', { class: 'ex-mark', text: '' });
      var label = h('span', { class: 'ex-check-label', text: pickLabel(check.label) || check.id });
      var detail = h('div', { class: 'ex-check-detail', hidden: '' });
      var row = h('li', { class: 'ex-check', 'data-check': check.id }, [
        mark,
        h('span', { class: 'ex-check-main' }, [label, detail]),
      ]);
      rowByCheck[check.id] = { row: row, mark: mark, detail: detail };
      checkList.appendChild(row);
    });

    var card = h('section', { class: 'ex-card', 'data-ex': id }, [
      h('header', { class: 'ex-head' }, [
        h('div', { class: 'ex-head-main' }, [
          h('span', { class: 'ex-kicker', text: TEXT.badge }),
          h('h3', { class: 'ex-title', text: title }),
        ]),
        h('div', { class: 'ex-meta' }, [
          h('span', { class: 'ex-chip ex-chip-lang', text: (spec.lang || 'cpp').toUpperCase() }),
          spec.category ? h('span', { class: 'ex-chip ex-chip-cat', text: (CATEGORY_LABELS[spec.category] || spec.category) + (spec.rank ? ' · R' + spec.rank : '') }) : null,
          h('span', { class: 'ex-chip', text: checks.length + ' ' + TEXT.tests }),
          variantSpec ? h('span', { class: 'ex-chip ex-chip-var', text: variant }) : null,
        ]),
      ]),
      progress,
      h('div', { class: 'ex-toolbar' }, [
        runBtn,
        resetBtn,
        h('span', { class: 'ex-spacer' }),
        status,
      ]),
      h('div', { class: 'ex-editor-wrap' }, [
        h('div', { class: 'ex-file' }, [
          h('i', { class: 'ex-dot' }),
          h('span', { class: 'ex-file-name', text: 'main.cpp' }),
          spec.harness ? h('span', { class: 'ex-file-note', text: '+ ' + spec.harness }) : null,
        ]),
        editorHost,
      ]),
      h('div', { class: 'ex-panel' }, [
        h('div', { class: 'ex-panel-head' }, [
          h('span', { text: TEXT.checks }),
          score,
        ]),
        checkList,
        diagBox,
        resultLine,
        outputBox,
      ]),
    ]);

    mount.innerHTML = '';
    mount.appendChild(card);

    var view = null;
    var busy = false;
    var lintTimer = null;
    var lintToken = 0;
    var timeoutMs = (spec.limits && spec.limits.timeoutMs) || 5000;

    function setStatus(text, tone) {
      status.textContent = text;
      status.setAttribute('data-tone', tone || 'muted');
    }

    function setProgress(opts) {
      if (!opts) {
        progress.hidden = true;
        progress.removeAttribute('data-mode');
        return;
      }
      progress.hidden = false;
      setStatus('', 'muted');
      progress.setAttribute('data-mode', opts.mode || 'determinate');
      if (opts.mode === 'indeterminate') {
        progressFill.style.width = '100%';
        progressText.textContent = opts.text || '';
        return;
      }
      var pct = Math.max(0, Math.min(100, Math.round((opts.value || 0) * 100)));
      progressFill.style.width = pct + '%';
      progressText.textContent = (opts.text || '') + (opts.text ? ' ' : '') + pct + '%';
    }

    function showDownload(loaded, total) {
      var text = downloadText(loaded, total);
      if (total > 0) {
        var pct = Math.round((loaded / total) * 100);
        setProgress({ value: loaded / total, text: text });
      } else {
        setProgress({ mode: 'indeterminate', text: text });
      }
    }

    function onWarmup(msg) {
      if (msg.type === 'ready') {
        setProgress(null);
        if (!busy) setStatus(TEXT.ready, 'muted');
      } else if (msg.type === 'download-start') {
        if (busy) return;
        if (msg.total > 0) showDownload(0, msg.total);
        else {
          setProgress({ mode: 'indeterminate', text: TEXT.connecting });
        }
      } else if (msg.type === 'download') {
        if (busy) return;
        showDownload(msg.loaded || 0, msg.total || 0);
      } else if (msg.type === 'phase' && msg.phase === 'install') {
        if (busy) return;
        setProgress({ mode: 'indeterminate', text: TEXT.unpacking });
      } else if (msg.type === 'progress') {
        if (busy) return;
        showDownload(Math.round((msg.value || 0) * 28900 * 1024), 28900 * 1024);
      } else if (msg.type === 'warmup-error') {
        setProgress(null);
        if (!busy) setStatus(TEXT.warmError, 'fail');
      }
    }
    warmup.listeners.add(onWarmup);
    if (warmup.state === 'ready') onWarmup({ type: 'ready' });
    else if (warmup.state === 'error') onWarmup({ type: 'warmup-error' });
    else if (warmup.state === 'loading') {
      if (warmup.total > 0) showDownload(warmup.loaded, warmup.total);
      else {
        setProgress({ mode: 'indeterminate', text: TEXT.connecting });
      }
    }

    function setBusy(on) {
      busy = on;
      runBtn.disabled = on;
      resetBtn.disabled = on;
      card.setAttribute('data-busy', on ? '1' : '');
    }

    function resetChecks() {
      Object.keys(rowByCheck).forEach(function (id) {
        var row = rowByCheck[id];
        row.row.setAttribute('data-ok', '');
        row.mark.textContent = '';
        row.detail.hidden = true;
        row.detail.innerHTML = '';
      });
      diagBox.innerHTML = '';
      diagBox.hidden = true;
      outputBox.hidden = true;
      outputBox.textContent = '';
      resultLine.hidden = true;
      resultLine.textContent = '';
      score.textContent = '—/' + checks.length;
      card.removeAttribute('data-result');
    }

    function detailBlock(detail) {
      var box = h('div', { class: 'ex-detail' });
      function kv(key, value) {
        if (value == null || value === '') return;
        box.appendChild(h('div', { class: 'ex-detail-row' }, [
          h('span', { class: 'ex-detail-key', text: key }),
          h('pre', { class: 'ex-detail-value', text: String(value) }),
        ]));
      }
      if (detail.input) kv(TEXT.stdin, detail.input);
      if (detail.expected != null) kv(TEXT.expected, detail.expected);
      if (detail.actual != null) kv(TEXT.actual, detail.actual);
      if (detail.compileError) box.appendChild(h('div', { class: 'ex-detail-note', text: TEXT.compileFailed }));
      return box;
    }

    function renderChecks(result) {
      var passed = 0;
      (result.checks || []).forEach(function (check) {
        var row = rowByCheck[check.id];
        if (!row) return;
        row.row.setAttribute('data-ok', check.ok ? 'true' : 'false');
        row.mark.textContent = check.ok ? '✓' : '✗';
        if (check.ok) ++passed;
        if (!check.ok && check.detail) {
          row.detail.innerHTML = '';
          row.detail.appendChild(detailBlock(check.detail));
          row.detail.hidden = false;
        }
        if (!check.ok && check.reported === false) {
          row.detail.textContent = ZH ? '这一项没有报告结果（检查可能没跑到）。' : 'This check never reported (it may not have run).';
          row.detail.hidden = false;
        }
      });
      var total = (result.checks || []).length;
      score.textContent = passed + '/' + total;
      card.setAttribute('data-result', result.ok ? 'pass' : 'fail');
      if (result.diagnostics && result.diagnostics.length) {
        showDiagnostics(result.diagnostics);
      }
      if (result.stdout) {
        outputBox.hidden = false;
        outputBox.textContent = result.stdout;
      }
    }

    function showDiagnostics(lines) {
      var items = parseDiagnostics(lines);
      diagBox.innerHTML = '';
      if (!items.length) return;
      diagBox.hidden = false;
      items.forEach(function (item) {
        var rule = matchRule(item.message);
        var human = rule && rule.human ? rule.human : '';
        var hint = rule && rule.hint ? rule.hint : '';
        var box = h('div', { class: 'ex-diag', 'data-sev': item.severity });
        if (human) {
          box.appendChild(h('div', { class: 'ex-diag-human', text: human }));
        }
        if (hint) {
          box.appendChild(h('div', { class: 'ex-diag-hint', text: TEXT.hint + '：' + hint }));
        }
        var where = item.file ? item.file + ':' + item.line + ':' + item.col : '';
        box.appendChild(h('code', { class: 'ex-diag-raw', text: (where ? where + '  ' : '') + item.message }));
        if (item.notes && item.notes.length) {
          var details = document.createElement('details');
          details.className = 'ex-diag-notes';
          details.appendChild(h('summary', { text: ZH ? '细节' : 'Details' }));
          details.appendChild(h('pre', { text: item.notes.join('\n') }));
          box.appendChild(details);
        }
        diagBox.appendChild(box);
      });
    }

    function showError(message, detail) {
      diagBox.innerHTML = '';
      diagBox.hidden = false;
      diagBox.appendChild(h('div', { class: 'ex-diag' }, [
        h('div', { class: 'ex-diag-human', text: message }),
        detail ? h('code', { class: 'ex-diag-raw', text: String(detail) }) : null,
      ]));
    }

    function forbiddenHit(code) {
      var patterns = (variantSpec && variantSpec.forbidPatterns) || [];
      for (var i = 0; i < patterns.length; ++i) {
        try {
          if (new RegExp(patterns[i]).test(code)) return patterns[i];
        } catch (e) { /* 忽略坏规则 */ }
      }
      return null;
    }

    function clearLint() {
      if (view && window.G4PCEditor) window.G4PCEditor.setDiagnostics(view, []);
    }

    // 停止输入 900ms 后编译一次；只取 clang 的 error/warning，画成行内波浪线。
    function scheduleLint() {
      if (!view) return;
      if (lintTimer) clearTimeout(lintTimer);
      lintTimer = setTimeout(function () {
        if (!view) return;
        if (busy) {
          scheduleLint();
          return;
        }
        var code = view.state.doc.toString();
        if (!code.trim()) {
          clearLint();
          return;
        }
        var token = ++lintToken;
        lintCode(code).then(function (res) {
          if (!view || token !== lintToken || busy) return;
          if (!res || !res.diagnostics || !res.diagnostics.length) {
            clearLint();
            return;
          }
          var doc = view.state.doc;
          var diags = [];
          for (var i = 0; i < res.diagnostics.length; ++i) {
            var d = res.diagnostics[i];
            var lineNo = Math.min(Math.max(1, d.line | 0), doc.lines);
            var line = doc.line(lineNo);
            var from = Math.min(line.from + Math.max(0, (d.col | 0) - 1), line.to);
            var to = Math.min(from + Math.max(1, d.length | 0), line.to);
            if (to <= from) to = Math.min(from + 1, line.to);
            diags.push({
              from: from,
              to: to,
              severity: d.severity === 'warning' ? 'warning' : 'error',
              message: d.message,
              source: 'clang',
            });
          }
          window.G4PCEditor.setDiagnostics(view, diags);
        });
      }, 900);
    }

    async function doRun() {
      if (busy || !view) return;
      var code = view.state.doc.toString();
      if (!code.trim()) return;

      resetChecks();
      var hit = forbiddenHit(code);
      if (hit) {
        showError(TEXT.forbidden + ' ' + hit);
        return;
      }

      setBusy(true);
      setProgress({ mode: 'indeterminate', text: TEXT.loading });
      setStatus(TEXT.loading, 'muted');
      try {
        var harnessCode = null;
        if (spec.harness) {
          harnessCode = await fetchText('ex/' + id + '/' + spec.harness);
        }
        var result = await runInWorker({
          spec: spec,
          code: code,
          harnessCode: harnessCode,
          variant: variant,
          timeoutMs: timeoutMs,
        }, {
          onMessage: function (msg) {
            if (msg.type === 'download-start') {
              if (msg.total > 0) showDownload(0, msg.total);
              else {
                setProgress({ mode: 'indeterminate', text: TEXT.connecting });
              }
              return;
            }
            if (msg.type === 'download') {
              showDownload(msg.loaded || 0, msg.total || 0);
              return;
            }
            if (msg.type === 'download-error') return;
            if (msg.type === 'phase') {
              if (msg.phase === 'loading') {
                setProgress({ mode: 'indeterminate', text: TEXT.loading });
              } else if (msg.phase === 'install') {
                setProgress({ mode: 'indeterminate', text: TEXT.unpacking });
              } else if (msg.phase === 'compile') {
                setProgress({ mode: 'indeterminate', text: TEXT.compiling });
              } else if (msg.phase === 'run') {
                var label = TEXT.runTest + (msg.index ? ' ' + msg.index + '/' + (msg.total || checks.length) : '');
                setProgress({ mode: 'indeterminate', text: label });
              }
            } else if (msg.type === 'progress') {
              setProgress({ value: msg.value, text: TEXT.downloading });
            } else if (msg.type === 'check') {
              var row = rowByCheck[msg.id];
              if (row) {
                row.row.setAttribute('data-ok', msg.ok ? 'true' : 'false');
                row.mark.textContent = msg.ok ? '✓' : '✗';
              }
            }
          },
        });
        setProgress(null);
        renderChecks(result);
        var ms = (result.compileMs || 0) + (result.runMs || 0);
        setStatus(
          (result.ok ? '✓ ' + (ZH ? '全部通过' : 'all checks passed') : (ZH ? '未全部通过' : 'not all passed')) +
            (ms ? ' · ' + (ms / 1000).toFixed(1) + 's' : ''),
          result.ok ? 'ok' : 'fail'
        );
      } catch (err) {
        setProgress(null);
        if (String(err && err.message) === 'timeout') {
          showError(TEXT.timeout);
          setStatus(TEXT.timeout, 'fail');
        } else {
          showError(TEXT.workerError, err && err.message ? err.message : String(err));
          setStatus(TEXT.workerError, 'fail');
        }
      } finally {
        setBusy(false);
      }
    }

    function resetEditor() {
      if (!view) return;
      view.dispatch({ changes: { from: 0, to: view.state.doc.length, insert: starter } });
      resetChecks();
      clearLint();
      setStatus(warmup.state === 'ready' ? TEXT.ready : TEXT.preparing, 'muted');
      card.removeAttribute('data-result');
    }

    runBtn.addEventListener('click', doRun);
    resetBtn.addEventListener('click', resetEditor);

    ensureEditorBundle().then(function () {
      view = window.G4PCEditor.createEditor({
        parent: editorHost,
        doc: starter,
        onDocChange: scheduleLint,
      });
      allViews.push(view);
    }).catch(function (err) {
      showError(TEXT.workerError, err && err.message);
    });

    return {
      run: doRun,
      reset: resetEditor,
      setCode: function (code) {
        if (!view) return;
        view.dispatch({ changes: { from: 0, to: view.state.doc.length, insert: code } });
      },
    };
  }

  function initMount(mount) {
    var id = mount.getAttribute('data-ex');
    var variant = mount.getAttribute('data-variant') || '';
    if (!id) return;
    mount.setAttribute('data-state', 'loading');

    Promise.all([loadSpec(id), loadDiagnostics()]).then(function (values) {
      var spec = values[0];
      var starterName = 'starter.cpp';
      (spec.variants || []).forEach(function (v) {
        if (v.id === variant && v.starter) starterName = v.starter;
      });
      return loadStarter(id, starterName).then(function (starter) {
        mount.setAttribute('data-state', 'ready');
        var handle = mountCard(mount, id, variant, spec, starter);
        mount._g4pc = handle;
      });
    }).catch(function (err) {
      mount.setAttribute('data-state', 'error');
      mount.innerHTML = '';
      mount.appendChild(h('div', { class: 'ex-error' }, [
        h('div', { text: TEXT.loadFail }),
        h('code', { text: err && err.message ? err.message : String(err) }),
      ]));
    });
  }

  function mountAll() {
    var mounts = document.querySelectorAll('.ex-mount');
    if (mounts.length) kickWarmup();
    for (var i = 0; i < mounts.length; ++i) {
      if (!mounts[i]._g4pc && !mounts[i].__g4pcStarted) {
        mounts[i].__g4pcStarted = true;
        initMount(mounts[i]);
      }
    }
    return mounts.length;
  }

  function applyCodeFontSize(size) {
    for (var i = 0; i < allViews.length; ++i) {
      try {
        allViews[i].requestMeasure();
      } catch (e) { /* 视图可能已销毁 */ }
    }
    return size;
  }

  window.G4PCPlayground = { mountAll: mountAll, applyCodeFontSize: applyCodeFontSize };

  // 自测钩子：?g4pc-autorun=1 时依次运行页面上所有练习，结果写到 body[data-g4pc-autorun]。
  // 加 &g4pc-solution=1 时先从 exercises/<id>/solution.cpp 载入参考实现再运行。
  function autorun() {
    var mounts = Array.prototype.slice.call(document.querySelectorAll('.ex-mount'));
    var useSolution = /[?&]g4pc-solution=1/.test(location.search);
    var report = [];

    function waitReady(tries) {
      var nodes = document.querySelectorAll('.ex-mount');
      if (nodes.length && Array.prototype.every.call(nodes, function (m) { return m._g4pc; })) {
        return Promise.resolve();
      }
      if (tries <= 0) return Promise.resolve();
      return new Promise(function (resolve) {
        setTimeout(function () { waitReady(tries - 1).then(resolve); }, 200);
      });
    }

    async function step(index) {
      if (index >= mounts.length) {
        document.body.setAttribute('data-g4pc-autorun', JSON.stringify(report));
        return;
      }
      var mount = mounts[index];
      var handle = mount._g4pc;
      var id = mount.getAttribute('data-ex');
      if (!handle) return step(index + 1);
      handle.reset();
      if (useSolution) {
        try {
          var solution = await fetchText('ex/' + id + '/solution.cpp');
          handle.setCode(solution);
        } catch (e) { /* 没有参考实现就按 starter 跑 */ }
      }
      await handle.run();
      var card = mount.querySelector('.ex-card');
      report.push({
        id: id,
        ok: card && card.getAttribute('data-result') === 'pass',
        checks: card ? Array.prototype.map.call(card.querySelectorAll('.ex-check'), function (row) {
          return row.getAttribute('data-ok');
        }) : [],
        status: card && card.querySelector('.ex-result') ? card.querySelector('.ex-result').textContent : '',
      });
      step(index + 1);
    }

    waitReady(100).then(function () { return step(0); });
  }

  function boot() {
    mountAll();
    if (/[?&]g4pc-autorun=1/.test(location.search)) autorun();
  }

  if (document.readyState === 'loading') document.addEventListener('DOMContentLoaded', boot);
  else boot();
})();
