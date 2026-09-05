# 第1节：Python 安装与虚拟环境

> 请检查自己是否安装了 Windows Terminal 以及 Powershell 7 。本章节将主要以Windows为主要操作系统进行教学，目的是使该教程更具普适性。如果尚未安装，请参照主书第一章的Section1的1.4节，不再赘述。  
> 你可以通过先按下 win+X 打开徽标键的菜单，然后按I或单击终端来打开 Windows Terminal 。  
> 实际上在本章节实际操作中基本不会用到相对于 VS Code 外置的 Windows Terminal ，你只需要在 VS Code 中打开终端就可以。如果你不知道 VS Code ，请使用界面左上角的搜索栏搜索 VS Code ，自行安装。安装后请下载开发者为 Microsoft 的 Python扩展。如果你已经装好了 VS Code ，你可以使用 Ctrl+Shift+` 来打开终端。  
> 关于 VS Code 还有很多技巧和使用方法，请自行探索。  

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 解释器 | 逐行读取并执行 Python 代码的程序 |
| Python Install Manager | Python 官方在 Windows 上的安装管理器（`pymanager`），管理多版本 Python |
| py | Python 启动器（Python Launcher），Windows 下选择和运行不同版本 Python 的命令 |
| pip | Python 的包管理工具，用来安装第三方库 |
| 虚拟环境 | 与系统 Python 隔离的独立 Python 环境，每个项目有自己的依赖 |
| venv | Python 自带的虚拟环境管理模块 |
| PATH | 系统环境变量，决定命令行里输入命令时去哪里找对应的程序 |

---

## 1.1 Python 是什么

Python 是一门编程语言，你用 Python 语法写出一段文本（叫“源代码”），然后由 **Python 解释器** 来执行它。

你可以用 Python 做很多事情：写脚本批量处理文件、分析数据、搭建网站、写自动化工具。大学里很多课程（数据结构、人工智能、数据处理）的作业都会用到它。

本书使用 **Python 3.14.7**。装别的版本也能用，但建议和本书保持一致。

---

## 1.2 安装 Python Install Manager（Windows）

从 Python 3.14 开始，官方在 Windows 上主推 **Python Install Manager**（也叫 `pymanager`）。它不再是安装某一个固定版本的 Python，而是安装一个“管理器”，再由它来安装和管理任意版本的 Python。

### 1.2.1 下载

打开浏览器，访问 Python 官方网站 `python.org`，点击 “Downloads” → “Windows”，下载 `python-manager-26.3.msix`。

`msix` 是 Windows 现代的打包格式。

### 1.2.2 安装

双击下载的 `python-manager-26.3.msix` 文件，会打开一个命令行界面，开始配置。

按以下步骤选择：

**第一步：配置全局快捷方式目录**

```
The global shortcuts directory is not configured.

Configuring this enables commands like python3.14.exe to run from your
terminal, but is not needed for the python or py commands (for example, py
-V:3.14).

We can add the directory (C:\Users\你的用户名\AppData\Local\Python\bin) to PATH now,
but you will need to restart your terminal to use it.

Add commands directory to your PATH now? [y/N]
```

输入 `y` 回车。这样以后就可以在终端里直接输入 `python3.14` 这样的命令。

**第二步：安装 CPython 运行时**

```
You do not have the latest Python runtime.

Install the current latest version of CPython? If not, you can use 'py install
default' later to install.

Install CPython now? [Y/n]
```

输入 `Y` 回车（或者直接回车，因为默认就是 Y）。管理器会自动下载并安装最新版本的 CPython（也就是 Python 解释器）。

安装完成后，命令行窗口会自动关闭。

> **注意**：`python-manager-26.3.msix` 安装的是 **管理器本身**，不是 Python 解释器。第二步安装的才是 CPython 运行时。如果以后需要安装其他版本（比如 Python 3.13），可以用 `py install 3.13` 命令。

### 1.2.3 验证安装

打开一个**新的** PowerShell 窗口（之前的窗口不会加载新的 PATH 设置），输入：

```powershell
py -V
```

或者：

```powershell
py --version
```

应该输出类似 `Python 3.14.7` 的信息。

再检查 pip：

```powershell
py -m pip --version
```

能正常输出版本信息就说明 pip 也可用。

如果输入 `py -V` 报错，或者弹出了 Microsoft Store，或者输出一条 * ，你只需要在终端中输入py，他会自动下载好CPython。  

---

## 1.3 Python Install Manager vs 传统安装方式

以前装 Python，是直接下载 `python-3.14.7-amd64.exe`，装完就只有一个版本。

现在用 `pymanager`，相当于先装一个“管理器”，再由它来安装和管理多个 Python 版本。

**好处**：

- 一个命令切换版本：`py -3.13` 用 Python 3.13，`py -3.14` 用 Python 3.14
- 查看所有已安装版本：`py list`
- 安装新版本：`py install 3.13`
- 卸载版本：`py uninstall 3.13`

从 Python 3.16 开始，传统的 `.exe` 安装包将逐步停止发布。`pymanager` 是未来的主流方式。

---

## 1.4 虚拟环境：为什么需要它

Python 有一个特点：所有通过 `pip install` 安装的第三方库，默认装到当前使用的 Python 环境里。这会导致几个问题：

- 项目 A 需要 `pandas 2.0`，项目 B 需要 `pandas 3.0`，但你只能装一个版本。
- 装多了之后，环境越来越乱，你自己都分不清哪些库是哪个项目用的。

**虚拟环境**解决的就是这个问题。

虚拟环境是在项目目录里创建一个独立的 Python 环境——它有自己独立的 `python` 解释器副本、独立的 `pip`、独立的库安装目录。每个项目有自己的虚拟环境，里面装的库互不干扰。

---

## 1.5 创建和使用虚拟环境

### 1.5.1 创建虚拟环境

假设你有一个项目放在 `~/my-project`。打开 PowerShell，进入项目目录：

```powershell
cd ~/my-project
```

然后执行：

```powershell
py -m venv .venv
```

这条命令会在当前目录下创建一个叫 `.venv` 的文件夹，里面装着一套独立的 Python 环境。

> 用 `py -m venv` 而不是 `python -m venv`，是因为 `py` 启动器能确保调用的是正确的 Python 版本。

### 1.5.2 激活虚拟环境

虚拟环境创建完之后，需要**激活**才能使用。

在 Windows PowerShell 中：

```powershell
.\.venv\Scripts\Activate.ps1
```

如果遇到执行策略限制（报错说“无法加载脚本”），可以先用：

```powershell
.\.venv\Scripts\activate.bat
```

激活成功后，终端提示符前面会出现 `(.venv)` 字样。

### 1.5.3 安装包

在虚拟环境激活的状态下，用 `pip install` 安装的包会装到 `.venv` 里：

```powershell
pip install requests
```

### 1.5.4 导出和恢复依赖

记录当前环境的所有依赖：

```powershell
pip freeze > requirements.txt
```

别人拿到项目后，用这条命令一次性装好所有依赖：

```powershell
pip install -r requirements.txt
```

### 1.5.5 退出虚拟环境

```powershell
deactivate
```

提示符前面的 `(.venv)` 消失。

---

## 1.6 .gitignore：不要把虚拟环境提交到 Git

如果你用 Git 管理代码，**不要把 `.venv` 目录提交到仓库里**。

在项目根目录创建 `.gitignore` 文件，写入：

```
.venv/
venv/
env/
__pycache__/
*.pyc
```

---

## 1.7 VS Code 集成

用 VS Code 打开项目文件夹，它会自动检测到 `.venv` 目录。按 `Ctrl+Shift+P`，输入 “Python: Select Interpreter”，选择带有 `('.venv')` 字样的那个 Python 解释器。

之后在 VS Code 里打开终端（`` Ctrl+` ``），终端会自动激活虚拟环境。

你的编辑界面应该类似这样。

![VS Code Interface Example](/img/VS Code Example.png)

---

## 1.8 requirements.txt：依赖管理与环境复现

当你安装了一些第三方库之后，需要把它们的名字和版本号记录下来。这样别人拿到你的项目，或者你自己换一台电脑继续开发时，能一次性装好所有依赖。

### 1.8.1 生成 requirements.txt

在虚拟环境**激活**的状态下，执行：

```powershell
pip freeze > requirements.txt
```

`pip freeze` 会列出当前虚拟环境中所有已安装的包及其版本号，格式是 `包名==版本号`。`> requirements.txt` 把这个输出写入到 `requirements.txt` 文件中。

生成的文件内容类似：

```
requests==2.32.0
pandas==2.2.3
numpy==2.2.0
```

### 1.8.2 安装 requirements.txt 中的依赖

当你在另一台电脑上（或者同一台电脑的新虚拟环境里）需要恢复同样的环境时：

```powershell
pip install -r requirements.txt
```

`-r` 表示从文件读取依赖列表。pip 会逐个安装文件里列出的包，并安装到当前激活的虚拟环境中。

### 1.8.3 什么时候用

- **提交代码到 Git 时**：把 `requirements.txt` 一起提交。这样其他人 clone 项目后，可以一键恢复环境。
- **部署到服务器时**：服务器上先创建虚拟环境，再执行 `pip install -r requirements.txt`。
- **重装系统或换电脑时**：先装好 Python，创建虚拟环境，然后直接用这个文件恢复所有依赖。

### 1.8.4 注意事项

- `requirements.txt` 应该和代码一起提交到 Git，但**虚拟环境目录（.venv）不要提交**。
- `pip freeze` 会列出当前环境**所有**已安装的包，包括那些被间接依赖的包。如果你只想要手动安装的包，可以用 `pip list --not-required` 查看顶层依赖。
- 当你卸载或升级了某个包，记得重新执行 `pip freeze > requirements.txt` 保持文件同步。
- 如果某个包安装失败，检查是否在正确的虚拟环境中，或者网络是否通畅。

---

## 1.9 本节你做了什么

1. 安装了 Python Install Manager，并通过它安装了 CPython 3.14.7 运行时
2. 验证了 `py -V` 和 `py -m pip --version` 能正常输出
3. 理解了虚拟环境的作用
4. 学会了创建、激活、退出虚拟环境
5. 学会了用 `pip install` 安装包
6. 学会了用 `pip freeze > requirements.txt` 导出依赖
7. 学会了用 `pip install -r requirements.txt` 恢复依赖
8. 配置了 `.gitignore` 忽略虚拟环境目录

从现在开始，**每一个新的 Python 项目，第一步都是创建并激活虚拟环境**。