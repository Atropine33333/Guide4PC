/*
 * CodeMirror 6 浏览器 bundle 的入口。
 * 由 playground/build.mjs 用 esbuild 打包成 IIFE（globalName = G4PCEditor），
 * 供 pg/pg.js（经典脚本，无需打包）调用 G4PCEditor.createEditor()。
 *
 * 缩进规则在这里定死，不依赖 basicSetup 的语言默认值：
 *   · 缩进单位固定为 4 个空格（indentUnit / tabSize）
 *   · Tab / Shift-Tab 走缩进（indentWithTab），不会把焦点移出编辑器
 *   · Ctrl/⌘ + Enter 运行
 *
 * 代码高亮用 Catppuccin（用户自己的配色偏好）：颜色全部写成 CSS 变量，
 * Latte/Mocha 的取值放在 pg.css 的 `[data-theme="dark"]` 覆盖里，
 * 切换站点主题时不需要重建高亮样式。
 *
 * 注意：这里只做 export，不再手动写 window.G4PCEditor —— 否则 esbuild 的
 * `var G4PCEditor = (...)` 会在最后把 window 上的同名属性覆盖成空 exports。
 */
import { EditorView, basicSetup } from 'codemirror';
import { keymap } from '@codemirror/view';
import { EditorState } from '@codemirror/state';
import { cpp } from '@codemirror/lang-cpp';
import { HighlightStyle, indentUnit, syntaxHighlighting } from '@codemirror/language';
import { indentWithTab } from '@codemirror/commands';
import { tags as t } from '@lezer/highlight';

const INDENT = '    ';

/* Catppuccin 语义 → CSS 变量。具体色值见 pg.css（Light = Latte，Dark = Mocha）。 */
const catppuccin = HighlightStyle.define([
  { tag: t.comment, color: 'var(--cm-overlay)', fontStyle: 'italic' },
  { tag: t.lineComment, color: 'var(--cm-overlay)', fontStyle: 'italic' },
  { tag: t.blockComment, color: 'var(--cm-overlay)', fontStyle: 'italic' },
  { tag: t.docComment, color: 'var(--cm-overlay)', fontStyle: 'italic' },

  { tag: t.keyword, color: 'var(--cm-mauve)' },
  { tag: t.controlKeyword, color: 'var(--cm-mauve)' },
  { tag: t.definitionKeyword, color: 'var(--cm-mauve)' },
  { tag: t.moduleKeyword, color: 'var(--cm-mauve)' },
  { tag: t.operatorKeyword, color: 'var(--cm-mauve)' },
  { tag: t.self, color: 'var(--cm-red)' },

  { tag: t.string, color: 'var(--cm-green)' },
  { tag: t.special(t.string), color: 'var(--cm-green)' },
  { tag: t.regexp, color: 'var(--cm-green)' },
  { tag: t.escape, color: 'var(--cm-pink)' },

  { tag: t.number, color: 'var(--cm-peach)' },
  { tag: t.bool, color: 'var(--cm-peach)' },
  { tag: t.null, color: 'var(--cm-peach)' },
  { tag: t.atom, color: 'var(--cm-peach)' },
  { tag: t.constant(t.variableName), color: 'var(--cm-peach)' },

  { tag: t.operator, color: 'var(--cm-sky)' },
  { tag: t.derefOperator, color: 'var(--cm-sky)' },
  { tag: t.processingInstruction, color: 'var(--cm-pink)' },
  { tag: t.meta, color: 'var(--cm-pink)' },

  { tag: t.typeName, color: 'var(--cm-yellow)' },
  { tag: t.className, color: 'var(--cm-yellow)' },
  { tag: t.namespace, color: 'var(--cm-yellow)' },
  { tag: t.attributeName, color: 'var(--cm-yellow)' },

  { tag: t.definition(t.variableName), color: 'var(--cm-text)' },
  { tag: t.variableName, color: 'var(--cm-text)' },
  { tag: t.propertyName, color: 'var(--cm-text)' },
  { tag: t.function(t.variableName), color: 'var(--cm-blue)' },
  { tag: t.function(t.definition(t.variableName)), color: 'var(--cm-blue)' },

  { tag: t.link, color: 'var(--cm-blue)', textDecoration: 'underline' },
  { tag: t.invalid, color: 'var(--cm-red)' },
  { tag: t.emphasis, fontStyle: 'italic' },
  { tag: t.strong, fontWeight: 'bold' },
]);

export function createEditor({ parent, doc, onRun }) {
  // Tab 必须排在 basicSetup 之前，保证它比其它 keymap 先被看见。
  const extensions = [
    keymap.of([indentWithTab]),
    basicSetup,
    cpp(),
    indentUnit.of(INDENT),
    EditorState.tabSize.of(4),
    syntaxHighlighting(catppuccin),
    EditorView.theme({
      '&': { fontSize: 'var(--pg-code-size, 15px)' },
      '.cm-content': { fontFamily: 'var(--font-mono)' },
    }),
    EditorView.domEventHandlers({
      keydown(event) {
        if ((event.ctrlKey || event.metaKey) && event.key === 'Enter') {
          event.preventDefault();
          if (onRun) onRun();
          return true;
        }
        return false;
      },
    }),
  ];

  return new EditorView({
    state: EditorState.create({ doc, extensions }),
    parent,
  });
}

export { EditorView, EditorState, basicSetup, cpp };
