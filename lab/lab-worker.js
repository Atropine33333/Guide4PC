'use strict';

// 独立 Playground 的编译/运行 worker：只编译并运行当前程序，不做判题。
var ASSET_BASE = 'https://cdn.jsdelivr.net/npm/@live-codes/clang-wasm@0.3.0/assets/';

self.importScripts('../docs/pg/vendor/clang-wasm.global.js');

var compilerPromise = null;

function ensureCompiler() {
  if (!compilerPromise) {
    compilerPromise = Promise.resolve(self.clangWasm.createCompiler('cpp', {
      baseUrl: ASSET_BASE,
      std: null,
      compileArgs: ['-O1', '-Wall'],
      onProgress: function (value) {
        self.postMessage({ type: 'progress', value: value });
      },
    }));
  }
  return compilerPromise;
}

self.onmessage = function (ev) {
  var msg = ev.data || {};
  if (msg.type !== 'run') return;

  (async function () {
    try {
      self.postMessage({ type: 'phase', phase: 'install' });
      var compiler = await ensureCompiler();

      self.postMessage({ type: 'phase', phase: 'compile' });
      var result = await compiler.run(msg.code || '', msg.stdin || '');

      var extra = '';
      if (result && Array.isArray(result.errors) && result.errors.length) {
        extra = result.errors.join('\n');
      }

      self.postMessage({
        type: 'done',
        result: {
          stdout: (result && result.stdout) || '',
          stderr: (result && result.stderr) || extra,
          exitCode: result && result.exitCode,
          compileMs: result && result.compileMs,
          runMs: result && result.runMs,
        },
      });
    } catch (err) {
      self.postMessage({ type: 'error', message: String((err && err.message) || err) });
    }
  })();
};
