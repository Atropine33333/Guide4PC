#!/usr/bin/env node
// 合并 grammar/parts/*.md 并逐个编译运行 cpp/text 示例。
// 用法：
//   node grammar/tools/build.mjs           # 合并 + 校验
//   node grammar/tools/build.mjs --merge   # 只合并
//   node grammar/tools/build.mjs --verify  # 只校验
import { readFileSync, writeFileSync, readdirSync, mkdtempSync, rmSync } from 'node:fs';
import { tmpdir } from 'node:os';
import path from 'node:path';
import { spawnSync } from 'node:child_process';

const ROOT = path.resolve(process.cwd());
const PARTS_DIR = path.join(ROOT, 'grammar', 'parts');
const OUT = path.join(ROOT, 'grammar', 'cpp-syntax-review.md');
const args = new Set(process.argv.slice(2));
const doMerge = !args.has('--verify');
const doVerify = !args.has('--merge');

function readParts() {
  const files = readdirSync(PARTS_DIR).filter((f) => f.endsWith('.md')).sort();
  if (!files.length) throw new Error('grammar/parts 下没有 md 分片');
  return files.map((f) => ({ name: f, text: readFileSync(path.join(PARTS_DIR, f), 'utf8').replace(/\r\n/g, '\n') }));
}

function merge(parts) {
  const heads = [];
  for (const p of parts) {
    for (const line of p.text.split('\n')) {
      const m = /^##\s+(\d+)\.\s+(.+?)\s*$/.exec(line);
      if (m) heads.push({ n: Number(m[1]), title: m[2] });
    }
  }
  heads.sort((a, b) => a.n - b.n);
  const toc = heads.map((h) => `${h.n}. ${h.title}`).join('\n');
  const head = [
    '# C++ 基础语法复习讲义',
    '',
    '> 面向期中/期末复习的单页提纲：规则 + 对比表 + 可直接运行、且输出已经逐字验证的示例。',
    '> 运行环境：C++20，`clang++ -std=c++20 -Wall -O0`；示例不读取 stdin，可以直接粘贴到练习站的 `/exercise/playground/` 运行。',
    '',
    '## 目录',
    '',
    toc,
    '',
    '---',
    '',
  ].join('\n');
  const body = parts.map((p) => p.text.trim() + '\n').join('\n');
  const doc = head + body;
  writeFileSync(OUT, doc, 'utf8');
  return { doc, heads };
}

function parseBlocks(md) {
  const lines = md.split('\n');
  const blocks = [];
  for (let i = 0; i < lines.length; i++) {
    const m = /^```cpp\s*$/.exec(lines[i]);
    if (!m) continue;
    const startLine = i + 1;
    const code = [];
    i++;
    while (i < lines.length && !/^```\s*$/.test(lines[i])) code.push(lines[i++]);
    if (i >= lines.length) throw new Error(`第 ${startLine} 行的 cpp 代码块没有闭合`);
    let j = i + 1;
    while (j < lines.length) {
      if (lines[j].trim() === '') { j++; continue; }
      if (/^\s*(输出|运行结果|程序输出|程序运行输出|Output)\s*[:：]?\s*$/i.test(lines[j])) { j++; continue; }
      break;
    }
    if (!/^```text\s*$/.test(lines[j] || '')) {
      throw new Error(`第 ${startLine} 行的 cpp 代码块后面没有紧跟 text 输出块`);
    }
    const expect = [];
    j++;
    while (j < lines.length && !/^```\s*$/.test(lines[j])) expect.push(lines[j++]);
    blocks.push({ startLine, code: code.join('\n'), expect: expect.join('\n') });
    i = j;
  }
  return blocks;
}

function norm(s) {
  return s.replace(/\r\n/g, '\n').replace(/[ \t]+$/gm, '').replace(/\n+$/, '');
}

function verify(blocks) {
  const dir = mkdtempSync(path.join(tmpdir(), 'g4pc-grammar-'));
  let passed = 0;
  const fails = [];
  try {
    blocks.forEach((b, idx) => {
      const src = path.join(dir, `ex${idx}.cpp`);
      const exe = path.join(dir, `ex${idx}.out`);
      writeFileSync(src, b.code, 'utf8');
      const cc = spawnSync('clang++', ['-std=c++20', '-Wall', '-O0', src, '-o', exe], { encoding: 'utf8' });
      const warnings = (cc.stderr || '').split('\n').filter((l) => /warning:/.test(l));
      if (cc.status !== 0) {
        fails.push({ idx, line: b.startLine, kind: 'compile', detail: (cc.stderr || '').split('\n').slice(0, 6).join('\n') });
        return;
      }
      if (warnings.length) {
        fails.push({ idx, line: b.startLine, kind: 'warning', detail: warnings.slice(0, 3).join('\n') });
        return;
      }
      const run = spawnSync(exe, [], { encoding: 'utf8', input: '', timeout: 10000 });
      if (run.error || run.status === null) {
        fails.push({ idx, line: b.startLine, kind: 'run', detail: String(run.error || 'timeout') });
        return;
      }
      const got = norm(run.stdout || '');
      const want = norm(b.expect);
      if (got !== want) {
        fails.push({
          idx,
          line: b.startLine,
          kind: 'output',
          detail: `--- expect ---\n${want}\n--- got ---\n${got}`,
        });
        return;
      }
      passed++;
    });
  } finally {
    rmSync(dir, { recursive: true, force: true });
  }
  return { passed, fails };
}

const parts = readParts();
let merged = null;
if (doMerge) {
  merged = merge(parts);
  console.log(`✓ 合并 ${parts.length} 个分片 → grammar/cpp-syntax-review.md（${merged.heads.length} 节）`);
}
if (doVerify) {
  const md = merged ? merged.doc : readFileSync(OUT, 'utf8');
  const blocks = parseBlocks(md);
  const { passed, fails } = verify(blocks);
  console.log(`· cpp 示例 ${blocks.length} 个，通过 ${passed} 个`);
  if (fails.length) {
    for (const f of fails.slice(0, 30)) {
      console.log(`\n✗ 第 ${f.line} 行（${f.kind}）：\n${f.detail}`);
    }
    if (fails.length > 30) console.log(`… 另有 ${fails.length - 30} 个失败`);
    process.exitCode = 1;
  } else {
    console.log('✓ 全部示例编译无警告、输出逐字匹配');
  }
}
