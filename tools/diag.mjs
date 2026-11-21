#!/usr/bin/env node
/**
 * Guide4PC diagnostic-rule tooling.
 *
 *   node tools/diag.mjs build          # merge rules/*.json → diagnostics/catalog.json
 *   node tools/diag.mjs check          # validate schema, tests, i18n and placeholders
 *   node tools/diag.mjs explain "..."  # print which rule matches and why
 *   node tools/diag.mjs list           # one line per rule
 *
 * Exit code: 0 ok, 1 validation errors.
 */
import { readFileSync, readdirSync, statSync, writeFileSync, existsSync } from 'node:fs';
import { createRequire } from 'node:module';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const require = createRequire(import.meta.url);
const engine = require('../playground/diag-engine.js');

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const RULES_DIR = path.join(ROOT, 'diagnostics', 'rules');
const CATALOG = path.join(ROOT, 'diagnostics', 'catalog.json');

const CATEGORIES = ['语法', '名字与作用域', '类型', '链接', '运行时限制'];
const FIELDS = new Set(['severity', 'message', 'file', 'line', 'column', 'sourceLine', 'lang']);
const OPS = new Set(['contains', 'startsWith', 'endsWith', 'equals', 'in', 'regex', 'present']);
const LANGS = new Set(['cpp', 'py']);
const BUILTIN_VARS = new Set(['line', 'column', 'file', 'severity']);
const ID_RE = /^[a-z0-9]+(?:-[a-z0-9]+)*$/;

const errors = [];
const warnings = [];

function err(where, message) {
  errors.push(`${where}: ${message}`);
}
function warn(where, message) {
  warnings.push(`${where}: ${message}`);
}

function walk(dir) {
  const out = [];
  if (!existsSync(dir)) return out;
  for (const name of readdirSync(dir)) {
    const full = path.join(dir, name);
    const st = statSync(full);
    if (st.isDirectory()) out.push(...walk(full));
    else if (name.endsWith('.json')) out.push(full);
  }
  return out.sort();
}

function loadRuleFiles() {
  const files = walk(RULES_DIR);
  if (!files.length) {
    err('rules', `no rule files under ${path.relative(ROOT, RULES_DIR)}`);
    return [];
  }
  const rules = [];
  for (const file of files) {
    const rel = path.relative(ROOT, file).replace(/\\/g, '/');
    let doc;
    try {
      doc = JSON.parse(readFileSync(file, 'utf8'));
    } catch (e) {
      err(rel, `invalid JSON: ${e.message}`);
      continue;
    }
    if (!doc || !LANGS.has(doc.lang)) {
      err(rel, 'missing or invalid "lang" (cpp|py)');
      continue;
    }
    if (!Array.isArray(doc.rules) || !doc.rules.length) {
      err(rel, 'missing non-empty "rules" array');
      continue;
    }
    for (const rule of doc.rules) {
      rule.lang = doc.lang;
      rule.__file = rel;
      rules.push(rule);
    }
  }
  return rules;
}

function checkRegex(where, source) {
  if (typeof source !== 'string' || !source) return;
  try {
    new RegExp(source, 'i');
  } catch (e) {
    err(where, `invalid regex: ${e.message}`);
  }
}

function checkPredicate(where, pred, pathPrefix = 'when') {
  if (!pred || typeof pred !== 'object' || Array.isArray(pred)) {
    err(where, `${pathPrefix}: predicate must be an object`);
    return;
  }
  if (Array.isArray(pred.all)) {
    if (pred.all.length === 0) err(where, `${pathPrefix}.all must not be empty`);
    pred.all.forEach((p, i) => checkPredicate(where, p, `${pathPrefix}.all[${i}]`));
    return;
  }
  if (Array.isArray(pred.any)) {
    if (pred.any.length === 0) err(where, `${pathPrefix}.any must not be empty`);
    pred.any.forEach((p, i) => checkPredicate(where, p, `${pathPrefix}.any[${i}]`));
    return;
  }
  if (pred.not) {
    checkPredicate(where, pred.not, `${pathPrefix}.not`);
    return;
  }
  if (!FIELDS.has(pred.field)) {
    err(where, `${pathPrefix}: unknown field "${pred.field}"`);
    return;
  }
  const op = pred.op || 'contains';
  if (!OPS.has(op)) {
    err(where, `${pathPrefix}: unknown op "${op}"`);
    return;
  }
  if (op === 'present') return;
  if (op === 'in') {
    if (!Array.isArray(pred.value) || pred.value.some((v) => typeof v !== 'string')) {
      err(where, `${pathPrefix}: op "in" needs a string array value`);
    }
    return;
  }
  if (typeof pred.value !== 'string' || !pred.value) {
    err(where, `${pathPrefix}: op "${op}" needs a non-empty string value`);
    return;
  }
  if (op === 'regex') checkRegex(where, pred.value);
}

function placeholdersOf(text) {
  const names = new Set();
  let s = String(text).replace(/\{\{/g, '').replace(/\}\}/g, '');
  const re = /\{([a-zA-Z_][\w-]*)\}/g;
  let m;
  while ((m = re.exec(s))) names.add(m[1]);
  return names;
}

function checkI18n(where, field, value, varNames) {
  if (!value || typeof value !== 'object' || Array.isArray(value)) {
    err(where, `${field} must be an object with chs/en/ja`);
    return;
  }
  let filled = 0;
  const sets = [];
  for (const lang of ['chs', 'en', 'ja']) {
    if (typeof value[lang] !== 'string') {
      err(where, `${field}.${lang} must be a string`);
      continue;
    }
    if (value[lang].trim()) filled++;
    const names = placeholdersOf(value[lang]);
    names.forEach((name) => {
      if (!BUILTIN_VARS.has(name) && !varNames.has(name)) {
        err(where, `${field}.${lang}: unknown placeholder {${name}}`);
      }
    });
    if (value[lang].trim()) sets.push({ lang, names });
  }
  if (!filled) err(where, `${field} needs at least one non-empty language`);
  if (sets.length > 1) {
    const base = sets[0];
    for (const item of sets.slice(1)) {
      const missing = [...base.names].filter((n) => !item.names.has(n));
      const extra = [...item.names].filter((n) => !base.names.has(n));
      if (missing.length || extra.length) {
        err(where, `${field}: placeholder mismatch between ${base.lang} and ${item.lang}` +
          (missing.length ? ` (missing ${missing.map((n) => '{' + n + '}').join(', ')})` : '') +
          (extra.length ? ` (extra ${extra.map((n) => '{' + n + '}').join(', ')})` : ''));
      }
    }
  }
}

function checkRule(rule) {
  const where = `${rule.__file}#${rule.id || '(no id)'}`;
  if (typeof rule.id !== 'string' || !ID_RE.test(rule.id)) {
    err(where, 'id must be lowercase-kebab-case');
  }
  if (!CATEGORIES.includes(rule.category)) {
    err(where, `category must be one of ${CATEGORIES.join(' / ')}`);
  }
  if (rule.priority != null && !Number.isInteger(rule.priority)) {
    err(where, 'priority must be an integer');
  }

  const varNames = new Set(Object.keys(rule.capture || {}));
  checkPredicate(where, rule.when);
  for (const [name, spec] of Object.entries(rule.capture || {})) {
    if (!spec || !FIELDS.has(spec.field)) err(where, `capture.${name}: unknown field`);
    if (!spec || typeof spec.regex !== 'string') err(where, `capture.${name}: regex must be a string`);
    else checkRegex(where, spec.regex);
  }

  checkI18n(where, 'human', rule.human, varNames);
  checkI18n(where, 'hint', rule.hint, varNames);

  if (rule.human && typeof rule.human.chs === 'string' && rule.human.chs.length > 120) {
    warn(where, `human.chs is ${rule.human.chs.length} chars (>120)`);
  }
  if (rule.hint && typeof rule.hint.chs === 'string' && rule.hint.chs.length > 70) {
    warn(where, `hint.chs is ${rule.hint.chs.length} chars (>70)`);
  }

  if (!Array.isArray(rule.tests) || !rule.tests.length) {
    err(where, 'tests must be a non-empty array');
    return;
  }
  let positives = 0;
  for (const test of rule.tests) {
    if (typeof test.message !== 'string' || typeof test.expect !== 'boolean') {
      err(where, 'each test needs { message: string, expect: boolean }');
      continue;
    }
    if (test.expect) positives++;
    const diag = {
      lang: rule.lang,
      severity: test.severity || 'error',
      message: test.message,
      file: 'main.cpp',
      line: 1,
      column: 1
    };
    const result = engine.matchRule(rule, diag);
    if (result.matched !== test.expect) {
      err(where, `test expected ${test.expect} but got ${result.matched}: ${JSON.stringify(test.message)}`);
    }
  }
  if (!positives) err(where, 'need at least one positive (expect: true) test');
}

function loadAndCheck() {
  const rules = loadRuleFiles();
  const seen = new Map();
  for (const rule of rules) {
    if (seen.has(rule.id)) {
      err(`${rule.__file}#${rule.id}`, `duplicate id, already in ${seen.get(rule.id)}`);
    } else {
      seen.set(rule.id, rule.__file);
    }
    checkRule(rule);
  }
  return rules;
}

function countMissingI18n(rules) {
  const missing = { en: 0, ja: 0 };
  for (const rule of rules) {
    for (const field of ['human', 'hint']) {
      for (const lang of ['en', 'ja']) {
        const text = rule[field] && rule[field][lang];
        if (!text || !String(text).trim()) missing[lang]++;
      }
    }
  }
  return missing;
}

function writeCatalog(rules) {
  const order = new Map(CATEGORIES.map((c, i) => [c, i]));
  const sorted = [...rules].sort((a, b) => {
    const ca = order.get(a.category) ?? 99;
    const cb = order.get(b.category) ?? 99;
    if (ca !== cb) return ca - cb;
    return String(a.id).localeCompare(String(b.id));
  });
  const out = {
    version: 2,
    generated: true,
    note: 'Generated by node tools/diag.mjs build — edit diagnostics/rules/**/*.json instead.',
    rules: sorted.map((rule) => {
      const clean = {};
      for (const key of ['id', 'lang', 'category', 'priority', 'when', 'capture', 'human', 'hint', 'example', 'docs']) {
        if (rule[key] !== undefined) clean[key] = rule[key];
      }
      return clean;
    })
  };
  writeFileSync(CATALOG, JSON.stringify(out, null, 2) + '\n', 'utf8');
  return out;
}

function printReport(label) {
  for (const w of warnings) console.warn(`  ! ${w}`);
  for (const e of errors) console.error(`  ✗ ${e}`);
  if (!errors.length) console.log(`  ✓ ${label}`);
}

function cmdBuild() {
  const rules = loadAndCheck();
  if (errors.length) {
    console.error(`✗ ${errors.length} error(s), catalog NOT written`);
    printReport('catalog.json unchanged');
    process.exit(1);
  }
  const catalog = writeCatalog(rules);
  const missing = countMissingI18n(rules);
  console.log(`✓ catalog.json: ${catalog.rules.length} rules`);
  if (missing.en || missing.ja) {
    console.log(`  · untranslated texts: en ${missing.en}, ja ${missing.ja} (human+hint slots)`);
  }
  if (warnings.length) printReport(`catalog.json written with ${warnings.length} warning(s)`);
}

function cmdCheck() {
  const rules = loadAndCheck();
  const missing = countMissingI18n(rules);
  if (missing.en || missing.ja) {
    console.log(`  · untranslated texts: en ${missing.en}, ja ${missing.ja} (human+hint slots)`);
  }
  printReport(`${rules.length} rules validated`);
  process.exit(errors.length ? 1 : 0);
}

function cmdExplain(args) {
  const message = args.filter((a) => !a.startsWith('--')).join(' ').trim();
  if (!message) {
    console.error('usage: node tools/diag.mjs explain "diagnostic message" [--severity error|warning]');
    process.exit(2);
  }
  const sevIdx = args.indexOf('--severity');
  const severity = sevIdx >= 0 ? (args[sevIdx + 1] || 'error') : 'error';
  const rules = loadAndCheck();
  if (errors.length) {
    printReport('rules invalid, cannot explain');
    process.exit(1);
  }
  const diag = { lang: 'cpp', severity, message, file: 'main.cpp', line: 1, column: 1 };
  const hit = engine.select(diag, rules, 'chs');
  if (!hit) {
    console.log('no rule matches that message');
    return;
  }
  console.log(`rule:    ${hit.rule.id}  (priority ${hit.rule.priority || 0}, score ${hit.score})`);
  console.log(`file:    ${hit.rule.__file}`);
  console.log('trace:');
  for (const leaf of hit.trace) {
    if (leaf.kind === 'leaf') {
      console.log(`  ${leaf.ok ? '✓' : '·'} ${leaf.field} ${leaf.op} ${JSON.stringify(leaf.value)}`);
    } else {
      console.log(`  ${leaf.ok ? '✓' : '·'} not`);
    }
  }
  for (const lang of ['chs', 'en', 'ja']) {
    const text = engine.render(hit.rule, diag, lang);
    console.log(`\n[${lang}] ${text.human || '(empty)'}`);
    console.log(`      hint: ${text.hint || '(empty)'}`);
  }
}

function cmdList() {
  const rules = loadAndCheck();
  if (errors.length) {
    printReport('rules invalid');
    process.exit(1);
  }
  for (const rule of rules) {
    console.log(`${rule.id.padEnd(28)} ${String(rule.category).padEnd(8)} prio=${String(rule.priority || 0).padStart(3)}  ${rule.__file}`);
  }
}

const [command, ...rest] = process.argv.slice(2);
try {
  if (command === 'build') cmdBuild();
  else if (command === 'check') cmdCheck();
  else if (command === 'explain') cmdExplain(rest);
  else if (command === 'list') cmdList();
  else {
    console.log('usage: node tools/diag.mjs <build|check|explain|list> [args]');
    process.exit(2);
  }
} catch (e) {
  console.error(`✗ ${e.message}`);
  process.exit(1);
}
