# Appendix 11: LaTeX

> This appendix introduces the basics of LaTeX. LaTeX is a typesetting system designed for academic papers, especially good for mathematical formulas and references. If you plan to publish papers or write a thesis, you will likely encounter it. This appendix uses Shanghai Jiao Tong University's thesis template (SJTUThesis) as an example to show how to write LaTeX documents with VS Code.  
> **Please note**: The content below is for reference only. Always follow the specific formatting requirements given by your university instructors or journal guidelines.  
> The goal is simply to help you get familiar with LaTeX.

## 11.1 What Is LaTeX?

LaTeX (pronounced "lah-tekh" or "lay-tekh") is different from Word. Word is "what you see is what you get" – you type, and it immediately shows the formatted result. LaTeX is "what you think is what you get" – you write code (a markup language), then compile it to produce a PDF.

**Advantages**:
- Formatting is automatically consistent – no accidental mixing of fonts or sizes
- Mathematical formulas look beautiful
- References are automatically numbered and sorted
- Long documents (dozens or hundreds of pages) rarely become sluggish

**Disadvantages**:
- There is a learning curve; you will need to look things up when errors occur
- You must compile to see the result, unlike Word's live update

Many science and engineering departments require theses and journal papers to be typeset in LaTeX. Your university may provide a template (e.g., the `sjtuthesis` document class), and you simply fill in your content.

## 11.2 Setting Up the Environment: TeX Distribution + VS Code

To write LaTeX, you first need a "compilation environment" – a TeX distribution. The most common one is **TeX Live** (works on Windows, Linux, and macOS).

### 11.2.1 Installing TeX Live

- Download the TeX Live ISO file from Tsinghua University's [TUNA](https://mirrors.tuna.tsinghua.edu.cn/) mirror.
- Extract or mount the ISO, then run `install-tl-windows.bat` (Windows) or `install-tl` (Linux/macOS).
- Choose "full installation" (scheme-full) to avoid missing packages later. The installation may take about half an hour.

After installation, open a command line and type `latex -v`. If you see version information, the installation succeeded.

> **Additional note: About latexmk and Perl (Windows users)**  
> The compilation tool `latexmk` is a Perl script. TeX Live comes with its own Perl interpreter (`tlatexperl`), so normally `latexmk` works right after installation. If you get a "command not found" error when typing `latexmk --version`, it means TeX Live's `bin` directory is not in your system PATH. You can re‑run the installer and check "Add to PATH", or add it manually (see Appendix 9 for environment variables).

### 11.2.2 Installing VS Code and the LaTeX Workshop Extension

If you have not already installed VS Code, do so first (see Appendix 7). Once installed, go to the Extensions Marketplace, search for **LaTeX Workshop**, and install it. This extension provides syntax highlighting, automatic compilation, forward/backward search, and more.

### 11.2.3 Configuring VS Code (Optional but Recommended)

Open VS Code settings (`Ctrl + ,`), search for `latex-workshop.latex.recipe.default`, and change the default compilation recipe to `latexmk (xelatex)`. This will automatically compile whenever you save a `.tex` file.

Alternatively, place a `.vscode/settings.json` file in your project folder:

```json
{
  "latex-workshop.latex.recipe.default": "latexmk (xelatex)",
  "latex-workshop.view.pdf.viewer": "tab"
}
```

## 11.3 A Real Template: SJTUThesis

Shanghai Jiao Tong University's thesis template, SJTUThesis, is a complete LaTeX project. You can find it on GitHub (`sjtug/SJTUThesis`). Download and unzip it, then open the folder in VS Code. You will see several `.tex` files and folders.

### 11.3.1 First Step After Downloading: Try to Compile

> **The three most common problems on the first compilation**  
> 
> **Problem 1: Missing fonts**  
> Many university templates require SimSun, SimHei, and Times New Roman fonts. Windows comes with these fonts, so no issue there. macOS users may need to install SimSun manually (copy it from a Windows machine, or use a command like `brew install font-simsum`). Linux users – I trust you can figure it out.
> 
> **Problem 2: Missing packages**  
> If the compilation error says `! LaTeX Error: File 'xxx.sty' not found.`, you are missing a package. Install it using TeX Live's `tlmgr`:
> ```bash
> tlmgr install package-name
> ```
> If `tlmgr` is not found, run it as administrator (Windows) or with `sudo` (Linux/macOS). You may need to switch to a different mirror – see the TUNA mirror documentation.
> 
> **Problem 3: Encoding errors**  
> An error like `! Package inputenc Error: Unicode char ...` usually means the `.tex` file is not saved as UTF-8. In VS Code, click the encoding indicator at the bottom‑right, then choose “Save with Encoding” → UTF-8.

### 11.3.2 Main File Structure

| File/folder | Purpose |
|-------------|---------|
| `main.tex` | The master file – the "skeleton" of your thesis. It includes chapters via `\input{}` |
| `setup.tex` | Configuration: your name, thesis title, department, etc. |
| `refs.bib` | Reference database (BibTeX format) |
| `contents/` | Holds the chapter `.tex` files, e.g., `intro.tex`, `summary.tex` |
| `figures/` | Holds image files |
| `sjtuthesis.cls` | The template's style file (generally you do not need to modify it) |

### 11.3.3 The Skeleton of `main.tex`

```latex
\documentclass[type=master]{sjtuthesis}   % Specify template and degree type
\input{setup}                              % Load configuration

\begin{document}

\maketitle                                 % Generate title page
\copyrightpage                             % Copyright page

\frontmatter                               % Front matter (abstract, TOC)
\input{contents/abstract}
\tableofcontents
\listoffigures
\listoftables

\mainmatter                                % Main body
\input{contents/intro}
\input{contents/math_and_citations}
\input{contents/floats}
\input{contents/summary}

\appendix                                  % Appendices
\input{contents/app_maxwell_equations}
\input{contents/app_flow_chart}

\backmatter                                % End matter (acknowledgements, etc.)
\input{contents/acknowledgements}
\input{contents/achievements}
\input{contents/resume}

\end{document}
```

When writing your thesis, you will only need to edit the chapter files under `contents/` and the personal information in `setup.tex`. The master file usually does not need to be touched.

## 11.4 How to Write Content

### 11.4.1 Chapters and Headings

Inside a chapter file (e.g., `intro.tex`):

```latex
\chapter{Introduction}          % Level‑1 heading, produces “Chapter 1 Introduction”
\section{Research Background}   % Level‑2 heading
\subsection{Domestic Status}    % Level‑3 heading
\subsubsection{An even smaller heading} % Level‑4 (generally not recommended)
```

Write your text directly. A blank line indicates a paragraph break.

### 11.4.2 Mathematical Formulas (Basics)

Inline formulas are wrapped in `$...$`:

```latex
Einstein's mass–energy equation $E = mc^2$ reveals the relationship between energy and mass.
```

Displayed equations (with numbering) use the `equation` environment:

```latex
\begin{equation}
  \int_0^\infty e^{-x^2} dx = \frac{\sqrt{\pi}}{2}
  \label{eq:gauss}
\end{equation}
```

Common symbols inside formulas:
- Superscripts and subscripts: `a^b`, `a_b`
- Fractions: `\frac{numerator}{denominator}`
- Square roots: `\sqrt{x}`, `\sqrt[n]{x}`
- Summation: `\sum_{i=1}^{n}`
- Integration: `\int_{a}^{b}`
- Greek letters: `\alpha`, `\beta`, `\gamma`, `\pi`, `\mu`, etc.

### 11.4.3 Mathematical Formulas (Intermediate)

**Multi‑line equations (align environment)**

When an equation is too long to fit on one line, or you need to align at equal signs, use the `align` environment:

```latex
\begin{align}
  (a + b)^2 &= a^2 + 2ab + b^2 \\
  (a - b)^2 &= a^2 - 2ab + b^2
\end{align}
```

`&` marks the alignment point (usually placed before the equal sign), and `\\` ends a line. To suppress numbering on a particular line, add `\nonumber` before `\\`:

```latex
\begin{align}
  E &= mc^2 \nonumber \\
  E^2 &= (pc)^2 + (m_0 c^2)^2 \label{eq:relativistic}
\end{align}
```

**Matrices**

Use `pmatrix` (parentheses), `bmatrix` (square brackets), `vmatrix` (vertical bars), etc.:

```latex
\begin{pmatrix}
  1 & 2 & 3 \\
  4 & 5 & 6 \\
  7 & 8 & 9
\end{pmatrix}
```

Ellipses: `\cdots` (horizontal), `\vdots` (vertical), `\ddots` (diagonal):

```latex
\begin{bmatrix}
  a_{11} & a_{12} & \cdots & a_{1n} \\
  a_{21} & a_{22} & \cdots & a_{2n} \\
  \vdots & \vdots & \ddots & \vdots \\
  a_{m1} & a_{m2} & \cdots & a_{mn}
\end{bmatrix}
```

**Piecewise functions (cases environment)**

```latex
\begin{equation}
  f(x) = 
  \begin{cases}
    x^2, & \text{if } x \geq 0 \\
    -x^2, & \text{if } x < 0
  \end{cases}
\end{equation}
```

### 11.4.4 Figures

Place your image files in the `figures/` folder (PDF, PNG, or JPG are recommended). Then insert a figure in a chapter file:

```latex
\begin{figure}[htbp]
  \centering
  \includegraphics[width=0.8\textwidth]{figures/example.png}
  \caption{Figure caption}
  \label{fig:example}
\end{figure}
```

- `[htbp]` tells LaTeX that the figure can be placed here, at the top of a page, at the bottom, or on a separate float page. LaTeX automatically chooses the best position.
- `\centering` centres the figure.
- `\includegraphics[width=0.8\textwidth]{...}` sets the figure width to 80% of the text width.
- `\caption` sets the caption; the figure number (e.g., “Figure 3.1”) is generated automatically.
- `\label` defines a label so you can reference it in the text: `As shown in Figure~\ref{fig:example}`.

> **Note**: Do **not** use spaces or Chinese characters in image file names or paths, otherwise LaTeX may not find them. Use English names like `experiment_setup.png`.

**Subfigures (side‑by‑side figures)**

When you need to place two or more images side‑by‑side with separate (a), (b) captions, use the `subcaption` package:

```latex
\begin{figure}[htbp]
  \centering
  \begin{subfigure}[b]{0.45\textwidth}
    \centering
    \includegraphics[width=\textwidth]{figures/result_a.png}
    \caption{Experimental group}
    \label{fig:result_a}
  \end{subfigure}
  \hfill  % adds horizontal space between the subfigures
  \begin{subfigure}[b]{0.45\textwidth}
    \centering
    \includegraphics[width=\textwidth]{figures/result_b.png}
    \caption{Control group}
    \label{fig:result_b}
  \end{subfigure}
  \caption{Comparison of experimental results}
  \label{fig:results}
\end{figure}
```

Make sure to load the `subcaption` package at the beginning of the document: `\usepackage{subcaption}`. University templates usually already include it.

### 11.4.5 Tables

Writing tables in LaTeX can be somewhat tedious. A practical approach is to use an online tool (like Tables Generator) to generate the code and then paste it. Here is a simple three‑line table example:

```latex
\begin{table}[htbp]
  \centering
  \caption{Experimental data}
  \label{tab:data}
  \begin{tabular}{ccc}
    \toprule
    ID & Temperature (℃) & Pressure (kPa) \\
    \midrule
    1 & 25 & 101.3 \\
    2 & 30 & 102.1 \\
    3 & 35 & 103.0 \\
    \bottomrule
  \end{tabular}
\end{table}
```

The `booktabs` package provides `\toprule`, `\midrule`, and `\bottomrule`. The template should already load it.

**Long tables that span multiple pages (longtable)**

If you have a very long table (e.g., several pages of experimental data), a normal `table` + `tabular` cannot break across pages – the table will be cut off at the page boundary. In this case, use the `longtable` package:

```latex
\usepackage{longtable}  % place in the preamble

...

\begin{longtable}{ccc}
  \caption{Summary of experimental data over many years} \\
  \toprule
  ID & Temperature (℃) & Pressure (kPa) \\
  \midrule
  \endfirsthead  % header for the first page
  \toprule
  ID & Temperature (℃) & Pressure (kPa) \\
  \midrule
  \endhead  % header for subsequent pages
  \bottomrule
  \multicolumn{3}{r}{Continued on next page} \\
  \endfoot  % footer for each page (except the last)
  \bottomrule
  \endlastfoot  % footer for the last page
  
  1 & 25 & 101.3 \\
  2 & 30 & 102.1 \\
  ...   (many rows can go here)
  100 & 28 & 101.8 \\
\end{longtable}
```

`longtable` differs from ordinary tables in that it can break across pages, and the header is repeated at the top of each page. The downside is less flexible float placement, but for long tables it is the only practical option.

### 11.4.6 References (Basics)

References are stored in a `.bib` file in BibTeX format. A sample entry (from `refs.bib`):

```bibtex
@article{Li1999,
  author  = {Xiaodong Li and Qinghong Zhang and Jinlin Ye},
  title   = {Some theoretical issues in climatology research},
  journal = {Journal of Peking University: Natural Science Edition},
  year    = {1999},
  volume  = {35},
  number  = {1},
  pages   = {101--106}
}
```

In the body of your thesis, cite it:

```latex
... some researchers point out \cite{Li1999} ... 
... you can also cite multiple works \cite{Li1999, Yu2001} ...
```

After compilation, the reference list is automatically generated where `\printbibliography` appears. Citation numbers and the list formatting follow the required standard (e.g., GB/T 7714 for Chinese theses).

**Important notes on the `author` field**:
- Separate multiple authors with `and` (not commas): `{Xiaodong Li and Qinghong Zhang and Jinlin Ye}`
- LaTeX uses `and` to distinguish authors, automatically producing “Li, X. D., Zhang, Q. H., & Ye, J. L.” or “Li et al.” as needed
- Do **not** write `{Xiaodong Li, Qinghong Zhang, Jinlin Ye}` – the whole string would be treated as a single author

### 11.4.7 References (Intermediate)

**Where to get BibTeX entries for Chinese publications**

- **CNKI (China National Knowledge Infrastructure)**: On the article detail page, look for “Export” → “BibTeX” → copy the content. However, CNKI‑exported BibTeX often has problems (e.g., missing `and` between authors, incorrect `year` format). After exporting, manually check and fix:
  - Check `author`: if it shows `{李晓东，张庆红，叶瑾琳}`, change it to `{李晓东 and 张庆红 and 叶瑾琳}`
  - Check `year`: it should be `{1999}`, not `{1999-01-01}`
  - Check journal name: CNKI sometimes puts the journal name in `booktitle` instead of `journal` – move it if needed

- **Google Scholar**: Below the search result, click “Cite” → “BibTeX” and copy directly. The quality is generally better than CNKI.

- **Recommended workflow**: Use **Zotero** (free reference manager). With the browser plugin, you can capture references with one click. Zotero automatically recognises the fields, and you can export a `.bib` file. Zotero also integrates with Word/LaTeX, greatly reducing manual entry errors.

**Why references might not show up**

If after compilation you see `[?]` in the text, or the reference list is empty, the compilation order is likely wrong. The correct sequence is:

1. `xelatex main.tex`   → generates auxiliary files
2. `biber main`         → processes the `.bib` file
3. `xelatex main.tex`   → writes references into the document
4. `xelatex main.tex`   → resolves cross‑references (optional but recommended)

With VS Code’s LaTeX Workshop using the `latexmk` recipe, this order is handled automatically. If you use the command line, run:

```bash
latexmk -xelatex main.tex
```

`latexmk` automatically detects how many passes are needed and whether `biber` is required.

### 11.4.8 Three Common Pitfalls for Beginners

**Pitfall 1: Blank lines causing paragraph indentation**

In LaTeX, a blank line means a paragraph break. If you have two consecutive lines of text separated by a blank line, the second paragraph will have an indented first line (as is customary in Chinese typography). Many people do not want this indentation and manually add `\noindent` – but doing that for every paragraph is tedious.

The correct approach:
- For Chinese theses, first‑line indentation is usually required, so you do not need to change anything. If you really need a paragraph without indentation (e.g., for a figure caption), write `\noindent` just before that paragraph.
- If you want to disable indentation globally, add `\setlength{\parindent}{0pt}` in the preamble, but this will deviate from most university formatting requirements – not recommended.

**Pitfall 2: Abusing `\\` to force line breaks**

Using `\\` inside a regular paragraph forces a line break but does **not** create a new paragraph. The next line will not have indentation, and line breaks may appear in odd places.

Wrong:
```latex
This is the first line\\
This is the second line
```

Right: Let LaTeX wrap lines automatically. If you genuinely need a line break without a new paragraph, use `\newline`. If you want a new paragraph, use a blank line.

**Pitfall 3: Figures not showing**

Missing figures usually have three causes:
- Wrong path: `\includegraphics{figures/图片.png}` but the folder is named `figure` (missing the ‘s’) – case sensitivity also matters
- Spaces or Chinese characters in the file name: `实验结果.png` → rename to `result.png`
- Unsupported format: When compiling with `xelatex`, supported formats are PNG, JPG, and PDF. If you use `pdflatex`, PDF is the most reliable. As a rule, use PDF (especially for vector graphics)

Debugging tip: Look at the compilation log (in VS Code, LaTeX Workshop’s “LaTeX Compiler” output window) and search for `! LaTeX Error: File 'xxx' not found`.

## 11.5 How to Compile

### 11.5.1 Compiling in VS Code

Press `Ctrl + Alt + B` (or click “Build LaTeX project” in the left sidebar) – this triggers automatic compilation. After a successful build, `main.pdf` is generated. LaTeX Workshop uses the `latexmk` recipe by default, which handles multiple passes and references automatically.

**If compilation fails**:
- Open VS Code’s “Output” panel (`Ctrl + Shift + U`), then choose “LaTeX Compiler” from the dropdown menu
- Look for error messages in red, and search online for the error text
- See the Extended Reading section for common error messages and solutions

### 11.5.2 Compiling from the Command Line

If you prefer the command line, navigate to the project folder (where `main.tex` is) and run:

```bash
latexmk -xelatex main.tex
```

If you get a “command not found” error for `latexmk`, the Perl environment is not set up correctly. Refer back to the extra note in section 11.2.1.

If you prefer not to use `latexmk`, you can manually run the steps:

```bash
xelatex main.tex
biber main
xelatex main.tex
xelatex main.tex
```

**Note**: The `biber` command handles references. Some older templates use `bibtex` instead (command: `bibtex main`). The modern version of SJTUThesis uses `biber`.

### 11.5.3 Using the Template’s Built‑in Compilation Script

Many LaTeX templates provide a `Makefile` (Linux/macOS) or `Compile.bat` (Windows). For SJTUThesis:
- Windows: double‑click `Compile.bat`, or run it from the command line
- Linux/macOS: run `make` in the terminal (requires `make` to be installed)

## 11.6 A Minimal Example

Below is a “minimal paper fragment” that does not rely on any complex template. Copy it into a `.tex` file and try compiling it.

```latex
\documentclass{article}
\usepackage[UTF8]{ctex}
\usepackage{graphicx}
\usepackage{amsmath}
\usepackage{subcaption}   % subfigures
\usepackage{longtable}     % multi‑page tables
\usepackage{booktabs}      % three‑line tables

\title{Schrödinger's Equation in Quantum Mechanics}
\author{San Zhang}
\date{\today}

\begin{document}

\maketitle

\section{Introduction}
Schrödinger's equation is the fundamental equation of quantum mechanics, proposed by the Austrian physicist Erwin Schrödinger in 1926.

\section{Time‑Independent Schrödinger Equation}
For one‑dimensional stationary problems, the Schrödinger equation can be written as:
\begin{equation}
  -\frac{\hbar^2}{2m} \frac{d^2\psi(x)}{dx^2} + V(x)\psi(x) = E\psi(x)
  \label{eq:schrodinger}
\end{equation}
where $\hbar$ is the reduced Planck constant, $m$ is the particle mass, $V(x)$ is the potential energy function, and $E$ is the energy.

\section{Infinite Square Well}
Consider an infinite square well:
\begin{equation}
  V(x) = 
  \begin{cases}
    0, & 0 < x < a, \\
    \infty, & \text{otherwise}
  \end{cases}
\end{equation}
Solving equation~\eqref{eq:schrodinger} yields the energy eigenvalues:
\begin{equation}
  E_n = \frac{n^2\pi^2\hbar^2}{2ma^2}, \quad n=1,2,3,\dots
\end{equation}

\section{Multi‑line Equation Example}
\begin{align}
  (a+b)^2 &= a^2 + 2ab + b^2 \\
  (a-b)^2 &= a^2 - 2ab + b^2 \nonumber
\end{align}

\section{Matrix Example}
\begin{pmatrix}
  1 & 2 \\
  3 & 4
\end{pmatrix}

\section{Conclusion}
This article briefly introduced Schrödinger's equation and its application in a one‑dimensional infinite square well.

\end{document}
```

Compiling this produces a PDF document with formulas, section headings, automatic numbering, etc.

---

# Extended Reading

> The following content covers more advanced LaTeX topics. Refer to it when you outgrow the basics.

## Extended 1: What Are Packages (Macros)?

LaTeX’s functionality is extended through “packages”. When you write `\usepackage{amsmath}` at the beginning of your document, you are loading a package for mathematical formulas. Commonly used packages:
- `graphicx`: inserting images
- `amsmath`, `amssymb`: advanced mathematical features
- `geometry`: setting page margins
- `hyperref`: generating hyperlinks and bookmarks
- `booktabs`: three‑line tables
- `longtable`: multi‑page tables
- `subcaption`: subfigures (side‑by‑side figures)
- `algorithm2e`: algorithm environments

## Extended 2: Reference Management Tools

If you have many references, consider using dedicated reference management software such as **Zotero** (free) or **JabRef**. They allow you to import reference entries from journal websites with a single click and then export a `.bib` file. Zotero also has a plugin (Better BibTeX) that generates stable citekeys and can integrate directly with VS Code or Overleaf.

## Extended 3: Common Compilation Errors and Solutions

| Error message | Likely cause | Solution |
|---------------|--------------|----------|
| `! Undefined control sequence.` | An undefined command was used | Check spelling, or load the required package |
| `! LaTeX Error: File 'xxx.sty' not found.` | Missing package | `tlmgr install xxx` |
| `! Package inputenc Error: Unicode char ...` | Encoding issue | Save the `.tex` file as UTF‑8 |
| `! LaTeX Error: Unknown float option 'H'` | Forgot to load the `float` package | Add `\usepackage{float}`, or change `[H]` to `[htbp]` |
| `! Package biblatex Error: Please (re)run Biber` | Didn't run `biber` | Run `biber main` then recompile |
| `[?]` appears in the text | Incorrect citation label | Check that the key in `\cite{xxx}` matches the key in the `.bib` file |
| Figure not shown; only the file name appears | Unsupported format or wrong path | Check the file name (no spaces or Chinese characters); try PDF format |

## Extended 4: Forward and Inverse Search

- **Forward search**: In the `.tex` file, hold `Ctrl` and click on a line – you jump to the corresponding location in the PDF.
- **Inverse search**: Double‑click a line in the PDF – you jump back to the corresponding line in the `.tex` file.

LaTeX Workshop supports forward search by default. Inverse search on Windows may require extra configuration (setting SumatraPDF as the default PDF viewer). For beginners, this is not essential.

## Extended 5: Template Document Class (`.cls`) and Configuration (`.sty`)

The style of the SJTU template is written in `sjtuthesis.cls`. Ordinary users do not need to modify it. Only if you are creating a document class from scratch would you need to write a `.cls` file. For a thesis, using the template provided by your university is the safest approach.

## Extended 6: Overleaf – Online LaTeX Editor

If you do not want to install TeX Live locally, you can use an online LaTeX editor – for example, **Overleaf** (`overleaf.com`), or Chinese alternatives like **TeXPage** or **SJTU LaTeX Document Assistant**. You simply register an account, upload a template (or choose one from the template library), and write LaTeX in your browser with real‑time preview. This is an excellent way for beginners to get started quickly.

**Comparison between Overleaf and a local environment**:

| Feature | Overleaf | Local (TeX Live + VS Code) |
|---------|----------|----------------------------|
| Installation cost | Zero – register and use | Download 4–5 GB, ~30 minutes to install |
| Compilation speed | Depends on server load | Depends on your computer (generally faster) |
| Offline use | No | Yes |
| Chinese template support | Need to upload fonts | Fonts managed by the OS |
| Collaborative editing | Native support | Needs Git or other tools |

For beginners, it is recommended to start with Overleaf to get your first document working, then decide whether to install a local environment.