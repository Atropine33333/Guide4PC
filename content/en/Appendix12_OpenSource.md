# Appendix 12: Open Source

> This appendix introduces the basic concepts of open source software, common open source licenses, and a few noteworthy open source projects. This material is not essential for everyday computer use, but it is good to understand – much of the computers, phones, and internet you use today are built on open source software.

## 12.1 What Is Open Source

**Open source** means that a piece of software's source code is open to everyone. Anyone can view, modify, and distribute that code.

The opposite of open source is **proprietary software** – the source code is not public. Users can only run the compiled program; they cannot see how it works or modify it. Windows, Microsoft Office, Photoshop, and Steam are all proprietary software.

Open source does **not** mean free of charge. Many open source programs are indeed free to use, but the core meaning of "open source" is about **freedom** – freedom to use, to study, to modify, and to share. Some open source products are sold (e.g., enterprise editions or paid support).

## 12.2 Free Software vs. Open Source Software

These two terms are often used interchangeably, but their underlying philosophies differ slightly.

**Free Software** is a concept promoted by the **Free Software Foundation (FSF)** , founded by Richard Stallman in 1985. Here "free" refers to freedom of speech, not free beer. The FSF emphasises the ethical dimension: users should have control over the software, not the other way around.

**Open Source Software** emerged a bit later (1998) and focuses more on technical advantages: opening the source code leads to better quality, higher security, and faster iteration. The term deliberately avoids the ideological language of "free software", making it more palatable to businesses and the commercial world.

In practice, the two overlap heavily – most free software is also open source, and vice versa. But in spirit, the FSF is more "hardcore", emphasising user rights and ethics, while the open source movement is more pragmatic, focusing on collaboration and efficiency.

## 12.3 Open Source Licenses

An open source license specifies what others can do with your code and what conditions they must follow. Different licenses vary significantly, and choosing the wrong one can create legal risks.

### 12.3.1 Permissive Licenses

These licenses place few restrictions on users. You can take the code, use it, modify it, and even release your modified version as closed source (without sharing your changes), as long as you retain the original copyright notice.

**MIT License**: One of the simplest licenses. It says only two things: keep the copyright notice, and don't hold the author liable. Many small Node.js and Python projects use it.

**Apache License 2.0**: Similar to MIT, but adds a few clauses: it explicitly grants patent rights (to avoid certain patent traps) and requires you to state which files you modified. Projects from the Apache Software Foundation – such as Hadoop and Kubernetes – use this license.

**BSD License**: Very similar to MIT. There are several variants (2‑Clause, 3‑Clause, 4‑Clause). The differences involve whether you can use the author's name for advertising and whether all derivative works must retain the original author's information. FreeBSD and early versions of the Go language use BSD licenses.

### 12.3.2 Strong (Copyleft) Licenses

These licenses require that your modifications and derivative works must also be open sourced under the same license.

**GPL (GNU General Public License)** : The most famous copyleft license. If a piece of software uses GPL‑licensed code, then the entire software must be released under the GPL. The Linux kernel, Git, and the GCC compiler all use the GPL.

- **GPLv2**: The version used by the Linux kernel.
- **GPLv3**: Released in 2007, adding provisions about patents and anti‑circumvention. Some projects (including the Linux kernel) consider v3 too strict and stick with v2.

**LGPL (Lesser GPL)** : A relaxed version of the GPL. If you only **link** to an LGPL library (e.g., use a `.dll` or `.so`), your own code does not need to be open source. However, if you modify the library itself, those modifications must be open sourced. Often used for function libraries such as GTK and parts of FFmpeg.

**AGPL (Affero GPL)** : The GPL has a loophole: if you run GPL code on a server and users only interact with it over the network (without receiving a copy of the software), you are not required to release your code. AGPL closes this loophole – providing a service over the network counts as distribution, so you must open source your code. Early versions of MongoDB used the AGPL.

### 12.3.3 Which One Should You Choose?

| Your goal | Recommended license |
|-----------|---------------------|
| Let anyone use the code, even in closed‑source commercial products | MIT / Apache 2.0 |
| Ensure the code and all derivatives remain forever open source | GPL |
| You are writing a library – allow others to link to it without forcing their code open | LGPL |
| You offer a network service and don't want someone to take it, tweak the UI, and sell it closed‑source | AGPL |

Choosing a "wrong" license is not a disaster – you can change it later. However, if you use GPL‑licensed code in your project, your code **must** be released under the GPL. This is a legal requirement, not a suggestion.

## 12.4 Mirror Sites

A **mirror site** is a replica of a large software repository or open source project. Because the original servers are often outside China, access can be slow and unreliable. Universities and companies inside China set up mirrors that synchronise regularly with the original sources, allowing domestic users to download at high speed.

Some of the best‑known mirrors in China:

- **Tsinghua University [TUNA](https://mirrors.tuna.tsinghua.edu.cn/) Mirror**: Large, fast‑updating, and comprehensive. Many of the software download links recommended earlier in this guide come from here.
- **USTC (University of Science and Technology of China) [UTSC](https://mirrors.ustc.edu.cn/) Mirror**: Similarly long‑standing and broad in coverage.
- **Shanghai Jiao Tong University [SJTUG](https://mirrors.sjtug.sjtu.edu.cn/) Mirror**: One of the download locations for the SJTUThesis template.
- **[Alibaba Cloud](https://developer.aliyun.com/mirror/), [Tencent Cloud](https://mirrors.cloud.tencent.com/), [Huawei Cloud](https://mirrors.huaweicloud.com/home)** : They also run open source mirrors, focusing on operating systems and development tools.

**How to use**: Different package managers have different methods for changing the mirror source. I recommend searching on your own. Example search query: `pip change source TUNA`

## 12.5 Notable Open Source Software

You may not have used the following open source applications, but they run quietly behind the scenes in many industries. It's enough to know their names and what they do.

### 12.5.1 The Linux Kernel

**Linux** is an open source operating system kernel, released by Linus Torvalds in 1991. It is not a complete operating system like Windows, but the lowest‑level core of the system – managing hardware, processes, memory, and the file system.

**Android** – the OS on your phone – runs on the Linux kernel. More than 70% of the world's servers run Linux. Routers, smart TVs, car infotainment systems, ATMs, supercomputers – the vast majority run Linux. Without Linux, the internet as we know it would not exist.

### 12.5.2 GIMP

**GIMP (GNU Image Manipulation Program)** : An open source image editing application, often described as the "open source alternative to Photoshop". It is very powerful – layers, filters, masks, paths, colour adjustments – all present. The interface is different from Photoshop and requires some learning. Suitable for people who do not want to pay for Photoshop but need to edit images occasionally.

### 12.5.3 LibreOffice

**LibreOffice**: An open source office suite, comprising Writer (documents), Calc (spreadsheets), Impress (presentations), Draw (drawing), and Base (databases). It is compatible with Microsoft Office formats (`.docx`, `.xlsx`, `.pptx` all open). The interface is a bit rougher than Office, but it is the standard default on Linux systems. If you do not want to subscribe to Office 365, give it a try.

### 12.5.4 Krita

**Krita**: An open source digital painting application, aimed at illustrators and concept artists. Its brush engine is very powerful, supporting pressure sensitivity, stabilisation, and brush smoothing. The comic features (storyboarding, speech bubbles) are also mature. The difference from GIMP: GIMP is more like Photoshop (general image editing), while Krita is more like Procreate / Painter (dedicated drawing). Many animation studios use it.

### 12.5.5 Blender

**Blender**: An open source 3D creation suite. Modelling, sculpting, materials, rendering, animation, video editing, compositing – it does everything. Once dismissed as "amateur software", it is now an industrial‑grade tool that can compete with Maya and 3ds Max. The Hollywood animated film *Spider‑Man: Into the Spider‑Verse* used Blender, and some promotional videos for *Arknights* and *Genshin Impact* were made with Blender. The learning curve is steep, but it is completely free.

### 12.5.6 VLC

**VLC**: An open source multimedia player. Similar to PotPlayer, but VLC is cross‑platform (Windows, macOS, Linux, Android, iOS). It can play almost any video format without requiring extra codecs. Small footprint, fast startup, and can also act as a network streaming client. We recommended PotPlayer earlier; VLC is a good alternative.

### 12.5.7 7‑Zip / NanaZip

**7‑Zip**: An open source compression tool. NanaZip, recommended earlier, is a modernised interface for 7‑Zip. The core 7z format achieves higher compression ratios than ZIP. Its command‑line version is used in many automation scripts.

### 12.5.8 FFmpeg

**FFmpeg**: An open source command‑line tool for audio and video processing. It is the Swiss Army knife for video professionals. You can use it to convert formats, cut clips, merge videos, extract audio, burn subtitles. OBS, VLC, YouTube, and Netflix all use FFmpeg behind the scenes. It was mentioned earlier in the video processing section.

### 12.5.9 OBS Studio

**OBS (Open Broadcaster Software)** : An open source tool for live streaming and screen recording. Compared to the paid XSplit, OBS is no less capable and is completely free. It supports multiple scenes, multiple sources, filters, and streaming. Many game streamers and online course recorders use it. Its configuration was covered in Chapter 4.

### 12.5.10 VS Code

**VS Code**: Microsoft's open source code editor. Recommended in Appendix 7 – with plugins, it can do almost anything. Although developed by Microsoft, the code is open source (MIT license). Some joke that "Microsoft is now one of the largest open source contributors" – Azure documentation, TypeScript, .NET Core are all open sourced.

### 12.5.11 osu!

**osu!**: An open source music rhythm game. Players click, drag, and spin along with the music. There are four modes (standard, taiko, catch the fruit, piano). The community is very active, with millions of user‑made beatmaps. It has excellent tablet support – many people play it with a drawing tablet. The game itself is free and open source, sustained by donations and paid skins.

## 12.6 A Final Thought

The computer you use today – opening a browser (Chromium engine), watching videos (FFmpeg decoding), chatting (Linux servers forwarding messages), browsing forums (hosted by Nginx or Apache) – all of it runs on open source projects.

You may never need to read a single line of kernel code, nor submit a pull request to any project. But knowing that these things exist, knowing where they came from and under what rules they operate, will deepen your understanding of the digital world.

**They are not perfect, but without them, today's world would not look the same.**

## 12.7 Creative Commons Licenses (CC)

**Creative Commons (CC)** is not a software open source license. It is a set of public licenses for **creative works** – articles, images, music, videos, blog posts, courseware, Wikipedia entries, and more. The goal is to allow creators to proactively declare "how others may use my work", instead of defaulting to "all rights reserved".

### 12.7.1 Why CC Licenses Exist

Traditional copyright defaults to "All Rights Reserved". You draw a picture and upload it. If someone wants to share it, translate it, or turn it into a wallpaper, legally they must ask your permission first. In practice this is very inefficient: many authors are unreachable, or they would not mind sharing but have never explicitly said so.

CC licenses provide a standardised set of "Some Rights Reserved" statements. A creator can clearly tell everyone: you may copy, share, modify, and even commercially use my work, as long as you meet my conditions (e.g., attribution, non‑commercial use).

### 12.7.2 The Four Core Elements

All CC licenses are built from combinations of the following four conditions:

| Icon | Abbr | Condition | Meaning |
|------|------|-----------|---------|
| ![BY](https://mirrors.creativecommons.org/presskit/buttons/88x31/png/by.png) | **BY** | Attribution | You must credit the author, the work's title, and the license. All CC licenses include BY. |
| ![SA](https://mirrors.creativecommons.org/presskit/buttons/88x31/png/by-sa.png) | **SA** | ShareAlike | Modified works must be released under the same license. Similar to GPL's copyleft. |
| ![NC](http://mirrors.creativecommons.org/presskit/buttons/88x31/png/by-nc.png) | **NC** | NonCommercial | Only non‑commercial uses are allowed. Commercial use requires separate permission. |
| ![ND](http://mirrors.creativecommons.org/presskit/buttons/88x31/png/by-nc-nd.png) | **ND** | NoDerivatives | You may copy and share, but not modify, adapt, or create derivatives. |

### 12.7.3 The Six Main CC Licenses

Combining the above conditions gives six common licenses, from most permissive to most restrictive:

| Abbreviation | Full name | Conditions | What is allowed |
|--------------|-----------|------------|------------------|
| **CC BY** | Attribution | BY | Any use (including commercial), can modify, can release derivatives under a different license. |
| **CC BY-SA** | Attribution-ShareAlike | BY + SA | Any use, can modify, but derivatives must use the same license. Wikipedia uses this. |
| **CC BY-NC** | Attribution-NonCommercial | BY + NC | Can share and modify, but non‑commercial only; can change the license. |
| **CC BY-NC-SA** | Attribution-NonCommercial-ShareAlike | BY + NC + SA | Non‑commercial only, can modify, but derivatives must use the same license. |
| **CC BY-ND** | Attribution-NoDerivatives | BY + ND | Can share (including commercially), but cannot modify. |
| **CC BY-NC-ND** | Attribution-NonCommercial-NoDerivatives | BY + NC + ND | Most restrictive. Can only share as‑is, no modifications, no commercial use. |

### 12.7.4 CC0 and the Public Domain

There is also a special version: **CC0 (Public Domain Dedication)** . A creator who chooses CC0 dedicates their work to the **public domain** – they waive all copyright rights. Anyone can use, modify, or commercially exploit the work without attribution and without any conditions.

CC0 is not one of the "six licenses". It is a separate "no copyright" declaration. Some images on Unsplash, OpenStreetMap data, and certain academic databases use CC0.

The **public domain** refers to works whose copyright has expired (e.g., the author died more than 70 years ago – varies by jurisdiction) or has been voluntarily waived. All CC licenses (including CC0) are logically "copyright‑based open permissions", whereas the public domain is "no copyright" – legally even more absolute.

### 12.7.5 License Versions

CC licenses have gone through several versions: 1.0, 2.0, 2.5, 3.0, and 4.0. **4.0 is the latest (released 2013)** . It fixes many legal issues, is more international, and is easier to interoperate with other licenses.

When choosing a license, prefer **version 4.0**. Some older works may be marked 3.0 or 2.5 – those are still legally valid, but their porting to different jurisdictions may vary.

### 12.7.6 Typical Choices for Different Scenarios

| Your goal | Recommended license |
|-----------|---------------------|
| You wrote a popular science article and want it widely read – you don't mind others adapting it into videos or even selling it, as long as you are credited | CC BY |
| You drew an illustration – others may use it as wallpaper or print it on T‑shirts, but cannot sell those items | CC BY-NC |
| You wrote a technical tutorial – others may translate, modify, and republish it, but the translated version must also be open (similar to GPL) | CC BY-SA |
| You took a photo – others may use it freely, but they cannot crop out your watermark or alter it beyond recognition | CC BY-ND |
| Your thesis is uploaded to the university repository – you want it to be downloadable and shared, but not modified or sold | CC BY-NC-ND |
| You took a casual snapshot and truly don't care if someone uses it on a commercial poster | CC0 |

### 12.7.7 How to Find CC‑Licensed Works

- **Images**: Flickr's "Advanced Search" lets you filter by CC license. Unsplash images are mostly CC0 or similar terms. Wikimedia Commons files all have clear CC labels.
- **Music**: Free Music Archive (FMA), Jamendo. The YouTube Audio Library also has a CC category.
- **Text**: Wikipedia (CC BY‑SA). Some articles on Medium (look at the footer).
- **Courseware**: MIT OpenCourseWare (CC BY‑NC‑SA). Some resources from Chinese university MOOCs.

### 12.7.8 A Friendly Reminder

CC licenses are legal tools based on copyright. They are effective in China as well (Chinese copyright law recognises the granting of partial rights through licenses). However, keep in mind:

- Works marked **NC (NonCommercial)** cannot be sold. What counts as "commercial"? Using them in internal company training, printing them on products for sale, putting them behind a paywall – all of these are commercial uses.
- Works marked **ND (NoDerivatives)** cannot be translated, remixed, or adapted. Simple copying and sharing is fine.
- If you fail to meet a condition (e.g., forget to provide attribution), your permission to use the work automatically ends – unless you obtain separate permission from the author.
- In addition to the human‑readable deed (a summary) and the machine‑readable metadata, every CC license has a full legal code. In case of disputes, the legal code governs.