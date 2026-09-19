# Guide4PC

A bilingual (Chinese/English) static documentation website powered by Markdown files, designed for GitHub Pages deployment.

## Features

- **Bilingual**: Chinese and English documents with one-click language switching
- **Sidebar navigation**: Collapsible directory tree with multi-level support
- **Full-text search**: Exact substring matching across all documents
- **Reading progress**: Automatically remembers your position by section heading
- **Code copy buttons**: One-click copy for code blocks
- **Mobile responsive**: Adaptive layout with floating sidebar on small screens
- **Print-friendly**: Optimized print stylesheet
- **Adwaita-inspired design**: Clean GNOME-style visual language

## Project Structure

```
Guide4PC/
├── sourcecode/          # Content (you edit this)
│   ├── chs/             # Chinese Markdown files
│   ├── en/              # English Markdown files
│   └── img/             # Shared images
├── index.html           # Single-page entry
├── css/main.css         # Stylesheet
├── js/
│   ├── main.js          # Frontend logic
│   └── build.js         # Build script (Node.js)
├── update.sh            # Build wrapper
└── dist/                # Deployment output (generated)
```

## How to Build

1. Make sure [Node.js](https://nodejs.org/) is installed.
2. Give the build script execute permission:

   ```bash
   chmod +x update.sh
   ```

3. Run the build:

   ```bash
   ./update.sh
   ```

4. The `dist/` directory will be created/updated with all necessary files.

## How to Deploy to GitHub Pages

### Option A: Deploy from `dist/` directly

Push the `dist/` directory to the `gh-pages` branch:

```bash
git subtree push --prefix dist origin gh-pages
```

Or configure GitHub Pages to serve from the `docs/` folder and rename `dist/` to `docs/`.

### Option B: GitHub Actions

Create a workflow that runs `./update.sh` and deploys `dist/` automatically on push.

## Adding New Documents

1. Create your Markdown file in `sourcecode/chs/` or `sourcecode/en/`.
2. Follow the naming convention: `Prefix_Title.md` (e.g., `Appendix5_networking.md`).
3. For bilingual pairing, use matching numeric prefixes in both languages:
   - Chinese: `附录5_网络.md`
   - English: `Appendix5_networking.md`
4. Images go in `sourcecode/img/` and are referenced as `../img/filename.png`.
5. Run `./update.sh` to rebuild.

## File Naming Rules

| File | Pairing Key |
|------|-------------|
| `0_欢迎信.md` / `0_WelcomeLetter.md` | `home` (always first) |
| `附录1_*.md` / `Appendix1_*.md` | `1` |
| `附录2_*.md` / `Appendix2_*.md` | `2` |
| … | … |

Subdirectories are supported and should mirror between `chs/` and `en/`.

### Sub-section numbering (decimals and letters)

Numbered sub-sections may use decimal parts and letter suffixes. The whole
number+letter combination is the pairing key, so files never collide and
each Chinese/English version must use the same one:

| File (chs / en) | Pairing Key |
|-----------------|-------------|
| `Section7_作业1.md` / `Section7_Homework1.md` | `7` |
| `Section7.9_数据来源.md` / `Section7.9_DataLogging.md` | `7.9` |
| `Section8A_多文件与导入.md` / `Section8A_Multifile.md` | `8A` |
| `SectionA_前言.md` / `SectionA_Preface.md` | `A` |

- Letter suffixes are case-insensitive (`8a` pairs with `8A`).
- Sidebar order is natural: `7 < 7.9 < 7.10 < 8 < 8A < 8B < 10`, with
  letter-only keys (`A`, `B`, …) after numeric ones.
- Pairing is driven by this key, so `Section8A` and `Section8B` switch
  languages independently.

## License

Content and code are provided as-is for personal use.
