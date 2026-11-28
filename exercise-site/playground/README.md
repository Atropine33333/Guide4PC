# C++ Playground（练习站子页面）

练习站里的“只编译并运行当前程序”页面，不判题、不加载练习题。入口：练习站顶栏的 `Playground`，或直接访问 `/exercise/playground/`。

## 本地查看

构建后从 `docs/` 起静态服务器：

```bash
node js/build.js
python -m http.server 8777 --directory docs
```

然后打开 <http://127.0.0.1:8777/exercise/playground/>。

不要直接用 `file://` 打开 `docs/exercise/playground/index.html`：worker 与 `importScripts` 需要 http(s)。

## 依赖

页面在 `docs/exercise/playground/`，运行时复用 `docs/pg/vendor/` 下的两个文件：

- `editor.bundle.js`（CodeMirror 6）
- `clang-wasm.global.js`（clang 编译器运行时）

这两个文件由构建从 `playground/` 同步；首次点击“运行”会从 jsDelivr 下载约 29MB 的 clang/lld/sysroot，之后由浏览器缓存。

## 功能

- 左侧编辑 C++，右侧填 stdin，点“运行”（或 Ctrl/Cmd+Enter）；
- 显示 stdout、stderr、退出码与编译/运行耗时；编译错误会标红；
- 草稿保存在 localStorage；运行超过 30 秒强制终止；
- 单文件、单翻译单元；不判题、不记录进度。

语法讲义里的每个 `cpp` 例子都可以直接粘贴进来运行，输出应与讲义里的 `text` 块一致。
