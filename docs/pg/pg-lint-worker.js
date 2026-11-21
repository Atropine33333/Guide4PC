/*
 * Guide4PC 在线练习 —— 编译诊断 worker
 *
 * 只编译、不链接运行：用 clang-wasm 的 toolchain 入口（clang-wasm-toolchain.global.js）
 * 调 runtime.compile()，把 clang 的原始输出解析成「行 / 列 / 严重级别 / 消息」，
 * 交给页面侧画红波浪线。
 *
 * 与 pg-worker.js 分开是故意的：
 *   · 编译诊断不需要 wasm 运行环境，也不该和「运行」抢同一个 worker 的锁；
 *   · 诊断 worker 卡住或崩掉时，练习卡本身的运行不受影响。
 */
'use strict';

importScripts('vendor/clang-wasm-toolchain.global.js');

var ASSET_BASE = 'https://cdn.jsdelivr.net/npm/@live-codes/clang-wasm@0.3.0/assets/';
var MAX_DIAGNOSTICS = 30;

var toolchain = null;
var acquiring = null;

function ensureToolchain() {
  if (toolchain) return Promise.resolve(toolchain);
  if (acquiring) return acquiring;
  acquiring = self.clangWasmToolchain
    .createToolchain({ baseUrl: ASSET_BASE })
    .then(function (tc) {
      toolchain = tc;
      return tc;
    })
    .catch(function (err) {
      acquiring = null;
      throw err;
    });
  return acquiring;
}

/* clang 的诊断行形如：main.cpp:3:12: error: expected ';' at end of declaration */
var DIAG_RE = /^(?:[A-Za-z]:)?[^:\n]*?main\.cpp:(\d+):(\d+):\s+(error|warning):\s+(.+)$/;

/* 波浪线落点：clang 的列号常落在空白上（比如缺分号时指向空格），
 * 这时标出前一个词，至少让人看得见位置。 */
function markAt(codeLines, line, col) {
  var text = codeLines[line - 1] || '';
  var start = Math.max(0, Math.min(col - 1, text.length));
  var wordRe = /[A-Za-z_][A-Za-z0-9_]*|\d+/;
  var rest = text.slice(start);

  // 光标正好在标识符/数字上
  var here = wordRe.exec(rest);
  if (here && here.index === 0) return { col: start + 1, len: here[0].length };

  // 落在空白上：优先前一个词
  var before = text.slice(0, start);
  var prev = null;
  var re = /[A-Za-z_][A-Za-z0-9_]*|\d+/g;
  var m;
  while ((m = re.exec(before))) prev = m;
  if (prev) return { col: prev.index + 1, len: prev[0].length };

  // 行首空白：用后一个词
  var next = wordRe.exec(rest);
  if (next) return { col: start + next.index + 1, len: next[0].length };
  return { col: start + 1, len: 1 };
}

function parseDiagnostics(lines, code) {
  var codeLines = String(code || '').split('\n');
  var out = [];
  for (var i = 0; i < lines.length; ++i) {
    var m = DIAG_RE.exec(lines[i]);
    if (!m) continue;
    var line = parseInt(m[1], 10);
    var mark = markAt(codeLines, line, parseInt(m[2], 10));
    out.push({
      line: line,
      col: mark.col,
      length: mark.len,
      severity: m[3],
      message: m[4],
    });
    if (out.length >= MAX_DIAGNOSTICS) break;
  }
  return out;
}

self.onmessage = function (evt) {
  var msg = evt.data || {};
  if (msg.type !== 'lint') return;

  var started = Date.now();
  ensureToolchain()
    .then(function (tc) {
      return tc
        .captureCompilerOutput(function () {
          return tc.runtime.compile({
            input: 'main.cpp',
            code: msg.code,
            obj: 'lint.o',
            language: 'CPP',
            compileArgs: ['-Wall', '-Wextra'],
          });
        })
        .then(function (out) {
          var raw = (out && out.raw) || '';
          var cleaned = self.clangWasmToolchain.compilerDiagnostics(raw);
          self.postMessage({
            type: 'lint-result',
            id: msg.id,
            diagnostics: parseDiagnostics(cleaned, msg.code),
            ms: Date.now() - started,
          });
        });
    })
    .catch(function (err) {
      self.postMessage({
        type: 'lint-result',
        id: msg.id,
        diagnostics: [],
        error: String((err && err.message) || err),
      });
    });
};
