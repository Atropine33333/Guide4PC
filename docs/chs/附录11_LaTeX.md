# 附录十一：LaTeX

> 这个附录介绍 LaTeX 的基本用法。LaTeX 是一种面向学术论文的排版系统，尤其适合写数学公式和参考文献。如果你将来要发论文、写毕业论文，可能会遇到它。本附录以上海交通大学的学位论文模板 SJTUThesis 为例，演示如何用 VS Code 编写 LaTeX 文档。  
> 请注意：以下内容仅供参考，具体格式请一定一定依照你的大学老师的要求或者期刊要求。  
> 只是为了帮助大家认识 LaTeX

## 11.1 LaTeX 是什么

LaTeX（读作“拉泰赫”或“雷泰赫”）和 Word 不同。Word 是“所见即所得”——你打字，它直接显示排版效果。LaTeX 是“所想即所得”——你写代码（标记语言），然后编译成 PDF。

**优点**：
- 格式自动统一，不会出现“这个标题是宋体三号，那个是黑体小三”
- 数学公式排版非常漂亮
- 参考文献自动编号、自动排序
- 长文档（几十页甚至几百页）不容易卡顿

**缺点**：
- 有学习成本，遇到错误需要查资料
- 编译一次才能看到结果，不像 Word 实时更新

大学里很多理工科院系的毕业论文、期刊论文都要求用 LaTeX 排版。学校可能会提供一个模板（比如 `sjtuthesis` 文档类），你只需要在模板里填入自己的内容。

## 11.2 环境搭建：TeX 发行版 + VS Code

要写 LaTeX，首先需要安装一个“编译环境”——TeX 发行版。最常用的是 **TeX Live**（Windows、Linux、macOS 都支持）。

### 11.2.1 安装 TeX Live

- 从清华大学 [TUNA](https://mirrors.tuna.tsinghua.edu.cn/#) 镜像站下载 TeX Live 的 ISO 文件。
- 解压或挂载后，运行 `install-tl-windows.bat`（Windows）或 `install-tl`（Linux/macOS）。
- 安装时选择“完整安装”（scheme-full），省得以后缺宏包。安装过程可能需要半小时。

安装完成后，打开命令行输入 `latex -v`，显示版本信息即成功。

> **补充：关于 latexmk 和 Perl（Windows 用户注意）**  
> LaTeX 编译工具 `latexmk` 是一个 Perl 脚本。TeX Live 自带了 Perl 解释器（`tlatexperl`），所以正常情况下安装 TeX Live 后 `latexmk` 就能直接用。如果你在命令行输入 `latexmk --version` 提示“不是内部或外部命令”，说明 TeX Live 的 `bin` 目录没有加入系统 PATH。可以重新运行安装程序勾选“添加到 PATH”，或者手动添加（附录九讲过环境变量）。

### 11.2.2 安装 VS Code 与 LaTeX Workshop 插件

如果你还没有装 VS Code，可以先装好（参考附录七）。装好后，在扩展商店里搜索 **LaTeX Workshop** 并安装。这个插件提供了语法高亮、自动编译、正向反向搜索等功能。

### 11.2.3 配置 VS Code（可选但推荐）

打开 VS Code 设置（`Ctrl + ,`），搜索 `latex-workshop.latex.recipe.default`，把默认编译方案改成 `latexmk (xelatex)`。这样每次保存 `.tex` 文件时会自动编译。

或者直接在项目里放一个 `.vscode/settings.json`：

```json
{
  "latex-workshop.latex.recipe.default": "latexmk (xelatex)",
  "latex-workshop.view.pdf.viewer": "tab"
}
```

## 11.3 一个真实模板：SJTUThesis

上海交通大学的学位论文模板 SJTUThesis 是一个完整的 LaTeX 项目。你可以在 GitHub 上搜到它（`sjtug/SJTUThesis`）。下载后解压，用 VS Code 打开文件夹，你会看到一堆 `.tex` 文件和文件夹。

### 11.3.1 下载后第一步：尝试编译

> **第一次编译最常见的三个问题**  
> 
> **问题1：缺少字体**  
> 很多学校模板要求宋体（SimSun）、黑体（SimHei）、Times New Roman。Windows 系统自带这些字体，一般没问题。macOS 用户需要手动安装 SimSun（可以从 Windows 电脑复制，或者用 `brew install font-simsim` 之类的命令）。Linux 用户我相信可以折腾明白，不再赘述。  
> 
> **问题2：缺少宏包**  
> 编译时报错 `! LaTeX Error: File 'xxx.sty' not found.`，说明缺少某个宏包。用 TeX Live 自带的 `tlmgr` 安装：
> ```bash
> tlmgr install 宏包名
> ```
> 如果 `tlmgr` 命令找不到，先以管理员身份运行（Windows）或 `sudo tlmgr`（Linux/macOS）。国内的网络可能需要换源，参考 TUNA 镜像站的说明。
> 
> **问题3：编码报错**  
> 报错 `! Package inputenc Error: Unicode char ...`，通常是因为 `.tex` 文件保存格式不是 UTF-8。在 VS Code 右下角点击编码，选择“Save with Encoding” → UTF-8。

### 11.3.2 主要文件结构

| 文件/文件夹 | 作用 |
|-------------|------|
| `main.tex` | 主文件，相当于论文的“骨架”，里面用 `\input{}` 包含各个章节 |
| `setup.tex` | 配置信息：个人信息、论文标题、院系等 |
| `refs.bib` | 参考文献数据库（BibTeX 格式） |
| `contents/` | 存放各章节的 `.tex` 文件，例如 `intro.tex`（绪论）、`summary.tex`（总结） |
| `figures/` | 存放图片文件 |
| `sjtuthesis.cls` | 模板的样式文件（一般不需要修改） |

### 11.3.3 主文件 `main.tex` 的骨架

```latex
\documentclass[type=master]{sjtuthesis}   % 指定模板和论文类型（硕士/博士/学士）
\input{setup}                              % 载入配置

\begin{document}

\maketitle                                 % 生成标题页
\copyrightpage                             % 版权页

\frontmatter                               % 前置部分（摘要、目录）
\input{contents/abstract}
\tableofcontents
\listoffigures
\listoftables

\mainmatter                                % 正文部分
\input{contents/intro}
\input{contents/math_and_citations}
\input{contents/floats}
\input{contents/summary}

\appendix                                  % 附录
\input{contents/app_maxwell_equations}
\input{contents/app_flow_chart}

\backmatter                                % 结尾部分（致谢、成果、简历）
\input{contents/acknowledgements}
\input{contents/achievements}
\input{contents/resume}

\end{document}
```

你平时写论文只需要修改 `contents/` 下面的各个章节文件，以及 `setup.tex` 里的个人信息。主文件一般不需要动。

## 11.4 如何写内容

### 11.4.1 章节与标题

在章节文件（如 `intro.tex`）里：

```latex
\chapter{绪论}          % 一级标题，生成“第1章 绪论”
\section{研究背景}      % 二级标题
\subsection{国内现状}   % 三级标题
\subsubsection{一个更小的标题} % 四级标题（一般不推荐用太多）
```

正文直接写文字，空一行表示分段。

### 11.4.2 数学公式（基础篇）

行内公式用 `$...$` 包裹：

```latex
爱因斯坦质能方程 $E = mc^2$ 揭示了能量与质量的关系。
```

独立公式（带编号）用 `equation` 环境：

```latex
\begin{equation}
  \int_0^\infty e^{-x^2} dx = \frac{\sqrt{\pi}}{2}
  \label{eq:gauss}
\end{equation}
```

公式内常用符号：
- 上下标：`a^b`, `a_b`
- 分数：`\frac{分子}{分母}`
- 根号：`\sqrt{x}`, `\sqrt[n]{x}`
- 求和：`\sum_{i=1}^{n}`
- 积分：`\int_{a}^{b}`
- 希腊字母：`\alpha`, `\beta`, `\gamma`, `\pi`, `\mu` 等

### 11.4.3 数学公式（进阶篇）

**多行公式（align 环境）**

当公式太长需要换行，或者需要对齐等号时，用 `align` 环境：

```latex
\begin{align}
  (a + b)^2 &= a^2 + 2ab + b^2 \\
  (a - b)^2 &= a^2 - 2ab + b^2
\end{align}
```

`&` 表示对齐的位置（通常放在等号前面），`\\` 表示换行。如果某一行不想编号，在 `\\` 前面加 `\nonumber`：

```latex
\begin{align}
  E &= mc^2 \nonumber \\
  E^2 &= (pc)^2 + (m_0 c^2)^2 \label{eq:relativistic}
\end{align}
```

**矩阵**

用 `pmatrix`（圆括号）、`bmatrix`（方括号）、`vmatrix`（竖线）等环境：

```latex
\begin{pmatrix}
  1 & 2 & 3 \\
  4 & 5 & 6 \\
  7 & 8 & 9
\end{pmatrix}
```

省略号可以用 `\cdots`（水平）、`\vdots`（垂直）、`\ddots`（对角）：

```latex
\begin{bmatrix}
  a_{11} & a_{12} & \cdots & a_{1n} \\
  a_{21} & a_{22} & \cdots & a_{2n} \\
  \vdots & \vdots & \ddots & \vdots \\
  a_{m1} & a_{m2} & \cdots & a_{mn}
\end{bmatrix}
```

**分段函数（cases 环境）**

```latex
\begin{equation}
  f(x) = 
  \begin{cases}
    x^2, & \text{if } x \geq 0 \\
    -x^2, & \text{if } x < 0
  \end{cases}
\end{equation}
```

### 11.4.4 图片

在 `figures/` 文件夹里放好图片（建议 PDF、PNG、JPG 格式），然后在章节文件里插入：

```latex
\begin{figure}[htbp]
  \centering
  \includegraphics[width=0.8\textwidth]{figures/example.png}
  \caption{这是图题}
  \label{fig:example}
\end{figure}
```

- `[htbp]` 表示图片可以放在当前位置、页面顶部、底部或单独一页。LaTeX 会自动选择最合适的位置。
- `\centering` 让图片居中。
- `\includegraphics` 的 `width=0.8\textwidth` 表示图片宽度占页面宽度的 80%。
- `\caption` 设置图题，会自动生成“图 3.1”之类的编号。
- `\label` 定义一个标签，方便在正文里引用：`如图~\ref{fig:example} 所示`。

> **注意**：图片文件名和路径里**不要有中文或空格**，否则编译可能找不到图片。推荐用英文命名，如 `experiment_setup.png`。

**子图（subfigure）—— 并列两张或多张图**

当你需要把两张图并排放置，分别有 (a)、(b) 子图题时，用 `subcaption` 宏包：

```latex
\begin{figure}[htbp]
  \centering
  \begin{subfigure}[b]{0.45\textwidth}
    \centering
    \includegraphics[width=\textwidth]{figures/result_a.png}
    \caption{实验组结果}
    \label{fig:result_a}
  \end{subfigure}
  \hfill  % 在两个子图之间加一些水平空白
  \begin{subfigure}[b]{0.45\textwidth}
    \centering
    \includegraphics[width=\textwidth]{figures/result_b.png}
    \caption{对照组结果}
    \label{fig:result_b}
  \end{subfigure}
  \caption{实验结果对比}
  \label{fig:results}
\end{figure}
```

注意需要在文档开头加载 `subcaption` 宏包：`\usepackage{subcaption}`。学校模板通常已经预置了。

### 11.4.5 表格

LaTeX 的表格写起来稍麻烦，推荐用在线工具（如 Tables Generator）生成代码后粘贴。下面是一个简单的三线表示例：

```latex
\begin{table}[htbp]
  \centering
  \caption{实验数据}
  \label{tab:data}
  \begin{tabular}{ccc}
    \toprule
    编号 & 温度 (℃) & 压力 (kPa) \\
    \midrule
    1 & 25 & 101.3 \\
    2 & 30 & 102.1 \\
    3 & 35 & 103.0 \\
    \bottomrule
  \end{tabular}
\end{table}
```

需要加载 `booktabs` 宏包才能用 `\toprule`、`\midrule`、`\bottomrule`。模板已经预置了。

**跨页长表格（longtable）**

如果你的表格非常长（比如好几页的实验数据），普通 `table` + `tabular` 无法跨页——表格会在当前页被截断。这时需要用 `longtable` 宏包：

```latex
\usepackage{longtable}  % 放在文档开头

...

\begin{longtable}{ccc}
  \caption{多年实验数据汇总} \\
  \toprule
  编号 & 温度 (℃) & 压力 (kPa) \\
  \midrule
  \endfirsthead  % 第一页的表头
  \toprule
  编号 & 温度 (℃) & 压力 (kPa) \\
  \midrule
  \endhead  % 后续页的表头
  \bottomrule
  \multicolumn{3}{r}{续下页} \\
  \endfoot  % 每页底部
  \bottomrule
  \endlastfoot  % 最后一页底部
  
  1 & 25 & 101.3 \\
  2 & 30 & 102.1 \\
  ...   （这里可以有很多行）
  100 & 28 & 101.8 \\
\end{longtable}
```

`longtable` 和普通表格的主要区别：表格可以跨页，表头会在每一页顶部重复出现。缺点是浮动位置控制不如 `table` 灵活，但长表格只能用这种方式。

### 11.4.6 参考文献（基础篇）

参考文献以 BibTeX 格式存放在 `.bib` 文件中。一条文献的条目如下（来自 `refs.bib`）：

```bibtex
@article{Li1999,
  author  = {李晓东 and 张庆红 and 叶瑾琳},
  title   = {气候学研究的若干理论问题},
  journal = {北京大学学报: 自然科学版},
  year    = {1999},
  volume  = {35},
  number  = {1},
  pages   = {101--106}
}
```

在正文中引用：

```latex
... 有学者指出 \cite{Li1999} ... 
... 也可以引用多篇 \cite{Li1999, Yu2001} ...
```

编译后，参考文献列表会自动生成在 `\printbibliography` 命令所在的位置。引文编号和列表条目会自动按照 GB/T 7714 国家标准格式化。

**关于 `author` 字段的写法**：
- 多个作者之间用 `and` 连接（不是逗号）：`{李晓东 and 张庆红 and 叶瑾琳}`
- LaTeX 会根据 `and` 来区分不同的作者，自动生成“李晓东, 张庆红, 叶瑾琳”或“李晓东 等”的格式
- 不要写成 `{李晓东, 张庆红, 叶瑾琳}`，否则整个字符串会被当成一个作者

### 11.4.7 参考文献（进阶篇）

**中文文献从哪里获取 BibTeX**

- **知网（CNKI）**：在文献详情页找到“导出” → “BibTeX” → 复制内容。但知网导出的 BibTeX 格式经常有问题（比如 `author` 字段的 `and` 缺失、`year` 字段格式不对）。建议导出后再手动检查和修正：
  - 检查 `author`：如果是 `{李晓东，张庆红，叶瑾琳}`，改成 `{李晓东 and 张庆红 and 叶瑾琳}`
  - 检查 `year`：确保是 `{1999}` 而不是 `{1999-01-01}` 这种带日期的格式
  - 检查期刊名：知网有时会把期刊名放到 `journal` 字段，有时放到 `booktitle`，需要手动移一下

- **Google Scholar**：在搜索结果下方点击“引用” → “BibTeX”，直接复制即可，质量比知网好。

- **推荐工作流**：使用 **Zotero**（免费文献管理工具）。从浏览器插件一键抓取文献，Zotero 会自动识别字段，然后导出 `.bib` 文件。Zotero 还可以和 Word/Latex 联动，大幅减少手动输入错误。

**参考文献不显示的常见原因**

编译后正文里有 `[?]`，或者参考文献列表是空的，通常是因为编译顺序不对。完整编译顺序应该是：

1. `xelatex main.tex`   → 生成辅助文件
2. `biber main`         → 处理 `.bib` 文件，生成参考文献数据
3. `xelatex main.tex`   → 把参考文献写入文档
4. `xelatex main.tex`   → 解决交叉引用（可选但推荐）

在 VS Code 中用 LaTeX Workshop 的 `latexmk` 方案会自动处理这些顺序，不需要手动执行。如果你用命令行，可以一次运行：

```bash
latexmk -xelatex main.tex
```

`latexmk` 会自动检测需要运行几次、是否需要运行 `biber`。

### 11.4.8 小白最容易踩的三个坑

**坑1：空行导致段落缩进**

在 LaTeX 中，空行表示分段。连续两行文字中间如果有一个空行，第二段的开头会自动缩进两个字符（中文习惯）。很多人不想要这个缩进，就手动加 `\noindent`，但每个段落都加太麻烦。

正确做法：
- 中文论文通常要求首行缩进，不需要动。如果你确实不需要缩进（比如图表的说明文字），可以在那个段落前面写 `\noindent`
- 如果全文都不想缩进，可以在导言区加 `\setlength{\parindent}{0pt}`，但这会偏离大多数学校的格式要求，不推荐

**坑2：滥用 `\\` 强行换行**

在正文段落里用 `\\` 会强制换行，但不会产生新的段落，下一行开头不会有缩进，还可能产生奇怪的断行位置。

错误示例：
```latex
这是第一行\\
这是第二行
```

正确做法：让 LaTeX 自动换行。如果确实需要单独成行且不缩进，用空行分段，或者用 `\newline`。

**坑3：图片不显示**

图片不显示通常有三个原因：
- 路径错误：`\includegraphics{figures/图片.png}`，但文件夹名字是 `figure`（少了一个 s）—— 大小写也要一致
- 文件名有中文或空格：`实验结果.png` → 改成 `result.png`
- 格式不支持：LaTeX 编译用 `xelatex` 时支持 PNG、JPG、PDF。如果用了 `pdflatex`，PDF 格式最稳定。建议统一用 PDF 格式（尤其是矢量图）

调试方法：查看编译日志（VS Code 中 LaTeX Workshop 的 “LaTeX Compiler” 输出窗口），找到类似 `! LaTeX Error: File 'xxx' not found` 的错误信息。

## 11.5 如何编译

### 11.5.1 在 VS Code 中编译

按 `Ctrl + Alt + B`（或者点左侧工具栏的 “Build LaTeX project”）就会自动编译。编译成功后，会生成 `main.pdf`。LaTeX Workshop 默认使用 `latexmk` 方案，会自动处理多次编译和参考文献。

**如果编译失败**：
- 打开 VS Code 的“输出”面板（`Ctrl + Shift + U`），在下拉菜单里选择 “LaTeX Compiler”
- 查看红色字体的错误信息，根据提示去搜索
- 常见错误信息及解决方法见补充阅读部分

### 11.5.2 在命令行中编译

如果你习惯用命令行，在项目文件夹（`main.tex` 所在的目录）里运行：

```bash
latexmk -xelatex main.tex
```

如果提示 `latexmk` 不是内部或外部命令，说明 Perl 环境没配好。回到 11.2.1 节查看补充说明。

如果你不想用 `latexmk`，也可以手动按顺序执行：

```bash
xelatex main.tex
biber main
xelatex main.tex
xelatex main.tex
```

**注意**：`biber` 命令是处理参考文献的，如果你的模板用的是 `bibtex`（老方法），命令是 `bibtex main`。SJTUThesis 新版用的是 `biber`。

### 11.5.3 使用模板自带的编译脚本

很多 LaTeX 模板会提供一个 `Makefile`（Linux/macOS）或 `Compile.bat`（Windows）。以 SJTUThesis 为例：

- Windows：双击 `Compile.bat`，或者打开命令行运行
- Linux/macOS：在终端运行 `make`（需要安装 `make` 工具）

## 11.6 一个最小示例

下面是一个不依赖任何复杂模板的“最小论文片段”，你可以把它复制到 `.tex` 文件里，编译试试看。

```latex
\documentclass{article}
\usepackage[UTF8]{ctex}
\usepackage{graphicx}
\usepackage{amsmath}
\usepackage{subcaption}   % 子图
\usepackage{longtable}     % 跨页长表格
\usepackage{booktabs}      % 三线表

\title{量子力学中的薛定谔方程}
\author{张三}
\date{\today}

\begin{document}

\maketitle

\section{引言}
薛定谔方程是量子力学的基本方程，由奥地利物理学家薛定谔在1926年提出。

\section{定态薛定谔方程}
对于一维定态问题，薛定谔方程可以写为：
\begin{equation}
  -\frac{\hbar^2}{2m} \frac{d^2\psi(x)}{dx^2} + V(x)\psi(x) = E\psi(x)
  \label{eq:schrodinger}
\end{equation}
其中 $\hbar$ 是约化普朗克常数，$m$ 是粒子质量，$V(x)$ 是势能函数，$E$ 是能量。

\section{无限深方势阱}
考虑一个无限深方势阱：
\begin{equation}
  V(x) = 
  \begin{cases}
    0, & 0 < x < a, \\
    \infty, & \text{其他}
  \end{cases}
\end{equation}
求解方程~\eqref{eq:schrodinger} 可得能量本征值：
\begin{equation}
  E_n = \frac{n^2\pi^2\hbar^2}{2ma^2}, \quad n=1,2,3,\dots
\end{equation}

\section{多行公式示例}
\begin{align}
  (a+b)^2 &= a^2 + 2ab + b^2 \\
  (a-b)^2 &= a^2 - 2ab + b^2 \nonumber
\end{align}

\section{矩阵示例}
\begin{pmatrix}
  1 & 2 \\
  3 & 4
\end{pmatrix}

\section{结论}
本文简要介绍了薛定谔方程及其在一维无限深势阱中的应用。

\end{document}
```

编译后会生成一篇 PDF 文档，包含公式、节标题、自动编号等。

---

# 补充阅读

> 以下内容为更深入的 LaTeX 知识，当你不满足于基础用法时可以查阅。

## 补充1：宏包（Package）是什么

LaTeX 的功能是通过“宏包”扩展的。你在文档开头写 `\usepackage{amsmath}` 就是加载一个数学公式宏包。常用的宏包：
- `graphicx`：插入图片
- `amsmath`、`amssymb`：增强数学功能
- `geometry`：设置页边距
- `hyperref`：生成超链接和书签
- `booktabs`：三线表
- `longtable`：跨页长表格
- `subcaption`：子图（并排多张图）
- `algorithm2e`：算法环境

## 补充2：文献管理工具

如果参考文献很多，建议使用专门的文献管理软件，如 **Zotero**（免费）或 **JabRef**。它们可以帮你从期刊网站一键导入文献条目，然后导出 `.bib` 文件。Zotero 还有插件（Better BibTeX）可以自动生成稳定的 citekey，并直接与 VS Code 或 Overleaf 联动。

## 补充3：常见编译错误及解决

| 错误信息 | 可能原因 | 解决方法 |
|----------|----------|----------|
| `! Undefined control sequence.` | 用了未定义的命令 | 检查拼写，或者加载相应的宏包 |
| `! LaTeX Error: File 'xxx.sty' not found.` | 缺少宏包 | `tlmgr install xxx` 安装 |
| `! Package inputenc Error: Unicode char ...` | 编码问题 | 确保 `.tex` 文件保存为 UTF-8 格式 |
| `! LaTeX Error: Unknown float option 'H'` | 忘记加载 `float` 宏包 | 加 `\usepackage{float}`，或者把 `[H]` 改成 `[htbp]` |
| `! Package biblatex Error: Please (re)run Biber` | 没有运行 `biber` | 运行 `biber main` 再重新编译 |
| 正文里显示 `[?]` | 引用标签写错了 | 检查 `\cite{xxx}` 里的 key 是否和 `.bib` 里的一致 |
| 图片不显示，只显示文件名 | 文件格式不支持或路径错误 | 检查文件名（不要有中文/空格），改用 PDF 格式 |

## 补充4：正向搜索与反向搜索

- **正向搜索**：在 `.tex` 文件中按住 `Ctrl` 点击某一行，可以直接跳转到 PDF 对应的位置。
- **反向搜索**：在 PDF 中双击某一行，会跳回 `.tex` 文件的对应行。LaTeX Workshop 默认支持，需要安装 SumatraPDF 或配置。

VS Code + LaTeX Workshop 的组合默认支持正向搜索。反向搜索在 Windows 上需要额外配置（将 SumatraPDF 设为默认 PDF 查看器），对于新手来说不是必须的。

## 补充5：模板文档类（`.cls`）与配置（`.sty`）

`sjtu` 模板的样式写在 `sjtuthesis.cls` 文件里，一般用户不需要修改。如果你要自己从头做一个文档样式，才需要学习编写 `.cls`。对于学位论文，使用学校提供的模板是最稳妥的方式。

## 补充6：Overleaf 在线编辑器

如果不想本地安装 TeX Live，可以使用在线的 LaTeX 编辑器，例如 **Overleaf**（`overleaf.com`）或国内的 **TeXPage**、**SJTU LaTeX 文档助手**。你只需要注册账号，上传模板或从模板库选择，就可以在浏览器里写 LaTeX，实时编译预览。非常适合新手快速上手。

**Overleaf 和本地环境的对比如下**：

| 特性 | Overleaf | 本地（TeX Live + VS Code） |
|------|----------|---------------------------|
| 安装成本 | 零安装，注册即用 | 需要下载 4-5 GB，安装 30 分钟 |
| 编译速度 | 取决于服务器负载 | 取决于电脑性能，一般更快 |
| 离线使用 | 不行 | 可以 |
| 中文模板支持 | 需要自己上传字体 | 字体由操作系统管理 |
| 协同编辑 | 原生支持 | 需要 Git 或其他工具 |

对于初学者，推荐先用 Overleaf 跑通第一个文档，之后再决定要不要装本地环境。
