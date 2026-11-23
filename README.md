# Guide4PC

A bilingual (Chinese/English) guide for your first computer, plus an interactive
exercise site where readers can write and run real C / C++ code in the browser.

Everything is static: the reading site is a small SPA shell rendered from
Markdown, and the exercise runner compiles code with a WebAssembly build of
Clang inside a Web Worker — no backend, no server-side execution.

## What's inside

- **Reading site** — `index.html` + `css/main.css` + `js/main.js`, rendered from
  the Markdown files in `content/`.
- **Exercise site** — `exercise-site/`, served at `/exercise/`: problem list,
  filters, daily problem, ready-made papers, per-problem editor + runner.
- **Problem bank** — `exercise/cpp/` and `exercise/c/`, one directory per
  problem; `exercise/papers/` holds preset papers.
- **Browser playground** — `playground/`: CodeMirror 6 bundle, the compile/run
  worker, the lint worker logic, and the home-page intro animation.
- **Diagnostics v2** — `diagnostics/`: rule JSON (`rules/`), generated
  `catalog.json`, JSON Schema, and the writing guide for localized messages.
- **Tools** — `tools/diag.mjs` validates/builds/explains diagnostic rules.
- **Internal test bench** — `test/`, not linked from the site; visit `/test/`
  directly if you need it.

## Project structure

```
Guide4PC/
├── content/                  # Source content (edit this)
│   ├── chs/                  # Chinese Markdown
│   ├── en/                   # English Markdown
│   ├── img/                  # Shared images
│   ├── dl/                   # Downloadable files
│   └── favicon/              # Site icons
├── index.html                # SPA shell template (GUIDE4PC_BUILD_VER placeholder)
├── css/main.css              # Reading-site styles and design tokens
├── js/
│   ├── main.js               # Reading-site front-end (routing, Markdown, search)
│   └── build.js              # Build script (Node.js, no npm dependency)
├── playground/               # Browser compiler runtime + intro scene source
├── exercise/
│   ├── cpp/<id>/             # spec.json, problem.chs.md, problem.en.md,
│   │                         # starter.cpp, solution.cpp, harness.cpp (optional)
│   ├── c/<id>/               # Same layout for C problems
│   └── papers/*.json         # Preset papers (id list + title)
├── exercise-site/            # /exercise/ mini app (index.html, app.js, app.css)
├── diagnostics/              # v2 diagnostic rules + catalog + guides
├── tools/diag.mjs            # Rule validator / catalog builder / explain CLI
├── test/                     # Internal test bench (not linked)
├── docs/                     # Build output — committed, and this is what deploys
└── update.sh                 # Thin wrapper around `node js/build.js`
```

## Build

Requires Node.js 24+ (the build uses only Node built-ins; there is no root
`package.json` and nothing to install).

```bash
node js/build.js           # incremental (uses .build-cache.json)
node js/build.js --force   # full rebuild
bash update.sh             # same as `node js/build.js`
```

A full build:

1. wipes and regenerates `docs/` (keeping non-generated files at the `docs/`
   root, such as the site-verification file);
2. renders/copies every page, image, font, stylesheet and script;
3. scans `exercise/<lang>/<id>/`, validates each `spec.json`, publishes problems
   to `docs/ex/<id>/` and `docs/exercise/<lang>/<id>/`, and generates
   `docs/ex/index.json` and `docs/exercise/index.json`;
4. copies preset papers and generates `docs/exercise/papers/index.json`;
5. rebuilds `docs/exercise/` from `exercise-site/`, substituting the build
   version placeholder;
6. writes `docs/js/manifest.json`, `docs/js/search_index.json`, `docs/js/version.json`
   and the versioned `docs/index.html`.

Preview locally:

```bash
python -m http.server 8766 --directory docs
# http://127.0.0.1:8766/          → reading site
# http://127.0.0.1:8766/exercise/ → exercise site
```

On Windows, stop the preview server before a full build; otherwise
`fs.rmSync(docs)` fails with `EPERM`.

## Build version / cache busting

The version mechanism is still in effect. Every build computes a 10-character
SHA-1 over the things that end up in `docs/`:

- the shell template (`index.html`), `css/main.css`, `js/main.js`, `js/build.js`;
- everything under `content/{chs,en,img,dl,favicon}`;
- the playground runtime (`playground/pg.js`, `pg-worker.js`, `pg.css`,
  `scene.js`, `scene.css`, the CodeMirror bundle, the clang-wasm toolchain
  bundle, `diag-engine.js`);
- `diagnostics/catalog.json`;
- `exercise/`, `exercise-site/`, `test/`.

That version is then:

- written into `docs/index.html` (replacing `GUIDE4PC_BUILD_VER`, so CSS/JS URLs
  become `?v=<version>`);
- stored in `docs/js/version.json`;
- exposed as `window.GUIDE4PC_VERSION`, which `main.js` uses to version every
  data fetch (manifest, search index, Markdown, images, downloads) and which
  `playground/pg.js` uses for worker/runtime assets;
- polled by the running page: when `version.json` reports a newer version, the
  page reloads onto the new deployment.

Same sources → same version (no pointless reloads). Any source change → new
version.

Important caveat with GitHub-connected hosting: the deployed `docs/` is
prebuilt, so the version only changes when you run the build locally **and
commit `docs/`**. EdgeOne Pages does not run a build for this repo.

Because `exercise/` is part of the hash, editing a problem also bumps the site
version — readers get the new problems on their next visit, at the cost of one
extra reload.

## Exercise system

Each problem lives in `exercise/<lang>/<id>/`:

| File | Meaning |
|------|---------|
| `spec.json` | id, lang, category, rank, title (chs/en), limits, checks, variants; `harness` for harness-style tasks |
| `problem.chs.md` / `problem.en.md` | Statement shown in the exercise site |
| `starter.cpp` | Template preloaded into the editor |
| `solution.cpp` | Reference solution (smoke tests use it) |
| `harness.cpp` | Optional hidden test driver, required for harness tasks |

Two grading modes:

- **cases** — the program reads stdin and writes stdout; each check compares the
  output with the expected value;
- **harness** — the student writes a class/function and no `main`; the hidden
  `harness.cpp` is appended and reports results with `@@G4PC {...}` protocol
  lines.

Rules are enforced at build time: ids, languages, categories, ranks, required
files and spec shape are validated; invalid problems are skipped with a warning.
See `exercise/README.md` and `exercise/schema.json` for the authoring contract.

## Diagnostics

Runtime diagnostics use the v2 engine: `playground/diag-engine.js` matches a
structured predicate tree (`when`) against a compiler message, then renders a
localized `human` explanation and `hint`.

- Rules: `diagnostics/rules/<lang>/*.json`
- Generated catalog: `diagnostics/catalog.json` (rebuilt automatically when
  rules are newer than the catalog)
- Schema: `diagnostics/schema.json`
- Authoring guide: `diagnostics/HUMAN-GUIDE.md`

```bash
node tools/diag.mjs check     # validate all rules and their positive tests
node tools/diag.mjs build     # rebuild catalog.json
node tools/diag.mjs explain "expected ';' at end of declaration"
```

`human` and `hint` are objects with `chs` / `en` / `ja`; missing locales fall
back to English, then Chinese.

## Deploy

### EdgeOne Pages (current setup)

The repository is connected to EdgeOne Pages and deploys the committed `docs/`
directory directly:

| Setting | Value |
|---------|-------|
| Framework preset | none |
| Root directory | `.` |
| Output directory | **`docs`** |
| Build command | *(empty)* |
| Install command | *(empty)* |

Push to `main` triggers a deployment. Do not run `node js/build.js` in CI:
`docs/` is already built and committed, and rebuilding in CI would only produce
a different version stamp.

### GitHub Pages (alternative)

`docs/` is a ready-to-serve static root and already contains `.nojekyll`;
configure Pages to publish from the `docs/` folder on `main`.

## Adding documents

1. Put Markdown in `content/chs/` and `content/en/`; images in `content/img/`
   (referenced as `../img/...`); downloads in `content/dl/`.
2. Use matching numeric prefixes for pairing (see below).
3. Run `node js/build.js` and commit the rebuilt `docs/`.

### File naming and pairing

| File (chs / en) | Pairing key |
|---|---|
| `0_开屏.md` / `0_Start.md` | `home` (always first) |
| `0.1_欢迎信.md` / `0.1_WelcomeLetter.md` | `0.1` |
| `附录1_*.md` / `Appendix1_*.md` | `1` |
| `Section7_作业1.md` / `Section7_Homework1.md` | `7` |
| `Section7.9_数据来源.md` / `Section7.9_DataLogging.md` | `7.9` |
| `Section8A_多文件与导入.md` / `Section8A_Multifile.md` | `8A` |
| `SectionA_前言.md` / `SectionA_Preface.md` | `A` |

- Numeric prefixes are compared naturally: `7 < 7.9 < 7.10 < 8 < 8A`.
- Letter suffixes are case-insensitive.
- Files whose name starts with `[test]` (or `[dlN][test]`) are built but hidden
  from navigation, search and language pairing; they remain reachable by URL.

## Third-party notices

- Clang/LLVM (WebAssembly build) — Apache-2.0 WITH LLVM-exception
- `@live-codes/clang-wasm` — MIT
- CodeMirror 6 / Lezer — MIT
- markdown-it — MIT
- KaTeX — MIT
- highlight.js — BSD-3-Clause
- Maple Mono NF CN (subset) — SIL OFL 1.1

Site content is licensed CC BY 4.0. The license dialog in the footer lists the
runtime dependencies.

## License

Content and code are provided as-is for personal use.
