# Appendix 9: Shortcuts, Tips, and Glossary

> This appendix collects some small tips you might find useful, along with a bunch of technical terms that may sound intimidating. No need to memorize everything at once – just get a rough idea. When you run into a problem later, or come across a term that looks familiar, you can come back and look it up.

## 1. Shortcut Keys

Most of these shortcuts work across Windows. Pick a few that seem useful – once you get used to them, they will be much faster than reaching for the mouse.

| Shortcut | What it does |
|----------|---------------|
| `Ctrl + C` | Copy |
| `Ctrl + V` | Paste |
| `Ctrl + X` | Cut |
| `Ctrl + Z` | Undo the last action |
| `Ctrl + Y` | Redo (bring back what you undid) |
| `Ctrl + A` | Select all |
| `Ctrl + S` | Save |
| `Ctrl + F` | Find (search for text in a webpage or document) |
| `Alt + Tab` | Switch windows (hold Alt and press Tab to choose) |
| `Win + Tab` | Open Task View; you can create new desktops here |
| `Win + E` | Open “This PC” (File Explorer) |
| `Win + D` | Show the desktop; press again to restore |
| `Win + L` | Lock the screen |
| `Win + R` | Open the “Run” dialog |
| `Win + I` | Open Settings |
| `Win + S` | Open Search |
| `Win + X` | Open the Quick Link menu (more on this in the next section) |
| `Win + Shift + S` | Take a screenshot |
| `Ctrl + Shift + Esc` | Open Task Manager directly |
| `Win + V` | Open clipboard history (must be enabled in Settings first) |
| `Win + .` or `Win + ;` | Open the emoji panel |
| `Win + number` | Open the nth program on the taskbar |
| `Win + arrow keys` | Snap windows (left half, right half, maximize, minimize) |
| `Ctrl + Shift + N` | Create a new folder in File Explorer |
| `Alt + F4` | Close the current window; when on the desktop, brings up shutdown options |
| `F2` | Rename the selected file |
| `F5` | Refresh |
| `Delete` | Delete a file (goes to Recycle Bin) |
| `Shift + Delete` | Permanently delete (does not go to Recycle Bin – use with care) |

## 2. Tips

### 2.1 Right‑click the Windows Logo (Win + X)

Right‑click the Start button at the bottom‑left corner of your screen, or press `Win + X`. A menu pops up. Everything in it is a built‑in system management tool – much faster than digging through the Control Panel.

| Menu option | What it is for |
|-------------|----------------|
| Apps and Features | Uninstall unwanted programs |
| Mobility Center | Adjust brightness, volume, screen projection |
| Power Options | Change sleep settings, power plan |
| Event Viewer | View system logs; crash reasons (blue screen) are often here |
| System | Check computer specs (CPU, RAM, Windows version) |
| Device Manager | View and update drivers |
| Network Connections | See network adapters, manually set IP addresses |
| Disk Management | Partition your hard drive |
| Computer Management | A bundle of several management tools |
| Windows PowerShell / Terminal | Open the command line |
| Task Manager | View processes, performance, close frozen programs |

Most items in this menu require administrator privileges. When you are troubleshooting, this is the fastest entry point.

### 2.2 File Selection Tricks in File Explorer

In File Explorer (the window that opens when you go to “This PC”), there are two very common ways to select files:

- **Hold `Ctrl` while clicking files**: Selects them one by one. For example, click one file, then hold Ctrl and click another – both are selected. Click one of them again to deselect it. This is great for “I want these few, but not that one”.

- **Click one file, then hold `Shift` and click another file**: Selects that file, the other file, and everything in between – all at once. Perfect for “I want everything from A to B”.

You can combine these. For example, use Shift to select a continuous range, then Ctrl to add a few scattered files.

### 2.3 You Can See the PID in Task Manager

Open Task Manager (`Ctrl + Shift + Esc`), click “Details”, and you will see a table. Right‑click on the header row (where it says “Name”, “CPU”, “Memory”) and you can add more columns. One of them is called **PID**.

**What is PID**: Process Identifier. A unique number the system assigns to each running program (process).

**Why it’s useful**: Sometimes a program freezes, but Task Manager shows several entries with the same name, and you cannot tell which one to kill. Looking at the PID solves that. Also, some command‑line tools (like `taskkill`) need the PID to know which process to terminate.

How to add it: Task Manager → Details → right‑click the header → Select columns → check “PID”.

### 2.4 Windows Terminal and PowerShell 7

**Windows Terminal**: A modern command‑line window program. Compared to the traditional CMD black box, it supports tabs, split panes, crisper font rendering, and custom themes. It was mentioned in Appendix 7 – after installing it, pin it to your taskbar and use it for all command‑line operations. Windows Terminal is just a “shell” – inside it you can run CMD, PowerShell, or any other command‑line tool.

**PowerShell 7**: A command‑line tool (more details appear in the Extended Reading of Appendix 7). It is a different version from the Windows PowerShell that comes with Windows – both can coexist. PowerShell 7 has more consistent commands, more features, and is cross‑platform (Windows, Mac, Linux). The average user may not need to install it, but if you later follow tutorials or write scripts, you might.

Their relationship: Windows Terminal is the “window”, PowerShell 7 is the “person sitting inside the window”. You open Windows Terminal, and inside you can create a new tab running PowerShell 7.

### 2.5 Environment Variables

**What are environment variables**: Think of them as “global sticky notes” in the system. Each sticky note has a name and a value. Any program can read them to know where to find things.

Common examples:
- **PATH**: A list of folder paths. When you type `notepad` in the command line, the system looks through these folders to find `notepad.exe`. When you install Python or FFmpeg and want to run them from the command line, you are essentially adding their installation folders to the PATH.
- **TEMP**: Where temporary files are stored.
- **USERNAME**: The currently logged‑in user’s name.

**How to change them**: Right‑click “This PC” → Properties → Advanced system settings → Environment Variables. There are “User variables” (affect only your account) and “System variables” (affect everyone).

**You do not need to know how to edit them yet**. But if later you type a command and get “is not recognized as an internal or external command”, you will know it might be a PATH issue – and you can search in that direction.

### 2.6 Registry

**What is the Registry**: Windows’ giant configuration database. Almost everything you change in Settings – wallpaper, startup items, folder options – ends up written in the Registry.

**How to open it**: `Win + R`, type `regedit`, press Enter.

**What it looks like**: On the left are “keys” (like folders), on the right are “values” (like files). The top‑level keys are called root keys:
- `HKEY_CURRENT_USER`: Settings for the current user
- `HKEY_LOCAL_MACHINE`: Global settings for the whole computer
- `HKEY_CLASSES_ROOT`: File associations (e.g., `.txt` opens with Notepad by default)

**An important warning**: Changing the Registry incorrectly can break software, and in severe cases even prevent your system from booting. Before you make any changes, always make a backup: right‑click the key you are about to modify → Export → save as a `.reg` file. If something goes wrong, double‑click that file to restore it.

**You do not need to change anything now**. But later, when you see an online tutorial that says “edit the Registry to fix X”, you will know what the Registry is, and you will know to back it up first. That is enough.

### 2.7 Group Policy

**What is Group Policy**: Think of it as “Registry with graphical switches”. Group Policy packages a bunch of related Registry changes into individual options; you just select “Enabled” or “Disabled”, and the system handles the Registry edits for you. No need to hunt through the Registry yourself.

**How to open it**: `Win + R`, type `gpedit.msc`, press Enter. (Note: Windows Home editions do not have Group Policy by default.)

**Example**: You want to disable USB storage so others cannot copy files from your PC using a USB drive. In Group Policy, find “Removable Storage Access”, select “Deny all access”, and enable it. Trying to find that setting in the Registry directly would take much longer.

If you have Windows Home and want Group Policy, you can search for tools like “gpedit‑enabler” – but they are not guaranteed to be stable, and you do not have to go through that trouble.

### 2.8 Memory Management (Page File and Commit)

You can see these three concepts in the “Performance” tab of Task Manager. They are often misunderstood, so here is a simple explanation.

**Page File**

The page file is a hidden file on your hard drive (usually named `pagefile.sys`). As mentioned in the Extended Reading of Appendix 2 (System Tuning), it is also called **SWAP**. When your physical RAM is full, the system moves data that is not currently needed from RAM into this file, freeing up RAM for applications that need it right now.

Think of it as: your desk (RAM) is too small, so you temporarily put documents you are not using right now into a drawer (hard drive), and take them back out when you need them. The hard drive is much slower than RAM, so if the system is constantly using the page file, your computer will become noticeably slower.

When drive C: runs low on space, the page file cannot expand properly, and your computer may stutter. This is why Appendix 2 stressed not to fill up drive C:.

**Commit**

The “Committed” number in Task Manager represents the total amount of memory that the system has promised to all running programs. This number can be larger than your physical RAM – because part of that promise is fulfilled using the page file.

Think of it like this: you only have 100 RMB in cash (physical RAM), but your credit card gives you 200 RMB of credit (page file space). You can “commit” to spending more than 100 RMB, as long as you make up the difference with the credit card.

When the total “Committed” amount exceeds physical RAM + page file size, the system will warn you that “Your computer is low on memory” and may even force‑close some programs.

**Paged Pool vs. Nonpaged Pool**

The Windows kernel (the core of the operating system) reserves some memory for its own critical data. This memory is split into two types:

- **Nonpaged Pool**: Always stays in physical RAM and never gets moved to the page file. Some critical data (e.g., for handling hardware interrupts) must always be instantly accessible – you cannot wait for it to be read back from the hard drive.
- **Paged Pool**: Can be moved to the page file. This holds kernel data that can tolerate a short delay if it is needed.

Ordinary users do not need to track the exact values of these pools. But when you see them on Task Manager’s Performance page, just know: “paged” can be moved to disk, “nonpaged” must stay in RAM.

### 2.9 Hardware Codecs

When we introduced PotPlayer in Appendix 7, hardware decoding was mentioned. Here is a bit more detail.

**Hardware codec**: A dedicated circuit inside your GPU (graphics card) that handles video encoding or decoding. This circuit is separate from the streaming processors that run games.

- **Hardware decoding**: When playing a video, the GPU uses that dedicated circuit to decode it. The CPU barely works, power consumption is low, heat is minimal, and even high‑bitrate 4K video plays smoothly.
- **Hardware encoding**: When recording your screen, streaming, or exporting a video, that same circuit compresses the video. It is fast and has little impact on game performance.

**Software codec**: The CPU does all the work. The CPU can do it, but it is much less efficient than dedicated hardware. Playing 4K video or exporting video this way can max out your CPU, making your computer sluggish and hot.

**Scenarios you will actually encounter**:
- When recording games with OBS, choose NVENC (for Nvidia GPUs) or QSV (for Intel integrated graphics) – that uses hardware encoding. It has little impact on game frame rates.
- When you play a high‑bitrate 4K video and see very high CPU usage, the player is not using hardware decoding. Go into the player’s settings and look for “Hardware Decoding” or “DXVA”, then turn it on.

In the “Performance” tab of Task Manager, the GPU section often has two small graphs – “Video Encode” and “Video Decode” – showing hardware codec usage. Keep an eye on them.

### 2.10 Disk Performance (IOPS, MB/s, Mbps)

**IOPS (Input/Output Operations Per Second)**

Measures how well a drive handles “many small, random files”. For example, opening a folder full of small images, loading a game level, or running a virtual machine – in these scenarios, IOPS matters much more than sequential speed.

- Mechanical hard drive (HDD): several dozen to ~100 IOPS
- Typical SATA SSD: several thousand to tens of thousands of IOPS
- A good NVMe SSD: hundreds of thousands or even millions of IOPS

This is why installing your operating system on an SSD makes booting and launching software so much faster – not because of sequential read/write speeds, but because IOPS are hundreds or thousands of times higher than on a mechanical drive.

**MB/s vs. Mbps**

These two units are often confused, especially when measuring internet speed.

- **MB/s** (megabytes per second): The speed you see when copying files. 1 MB/s means transferring 1 megabyte of data per second.
- **Mbps** (megabits per second): The speed advertised in broadband plans. For example, a “100 Mbps” plan.

**The conversion**: 1 MB/s = 8 Mbps. So a 100 Mbps connection has a theoretical maximum download speed of 100 ÷ 8 = 12.5 MB/s.

Remember: lowercase ‘b’ stands for bit, uppercase ‘B’ stands for byte, and 1 byte = 8 bits. To get the download speed you actually see, divide the advertised Mbps by 8.

**MB/s vs. IOPS**: These are not the same thing. MB/s is “how much data per second”, IOPS is “how many requests per second”. Copying a large movie file stresses MB/s; opening a thousand small images at once stresses IOPS. A good drive should be good at both – but it will also cost more.

### 2.11 Just Get a Rough Impression

Environment variables, the Registry, Group Policy, and the page file – you do not need to change any of them now, and definitely should not mess with them randomly. But “getting a rough impression” matters:

- Later, if you install something and the command line says “not recognized as an internal or external command” – you will know it might be a PATH problem.
- Later, if an online tutorial says “edit the Registry” – you will know what that is, and that you should back it up before changing anything.
- Later, if the system complains that it is “low on memory” – you will know it might be because physical RAM or page file space is running out.
- Later, if your computer suddenly becomes sluggish and drive C: is full – you will know the page file might be affected.
- Later, if video playback is choppy or CPU usage is very high – you will know hardware decoding might not be enabled.
- Later, if a speed test shows a number that does not match the download speed you see – you will remember to divide Mbps by 8.

Just knowing roughly what these things are, and which direction to search when something goes wrong, already puts you ahead of most users.

## 3. Glossary

### 3.1 Process

A **process** is simply a running program. When you open a browser, a game, or Notepad, the system creates a process for it. Each process has its own memory space, separate from others.

When you end a frozen program in Task Manager, you are essentially killing its process. Be careful not to end things that look like system processes (e.g., `svchost.exe`), as that might cause problems.

### 3.2 Thread

A process can have multiple **threads**. Threads are the ones that actually do the work.

Think of it this way: a process is a factory, and threads are the workers inside it. A factory can have just one worker (single‑threaded) or many workers working at the same time (multi‑threaded). The advantage of multi‑threading is being able to do several things at once – your browser downloading a file while still rendering a webpage is the work of multiple threads.

The CPU usage percentage you see in Task Manager is actually the sum of usage across all threads. A heavy application often runs dozens or even hundreds of threads.

### 3.3 Handle

A **handle** is an integer number that Windows uses to refer to some resource (an open file, a window, a thread, etc.). When you open a file, the system gives you a handle; every operation on that file (read, write, close) uses that handle.

**Handle leak**: A program forgets to close a handle when it is done with the resource, so the resource remains occupied. The most common symptom: you try to delete a file, and Windows says “The file is in use”, even though you have closed every program that might have opened it. Very likely, some process has a handle that it never closed.

You can use tools like Process Explorer or LockHunter to see which process is holding onto the file.

### 3.4 Quick Reference for More Terms

| Term | Simple explanation |
|------|---------------------|
| API | Application Programming Interface – a contract that lets programs call each other |
| BIOS / UEFI | The low‑level system on the motherboard, responsible for booting and initialising hardware |
| DLL | Dynamic Link Library – a piece of code that can be shared by multiple programs |
| Driver | The “translator” that lets the operating system recognise and control hardware |
| GUI | Graphical User Interface – the windows, buttons, and things you click |
| CLI | Command‑Line Interface – the black box where you type commands |
| Kernel | The core of the operating system – manages hardware, processes, and memory |
| RAM | Random Access Memory – loses its data when power is off |
| ROM | Read‑Only Memory – usually stores firmware; the user cannot change it |
| SDK | Software Development Kit – a collection of tools needed to develop for a specific platform |
| Shell | A command‑line interpreter – both CMD and PowerShell are shells |
| UUID / GUID | A very long, almost‑guaranteed‑unique identifier, used to label something uniquely |