# Section 1: WSL2

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Kernel | The core of an operating system, responsible for managing hardware, processes, memory, and the file system |
| Linux kernel | The kernel of the Linux operating system, initiated by Linus Torvalds in 1991 |
| Distribution | A complete system built on top of the kernel, bundled with various software – Ubuntu is one example |
| Ubuntu 26.04 | The latest long‑term support release of Ubuntu, codenamed Resolute |
| WSL2 | Windows Subsystem for Linux 2 – a way to run Linux inside Windows |
| Virtual Machine Platform | A Windows virtualization feature that WSL2 depends on to run |
| Hyper‑V | The full‑featured virtualization platform included with Windows Pro/Education editions |
| apt | Ubuntu's package manager – handles installing, updating, and removing software |
| Software repository | The server address from which apt downloads packages – domestic users often use mirrors like Aliyun for faster speeds |

---

## 1.1 Why You Need Linux

The lab work for the core CS courses you'll take in college – operating systems, computer networks, compilers, parallel computing – almost always assumes a Linux environment.

There are roughly three reasons:

- Servers run Linux. The code you write will eventually be deployed to servers, so developing locally on Linux avoids environment‑mismatch issues.
- Supercomputing centres run Linux. For research, simulations, and model tuning, you'll SSH into those machines and work from the command line.
- Open‑source toolchains are built for Linux first. Tools like gcc, clang, meson, git, and Python work most smoothly on Linux, have the richest documentation, and yield the most easily searchable solutions when you run into problems.

WSL2 provides the lowest‑barrier, smoothest way to get a Linux environment on Windows. No separate partitions, no VM lag, no waiting for Linux to boot every time.

---

## 1.2 What Is the Linux Kernel

Strictly speaking, "Linux" does not refer to a complete operating system – it refers to the lowest layer of the OS: the **kernel**.

The kernel is the heart of the operating system. It manages:

- **CPU**: decides which program runs when and for how long.
- **Memory**: allocates address space to each program and prevents them from stepping on each other.
- **File system**: manages how data is stored, read, and indexed on disk.
- **Device drivers**: makes hardware like keyboards, mice, network cards, and graphics cards usable by programs.
- **Networking**: handles sending, receiving, connecting, and routing network data.

When you open an Ubuntu terminal and type commands, that program called Bash is not the kernel. It runs in user space and uses system calls provided by the kernel to read/write files, create processes, send and receive network data, and so on.

In 1991, Linus Torvalds released the first version of the Linux kernel while studying at the University of Helsinki. He was 21 at the time. More than three decades later, the Linux kernel has become the world's largest open‑source project – over 90% of servers run it, Android phones run it, and the vast majority of supercomputers run it.

You don't need to understand the kernel's internals right now, but you should know one thing: **this is exactly the kernel that runs inside WSL2** – Microsoft maintains their own version specially adapted for Windows virtualization.

---

## 1.3 Enabling Windows Features

To run WSL2 on Windows 11, you need to enable two Windows features: **Virtual Machine Platform** and **Windows Subsystem for Linux**.

Open "Turn Windows features on or off":
- Press `Win + R`, type `optionalfeatures`, and hit Enter
- Or search for "Turn Windows features on or off" directly in the Start menu

In the pop‑up window, check both of the following:
- **Virtual Machine Platform**
- **Windows Subsystem for Linux**

**About Hyper‑V**: If you're using Windows 11 Pro or Education, you'll see a "Hyper‑V" option in the same window. Checking it won't cause any harm – WSL2 doesn't depend on it, but they don't conflict either. If you're on Windows 11 Home, the option simply isn't there, so you can ignore it. Either way, this setting doesn't affect normal WSL2 usage.

After confirming, Windows will begin installing these features and prompt you to restart when finished. **You must restart** – subsequent steps will fail otherwise.

---

## 1.4 Installing PowerShell 7

The built‑in PowerShell version in Windows is 5.1 (labelled "Windows PowerShell" in Windows Terminal). We need to install PowerShell 7, which offers a better experience for WSL integration.

Open Windows Terminal **as Administrator**, then open a **Windows PowerShell** tab (the system‑provided one) and run:

```powershell
winget install --id Microsoft.PowerShell --source winget
```

Once installation finishes, close Windows Terminal and reopen it. You should now see a "PowerShell 7" option when creating a new tab (the icon is usually dark blue). From this point on, run all commands in a **PowerShell 7** tab – not in the "Windows PowerShell" one.

---

## 1.5 Installing Ubuntu 26.04

In a PowerShell 7 tab, run:

```powershell
wsl --install -d Ubuntu-26.04
```

Wait for the download and installation to complete. Ubuntu 26.04 is a long‑term support release from April 2026, codenamed Resolute. All commands and operations in this book have been verified on this version.

Once installation finishes, launch Ubuntu 26.04 from the Start Menu, or run `wsl -d Ubuntu-26.04` from PowerShell.

On first launch, you'll be prompted to create a username and password:
- Don't use `root` as your username – `root` is the system administrator account; use your own username for daily work.
- When typing your password, nothing will appear on screen (not even asterisks). This is normal – just type it and hit Enter.

---

## 1.6 Configuring WSL Networking

WSL's default networking behaviour can be less than ideal for real‑world use: IP addresses change frequently, DNS resolution occasionally fails, and memory isn't released. I recommend using a graphical tool called **WSL Settings** for configuration.

Installing WSL Settings:
- Search for "WSL Settings" in the Microsoft Store and install it
- Or visit its GitHub repository for the latest version

Open WSL Settings and make the following adjustments under the **Network** section:

- **Network Mode**: set to `mirrored`. WSL's IP will now match Windows' – accessing services running inside WSL from other devices on your LAN no longer requires extra port‑forwarding setup.
- **Host Address Loopback**: enable. Allows WSL to access ports that Windows is listening on locally.
- **Auto Proxy**: enable. Command‑line tools inside WSL (curl, apt, git, etc.) will automatically use Windows' proxy settings.
- **DNS Tunneling**: enable. DNS requests from WSL are forwarded to Windows for resolution, avoiding failures caused by campus networks or ISPs hijacking DNS.
- **Best‑Effort DNS Parsing**: enable. If DNS resolution fails, it automatically tries other methods, improving stability.

Under **Experimental**, find **Memory Reclaim** and set it to `gradual`. When WSL is idle, it gently releases unused memory back to Windows, preventing it from becoming sluggish over time.

After configuration, **restart Windows Terminal** (not the whole computer – just close the terminal window and open it again). After restarting, you should see an "Ubuntu-26.04" distribution available when creating a new terminal tab.

---

## 1.7 Changing the Software Repository and Updating the System

Ubuntu's default software repository servers are overseas, which can be slow to access from within China. We'll switch to a domestic mirror.

### 1.7.1 Backing Up the Original Configuration

In the Ubuntu terminal, run:

```bash
sudo cp /etc/apt/sources.list.d/ubuntu.sources /etc/apt/sources.list.d/ubuntu.sources.bak
```

This makes a backup copy of the original software repository configuration file. If anything goes wrong later, you can restore it with:

```bash
sudo cp /etc/apt/sources.list.d/ubuntu.sources.bak /etc/apt/sources.list.d/ubuntu.sources
```

### 1.7.2 Editing the Repository Configuration

For Ubuntu 26.04, the repository configuration file is located at `/etc/apt/sources.list.d/ubuntu.sources`. Open it with vim:

```bash
sudo vim /etc/apt/sources.list.d/ubuntu.sources
```

If you're not familiar with vim, you can use nano instead:

```bash
sudo nano /etc/apt/sources.list.d/ubuntu.sources
```

If using vim, follow these steps:
1. Press the Down arrow key to move the cursor past the comment area
2. Type `13dd` to delete the original 13 lines of configuration
3. Press `o` to open a new line below and enter insert mode
4. Paste the following content (right‑click or `Shift+Insert`):

```
Types: deb
URIs: https://mirrors.aliyun.com/ubuntu/
Suites: resolute resolute-updates resolute-backports
Components: main restricted universe multiverse
Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg

Types: deb
URIs: https://mirrors.aliyun.com/ubuntu/
Suites: resolute-security
Components: main restricted universe multiverse
Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg
```

5. Press `Esc` to exit insert mode
6. Type `:w` and press Enter to save
7. Type `:q` and press Enter to quit

If using nano, it's more straightforward: paste the content, then press `Ctrl+O` to save and `Ctrl+X` to exit.

### 1.7.3 Updating the Package Lists

After changing the repository, run:

```bash
sudo apt update
```

This pulls the latest package lists from the Aliyun mirror. Then run:

```bash
sudo apt full-upgrade -y
```

`full-upgrade` updates all installed packages to their latest versions, including the kernel and system components. The `-y` flag automatically confirms all update prompts – you won't need to manually type `Y`.

The first update may take anywhere from a few minutes to over ten, depending on your network speed.

---

## 1.8 What You've Accomplished in This Section

Let's pause and review. In this section, you've completed the following:

1. Enabled Virtual Machine Platform and Windows Subsystem for Linux in Windows Features
2. Installed PowerShell 7 and confirmed you're using the PowerShell 7 tab in Windows Terminal
3. Installed Ubuntu 26.04
4. Configured network mode, DNS, proxy, and memory reclamation using WSL Settings
5. Switched Ubuntu's software repository to the Aliyun mirror
6. Updated all system packages

Your WSL2 Ubuntu is now a development environment with a working network connection, a properly configured repository, and up‑to‑date system packages. Next, we'll install compilers, build tools, and the Python environment to complete your development toolchain.