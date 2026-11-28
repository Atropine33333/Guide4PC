(function () {
  'use strict';

  var STORAGE_KEY = 'g4pc_lab_code';
  var STARTER = [
    '#include <iostream>',
    '#include <vector>',
    '#include <algorithm>',
    '',
    'int main() {',
    '    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};',
    '    std::sort(v.begin(), v.end());',
    '',
    '    for (int x : v) std::cout << x << \' \';',
    '    std::cout << \'\\n\';',
    '}',
    '',
  ].join('\n');

  var $ = function (id) { return document.getElementById(id); };
  var editorHost = $('editor');
  var btnRun = $('btn-run');
  var btnReset = $('btn-reset');
  var btnClear = $('btn-clear');
  var stdinBox = $('stdin');
  var outBox = $('out');
  var errBox = $('err');
  var metaBox = $('meta');
  var statusBox = $('status');

  var view = null;
  var worker = null;
  var busy = false;
  var lastRun = null;

  function setStatus(text, tone) {
    statusBox.textContent = text;
    statusBox.setAttribute('data-tone', tone || 'idle');
  }

  function getCode() {
    return view ? view.state.doc.toString() : '';
  }

  function setCode(code) {
    if (!view) return;
    view.dispatch({ changes: { from: 0, to: view.state.doc.length, insert: code } });
  }

  function loadDraft() {
    try {
      var saved = localStorage.getItem(STORAGE_KEY);
      if (saved && saved.length) return saved;
    } catch (e) { /* 忽略存储失败 */ }
    return STARTER;
  }

  function saveDraft() {
    try { localStorage.setItem(STORAGE_KEY, getCode()); } catch (e) { /* 忽略 */ }
  }

  function clearOutput() {
    outBox.textContent = '（还没有运行）';
    errBox.hidden = true;
    errBox.textContent = '';
    metaBox.textContent = '';
  }

  function setError(text) {
    errBox.hidden = false;
    errBox.textContent = text;
  }

  function ensureEditor() {
    if (!window.G4PCEditor || typeof window.G4PCEditor.createEditor !== 'function') {
      setStatus('编辑器加载失败', 'bad');
      setError('找不到 G4PCEditor.createEditor，请确认 docs/pg/vendor/editor.bundle.js 存在。');
      return false;
    }
    view = window.G4PCEditor.createEditor({ parent: editorHost, doc: loadDraft() });
    var timer = null;
    view.dom.addEventListener('keydown', function (e) {
      if (e.key === 'Enter' && (e.ctrlKey || e.metaKey)) {
        e.preventDefault();
        run();
      }
    });
    return true;
  }

  function ensureWorker() {
    if (worker) return worker;
    worker = new Worker('lab-worker.js');
    worker.onmessage = function (ev) {
      var msg = ev.data || {};
      if (msg.type === 'progress') {
        var pct = Math.round((msg.value || 0) * 100);
        if (pct > 0) setStatus('正在下载编译器 ' + pct + '%', 'busy');
        else setStatus('正在下载编译器…（首次约 29MB）', 'busy');
        return;
      }
      if (msg.type === 'phase') {
        if (msg.phase === 'install') setStatus('正在解压并启动编译器…', 'busy');
        else if (msg.phase === 'compile') setStatus('编译中…', 'busy');
        else if (msg.phase === 'run') setStatus('运行中…', 'busy');
        return;
      }
      if (msg.type === 'done') {
        finish(true, msg.result);
        return;
      }
      if (msg.type === 'error') {
        finish(false, { message: msg.message || '未知错误' });
      }
    };
    worker.onerror = function (e) {
      finish(false, { message: e.message || 'worker 出错' });
      try { worker.terminate(); } catch (err) { /* 忽略 */ }
      worker = null;
    };
    return worker;
  }

  function finish(ok, result) {
    busy = false;
    btnRun.disabled = false;
    clearTimeout(lastRun && lastRun.timer);
    if (!ok) {
      setStatus('出错了', 'bad');
      outBox.textContent = '（没有输出）';
      setError(result.message || '未知错误');
      metaBox.textContent = '';
      return;
    }
    var stdout = result.stdout || '';
    outBox.textContent = stdout.length ? stdout.replace(/\s+$/, '') || '（只有空白输出）' : '（没有标准输出）';
    if (result.stderr && result.stderr.length) {
      setError(result.stderr.replace(/\s+$/, ''));
    } else {
      errBox.hidden = true;
      errBox.textContent = '';
    }
    var failed = (typeof result.exitCode === 'number' && result.exitCode !== 0) || /\berror:/.test(result.stderr || '');
    var meta = [];
    if (typeof result.exitCode === 'number') meta.push('exit ' + result.exitCode);
    if (typeof result.compileMs === 'number') meta.push('编译 ' + result.compileMs + ' ms');
    if (typeof result.runMs === 'number') meta.push('运行 ' + result.runMs + ' ms');
    metaBox.textContent = meta.join(' · ');
    setStatus(failed ? '编译失败' : '完成', failed ? 'bad' : 'ok');
  }

  function run() {
    if (busy) return;
    if (!view) return;
    saveDraft();
    busy = true;
    btnRun.disabled = true;
    errBox.hidden = true;
    errBox.textContent = '';
    setStatus('准备编译器…', 'busy');
    var w = ensureWorker();
    lastRun = {
      timer: setTimeout(function () {
        if (!busy) return;
        try { w.terminate(); } catch (e) { /* 忽略 */ }
        worker = null;
        finish(false, { message: '运行超时（30 秒）。检查是否有死循环，或减少输入数据。' });
      }, 30000),
    };
    w.postMessage({ type: 'run', code: getCode(), stdin: stdinBox.value || '' });
  }

  function boot() {
    if (!ensureEditor()) return;
    btnRun.addEventListener('click', run);
    btnReset.addEventListener('click', function () {
      setCode(STARTER);
      saveDraft();
      clearOutput();
      setStatus('已重置', 'idle');
    });
    btnClear.addEventListener('click', clearOutput);
    document.addEventListener('keydown', function (e) {
      if (e.key === 'Enter' && (e.ctrlKey || e.metaKey)) {
        e.preventDefault();
        run();
      }
    });
    setStatus('编译器待命', 'idle');
  }

  if (document.readyState === 'loading') document.addEventListener('DOMContentLoaded', boot);
  else boot();
})();
