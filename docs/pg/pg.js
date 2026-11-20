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
    compiling: ZH ? '编译中…' : 'Compiling…',
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

  var worker = null;
  var activeRun = null;
  var specs = new Map();
  var starters = new Map();
  var diagRules = null;
  var diagLoading = null;

  function url(path) {
    return new URL(path, document.baseURI).href;
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
      if (!activeRun || !activeRun.onMessage) return;
      activeRun.onMessage(event.data);
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

  function mountCard(mount, id, variant, spec, starter) {
    var variantSpec = null;
    (spec.variants || []).forEach(function (v) { if (v.id === variant) variantSpec = v; });
    var title = (variantSpec && pickLabel(variantSpec.label)) || pickLabel(spec.title) || id;
    var checks = spec.checks || [];

    var editorHost = h('div', { class: 'ex-editor' });
    var status = h('span', { class: 'ex-status', text: TEXT.badge });
    var runBtn = h('button', { class: 'ex-run', type: 'button', text: TEXT.run });
    var resetBtn = h('button', { class: 'ex-reset', type: 'button', text: TEXT.reset });
    var hint = h('span', { class: 'ex-kbd', text: 'Ctrl/⌘ + Enter' });
    var checkList = h('ol', { class: 'ex-checks' });
    var outputBox = h('pre', { class: 'ex-output', hidden: '' });
    var diagBox = h('div', { class: 'ex-diagnostics' });
    var resultLine = h('div', { class: 'ex-result', hidden: '' });

    var rowByCheck = {};
    checks.forEach(function (check) {
      var mark = h('span', { class: 'ex-mark', text: '·' });
      var label = h('span', { class: 'ex-check-label', text: pickLabel(check.label) || check.id });
      var detail = h('div', { class: 'ex-check-detail', hidden: '' });
      var row = h('li', { class: 'ex-check', 'data-check': check.id }, [
        mark, label, detail,
      ]);
      rowByCheck[check.id] = { row: row, mark: mark, detail: detail };
      checkList.appendChild(row);
    });

    var card = h('section', { class: 'ex-card', 'data-ex': id }, [
      h('header', { class: 'ex-head' }, [
        h('span', { class: 'ex-badge', text: TEXT.badge }),
        h('span', { class: 'ex-title', text: title }),
        h('span', { class: 'ex-lang', text: (spec.lang || 'cpp').toUpperCase() }),
      ]),
      h('div', { class: 'ex-toolbar' }, [runBtn, resetBtn, hint, status]),
      editorHost,
      h('div', { class: 'ex-panel' }, [checkList, diagBox, resultLine, outputBox]),
    ]);

    mount.innerHTML = '';
    mount.appendChild(card);

    var view = null;
    var busy = false;
    var timeoutMs = (spec.limits && spec.limits.timeoutMs) || 5000;

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
        row.mark.textContent = '·';
        row.detail.hidden = true;
        row.detail.innerHTML = '';
      });
      diagBox.innerHTML = '';
      diagBox.hidden = true;
      outputBox.hidden = true;
      outputBox.textContent = '';
      resultLine.hidden = true;
      resultLine.textContent = '';
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
          var parts = [];
          if (check.detail.input) parts.push(TEXT.stdin + '\n' + check.detail.input);
          if (check.detail.expected != null) parts.push(TEXT.expected + '\n' + check.detail.expected);
          if (check.detail.actual != null) parts.push(TEXT.actual + '\n' + check.detail.actual);
          if (check.detail.compileError) parts.push(TEXT.compileFailed);
          row.detail.textContent = parts.join('\n\n');
          row.detail.hidden = false;
        }
        if (!check.ok && check.reported === false) {
          row.detail.textContent = ZH ? '这一项没有报告结果（检查可能没跑到）。' : 'This check never reported (it may not have run).';
          row.detail.hidden = false;
        }
      });
      var total = (result.checks || []).length;
      resultLine.hidden = false;
      resultLine.textContent = result.ok
        ? '✓ ' + (ZH ? '全部通过 ' : 'All ') + passed + '/' + total
        : (ZH ? '通过 ' : 'Passed ') + passed + '/' + total;
      resultLine.setAttribute('data-ok', result.ok ? 'true' : 'false');
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

    async function doRun() {
      if (busy || !view) return;
      var code = view.state.doc.toString();
      if (!code.trim()) return;

      resetChecks();
      var hit = forbiddenHit(code);
      if (hit) {
        showError(TEXT.forbidden + ' `' + hit + '`');
        return;
      }

      setBusy(true);
      status.textContent = TEXT.loading;
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
            if (msg.type === 'phase') {
              if (msg.phase === 'loading') status.textContent = TEXT.loading;
              else if (msg.phase === 'compile') status.textContent = TEXT.compiling;
              else if (msg.phase === 'run') status.textContent = TEXT.running;
            } else if (msg.type === 'progress') {
              status.textContent = TEXT.loading + ' ' + Math.round((msg.value || 0) * 100) + '%';
            } else if (msg.type === 'check') {
              var row = rowByCheck[msg.id];
              if (row) {
                row.row.setAttribute('data-ok', msg.ok ? 'true' : 'false');
                row.mark.textContent = msg.ok ? '✓' : '✗';
              }
            }
          },
        });
        renderChecks(result);
        status.textContent = result.ok ? '✓' : '';
      } catch (err) {
        if (String(err && err.message) === 'timeout') {
          showError(TEXT.timeout);
          status.textContent = '';
        } else {
          showError(TEXT.workerError, err && err.message ? err.message : String(err));
          status.textContent = '';
        }
      } finally {
        setBusy(false);
      }
    }

    function resetEditor() {
      if (!view) return;
      view.dispatch({ changes: { from: 0, to: view.state.doc.length, insert: starter } });
      resetChecks();
      status.textContent = '';
      card.removeAttribute('data-result');
    }

    runBtn.addEventListener('click', doRun);
    resetBtn.addEventListener('click', resetEditor);

    ensureEditorBundle().then(function () {
      var G = window.G4PCEditor;
      var extensions = [G.basicSetup, G.cpp()];
      if (G.EditorView.domEventHandlers) {
        extensions.push(G.EditorView.domEventHandlers({
          keydown: function (event) {
            if ((event.ctrlKey || event.metaKey) && event.key === 'Enter') {
              event.preventDefault();
              doRun();
              return true;
            }
            return false;
          },
        }));
      }
      view = new G.EditorView({
        state: G.EditorState.create({ doc: starter, extensions: extensions }),
        parent: editorHost,
      });
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
    for (var i = 0; i < mounts.length; ++i) {
      if (!mounts[i]._g4pc && !mounts[i].__g4pcStarted) {
        mounts[i].__g4pcStarted = true;
        initMount(mounts[i]);
      }
    }
    return mounts.length;
  }

  window.G4PCPlayground = { mountAll: mountAll };

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
