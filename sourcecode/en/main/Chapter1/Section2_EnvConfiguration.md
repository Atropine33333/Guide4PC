# Section 2: Setting Up the Ubuntu Environment

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Compiler | A tool that converts source code into an executable program that the machine can run |
| GCC | The GNU Compiler Collection – the most widely used C/C++ compiler on Linux |
| Clang | The LLVM project's C/C++ compiler, known for more user‑friendly error messages than GCC |
| Interpreter | A program that executes code line by line, without requiring prior compilation to machine code |
| pip | Python's package manager – used to install third‑party libraries |
| Virtual environment | An isolated Python environment where different projects can use different library versions without conflicts |
| venv | Python's built‑in tool for managing virtual environments |
| Shell | The command‑line interpreter – you type commands, and it executes them |
| Bash | The most common shell program – Ubuntu's default shell |
| Meson | A build system used to configure and compile large projects |
| Ruby | A programming language that some tools (like Jekyll) depend on |

---

## 2.1 Directory Structure – Putting Your Code in the Right Place

On Windows, your documents, downloads, and desktop live under `C:\Users\YourUsername\`. In WSL, the corresponding location is `/home/yourusername/`.

I recommend creating a dedicated folder under this directory to hold all your coursework and personal code. I usually call mine `project`. You can name it whatever you like, but the rest of this guide will assume `project` as the default.

Create a `project` directory:

```bash
cd ~
mkdir project
```

`~` is shorthand for your current user's home directory. The two commands above are equivalent to:

```bash
mkdir /home/yourusername/project
```

From now on, put all your code projects inside `~/project/`. Give each project its own subdirectory. For example:

```
~/project/
├── os-lab-1/
├── compiler-assignment/
├── personal-website/
└── meson-demo/
```

There's a simple reason for this: when you later need to back up, package, or manage your code with Git, you only need to operate on this one folder. It's kept separate from system files, so you won't accidentally delete critical system components, and system updates won't mess with it.

---

## 2.2 Compilers – GCC and Clang

For C/C++ development on Linux, you have two mainstream compilers: GCC and Clang. Install both – they don't conflict.

### 2.2.1 Installing GCC

GCC is the most established compiler in the Linux ecosystem. Ubuntu doesn't ship it by default, so you need to install it manually:

```bash
sudo apt update
sudo apt install gcc g++
```

`gcc` is the C compiler, and `g++` is the C++ compiler – they are separate programs, usually installed together.

Verify the installation:

```bash
gcc --version
g++ --version
```

The first shows the GCC version information, the second shows the G++ version.

### 2.2.2 Installing Clang

Clang is the LLVM project's C/C++ compiler. Compared to GCC, its error messages are generally clearer, more precise, and more beginner‑friendly.

```bash
sudo apt install clang
```

Verify:

```bash
clang --version
```

### 2.2.3 GCC vs. Clang – A Quick Summary

You don't need to understand their deep implementation differences right now. Just keep these three things in mind:

- The vast majority of code compiles fine with either compiler.
- Some courses or projects may require a specific one (e.g., operating systems labs sometimes require GCC).
- Installing both takes little space – use whichever one you need at the time.

For everyday assignments, starting with Clang is often a smoother experience because its error messages are easier to decipher.

---

## 2.3 Python Environment – pip and venv

Ubuntu 24.04 comes with Python 3 pre‑installed. Open a terminal and type `python3 --version` to confirm.

However, the pre‑installed version usually doesn't include pip (the package manager) and isn't set up with virtual environments by default. You'll need to configure these yourself.

### 2.3.1 Installing pip

```bash
sudo apt install python3-pip
```

Verify:

```bash
pip3 --version
```

Note that the command is `pip3`, not `pip`.

### 2.3.2 Why You Need Virtual Environments

Python has a particular characteristic: all third‑party libraries installed via pip go into the system‑wide Python directory by default. This leads to a few problems:

- Project A needs `requests 2.31.0`, while Project B needs `requests 2.20.0`. Each project requires a different version, but you can only have one installed globally.
- After installing many libraries, the system Python directory becomes cluttered, and you lose track of which library belongs to which project.
- Removing a library is risky – you might inadvertently break another project's dependencies.

Virtual environments solve this problem.

**What a virtual environment is**: Inside an isolated directory, it creates a copy of the Python interpreter and pip. All libraries you install go into this directory, completely separated from the system Python. Each project gets its own virtual environment, and the libraries inside don't interfere with each other.

Here's a way to think about it: the system Python is a shared public kitchen where everyone cooks, using the same utensils and spices – eventually it becomes a mess. A virtual environment is like each person bringing their own portable workbench, with their own ingredients, cleaning up after themselves without disturbing anyone else.

### 2.3.3 Installing venv

Ubuntu packages the `venv` module separately – you need to install it manually:

```bash
sudo apt install python3-venv
```

Once installed, you can create virtual environments normally.

Create a virtual environment:

```bash
python3 -m venv ~/project/myproject/venv
```

This creates an independent Python environment inside `~/project/myproject/venv/`.

Activate the virtual environment:

```bash
source ~/project/myproject/venv/bin/activate
```

After activation, your terminal prompt will show `(venv)` at the beginning. Any library you install with `pip` from this point will go only into this virtual environment.

Deactivate it:

```bash
deactivate
```

A common practice is to place a `venv` folder at the root of each project and activate it by running `source venv/bin/activate` from that project's root.

### 2.3.4 One Important Caveat

`venv` only manages Python libraries. If you need a different version of Python itself (e.g., a project requires Python 3.10, but your system has 3.12), venv won't help. That's a job for tools like `pyenv` – but we'll leave that for another time.

---

## 2.4 Bash Basics

You're already interacting with Bash every time you open your Ubuntu terminal. Bash is the program that accepts and interprets the commands you type.

Below are the most frequently used commands. If you're not yet comfortable with them, follow along and type them out. Each line includes a short comment.

### 2.4.1 File and Directory Operations

```bash
pwd                     # Print the full path of the current directory
ls                      # List files and folders in the current directory
ls -la                  # List all files (including hidden ones) with detailed info
cd directory_name       # Change to the specified directory
cd ..                   # Go up one level (to the parent directory)
cd ~                    # Go back to your home directory
mkdir directory_name    # Create a new directory
rmdir directory_name    # Remove an empty directory
rm filename             # Delete a file
rm -rf directory_name   # Force‑delete a directory and everything inside it (use with caution!)
cp source destination   # Copy a file
mv source destination   # Move a file (also used for renaming)
```

### 2.4.2 Viewing and Editing Files

```bash
cat filename            # Print the entire file content to the screen
less filename           # View a large file page by page (press q to quit)
head -n 10 filename     # Show the first 10 lines of a file
tail -n 10 filename     # Show the last 10 lines of a file
nano filename           # Open the file in the nano editor
code filename           # Open the file in VS Code (if the Remote‑WSL extension is installed)
```

### 2.4.3 Permissions

```bash
chmod +x filename       # Make a file executable
sudo command            # Run a command as an administrator (you'll be prompted for a password)
```

### 2.4.4 Wildcards and Auto‑Completion

Press the `Tab` key to auto‑complete commands and file names. For example, type `cd ~/pro` and press `Tab` – if `~/project` exists, Bash will expand it to `cd ~/project`.

`*` is a wildcard that matches any string. For instance, `ls *.c` lists all files ending with `.c`.

### 2.4.5 Command History

Press the Up and Down arrow keys to scroll through previously entered commands. Type `history` to see a full list of your command history.

You don't need to memorize these commands – they'll become second nature through regular use. If you forget a specific option, use the `--help` flag, e.g., `ls --help`.

---

## 2.5 Meson – Installing and Verifying the Build System

The Appendix already walked you through installing Meson, but that was mainly to ensure the toolchain existed. We'll verify it again here to make sure your environment is fully set up.

Check that Meson is installed:

```bash
meson --version
```

If the command isn't found, install it:

```bash
sudo apt install meson ninja-build
```

Meson itself is a Python program (you could also install it via pip), but using `apt` is simpler – dependency resolution is handled by the system package manager.

Verify Ninja (Meson's backend build tool):

```bash
ninja --version
```

If you decide to use Meson for your projects later, you'll learn the specifics then. For now, just know: `meson setup builddir` generates the build files, and `meson compile -C builddir` runs the compilation. We'll start using it in Chapter 2.

---

## 2.6 Ruby – A Dependency for Certain Tools

Not every CS student will need Ruby. However, it becomes unavoidable if you plan to:

- Build a personal blog with Jekyll (GitHub Pages natively supports Jekyll)
- Write automation scripts for Chef or Puppet
- Maintain a Ruby on Rails project (if you take a relevant course or land such an internship)

Install it:

```bash
sudo apt install ruby ruby-dev
```

Verify:

```bash
ruby --version
```

You don't need to learn Ruby syntax right now – just have it installed.

---

## 2.7 Section Validation Checklist

Go through these checks to confirm that all your tools are ready:

1. **GCC**: `gcc --version` should output normally without errors.
2. **Clang**: `clang --version` should output normally.
3. **pip**: `pip3 --version` should output normally.
4. **venv**: Run `python3 -m venv /tmp/test-venv`, then `ls /tmp/test-venv/bin/` – you should see the `activate` file. Clean up with `rm -rf /tmp/test-venv` when done.
5. **Meson**: `meson --version` should output normally.
6. **Ninja**: `ninja --version` should output normally.
7. **Ruby**: `ruby --version` should output normally.

---

## 2.8 Common Issues

**`sudo apt update` fails with "Failed to resolve domain names"**  
Network issue. Check if WSL can ping external sites. If your `.wslconfig` is set up correctly and it still fails, try temporarily disabling your proxy tool.

**Packages installed via `pip3` are not found in the command line**  
Pip installs executables under `~/.local/bin/`. If this directory isn't in your PATH, run `echo 'export PATH=$PATH:~/.local/bin' >> ~/.bashrc && source ~/.bashrc` to add it.

**Meson and Ninja versions are too old**  
The versions shipped with Ubuntu 24.04 are generally sufficient. If a project later requires a newer version, you can install via pip: `pip3 install meson`. Then make sure `~/.local/bin` appears earlier in your PATH than the system directory (so it takes precedence).