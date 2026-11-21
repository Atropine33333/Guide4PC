#!/usr/bin/env node
/**
 * One-off migration: diagnostics/messages.json (regex era) → diagnostics/rules/cpp/*.json (v2).
 * Preserves the hand-written Chinese human/hint texts and leaves en/ja empty for the author.
 */
import { readFileSync, writeFileSync, mkdirSync } from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const src = JSON.parse(readFileSync(path.join(ROOT, 'diagnostics', 'messages.json'), 'utf8'));

const S = { field: 'severity', op: 'in', value: ['error', 'fatal error'] };

function leaf(field, op, value) {
  return value === undefined ? { field, op } : { field, op, value };
}

const T = {
  'missing-semicolon': {
    cat: '语法',
    when: { all: [S, leaf('message', 'startsWith', "expected ';'")] },
    capture: { token: { field: 'message', regex: "expected '([^']+)'" } },
    negative: 'expected expression'
  },
  'missing-brace': {
    cat: '语法',
    when: { all: [S, leaf('message', 'startsWith', "expected '}'")] },
    capture: { token: { field: 'message', regex: "expected '([^']+)'" } },
    negative: "expected ')'"
  },
  'missing-paren': {
    cat: '语法',
    when: { all: [S, leaf('message', 'startsWith', "expected ')'")] },
    capture: { token: { field: 'message', regex: "expected '([^']+)'" } },
    negative: "expected '}'"
  },
  'expected-expression': {
    cat: '语法',
    when: { all: [S, leaf('message', 'contains', 'expected expression')] },
    negative: "use of undeclared identifier 'x'"
  },
  'expected-unqualified-id': {
    cat: '语法',
    when: { all: [S, leaf('message', 'contains', 'expected unqualified-id')] },
    negative: 'expected expression'
  },
  'undeclared-identifier': {
    cat: '名字与作用域',
    when: { all: [S, leaf('message', 'contains', 'use of undeclared identifier')] },
    capture: { name: { field: 'message', regex: "undeclared identifier '([^']+)'" } },
    negative: 'expected expression'
  },
  'unknown-type-name': {
    cat: '名字与作用域',
    when: { all: [S, leaf('message', 'contains', 'unknown type name')] },
    capture: { name: { field: 'message', regex: "unknown type name '([^']+)'" } },
    negative: 'expected expression'
  },
  'no-member': {
    cat: '名字与作用域',
    priority: 10,
    when: { all: [S, leaf('message', 'contains', 'no member named')] },
    capture: { member: { field: 'message', regex: "no member named '([^']+)'" } },
    negative: 'expected expression'
  },
  'missing-namespace-member': {
    cat: '名字与作用域',
    priority: 20,
    when: {
      all: [
        leaf('message', 'contains', 'no member named'),
        leaf('message', 'contains', 'in namespace')
      ]
    },
    capture: {
      member: { field: 'message', regex: "no member named '([^']+)'" },
      ns: { field: 'message', regex: "in namespace '([^']+)'" }
    },
    negative: "no member named 'foo' in 'std::vector<int>'"
  },
  redefinition: {
    cat: '名字与作用域',
    when: { all: [S, leaf('message', 'contains', 'redefinition of')] },
    capture: { name: { field: 'message', regex: "redefinition of '([^']+)'" } },
    negative: 'expected expression'
  },
  'header-not-found': {
    cat: '名字与作用域',
    when: {
      all: [
        S,
        leaf('message', 'contains', 'file not found')
      ]
    },
    capture: { header: { field: 'message', regex: "'([^']+)' file not found" } },
    negative: 'expected expression'
  },
  'bad-init-type': {
    cat: '类型',
    priority: 10,
    when: {
      all: [
        S,
        leaf('message', 'contains', 'cannot initialize a variable of type')
      ]
    },
    capture: {
      type: { field: 'message', regex: "variable of type '([^']+)'" },
      valueType: { field: 'message', regex: "with an? (?:rvalue|lvalue) of type '([^']+)'" }
    },
    negative: 'expected expression'
  },
  'invalid-operands': {
    cat: '类型',
    when: { all: [S, leaf('message', 'contains', 'invalid operands to binary expression')] },
    negative: 'expected expression'
  },
  'no-viable-conversion': {
    cat: '类型',
    when: { all: [S, leaf('message', 'contains', 'no viable conversion from')] },
    negative: 'expected expression'
  },
  'assign-incompatible': {
    cat: '类型',
    priority: 10,
    when: {
      all: [
        S,
        leaf('message', 'contains', 'assigning to'),
        leaf('message', 'contains', 'incompatible type')
      ]
    },
    capture: {
      target: { field: 'message', regex: "assigning to '([^']+)'" },
      sourceType: { field: 'message', regex: "incompatible type '([^']+)'" }
    },
    negative: 'expected expression'
  },
  'no-matching-function': {
    cat: '类型',
    priority: 30,
    when: { all: [S, leaf('message', 'contains', 'member function for call to')] },
    capture: { callee: { field: 'message', regex: "function for call to '([^']+)'" } },
    negative: "no matching constructor for initialization of 'Buffer'"
  },
  'no-matching-constructor': {
    cat: '类型',
    priority: 20,
    when: { all: [S, leaf('message', 'contains', 'no matching constructor for initialization of')] },
    capture: { type: { field: 'message', regex: "initialization of '([^']+)'" } },
    negative: "no matching member function for call to 'append'"
  },
  'too-few-arguments': {
    cat: '类型',
    when: { all: [S, leaf('message', 'contains', 'too few arguments to function call')] },
    negative: 'expected expression'
  },
  'too-many-arguments': {
    cat: '类型',
    when: { all: [S, leaf('message', 'contains', 'too many arguments to function call')] },
    negative: 'expected expression'
  },
  'missing-return-value': {
    cat: '类型',
    when: {
      all: [
        { field: 'severity', op: 'in', value: ['warning', 'error'] },
        {
          any: [
            leaf('message', 'contains', 'non-void function does not return a value'),
            leaf('message', 'contains', 'should return a value')
          ]
        }
      ]
    },
    negative: 'expected expression'
  },
  'indirection-not-pointer': {
    cat: '类型',
    when: { all: [S, leaf('message', 'contains', 'indirection requires pointer operand')] },
    negative: 'expected expression'
  },
  'member-of-non-class': {
    cat: '类型',
    when: {
      all: [
        S,
        leaf('message', 'contains', 'member reference base type'),
        leaf('message', 'contains', 'is not a structure or union')
      ]
    },
    negative: 'expected expression'
  },
  'undefined-symbol': {
    cat: '链接',
    when: { all: [leaf('message', 'contains', 'undefined symbol:')] },
    capture: { symbol: { field: 'message', regex: 'undefined symbol: ([^\\s(]+)' } },
    negative: 'expected expression'
  },
  'undefined-reference': {
    cat: '链接',
    when: { all: [leaf('message', 'contains', 'undefined reference to')] },
    capture: { symbol: { field: 'message', regex: "undefined reference to '([^']+)'" } },
    negative: 'expected expression'
  },
  'throw-disabled': {
    cat: '运行时限制',
    when: { all: [S, leaf('message', 'contains', "cannot use 'throw' with exceptions disabled")] },
    negative: 'expected expression'
  },
  'try-disabled': {
    cat: '运行时限制',
    when: { all: [S, leaf('message', 'contains', "cannot use 'try' with exceptions disabled")] },
    negative: 'expected expression'
  },
  'no-viable-overload': {
    cat: '类型',
    priority: 5,
    when: {
      all: [
        S,
        leaf('message', 'contains', 'function for call to'),
        { not: leaf('message', 'contains', 'member function for call to') }
      ]
    },
    capture: { callee: { field: 'message', regex: "function for call to '([^']+)'" } },
    negative: "no matching member function for call to 'append'"
  },
  'array-subscript-not-integer': {
    cat: '类型',
    when: { all: [S, leaf('message', 'contains', 'array subscript is not an integer')] },
    negative: 'expected expression'
  }
};

const FILES = {
  语法: 'syntax.json',
  名字与作用域: 'naming.json',
  类型: 'types.json',
  链接: 'link.json',
  运行时限制: 'runtime.json'
};

function stripExample(example) {
  return String(example || '').replace(/^\s*(?:fatal error|error|warning):\s*/, '');
}

const buckets = {};
const missing = [];

for (const old of src.rules) {
  const t = T[old.id];
  if (!t) {
    missing.push(old.id);
    continue;
  }
  const positive = stripExample(old.example || old.match);
  const severity = /warning:/.test(old.example || '') ? 'warning' : 'error';
  const rule = {
    id: old.id,
    category: t.cat,
    priority: t.priority || 0,
    when: t.when,
    capture: t.capture || {},
    human: { chs: old.human || '', en: '', ja: '' },
    hint: { chs: old.hint || '', en: '', ja: '' },
    example: old.example || '',
    tests: [
      { message: positive, severity, expect: true },
      { message: t.negative, severity: 'error', expect: false }
    ]
  };
  if (!rule.capture || !Object.keys(rule.capture).length) delete rule.capture;
  const file = FILES[t.cat];
  (buckets[file] = buckets[file] || []).push(rule);
}

if (missing.length) {
  console.error('unmapped ids:', missing.join(', '));
  process.exit(1);
}

const dir = path.join(ROOT, 'diagnostics', 'rules', 'cpp');
mkdirSync(dir, { recursive: true });
const lang = 'cpp';
for (const [file, rules] of Object.entries(buckets)) {
  rules.sort((a, b) => a.id.localeCompare(b.id));
  writeFileSync(path.join(dir, file), JSON.stringify({ lang, rules }, null, 2) + '\n', 'utf8');
  console.log(`${file}: ${rules.length} rules`);
}
console.log(`total: ${Object.values(buckets).reduce((n, r) => n + r.length, 0)}`);
