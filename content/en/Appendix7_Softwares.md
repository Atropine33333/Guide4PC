# Appendix 7: Useful Software

> This appendix introduces several pieces of software that are frequently used in daily work, covering compression, playback, downloading, editing, and system enhancement.

## 7.1 Compression Software: NanaZip / WinRAR

The built‑in extraction tool in Windows only supports `.zip` files. When you encounter `.rar` or `.7z` archives, you need a third‑party compression tool.

**[NanaZip](https://nanazip.org/)**: Open‑source, based on 7‑Zip, with a modern interface that fits Windows 11 and supports dark mode. You can install it from the Microsoft Store or download the `.msixbundle` directly. It is fully featured for both extracting and creating archives.

**[WinRAR](https://www.win-rar.com/)**: A classic compression tool that supports creating `.rar` archives (using a proprietary algorithm). After installation it shows a 40‑day trial reminder, but you can continue using it afterward. If you need to send encrypted or split archives to others, either WinRAR or 7‑Zip will work.

Both are fine for everyday use – just pick one and install it.

## 7.2 Video Player: PotPlayer

The built‑in “Movies & TV” player on Windows has limited codec support and cannot open many formats (such as `.mkv`, `.flv`, `.mov`). [PotPlayer](https://potplayer.tv/) launches quickly, supports almost all codecs out of the box, and can play the vast majority of video files without tweaking.

**Note**: PotPlayer is not the absolute best choice from a technical standpoint. More advanced users might prefer MPV with custom‑compiled codecs, or a specific version that supports Dolby Vision and HDR mapping. But for ordinary users, PotPlayer is stable and easy to use. If you already know how to set up a complex playback environment, this recommendation is not for you – your skill level is beyond the target audience of this appendix.

## 7.3 BitTorrent Client: qBittorrent

For downloading torrents or magnet links, **[qBittorrent](https://www.qbittorrent.org/)** is recommended. It is open‑source, ad‑free, and feature‑complete. It supports search plugins, sequential downloading, speed limits, IP filtering, and more.

**Common link types**:
- **Torrent file (`.torrent`)**: A small file (typically tens to hundreds of KB) containing metadata about the resource – file names, piece hashes, tracker addresses, etc. The download client opens the torrent and uses the information inside to connect to other peers and download the data.
- **Magnet link (`magnet:?xt=urn:btih:...`)**: An “upgraded” version of a torrent. You do not need to download a `.torrent` file first. The client uses the hash inside the link to locate the resource via the DHT network. Copy the whole link and add it to qBittorrent.
- **ed2k link (`ed2k://...`)**: A link for the eMule protocol, used on older P2P networks. Few resources use it now, though some old archives still do. qBittorrent does not support ed2k; you would need eMule or another client that supports that protocol.

**A few settings suggestions**:
- Add public tracker lists (see `trackerslist.com`) to improve connection success rates.
- Limit your upload speed (e.g., to 1 MB/s) to avoid hogging bandwidth and slowing down your network.
- Seed after downloading completes (e.g., to a 1:1 ratio or for 24 hours) – this is part of the BitTorrent sharing spirit.

## 7.4 Code and Plain‑Text Editor: VS Code

Even if you do not write code, **[VS Code](https://code.visualstudio.com/)** is a plain‑text editor worth installing. Compared to Notepad, it offers: syntax highlighting (automatically colors `.json`, `.xml`, `.md` files), tabs, regex search, and extensions (Markdown preview, spell checking, etc.).

Two simple things to do after installation:
1. Set the font: In settings, search for `Font Family` and enter `'JetBrains Mono', Consolas, monospace`; also enable `editor.fontLigatures` to support ligatures.
2. Install the Chinese language pack: Search the extension marketplace for “Chinese (Simplified)”.

From then on, use VS Code to view and edit any configuration file, log file, script, or note.

## 7.5 Image Extensions for Apple Devices: HEIF / HEVC

Photos taken on iPhones may be in `.heic` format (HEIF), and videos in `.hevc` format (HEVC / H.265). Windows cannot open them or show thumbnails by default.

You need to install two official extensions from the **Microsoft Store**:
- HEIF Image Extensions (free)
- HEVC Video Extensions (from the device manufacturer, free)

After installation, you can view photos and videos from Apple devices directly in File Explorer.

## 7.6 Terminal Enhancement: Windows Terminal

The default windows for CMD and PowerShell are rather basic – no tabs, mediocre font rendering, awkward copy‑and‑paste. **[Windows Terminal](https://apps.microsoft.com/detail/9n0dx20hk701)** is a modern terminal from Microsoft that supports:
- Multiple tabs and split panes
- GPU‑accelerated rendering, crisp fonts
- Custom themes, background images, transparency
- Simultaneously running PowerShell, CMD, and WSL in any combination

Pin it to your taskbar after installation, and use it for all command‑line operations from then on.

---

# Extended Reading

> The following sections cover technical details and alternatives for the software mentioned above. They are of medium difficulty. Read selectively depending on your interest.

## Extended 1: 7‑Zip Command Line Usage

The core of 7‑Zip is the command‑line version `7z.exe`. Many automation scripts (e.g., batch extraction, compressed backups) rely on it. Common commands:
- Extract: `7z x archive.7z -oC:\target`
- Compress: `7z a archive.7z C:\source`
- Test integrity: `7z t archive.7z`

NanaZip does not provide the command‑line version by default. If you need it, download the standalone command‑line program from the 7‑Zip website.

## Extended 2: PotPlayer and Hardware Decoding

PotPlayer uses its built‑in decoders by default, which may cause high CPU usage when playing 4K HDR content. You can switch to hardware decoding via your graphics card:
- Right‑click the video → Options → Filters → Video Decoder → Built‑in Decoder → Hardware Decoding (DXVA2 / D3D11) → set to “Automatic”.
- If your GPU supports it, you can also enable “HDR Passthrough” (requires a compatible display) or “Pixel Shader HDR Conversion”.

## Extended 3: qBittorrent Search Plugins

qBittorrent has a built‑in search feature, but you need to install Python and download search plugins (from the official GitHub repository). Once installed, you can search various resource sites directly from within the program (plugins must be added manually; none are provided by default). This feature involves copyright issues; use it in compliance with local laws.

## Extended 4: Recommended VS Code Extensions

- **PDF Viewer**: Browse PDF files inside VS Code.
- **Remote - SSH**: Connect to remote servers via SSH and edit files.

## Extended 5: Windows Terminal Configuration File Location

Windows Terminal settings are stored in `settings.json` (accessible via the drop‑down menu → Settings → “Open JSON file” in the lower‑left corner). You can edit this file directly to add custom color schemes, background images, etc. There are many ready‑made themes from the community (e.g., `catppuccin`, `dracula`).

## Extended 6: Two Versions of the HEVC Video Extension

There are two HEVC extensions in the Microsoft Store:
- **HEVC Video Extensions (paid)** : Costs 7 RMB, officially published.
- **HEVC Video Extensions from Device Manufacturer**: Free, but may require specific hardware conditions (such as a driver provided by the manufacturer). Most laptops come with Intel graphics drivers that include this extension; you can search for and install it directly.

If the free version tells you that you do not meet the installation requirements, try searching for the direct Store link for “HEVC Video Extensions from Device Manufacturer” via a search engine – often you can install it anyway.