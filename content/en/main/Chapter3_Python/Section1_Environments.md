# Section 1: Python Installation and Virtual Environments

> Please make sure you have Windows Terminal and PowerShell 7 installed. This chapter will primarily use Windows as the teaching platform to keep the tutorial broadly accessible. If you haven't installed them yet, refer to Section 1.4 of Chapter 1 in the main text – I won't repeat the instructions here.
> You can open Windows Terminal by pressing `Win+X` to bring up the Start menu, then pressing `I` or clicking on Terminal.
> Actually, for most of the hands‑on work in this chapter, you won't even need a standalone Windows Terminal – you can just open a terminal inside VS Code. If you don't know what VS Code is, search for it using the search bar in the top‑left corner of your screen and install it. After installation, make sure to download the Python extension from Microsoft. If you already have VS Code set up, you can open the terminal with Ctrl+Shift+`.
> There's a lot more you can do with VS Code – feel free to explore its features on your own.

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Interpreter | A program that reads and executes Python code line by line |
| Python Install Manager | Python's official Windows installation manager (`pymanager`), used to manage multiple Python versions |
| py | The Python Launcher for Windows – a command that selects and runs different Python versions |
| pip | Python's package manager – used to install third‑party libraries |
| Virtual environment | An isolated Python environment separate from the system Python – each project gets its own dependencies |
| venv | Python's built‑in module for managing virtual environments |
| PATH | A system environment variable that tells the command line where to look for programs when you type a command |

---

## 1.1 What Is Python

Python is a programming language. You write text in Python syntax (called "source code"), and the **Python interpreter** executes it.

Python can be used for many things: writing scripts to batch‑process files, analysing data, building websites, creating automation tools, and more. Many university courses – data structures, artificial intelligence, data processing – use it for assignments.

This book uses **Python 3.14.7**. Other versions will work, but I recommend using the same version as in this guide.

---

## 1.2 Installing Python Install Manager (Windows)

Starting from Python 3.14, the official recommendation on Windows is to use the **Python Install Manager** (also called `pymanager`). Instead of installing a single fixed version of Python, you install a "manager" that then allows you to install and manage any version of Python.

### 1.2.1 Download

Open your browser, go to the official Python website at `python.org`, click "Downloads" → "Windows", and download `python-manager-26.3.msix`.

`msix` is the modern packaging format for Windows.

### 1.2.2 Installation

Double‑click the downloaded `python-manager-26.3.msix` file – a command‑line interface will open and begin configuration.

Follow these steps:

**Step 1: Configure the global shortcuts directory**

```
The global shortcuts directory is not configured.

Configuring this enables commands like python3.14.exe to run from your
terminal, but is not needed for the python or py commands (for example, py
-V:3.14).

We can add the directory (C:\Users\YourUsername\AppData\Local\Python\bin) to PATH now,
but you will need to restart your terminal to use it.

Add commands directory to your PATH now? [y/N]
```

Type `y` and press Enter. This allows you to type commands like `python3.14` directly in your terminal later.

**Step 2: Install the CPython runtime**

```
You do not have the latest Python runtime.

Install the current latest version of CPython? If not, you can use 'py install
default' later to install.

Install CPython now? [Y/n]
```

Type `Y` and press Enter (or just press Enter, since `Y` is the default). The manager will automatically download and install the latest version of CPython (the Python interpreter).

Once the installation finishes, the command‑line window will close automatically.

> **Note**: `python-manager-26.3.msix` installs the **manager itself**, not the Python interpreter. The CPython runtime is installed in the second step. If you later need another version (e.g., Python 3.13), you can use the `py install 3.13` command.

### 1.2.3 Verifying the Installation

Open a **new** PowerShell window (the old one won't have the updated PATH settings loaded) and type:

```powershell
py -V
```

Or:

```powershell
py --version
```

You should see output similar to `Python 3.14.7`.

Also check that pip is available:

```powershell
py -m pip --version
```

If it prints version information without errors, pip is ready to use.

If `py -V` throws an error, or opens the Microsoft Store, or just prints a single `*` character, simply type `py` in the terminal – it will automatically download CPython for you.

---

## 1.3 Python Install Manager vs. Traditional Installation

In the past, installing Python meant downloading `python-3.14.7-amd64.exe` – you ended up with just that one version.

With `pymanager`, it's like installing a "manager" first, and then using that manager to install and manage multiple Python versions.

**Benefits**:

- Switch versions with a single command: `py -3.13` uses Python 3.13, `py -3.14` uses Python 3.14
- List all installed versions: `py list`
- Install a new version: `py install 3.13`
- Uninstall a version: `py uninstall 3.13`

Starting from Python 3.16, traditional `.exe` installers will gradually stop being released. `pymanager` is the future standard approach.

---

## 1.4 Virtual Environments – Why You Need Them

Python has one particular trait: by default, every library you install via `pip install` goes into the currently active Python environment. This causes a few problems:

- Project A needs `pandas 2.0`, while Project B needs `pandas 3.0` – but you can only have one version installed globally.
- After installing many libraries, your environment becomes cluttered, and you lose track of which library belongs to which project.

**Virtual environments** solve this problem.

A virtual environment is an independent Python environment created inside your project directory – it has its own copy of the `python` interpreter, its own `pip`, and its own library installation directory. Each project gets its own virtual environment, and the libraries inside don't interfere with each other.

---

## 1.5 Creating and Using Virtual Environments

### 1.5.1 Creating a Virtual Environment

Suppose you have a project located in `~/my-project`. Open PowerShell, navigate to the project directory:

```powershell
cd ~/my-project
```

Then run:

```powershell
py -m venv .venv
```

This creates a folder called `.venv` in the current directory, containing an independent Python environment.

> Use `py -m venv` rather than `python -m venv` – the `py` launcher ensures you're calling the correct Python version.

### 1.5.2 Activating the Virtual Environment

After creation, you need to **activate** the virtual environment before using it.

In Windows PowerShell:

```powershell
.\.venv\Scripts\Activate.ps1
```

If you encounter an execution policy restriction (an error saying "cannot load script"), you can use the batch file instead:

```powershell
.\.venv\Scripts\activate.bat
```

Once activated, you'll see `(.venv)` appear at the beginning of your terminal prompt.

### 1.5.3 Installing Packages

With the virtual environment active, any package installed via `pip install` will go into `.venv`:

```powershell
pip install requests
```

### 1.5.4 Exporting and Restoring Dependencies

To record all the dependencies in the current environment:

```powershell
pip freeze > requirements.txt
```

When someone else gets your project, they can install all dependencies with one command:

```powershell
pip install -r requirements.txt
```

### 1.5.5 Deactivating the Virtual Environment

```powershell
deactivate
```

The `(.venv)` prefix in your prompt disappears.

---

## 1.6 .gitignore – Don't Commit Your Virtual Environment

If you're using Git for version control, **do not commit the `.venv` directory** to your repository.

Create a `.gitignore` file in your project root with the following content:

```
.venv/
venv/
env/
__pycache__/
*.pyc
```

---

## 1.7 VS Code Integration

Open your project folder in VS Code – it will automatically detect the `.venv` directory. Press `Ctrl+Shift+P`, type "Python: Select Interpreter", and choose the one marked with `('.venv')`.

After that, when you open a terminal inside VS Code (`` Ctrl+` ``), it will automatically activate the virtual environment.

Your editor should look something like this.

![VS Code Interface Example](/img/VS Code Example.png)

---

## 1.8 requirements.txt – Dependency Management and Environment Reproduction

After you install some third‑party libraries, you need to record their names and versions. This way, others who get your project – or you yourself on a different machine – can install all the dependencies in one go.

### 1.8.1 Generating requirements.txt

With your virtual environment **activated**, run:

```powershell
pip freeze > requirements.txt
```

`pip freeze` lists all installed packages in the current virtual environment with their version numbers, in the format `package==version`. `> requirements.txt` writes that output to the `requirements.txt` file.

The generated file will look something like:

```
requests==2.32.0
pandas==2.2.3
numpy==2.2.0
```

### 1.8.2 Installing Dependencies from requirements.txt

When you need to recreate the same environment on another machine (or in a fresh virtual environment on the same machine):

```powershell
pip install -r requirements.txt
```

The `-r` flag tells pip to read the dependency list from the file. It will install each package listed, placing them in the currently active virtual environment.

### 1.8.3 When to Use This

- **When committing code to Git**: commit `requirements.txt` alongside your code. Others can clone the repo and restore the environment with one command.
- **When deploying to a server**: create a virtual environment on the server and run `pip install -r requirements.txt`.
- **When reinstalling your OS or moving to a new computer**: install Python, create a virtual environment, and restore everything from this file.

### 1.8.4 Important Notes

- `requirements.txt` should be committed to Git, but **the virtual environment directory (`.venv`) should not**.
- `pip freeze` lists **all** packages installed in the current environment, including those pulled in as dependencies of other packages. If you only want manually installed packages, you can use `pip list --not-required` to see top‑level dependencies.
- When you uninstall or upgrade a package, remember to run `pip freeze > requirements.txt` again to keep the file up to date.
- If a package fails to install, check that you're in the correct virtual environment and that your network connection is working.

---

## 1.9 What You've Accomplished in This Section

1. Installed Python Install Manager and, through it, the CPython 3.14.7 runtime
2. Verified that `py -V` and `py -m pip --version` produce normal output
3. Understood the purpose of virtual environments
4. Learned how to create, activate, and deactivate virtual environments
5. Learned how to install packages with `pip install`
6. Learned how to export dependencies with `pip freeze > requirements.txt`
7. Learned how to restore dependencies with `pip install -r requirements.txt`
8. Configured `.gitignore` to exclude the virtual environment directory

From now on, the first step for **every new Python project** is to create and activate a virtual environment.