/**
 * Guide4PC diagnostics matcher (v2)
 *
 * A tiny, dependency-free predicate engine shared by the browser playground and
 * the `tools/diag.mjs` CLI. Rules are plain JSON; this file only knows how to
 * evaluate them and how to pick translated strings.
 *
 *   window.G4PCDiag = { pickLocale, interpolate, matchRule, select, render }
 *   const { select } = require('./playground/diag-engine.js');
 */
(function (root, factory) {
  var api = factory();
  if (typeof module === 'object' && module.exports) module.exports = api;
  else root.G4PCDiag = api;
})(typeof globalThis !== 'undefined' ? globalThis : this, function () {
  'use strict';

  var LOCALES = ['chs', 'en', 'ja'];
  var FIELDS = ['severity', 'message', 'file', 'line', 'column', 'sourceLine', 'lang'];

  function pickLocale(value, lang) {
    if (typeof value === 'string') return value;
    if (!value) return '';
    if (value[lang]) return value[lang];
    if (value.en) return value.en;
    return value.chs || '';
  }

  function normalize(value) {
    return value == null ? '' : String(value);
  }

  function readField(diag, field) {
    var v = diag ? diag[field] : undefined;
    if (field === 'line' || field === 'column') {
      return v == null ? -1 : Number(v);
    }
    return normalize(v);
  }

  /** Evaluate one predicate, collecting every leaf it visits. */
  function evalPredicate(pred, diag, trace) {
    if (!pred || typeof pred !== 'object') return false;

    if (Array.isArray(pred.all)) {
      var allOk = true;
      for (var i = 0; i < pred.all.length; i++) {
        if (!evalPredicate(pred.all[i], diag, trace)) allOk = false;
      }
      return allOk;
    }

    if (Array.isArray(pred.any)) {
      var anyOk = false;
      for (var j = 0; j < pred.any.length; j++) {
        if (evalPredicate(pred.any[j], diag, trace)) anyOk = true;
      }
      return anyOk;
    }

    if (pred.not) {
      var inner = evalPredicate(pred.not, diag, []);
      if (trace) trace.push({ kind: 'not', ok: !inner });
      return !inner;
    }

    var field = pred.field;
    if (FIELDS.indexOf(field) === -1) return false;
    var actual = readField(diag, field);
    var op = pred.op || 'contains';
    var value = pred.value;
    var ok = false;

    switch (op) {
      case 'present':
        ok = actual !== '' && actual !== -1;
        break;
      case 'contains':
        ok = String(actual).indexOf(String(value)) !== -1;
        break;
      case 'startsWith':
        ok = String(actual).indexOf(String(value)) === 0;
        break;
      case 'endsWith':
        ok = String(actual).slice(-String(value).length) === String(value);
        break;
      case 'equals':
        ok = String(actual) === String(value);
        break;
      case 'in':
        ok = Array.isArray(value) && value.indexOf(String(actual)) !== -1;
        break;
      case 'regex':
        try {
          ok = new RegExp(String(value), 'i').test(String(actual));
        } catch (e) {
          ok = false;
        }
        break;
      default:
        ok = false;
    }

    if (trace) trace.push({ kind: 'leaf', field: field, op: op, value: value, ok: ok });
    return ok;
  }

  function countLeaves(pred) {
    if (!pred || typeof pred !== 'object') return 0;
    if (Array.isArray(pred.all)) return pred.all.reduce(function (n, p) { return n + countLeaves(p); }, 0);
    if (Array.isArray(pred.any)) return pred.any.reduce(function (n, p) { return n + countLeaves(p); }, 0);
    if (pred.not) return countLeaves(pred.not);
    return pred.field ? 1 : 0;
  }

  function matchRule(rule, diag) {
    var trace = [];
    var matched = evalPredicate(rule.when, diag, trace);
    var leaves = countLeaves(rule.when);
    var score = (rule.priority || 0) * 1000 + leaves;
    return { matched: matched, score: score, leaves: leaves, trace: trace };
  }

  function captureVars(rule, diag) {
    var vars = {};
    var caps = rule.capture || {};
    Object.keys(caps).forEach(function (name) {
      var spec = caps[name] || {};
      var text = readField(diag, spec.field);
      try {
        var m = new RegExp(String(spec.regex), 'i').exec(String(text));
        if (m) {
          if (m.groups) {
            Object.keys(m.groups).forEach(function (g) { if (m.groups[g] != null) vars[g] = m.groups[g]; });
          }
          vars[name] = m[1] != null ? m[1] : m[0];
        }
      } catch (e) { /* broken capture — check will report it */ }
    });
    return vars;
  }

  function interpolate(text, vars) {
    if (!text) return '';
    return String(text).replace(/\{([a-zA-Z_][\w-]*)\}/g, function (whole, name) {
      return Object.prototype.hasOwnProperty.call(vars, name) ? String(vars[name]) : '';
    });
  }

  function buildVars(rule, diag) {
    var vars = {
      line: readField(diag, 'line'),
      column: readField(diag, 'column'),
      file: readField(diag, 'file'),
      severity: readField(diag, 'severity')
    };
    var caps = captureVars(rule, diag);
    Object.keys(caps).forEach(function (k) { vars[k] = caps[k]; });
    return vars;
  }

  function render(rule, diag, lang) {
    var vars = buildVars(rule, diag);
    return {
      human: interpolate(pickLocale(rule.human, lang), vars),
      hint: interpolate(pickLocale(rule.hint, lang), vars),
      vars: vars
    };
  }

  /**
   * Pick the best rule for a diagnostic.
   * Ordering: priority desc → specificity (leaf count) desc → declaration order.
   */
  function select(diag, rules, lang) {
    if (!Array.isArray(rules)) return null;
    var best = null;
    for (var i = 0; i < rules.length; i++) {
      var rule = rules[i];
      if (!rule || (rule.lang && diag && diag.lang && rule.lang !== diag.lang)) continue;
      var m = matchRule(rule, diag);
      if (!m.matched) continue;
      if (!best || m.score > best.score) {
        best = { rule: rule, score: m.score, trace: m.trace, order: i };
      }
    }
    if (!best) return null;
    var text = render(best.rule, diag, lang || 'chs');
    return {
      rule: best.rule,
      score: best.score,
      trace: best.trace,
      human: text.human,
      hint: text.hint,
      vars: text.vars
    };
  }

  return {
    version: 2,
    LOCALES: LOCALES,
    pickLocale: pickLocale,
    interpolate: interpolate,
    matchRule: matchRule,
    captureVars: captureVars,
    render: render,
    select: select
  };
});
