/*
 * 把运行器需要的浏览器文件准备好：
 *   1. 用 esbuild 把 CodeMirror 6 打包成 IIFE（vendor/editor.bundle.js）
 *   2. 从 node_modules 复制 clang-wasm 的自包含 IIFE（vendor/clang-wasm.global.js）
 *      —— 编译器本体（29MB 资产）走 CDN 或 docs/pg/clang，不在这里复制。
 *
 * 用法：npm run build（在 playground/ 目录下）
 */
import { build } from 'esbuild';
import { copyFileSync, mkdirSync, statSync } from 'node:fs';
import { dirname, join } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = dirname(fileURLToPath(import.meta.url));
const vendor = join(root, 'vendor');
mkdirSync(vendor, { recursive: true });

await build({
  entryPoints: [join(root, 'src/editor.js')],
  bundle: true,
  format: 'iife',
  globalName: 'G4PCEditor',
  minify: true,
  outfile: join(vendor, 'editor.bundle.js'),
  logLevel: 'info',
});

const clangIife = join(root, 'node_modules/@live-codes/clang-wasm/dist/clang-wasm.global.js');
copyFileSync(clangIife, join(vendor, 'clang-wasm.global.js'));
console.log('clang-wasm.global.js', (statSync(join(vendor, 'clang-wasm.global.js')).size / 1024).toFixed(0) + 'KB');
