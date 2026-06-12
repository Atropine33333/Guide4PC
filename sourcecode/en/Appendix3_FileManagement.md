# Turn Off BitLocker

# Turn Off BitLocker

# Turn Off BitLocker

**The very first thing you do after getting a new laptop: turn off BitLocker.**

BitLocker is Windows's full‑disk encryption feature. It encrypts your drive, and only a recovery key can unlock it. That sounds secure, but for individual users, the downsides heavily outweigh any benefit:

- You will almost never need it. BitLocker is designed for corporate or government scenarios where a lost device might leak sensitive data. The chance of losing your personal laptop is tiny, and your data is not that valuable to an attacker.
- If your system crashes, the motherboard fails, or you lose the recovery key, all your data is gone forever. Microsoft will not help you decrypt it.
- It slightly reduces drive performance, especially for small file reads and writes.

Your data is priceless. I strongly suggest you turn BitLocker off to avoid the risk of permanent data loss.

**How to check and disable it:**

1. Open Settings → Privacy & security → Device encryption (or search for "BitLocker"). If it says "Off", you are fine.
2. If it says "On", open PowerShell or Command Prompt as Administrator and run:
   ```powershell
   manage-bde -off C:
   ```
3. Wait for decryption to complete. You can check progress with `manage-bde -status` in the same terminal. Decryption may take from a few tens of minutes to hours. You can use your computer normally during this time, but keep it plugged in and powered on until done.

Ignore anyone who tells you to "keep BitLocker". For you, turning it off is the smarter choice.

# Appendix 3: File Management and Storage Philosophy

> This chapter covers only the most essential file management and storage operations. More in‑depth discussions are in the "Extended Reading" section.

## 3.1 Partition Your Drive: Separate C: and D:

New computers usually come with a single C: drive. Putting everything (system, documents, downloads) on C: means that if the system crashes and you need to reinstall Windows, all your data is lost. So it is wise to split the drive into two partitions.

**Example:** Your laptop has a 1TB SSD. You can leave 300GB for C: and use the remaining ~650GB for D:.

- **C: drive (system)**: Only Windows, drivers, and a few programs that must be on C: (e.g., certain development tools). Nothing else.
- **D: drive (data)**: Your documents, photos, videos, downloaded installers, Steam library, project code, etc.

**Why do this?** If Windows breaks and you have to reinstall, you only need to format C:. Everything on D: remains untouched. After reinstalling Steam, you can add the D: Steam library folder in settings – no need to redownload games.

**How to do it:** Use the built‑in Disk Management (right‑click "This PC" → Manage → Disk Management). Right‑click C: and choose "Shrink Volume" to free up space, then create a new simple volume with the free space and assign it drive letter D:. If unsure, search for "how to partition laptop drive" on YouTube and follow a recent tutorial.

## 3.2 Organise Files with Numbered Prefixes

Do not just pile folders directly on D:. It is much better to use a category + number scheme. For example:

```
D:\
├── 01_Work
│   ├── 2026_resume.docx
│   └── lab_reports
├── 02_Study
│   ├── textbooks
│   ├── course_notes
│   └── projects
├── 03_Media
│   ├── photos
│   ├── music
│   └── videos
├── 04_Downloads
│   └── (temporary – clean regularly)
└── 05_Archive
    └── old_files_backup
```

- **01_Work**: Internships, part‑time jobs, student club documents.
- **02_Study**: Course materials, papers, eBooks.
- **03_Media**: Personal photos, music, movies.
- **04_Downloads**: Browser cache. Clean it once a week or month – move useful files to other folders, delete the rest.
- **05_Archive**: Things you no longer use but cannot bear to delete (e.g., old versions of your graduation project).

**Why the number prefix?** Windows sorts by character order. `01_` appears before `02_`, so your most important folders are always the first you see when you open D:.

## 3.3 Game Storage: Delete When Done, Keep Cloud Saves

Games on Steam and Epic are now huge (50–100GB). You usually play only two or three at a time. Games you have finished and will never touch again just waste space.

**My approach:** Keep only the games you are actively playing. When you finish a game (or know you will not touch it for a while), first make sure your cloud saves are synchronised (Steam does this by default), then uninstall it from Steam. When you want to replay it, just download it again.

## 3.4 Backup Strategy: The 3‑2‑1 Rule (Simplified)

The 3‑2‑1 rule is a basic consensus in data protection:

- **3** copies of your data (original + two backups)
- **2** different storage media (e.g., internal SSD + external hard drive)
- **1** off‑site copy (e.g., cloud storage)

For students, you can simplify:

1. **Original data**: On your laptop D: drive.
2. **Local backup**: Every two weeks or every month, copy your most important folders (documents, photos, code projects) to an external hard drive.
3. **Off‑site backup**: Use a cloud service like Baidu Netdisk, OneDrive, or Aliyun Drive to sync a copy of your important files.

**Never use a no‑name USB flash drive as your only backup!** Cheap drives can die after just a few uses. If you need a flash drive, buy a reputable brand (e.g., SanDisk CZ74). For more details on hard drive selection and backup, see the Extended Reading section.

## 3.5 Simple Cloud Storage Choices

- **If possible, prefer international services**: OneDrive (the 1TB that comes with Office 365), Google Drive, Dropbox. Their clients are stable and they treat privacy somewhat better.
- **Chinese alternatives**: Baidu Netdisk (speed limited but usable for incremental backup), Aliyun Drive (currently less throttling).
- **Highly sensitive data** (e.g., ID card scans, password backups): **Never upload them directly to any cloud.** Use an open‑source encryption tool like VeraCrypt or a password‑protected AES‑256 7‑Zip archive, then upload the encrypted file.

## 3.6 Keep Your Computer Clock Accurate: Change the NTP Server

If your computer clock is often wrong (e.g., a few minutes slow after boot), it is because Windows's default time server is overseas and has high latency.

**Manual sync steps:**

1. Right‑click the clock on the taskbar → "Adjust date/time".
2. Under "Synchronise your clock", click "Sync now". If it fails, click "Add clocks for different time zones" → switch to the "Internet Time" tab → "Change settings".
3. In the "Server" box, enter one of these:
   - `ntp.tencent.com` (Tencent Cloud)
   - `ntp.aliyun.com` (Alibaba Cloud)
4. Click "Update now". Wait a few seconds for success.

If you are a university student, search for "your university name + ntp server" to get an on‑campus server (e.g., Tsinghua has `ntp.tuna.tsinghua.edu.cn`), which has even lower latency.

After this, your clock will automatically stay accurate.

---

# Extended Reading

> The following sections contain more detailed technical discussions and advanced knowledge. Each is independent – you can read any of them without relying on the main text.

## Extended 1: When BitLocker Actually Makes Sense (You Don't Need It)

BitLocker is only useful for the very few people who need extremely high security – handling trade secrets, legally sensitive information. If that is not you, ignore it. Just turn it off.

## Extended 2: A Closer Look at Partitioning

When using Disk Management to partition:

- The C: drive on a new computer has lots of free space. The "Shrink Volume" option shows the maximum you can shrink.
- After shrinking, you see unallocated space. Right‑click it → "New Simple Volume", follow the wizard, assign D: as the drive letter, and choose NTFS file system.
- If you accidentally delete the system partition, do not panic. Search for "Windows installation media repair boot".

Recommended video tutorial: Search for "how to partition laptop drive" on Bilibili or YouTube, pick a recent one with high views.

## Extended 3: Manually Backing Up Game Saves

Although platforms like Steam offer cloud saves, some older games or pirated copies do not sync. Saves are usually located at:

- `C:\Users\YourUserName\Documents\My Games`
- `C:\Users\YourUserName\AppData\Local` or `Roaming` (AppData is hidden; you can type the path directly into File Explorer's address bar)

After finishing a game, consider manually copying its save folder to `D:\GameSaves`.

## Extended 4: Buying and Using Hard Disk Drives for Backup (In‑Depth)

If you plan to buy a mechanical hard drive for cold backups (rarely powered on), here are some practical points.

**HDDs are sensitive to vibration while running. Do not move or bump them when powered on.**

**CMR vs. SMR – My personal preference**

CMR (Conventional Magnetic Recording) offers stable random write performance and easier data recovery – even if a small part is damaged, the rest is usually still accessible. SMR (Shingled Magnetic Recording) overlaps tracks to increase density (about 25% more capacity than CMR at the same platter count), which lowers cost per gigabyte (typically 10‑20% cheaper). For purely sequential writes of cold data, SMR is technically fine.

But personally, I simply dislike SMR. Its random write IOPS can drop by more than 50%, and localised damage may affect related tracks, making recovery trickier. So I only recommend CMR drives. Among consumer drives, Western Digital Red Pro and Seagate IronWolf Pro are typically CMR – search for "model number + CMR" before buying.

**Can SSDs be used for cold backups?**

Not recommended. SSDs store data as electrical charge, and over long periods without power (2‑3 years) the charge gradually leaks, causing data corruption. At higher temperatures, retention time drops further (about one year at 30°C, only half a year at 55°C). For important cold data, a mechanical hard drive is safer – but of course, always keep multiple copies everywhere.

**Using spare SSDs with an M.2 enclosure**

If you have a spare M.2 SSD, you can buy an M.2 enclosure to turn it into a fast portable drive. A brand I know with decent value is ITGZ (also called IGTZ). They offer an all‑metal enclosure with a 9210B controller, supporting 2230/2242/2280 drives, priced around 6‑10 USD. Note that the enclosure itself is cheap, but since SSD prices are currently high, buying a brand new SSD just for this purpose may not be cost‑effective.

**B Key vs. M Key – for those who happen to need it**

M.2 slots come in three key types:

- **M Key**: Supports PCIe x4 (high‑performance NVMe SSDs), notch on the right, bandwidth up to 32Gbps.
- **B Key**: Mainly supports SATA and PCIe x2, notch on the left, lower bandwidth.
- **B+M Key**: Both notches present, compatible with both slot types, but actual protocol depends on the slot.

**Regular power‑on – for all electronics**

Whether mechanical or solid‑state, leaving a drive without power for too long can cause issues. Mechanical drives need the spindle motor to spin occasionally for lubrication. For SSDs, JEDEC specifications state that data retention is measured under specific temperature conditions beyond which data may be lost. It is advisable to power on your backup drives every 3‑6 months, let them run for a few minutes, and check their health status.

**USB‑to‑SATA adapters and safe removal**

If you use a USB‑to‑SATA adapter (often called an "easy cable") with a 3.5‑inch mechanical drive, make sure it comes with a separate 12V power supply. USB power alone is often insufficient and may cause recognition failures or damage.

When unplugging, always click the "Safely Remove Hardware" icon in the Windows system tray and wait for confirmation before unplugging. The safest method is simply shutting down the computer – on shutdown, the system will automatically park the heads. Never pull the USB cable while the drive activity light is still blinking; that can easily cause the heads to crash onto the platters, creating physical bad sectors.

**SATA connector insertion limit**

SATA connectors have a limited insertion cycle (typically around 50‑100 times). Do not plug and unplug just for fun. If you use a USB‑to‑SATA adapter board, you can leave the drive attached to the board and only unplug the USB cable – this spares the SATA connector. The same applies to USB‑to‑SATA cables: keep the whole assembly connected and avoid repeatedly disconnecting the SATA end.

**Cable recommendation**

When buying Type‑C cables for external drives or laptop charging, consider looking for **Luxshare** OEM cables. Luxshare is a key Apple supply chain manufacturer and a leader in high‑speed cables. On second‑hand markets (e.g., Xianyu), you can sometimes find Luxshare 240W PD 3.1 USB‑C cables for less than 3 USD. Their build quality and electrical performance far exceed no‑name cables at the same price, making them great for drive enclosures or laptop charging. Another channel called "Scientific Picking" may also offer good deals on factory‑surplus cables.

## Extended 5: Practical Cloud Encryption

For sensitive files (ID scans, financial records, password backups), you must encrypt them before uploading.

**Method 1 – VeraCrypt**

- Download [VeraCrypt](https://veracrypt.io/en/Downloads.html) and create an encrypted volume (a virtual hard drive).
- Drag your sensitive files into it. After closing, you get a `.hc` file. Upload that file to the cloud.
- To decrypt, you need VeraCrypt and the password. The cloud provider cannot read the contents.

**Method 2 – 7‑Zip encryption**

- Right‑click the files/folder → 7‑Zip → Add to archive.
- Set a password, choose AES‑256 as the encryption method.
- Upload the resulting `.7z` or `.zip` file. Remember to also check "Encrypt file names" so that the contents are truly hidden.

## Extended 6: How NTP Time Synchronisation Works

NTP (Network Time Protocol) is used to calibrate your computer's clock over the internet. The motherboard has a CMOS battery that keeps time when the PC is off, but the battery eventually dies (usually after 3‑5 years). If your computer resets to 1970 every time you boot, the CMOS battery is dead – replace it with a CR2032.

Changing to a domestic NTP server greatly improves sync success rates. Besides Tencent and Alibaba, you can also try:

- `time.edu.cn` (for Chinese educational networks)

