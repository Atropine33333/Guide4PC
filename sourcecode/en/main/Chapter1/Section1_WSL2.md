# Section 1: WSL2

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Linux | An operating system kernel – a completely different foundation from Windows |
| Distribution | A full system built on top of the Linux kernel, bundled with various software tools – Ubuntu is one example |
| Ubuntu | One of the most widely used Linux distributions; all commands in this book are tested on Ubuntu 24.04 |
| WSL2 | Windows Subsystem for Linux 2 – a way to run Linux inside Windows |
| Virtual Machine | Software that emulates an entire computer, allowing another operating system to run inside it |
| Hyper-V | Windows' built-in virtualization technology – both WSL2 and traditional VMs rely on it |
| Mirrored Networking | A WSL2 networking mode that gives WSL the same IP address as Windows |
| DNS Tunneling | Allows WSL to resolve domain names through Windows, fixing DNS issues in certain network environments |

---

## 1.1 Why You Need Linux

If you're fortunate enough to attend a decent university – one that doesn't force you to use a fossil like VC6.0 as your IDE – then the lab code for your CS courses (operating systems, computer networks, compilers, parallel computing) will almost always assume a Linux environment.

Here's why:

- The vast majority of servers run Linux. The code you write will eventually be deployed to servers. Developing locally on Linux helps avoid the classic "but it works on my machine" embarrassment.
- Supercomputing centers run Linux. If you eventually do research, run simulations, or fine‑tune models, you'll SSH into those machines and work from the command line.
- Open‑source toolchains are built for Linux first. They work most smoothly on Linux, have the richest documentation, and yield the most searchable solutions when you hit a problem.

Yes, you could run Linux in a VM on Windows, or go through the extra steps on macOS. But WSL2 provides the lowest barrier to getting a Linux environment on Windows, with the smoothest experience. No separate partitions, no lag inside a VM, no waiting for Linux to boot every time you turn on your machine.

WSL2 isn't a traditional VM, yet it still relies on virtualization technology. Let's break down how they relate.

---

## 1.2 Installing WSL2

These steps assume you're on Windows 11.

**Step 1: Turn on the necessary Windows features**

Open PowerShell or Windows Terminal as Administrator and run:

```powershell
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
```

The first command enables WSL. The second enables the Virtual Machine Platform. WSL2 needs virtualization support to run a full Linux kernel.

**Restart your computer** after this step.

**Step 2: Set WSL2 as the default version**

After rebooting, run this as Administrator:

```powershell
wsl --set-default-version 2
```

From now on, any Linux distribution you install will use WSL2 by default. To switch between WSL1 and WSL2 for a specific distribution, use `wsl --set-version <distro name> 1` or `2`.

**Step 3: Install Ubuntu 24.04**

In your terminal:

```powershell
wsl --install -d Ubuntu-24.04
```

Once the installation finishes, launch Ubuntu 24.04 from the Start Menu, or type `wsl -d Ubuntu-24.04` in your terminal.

**First launch**: You'll be prompted to create a username and password. A few things to keep in mind:

- Don't use `root` as your username. `root` is the system administrator account – your daily work should be done under your own username.
- When you type your password, nothing will show on screen (not even asterisks). That's normal. Just type it and hit Enter.

**Verify the installation**:

```bash
wsl -l -v
```

You should see something like:

```
  NAME                   STATE           VERSION
* Ubuntu-24.04           Running         2
```

The asterisk `*` marks the default distribution. When you type `wsl` without any arguments, this is the one you'll enter.

---

## 1.3 How WSL2 Differs from a Virtual Machine

A common question: "Isn't WSL2 just a virtual machine?"

The answer: It uses VM technology (Hyper‑V) under the hood, but it doesn't feel like one.

**What a traditional VM (like VirtualBox or VMware) does**:

It emulates a complete set of hardware – CPU, memory, disk, network adapter, graphics card, sound card, USB controller – and then installs a full OS on top of that emulated hardware. That guest OS never touches your real hardware directly; it only sees what the VM software simulates.

The result: slow startup, heavy memory consumption, poor graphical performance, and clumsy file sharing. Yes, isolation is strong, but for everyday development, the trade‑off is often too costly.

**What WSL2 does**:

WSL2 also runs inside a lightweight virtual machine (based on Hyper‑V), but this VM is special:

- **No full hardware emulation**. It only virtualizes the hardware required to run the Linux kernel. Everything else (graphics, audio, USB) goes straight through Windows' drivers.
- **A custom‑built kernel**. Microsoft maintains its own Linux kernel, heavily tuned for WSL scenarios – especially file system and network performance.
- **Seamless integration with Windows**. You can access WSL's file system directly via `\\wsl.localhost`. Windows processes and WSL processes can talk to each other.

**When should you use a traditional VM?** You need a full graphical Linux desktop (not just a terminal), direct USB device access, or several completely isolated environments.

**When should you use WSL2?** You need a Linux command line and/or you need to compile code in a Linux environment.

---

## 1.4 Configuration File: .wslconfig

WSL2's default network settings can be problematic in certain scenarios. The most common pain points:

- WSL gets an IP address on a different subnet than Windows, so accessing services running inside WSL (like Jupyter or an HTTP server) from other devices on your LAN requires extra port‑forwarding setup.
- DNS resolution occasionally fails – you can ping an IP address from WSL, but not a domain name.
- After running for a long time, WSL2 consumes a large amount of memory (Linux likes to use spare RAM for disk caching and doesn't give it back aggressively).

A single configuration file can address all of these.

In your `%UserProfile%` directory (that's `C:\Users\YourUserName\`), create a new file named `.wslconfig`. Pay attention: the name starts with a dot and has no file extension.

Paste the following content:

```ini
[wsl2]
networkingMode=mirrored
dnsTunneling=true
firewall=true
autoProxy=true
autoMemoryReclaim=gradual
```

**What each setting does**:

`networkingMode=mirrored`  
Mirrored network mode. When enabled, WSL gets the exact same IP address as Windows, and all network interfaces are "mirrored" inside WSL. The big win: other devices on your LAN can directly access services running inside WSL without any port‑forwarding tricks. Any port your WSL app binds to will automatically be listened to on the same port by Windows.

`dnsTunneling=true`  
DNS tunneling. DNS requests from inside WSL are forwarded to Windows, which handles the actual domain resolution. This bypasses all sorts of network quirks that can occur when WSL sends DNS packets directly (for example, DNS hijacking on certain campus networks). With this turned on, operations like `apt update`, `git clone`, and `curl` become much more reliable.

`firewall=true`  
Makes the Windows Firewall apply to WSL traffic. By default, WSL traffic bypasses the Windows Firewall, which can cause network policies (in corporate or campus environments) to block it. Turning this on subjects all WSL inbound and outbound traffic to Windows Firewall rules.

`autoProxy=true`  
Automatically inherits Windows system proxy settings. If you're running a proxy tool on Windows (like Clash or v2ray), command‑line tools inside WSL (curl, apt, git, etc.) will automatically use that proxy – no separate configuration needed inside WSL.

`autoMemoryReclaim=gradual`  
Automatic memory reclamation in gradual mode. When WSL2 is idle, the background system will slowly return memory from the Linux kernel back to Windows. The default is `disabled`. Enabling this prevents WSL from hogging more and more memory over time until you're forced to run `wsl --shutdown` to free it. `gradual` means gentle reclamation that avoids performance hits on running programs. There's also `dropcache`, which is more aggressive but can make file accesses slower the first time after reclamation.

**How to apply the changes**: After editing `.wslconfig`, you need to fully shut down WSL for the new settings to take effect. Run this in PowerShell:

```powershell
wsl --shutdown
```

Then restart WSL (open your Ubuntu terminal or just type `wsl`). The configuration will be loaded.

**Verification**: After restarting, run `ip addr` inside WSL. You'll see that the network interface configuration has changed. The exact differences depend on your network environment – don't worry too much about the details.

---

## 1.5 Section Validation Checklist

Go through these checks to confirm your WSL2 environment is ready:

1. **Verify the version**: `wsl -l -v` should show Ubuntu 24.04 with VERSION set to 2.
2. **Confirm network connectivity**: Inside your WSL terminal, run `ping -c 4 www.baidu.com`. You should see replies.
3. **Confirm file system basics**: Run `ls /`. You should see directories like `bin`, `home`, `usr`. Then run `ls /mnt/c/` – you should see the contents of your Windows C: drive.

If step 2 fails (no ping response), first double‑check that `.wslconfig` contains `dnsTunneling=true` and `autoProxy=true` correctly, then run `wsl --shutdown` and try again.

---

## 1.6 Common Issues

**Installation tells you "need to update the WSL kernel"**  
Search Microsoft's official site for "WSL2 kernel update package", download and install it, then retry.

**Ubuntu gets stuck on "Installing..." for a very long time when launching**  
Network issues. Try turning off any proxy tools, or wait for a different time of day.

**Ubuntu starts slowly after running `wsl --shutdown`**  
The first launch rebuilds certain caches – this is normal. The next launch will be faster.

**Configuration file changes don't take effect**  
Make sure the file is named exactly `.wslconfig` (not `.wslconfig.txt`). Double‑check that it's placed in `%UserProfile%` (`C:\Users\YourUserName\`), not inside WSL. After editing, don't forget to run `wsl --shutdown`.