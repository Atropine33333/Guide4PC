/*
 * Guide4PC 在线练习运行器 —— Web Worker
 *
 * 经典 worker（importScripts），不依赖打包：
 *   vendor/clang-wasm.global.js 是 @live-codes/clang-wasm 自带的自包含 IIFE，
 *   在这里以 self.clangWasm 暴露 createCompiler()。
 *
 * 协议（postMessage）：
 *   收: { type: 'run', payload: { spec, code, harnessCode, variant } }
 *         spec   = exercises/<id>/spec.json 的内容
 *         code   = 编辑器里的学生代码
 *         harnessCode = spec.harness 指向的 harness 源码（cases 模式为 null）
 *   发: { type: 'progress', value }            资源下载进度 0..1
 *       { type: 'phase', phase }              'loading' | 'compile' | 'run' | 'done'
 *       { type: 'check', id, ok, detail }     单个检查项的结果（harness 模式运行后一次性发）
 *       { type: 'done', result }              最终结果
 *       { type: 'error', message }            运行器自身异常（不是编译错误）
 */

/* global clangWasm */

importScripts('vendor/clang-wasm.global.js');

// 编译器资产（约 29MB）走 jsDelivr；本地化时改成 'pg/clang/' 或绝对地址即可。
const ASSET_BASE = 'https://cdn.jsdelivr.net/npm/@live-codes/clang-wasm@0.3.0/assets/';

let compilerPromise = null;

function send(type, extra) {
  self.postMessage(Object.assign({ type }, extra || {}));
}

function ensureCompiler() {
  if (!compilerPromise) {
    send('phase', { phase: 'loading' });
    compilerPromise = self.clangWasm.createCompiler('cpp', {
      baseUrl: ASSET_BASE,
      std: null,
      compileArgs: ['-O1', '-Wall'],
      onProgress: (value) => send('progress', { value }),
    });
  }
  return compilerPromise;
}

/** 把 spec 里的行数组/字符串统一成 stdin 文本。 */
function toInput(value) {
  if (value == null) return '';
  const lines = Array.isArray(value) ? value : String(value).split('\n');
  return lines.join('\n') + '\n';
}

/** 比较输出时忽略行尾空白和末尾空行，保留行首与行内空格。 */
function normalize(text) {
  return String(text == null ? '' : text)
    .replace(/\r\n?/g, '\n')
    .split('\n')
    .map((line) => line.replace(/[ \t]+$/, ''))
    .join('\n')
    .replace(/\n+$/, '');
}

function limitText(text, maxBytes) {
  const s = String(text == null ? '' : text);
  if (!maxBytes || s.length <= maxBytes) return { text: s, truncated: false };
  return { text: s.slice(0, maxBytes) + '\n…（输出太长，已截断）', truncated: true };
}

function parseHarnessOutput(stdout) {
  const seen = new Map();
  const programLines = [];
  let doneInfo = null;
  for (const line of String(stdout || '').split(/\r?\n/)) {
    const match = /^@@G4PC\s+(\{.*\})\s*$/.exec(line);
    if (!match) {
      programLines.push(line);
      continue;
    }
    let obj;
    try {
      obj = JSON.parse(match[1]);
    } catch (e) {
      programLines.push(line);
      continue;
    }
    if (obj.t === 'check' && obj.id) seen.set(String(obj.id), !!obj.ok);
    else if (obj.t === 'done') doneInfo = obj;
  }
  return { seen, doneInfo, program: programLines.join('\n') };
}

async function runHarness(compiler, spec, code, harnessCode, limits) {
  const source = code + '\n' + (harnessCode || '');
  send('phase', { phase: 'compile' });
  const r = await compiler.run(source, '', { compileArgs: ['-O1', '-Wall'] });
  if (r.errors && r.errors.length) {
    return {
      mode: 'harness',
      ok: false,
      checks: [],
      diagnostics: r.errors,
      compileMs: r.compileMs,
      runMs: r.runMs,
      exitCode: r.exitCode,
      stdout: '',
      stderr: '',
      truncated: false,
    };
  }
  send('phase', { phase: 'run' });
  const { seen, doneInfo, program } = parseHarnessOutput(r.stdout);
  const checks = (spec.checks || []).map((check) => {
    const ok = seen.has(check.id) ? seen.get(check.id) : false;
    return { id: check.id, ok, detail: null, reported: seen.has(check.id) };
  });
  const passed = checks.filter((c) => c.ok).length;
  const truncated = limitText(program, (limits && limits.outputBytes) || 65536);
  const ok = !!(doneInfo && doneInfo.total === checks.length && doneInfo.passed === checks.length) &&
    passed === checks.length;
  return {
    mode: 'harness',
    ok,
    checks,
    diagnostics: [],
    compileMs: r.compileMs,
    runMs: r.runMs,
    exitCode: r.exitCode,
    stdout: truncated.text,
    stderr: limitText(r.stderr, (limits && limits.outputBytes) || 65536).text,
    truncated: truncated.truncated,
  };
}

async function runCases(compiler, spec, code, limits) {
  const maxBytes = (limits && limits.outputBytes) || 65536;
  const checks = [];
  let compileMs = 0;
  let runMs = 0;
  let firstDiagnostics = null;
  let firstFail = null;

  const allChecks = spec.checks || [];
  for (let checkIndex = 0; checkIndex < allChecks.length; ++checkIndex) {
    const check = allChecks[checkIndex];
    const cases = check.cases && check.cases.length ? check.cases : [{}];
    let ok = true;
    let detail = null;
    for (let i = 0; i < cases.length; ++i) {
      const testCase = cases[i] || {};
      const input = toInput(testCase.in);
      send('phase', { phase: i === 0 && checks.length === 0 ? 'compile' : 'run' });
      const r = await compiler.run(code, input, { compileArgs: ['-O1', '-Wall'] });
      compileMs += r.compileMs || 0;
      runMs += r.runMs || 0;
      if (r.errors && r.errors.length) {
        ok = false;
        detail = { caseIndex: i, input, expected: null, actual: null, compileError: true };
        if (!firstDiagnostics) firstDiagnostics = r.errors;
        break;
      }
      const expected = normalize(toInput(testCase.out));
      const actual = normalize(r.stdout);
      if (actual !== expected) {
        ok = false;
        detail = {
          caseIndex: i,
          input,
          expected,
          actual: limitText(r.stdout, maxBytes).text,
          exitCode: r.exitCode,
        };
        if (!firstFail) firstFail = detail;
        break;
      }
    }
    checks.push({ id: check.id, ok, detail });
    send('check', { id: check.id, ok, detail });
    if (firstDiagnostics) {
      // 编译失败时后面的检查根本跑不到，仍按“未通过”补齐，清单不缩水。
      for (let rest = checkIndex + 1; rest < allChecks.length; ++rest) {
        const missing = { id: allChecks[rest].id, ok: false, detail: { compileError: true } };
        checks.push(missing);
        send('check', { id: missing.id, ok: false, detail: missing.detail });
      }
      break;
    }
  }

  const passed = checks.filter((c) => c.ok).length;
  return {
    mode: 'cases',
    ok: passed === checks.length && !firstDiagnostics,
    checks,
    diagnostics: firstDiagnostics || [],
    compileMs,
    runMs,
    exitCode: firstFail ? firstFail.exitCode : 0,
    stdout: '',
    stderr: '',
    truncated: false,
  };
}

self.onmessage = async (event) => {
  const msg = event.data || {};
  if (msg.type !== 'run') return;
  try {
    const payload = msg.payload || {};
    const spec = payload.spec || {};
    const limits = spec.limits || {};
    const compiler = await ensureCompiler();
    const result = spec.harness
      ? await runHarness(compiler, spec, payload.code || '', payload.harnessCode, limits)
      : await runCases(compiler, spec, payload.code || '', limits);
    send('phase', { phase: 'done' });
    send('done', { result });
  } catch (err) {
    send('error', { message: String((err && err.stack) || err) });
  }
};
