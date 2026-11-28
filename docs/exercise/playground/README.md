# C++ Playground（本地）

一个只做“编译并运行当前程序”的最小页面，不判题、不加载练习题。

## 启动

从仓库根目录起一个静态服务器：

```bash
python -m http.server 8777
```

然后打开 <http://127.0.0.1:8777/lab/>。

不要把 `lab/index.html` 直接用 `file://` 打开：worker 和 `importScripts` 需要 http(s)。

## 依赖

- `docs/pg/vendor/editor.bundle.js`（CodeMirror 6）
- `docs/pg/vendor/clang-wasm.global.js`（clang 编译器运行时）

这两个文件已在仓库里。首次点击“运行”会从 jsDelivr 下载约 29MB 的 clang/lld/sysroot，之后由浏览器缓存，再打开就很快。

## 功能

- 左侧编辑 C++，右侧填 stdin，点“运行”（或 Ctrl/Cmd+Enter）；
- 显示 stdout、stderr、退出码与编译/运行耗时；编译错误会标红；
- 草稿保存在 localStorage（换代码不会丢）；
- 单文件、单翻译单元；运行超过 30 秒会强制终止。

语法讲义里的每个 `cpp` 例子都可以直接粘贴进来运行，输出应与讲义里的 `text` 块一致。
