/*
 * CodeMirror 6 浏览器 bundle 的入口。
 * 由 playground/build.mjs 用 esbuild 打包成 IIFE（globalName = G4PCEditor），
 * 供 pg/pg.js（经典脚本，无需打包）使用：window.G4PCEditor.{EditorView,EditorState,basicSetup,cpp}
 *
 * 注意：这里只做 export，不再手动写 window.G4PCEditor —— 否则 esbuild 的
 * `var G4PCEditor = (...)` 会在最后把 window 上的同名属性覆盖成空 exports。
 */
import { EditorView, basicSetup } from 'codemirror';
import { EditorState } from '@codemirror/state';
import { cpp } from '@codemirror/lang-cpp';

export { EditorView, EditorState, basicSetup, cpp };
