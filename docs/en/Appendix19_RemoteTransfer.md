# Appendix 19: Remote File Transfer (Between Phone and Computer)

> This appendix introduces several ways to transfer files between a phone and a computer. No data cables, no hunting through confusing folder names in your phone's file manager. Pick the method that suits your needs.

## 19.1 Why Is Transferring Files So Annoying?

Common methods for transferring files between a phone and a computer each have their own frustrations:

- **WeChat / QQ**: Compresses images, blocks large files, and you must be logged into the same account on both devices.
- **USB cable**: You have to find the cable, plug it in, select the right mode on the phone, and then dig through a bunch of folders with cryptic names.
- **Bluetooth**: Slow and prone to disconnecting.
- **Cloud storage**: You upload first, then download – a long detour, often with speed limits.

There are better ways. The methods below are ordered by how strongly they are recommended.

## 19.2 Snapdrop: The Most Hassle‑Free Option

**Snapdrop** is a web‑based local network file transfer tool. No software installation required, no login needed. As long as your phone and computer are on the same Wi‑Fi network, you can transfer files by opening a browser.

**How to use**:
1. Connect both your phone and computer to the same Wi‑Fi network.
2. On both devices, open a browser and go to `snapdrop.net`.
3. The page automatically discovers other devices on the same network, showing them as circular icons.
4. Tap the icon of the target device, select a file, and on the receiving device click "Save" to accept.

**Advantages**:
- Nothing to install – just open a webpage.
- Files do not pass through any server; they are transferred directly between devices (peer‑to‑peer), so it is secure.
- Works on any device (Safari on iPhone, Chrome on Android, Edge on Windows – all fine).

**Disadvantages**:
- Both devices must be on the same Wi‑Fi network (cannot work across different networks).
- Very large files may be slow – it is best for files under 100 MB.

**In one sentence**: For quickly transferring a few photos or a small document, Snapdrop is the fastest.

## 19.3 KDE Connect: The Most Feature‑Rich Option

**KDE Connect** is an open‑source, all‑purpose cross‑device communication tool. Originally developed for the KDE desktop environment on Linux, it now supports Windows, macOS, Android, and iOS.

**What it can do besides file transfer**:
- Share clipboard between phone and computer (copy on your phone, paste on your computer with Ctrl+V).
- Sync phone notifications to your computer (read WeChat messages on your PC without picking up your phone).
- Use your phone as a remote control for your computer (control music playback, flip through PowerPoint slides).
- Find your phone (make it ring).
- Bidirectional file transfer.

**Installation**:
- **Windows**: Search the Microsoft Store for "KDE Connect".
- **Android**: Search Google Play or F‑Droid for "KDE Connect".
- **iOS**: Search the App Store for "KDE Connect".
- No login required on the computer side. As long as the phone and computer are on the same Wi‑Fi, they can pair.

**Pairing steps**:
1. Connect both devices to the same Wi‑Fi network.
2. Open KDE Connect on both devices. The phone should automatically discover the computer.
3. Tap "Pair" and confirm on the computer.

**In one sentence**: If you frequently switch between working on your phone and your computer, installing KDE Connect will make your life much smoother.

## 19.4 Shared Folder (SMB)

Snapdrop and KDE Connect both require action on both ends (one sends, one receives). If you simply want to "browse files on my computer from my phone at any time", the best approach is to set up a shared folder on your computer and mount it as a network drive on your phone.

**Setting up on the computer (Windows)** :
1. Right‑click the folder you want to share → Properties → Sharing → Advanced Sharing.
2. Check "Share this folder". In the permissions, add `Everyone` or `Guest` and give "Read" permission.
3. Note down your computer's local IP address: press `Win + R`, type `cmd`, then type `ipconfig`. Look for the "IPv4 Address" (e.g., `192.168.1.100`).

**Accessing from your phone**:

**Android**: Recommended app: **CX File Explorer** (from Google Play or the official website).
1. Open CX File Explorer → Network → Remote Storage → SMB.
2. Enter your computer's IP address (e.g., `192.168.1.100`) and your Windows username/password if required.
3. Once connected, you can browse files on your computer just like local files.

**iOS**: The built‑in "Files" app is sufficient.
1. Open the "Files" app → tap the `...` (three dots) at the top‑right → Connect to Server.
2. Enter `smb://192.168.1.100` (note the `smb://` prefix).
3. Enter your Windows username and password (the same credentials you use to log into Windows).
4. After connecting, the shared folder appears under "Shared".

**Advantages**:
- Set it up once, and it is always available.
- You can directly watch videos or open documents from your computer on your phone – no need to download them first.
- Multiple devices can access the same shared folder simultaneously.

**Disadvantages**:
- Only works on the local network (you cannot use it when away from your Wi‑Fi).

## 19.5 Temporary HTTP / FTP File Server

If you only occasionally need to download files from your computer to your phone (and never upload), you can start a temporary HTTP or FTP server on your computer. Your phone can then access it through a browser.

**Quick HTTP server on Windows (no extra software)** :
1. Open the folder you want to share. In the address bar, type `cmd` and press Enter.
2. In the command prompt, type `python -m http.server 8000` (requires Python to be installed).
3. On your phone, open a browser and go to `http://your_computer_IP:8000`. You will see a file listing; tap any file to download it.

**Using FTP** (better if you need to upload as well):
- Download a small FTP server application (e.g., Quick'n Easy FTP Server). It takes about 5 minutes to set up.
- On your phone, connect using CX File Explorer or a browser (`ftp://your_computer_IP`).

## 19.6 Summary

**Simple choice**:
- For a one‑off file transfer → use **Snapdrop**.
- If you want to deeply integrate your phone and computer → use **KDE Connect**.
- If you want to always have access to all files on your computer from your phone → set up a **shared folder** + CX File Explorer (Android) or the Files app (iOS).

All of the methods above stay **within your local network** (same Wi‑Fi). They do not consume mobile data and do not raise privacy concerns. Once you leave your home or dorm, they will no longer work – but that is a NAT traversal problem, covered in Appendix 18.