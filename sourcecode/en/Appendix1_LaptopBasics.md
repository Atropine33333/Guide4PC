# Appendix 1: Laptop Basics

>This chapter covers only the most essential concepts and operations. If you have never used a computer before, start here. More in‑depth discussions are in the "Extended Reading" section at the end.

## 1.1 Hardware Basics

### 1.1.1 What is a CPU

The CPU (Central Processing Unit) is the computing and control core of a computer. It interprets instructions, processes data, and coordinates all components. Think of it as the "brain" of the machine.

### 1.1.2 What is a GPU

The GPU (Graphics Processing Unit) specializes in graphics and parallel computing. Laptop GPUs come in two forms:

- Integrated GPU (iGPU): packaged inside the same chip as the CPU, has no dedicated video memory – it borrows a portion of system RAM.
- Discrete GPU (dGPU): a separate chip with its own video memory.

### 1.1.3 What is RAM (Memory)

RAM is the temporary warehouse where the CPU reads and writes data directly. It is fast, but loses all data when power is cut. When you open a program, the system copies its data from the storage drive to RAM.

### 1.1.4 What is a Storage Drive

The storage drive holds data permanently – data survives after shutdown. Modern laptops almost exclusively use Solid‑State Drives (SSDs).

### 1.1.5 The Difference Between RAM and Storage

Use a kitchen counter and a fridge as a metaphor:

- RAM is the countertop: the ingredients you are actively chopping and preparing are right in front of you for quick access. Counter space is limited, and when you finish cooking (power is cut), any leftover ingredients on the counter are thrown away.
- Storage is the refrigerator: all ingredients are kept there for the long term, and you take them out to the counter when needed.

## 1.2 Integrated vs. Discrete GPU (Basic Concepts)

- iGPU: low power, low heat – suitable for office work, web browsing, and light gaming.
- dGPU: high performance – suitable for demanding games, 3D modelling, video editing, etc.

Real‑world gaming performance of iGPUs is limited by thermal and power constraints; see Extended Reading.

## 1.3 Touchpads (What You Need to Know)

High‑end thin‑and‑light laptops in 2026 use mostly two types of touchpads:

- Haptic touchpad: cannot be pressed when the device is off. When powered, it detects pressure via sensors and provides vibration feedback through a haptic motor. The feel depends entirely on the manufacturer's tuning.
- Full‑area mechanical touchpad: the entire surface clicks down (unlike traditional touchpads where only the bottom half moves). However, on many Windows laptops, the required force differs between the top and bottom areas.

Strongly recommended: go to an Apple store and try the MacBook Pro's haptic touchpad (the industry benchmark for haptic feedback) and the MacBook Neo's full‑area mechanical touchpad (the benchmark for that type). Get a feel for their uniform, responsive clicks. Then come back and try Windows counterparts – you will immediately know what level of quality you can accept.

## 1.4 Laptop Classifications

- Thin and light: portable, long battery life, quiet. Uses low‑power CPU, no discrete GPU. Thickness around 15mm, weight under 1.3kg. Suitable for office work, study, light entertainment.
- Gaming laptop: high performance, with a high‑power CPU and mid‑range to high‑end discrete GPU. Heavy cooling, weight often exceeds 2kg. Suitable for demanding games, professional video editing, etc.
- All‑rounder / creator laptop: sits between the two – somewhat portable (around 1.8kg), with an entry‑level or mid‑range discrete GPU. Can handle office work and light gaming.

## 1.5 Buying Reference: 笔吧评测室

笔吧评测室 is one of the more authoritative laptop review sources in China. Follow their WeChat public account or Bilibili channel (also on Douyin and Xiaohongshu). Their WeChat account publishes weekly curated recommendations by price range, with detailed explanations. Read those to narrow down a few candidate models, then search for in‑depth reviews and user feedback.

## 1.6 Purchase Advice (2026)

- Prices are high: laptop prices are at historical highs in 2026. If you are not in urgent need (e.g., your current computer still works), you can wait – prices will likely drop.
- If you must buy: thin‑and‑light laptops offer better value in the current market. The iGPU performance of modern high‑end thin‑and‑light laptops is sufficient for many games and daily software.
- Government subsidies: many provinces offer 10‑20% consumer subsidies. If possible, consider travelling to a province with such subsidies to buy your laptop. Search for "province name + appliance subsidy" for details.

## 1.7 From Unboxing to Activation: Inspection Procedure (Core Steps)

When you receive your laptop, follow these steps. Before activation, you have unconditional return rights. If you find any problem, return it immediately – do not attempt to fix it yourself.

1. **Check the packaging and exterior**
   - Confirm that the seals are intact.
   - Inspect the lid (A‑cover) and bottom (D‑cover) for scratches or dents.
   - Check the power adapter and charging port for signs of use.
   - Gently open and close the lid a few times; listen for creaking.
   - For a mechanical (traditional hinged) touchpad: gently wiggle the edges to see if it rocks, and press all four corners to feel for consistent resistance. For a haptic touchpad: test the feedback in different areas after booting into the OS.
   - Lightly press different areas of the keyboard to feel for overall rigidity.
   If you find any anomaly that you couldn't accept, take photos at once and contact customer service for a return.

2. **Transport mode test**
   - **Do not plug in the power adapter.** Press the power button once.
   - If it does **not** turn on → normal, proceed.
   - If it turns on and boots into the OS immediately → the unit has been used. Return it immediately.

3. **First boot setup (skip internet)**
   - When you reach the login screen, **do not connect to Wi‑Fi**.
   - To bypass Windows 11's forced internet requirement:
     - At the region selection screen, press `Shift + F10` to open Command Prompt.
     - Type `OOBE\BYPASSNRO` and press Enter. The system will restart.
     - After reboot, when you reach the "Let's connect you to a network" screen, click **"I don't have internet"** → **"Continue with limited setup"**.
     - Log in with a **local account** (use an English alphanumeric username).

4. **Screen inspection**
   - After entering the desktop, right‑click → Personalize → Background → Choose a solid color. Cycle through pure white, pure black, pure red, pure green, and pure blue. Carefully look for dead pixels (unexpected bright or dark dots).
   - On a pure black background, check for backlight bleed. Minor IPS glow at the edges is normal, but if a corner glows like a small flashlight, consider a return.

5. **Verify the configuration**
   - Press Win + X → Task Manager → Performance to confirm CPU model and RAM capacity.
   - Download a drive inspection tool (e.g., CrystalDiskInfo) to check the SSD's brand and model against reviews.
   - [CrystalDiskInfo's Offical Download Site](https://crystalmark.info/en/download/#CrystalDiskInfo): <https://crystalmark.info/en/download/#CrystalDiskInfo>
   - Or you may download with `winget install -e --id CrystalDewWorld.CrystalDiskInfo`

6. **Usability test**
   - Lift the laptop with one hand – any unusual creaking?
   - Plug in a USB drive and a mouse; confirm all ports work.
   - After fully charging, use it on battery for an hour of light use to rule out abnormal battery drain.

7. **7‑day no‑questions‑asked return**
   - If you find any of the following: dead pixels (especially in the centre), backlight bleed affecting normal viewing, abnormal touchpad click feel, unusual battery wear, or widespread reports of a defect in this batch – directly request a **return (not an exchange)** and either reorder or switch to another brand.

8. **Create a Microsoft account and activate**
   - Go to [signup.live.com](https://signup.live.com) and create a free account.
   - Log in to activate the pre‑installed Office suite (within 30 days, otherwise the license may expire).
   - With an account, you can digitally reactivate Windows after a clean reinstall without entering a product key.

9. **Finally, connect to the internet**
   - After all checks are complete, connect to Wi‑Fi. Windows and Office will activate automatically.

---

# Extended Reading

>The following sections contain detailed technical discussions and advanced knowledge. Each is independent – you can read any of them without relying on the must‑read part.

## Extended 1: CPU Models and Performance

Mainstream thin‑and‑light laptops in 2026 use CPUs such as the Intel Core Ultra 5 338H、Ultra X7 358H、Ultra X9 388H. Mainstream gaming laptops often pair an Intel Ultra 7 356H with an NVIDIA RTX 5060 or 5070. Laptop CPU naming can be confusing – the same architecture may spawn multiple model numbers. For example, AMD's 8845H is essentially a rebranded 7845H (identical specifications, only the name differs). Always search for performance reviews of the exact model before buying; do not judge by the numbers alone.

## Extended 2: iGPU Gaming – Expectations vs. Reality

Some marketing claims "iGPU performance close to an RTX 3050 Ti". However, note that an iGPU has no dedicated video memory – it borrows system RAM – and the CPU and iGPU share the same thermal solution. Under sustained gaming loads, the CPU and iGPU compete for power and thermal headroom, so real‑world performance often falls short of benchmark numbers. If you plan to play AAA games on an iGPU laptop, watch game‑specific tests by reviewers like 笔吧评测室 before deciding.

## Extended 3: Sleep States (S0ix vs. S3)

Windows defines several power sleep states. S3, known as "Suspend to RAM", powers down the CPU and most hardware while keeping RAM refreshed – wake‑up is very fast (about 1‑2 seconds). S0ix (Modern Standby, also called S2idle) is a low‑power idle state promoted by Microsoft since Windows 8, allowing the system to stay connected and update while the screen is off – similar to a smartphone. However, it can cause heat, battery drain, and unreliable wake‑up. Since Intel Meteor Lake (12th‑gen Core), Microsoft has required hardware vendors to drop full support for S3. Consequently, many new laptops support only S0ix. Even running Linux, the ACPI tables are stripped of S3 code, making reliable sleep impossible. If sleep reliability matters to you, check community feedback before buying – some business laptops (e.g., certain ThinkPad models) still retain S3 options.

## Extended 4: Touchpad Tuning Differences

Even among haptic touchpads, the vibration feedback varies greatly by manufacturer – for example, Xiaomi and Honor feel completely different. Therefore, it is worth visiting a physical store to try them yourself. If you are unsure what you like, first try the MacBook Pro's haptic touchpad (the industry benchmark) and the MacBook Neo's full‑area mechanical touchpad (also a benchmark). Then compare with Windows laptops – you will clearly see what level of quality you can accept.

## Extended 5: Storage Drive Selection

**NAND Manufacturers vs. Assembly Houses**

The core of an SSD is the NAND flash chip. Companies that manufacture their own NAND are called NAND manufacturers or IDMs. Major active NAND manufacturers include Samsung, SK hynix (including the acquired Intel NAND business, now Solidigm), Micron, Kioxia, Western Digital, and China's YMTC. YMTC's Xtacking architecture (e.g., X4‑9040) is competitive in both performance and cost.

The controller is the "small CPU" of an SSD, handling data management, wear leveling, garbage collection, etc. Notable retail controller vendors include Silicon Motion, Phison, Maxio, and InnoGrit.

**TLC vs. QLC**

- TLC (Triple‑Level Cell): stores 3 bits per cell – good write endurance and speed.
- QLC (Quad‑Level Cell): stores 4 bits per cell – higher density, lower cost, but significantly worse write endurance and write speed.

For a system drive or frequent large writes, QLC is generally not recommended. The Solidigm P41 Plus is a rare exception – its excellent firmware mitigates QLC's drawbacks.

**Original vs. Third‑party SSDs**

- Original SSD: designed, packaged, and sold by the NAND manufacturer itself – e.g., Samsung, Western Digital, Kioxia, ZhiTai.
- Third‑party SSD: buys raw NAND wafers and then packages, tests, and assembles them – e.g., Kingston, ADATA.

Quality depends on the vendor's packaging capability. Vendors with their own packaging and rigorous testing (e.g., UMIS's UnionMemory brand) are acceptable. Avoid any brand that only does sticker assembly – their NAND sources are unknown, and the drive may fail at any moment.

**OEM vs. Retail Drives**

OEM and retail drives are identical hardware. Retail drives come with official warranty and driver support; OEM drives are warrantied through the channel vendor at a lower price. OEM drives are typically sourced through secondary channels – always verify the seller's warranty terms before purchase.

**Personal Recommendations (for reference only)**

My first recommendation is original SSDs, e.g., ZhiTai TiPro 9000. Among OEM drives, the following three are worth considering:

- PVC10 (SK hynix)
- PCB01 (SK hynix)
- AM6C1 (UnionMemory, under UMIS)

Avoid Samsung PM9A1 and any model known for "0e" errors (uncorrectable data corruption leading to permanent data loss). Also avoid any white‑label, gray‑label, or no‑name SSDs.

Your storage drive holds all your important data – essays, photos, code, game saves. A low‑quality drive may die suddenly, and data recovery is expensive and not guaranteed. Saving a few dollars is not worth that risk.

**Considerations for Adding an SSD to a Laptop**

Before adding an SSD to your laptop, check the following:

- Empty M.2 slot: search for motherboard photos or disassembly videos of your model. The second slot on some laptops may only support short drives (2242 or 2230); installing a 2280 drive may not fit.
- M.2 form factors: common lengths are 2230 (30mm), 2242 (42mm), and 2280 (80mm). 2280 is the most common. For 2242, the PVC10 and PC411 are worthwhile choices.
- Power consumption and drop‑out risk: laptops have far less power delivery and cooling than desktops. Verify the drive's sustained power consumption – a high‑power drive can overheat and disconnect in a thin‑and‑light.
- Recommended reviewers: Bilibili creators "前进的四火" and "HOMOLAB" produce high‑quality SSD reviews – they use professional tools like fio for steady‑state testing, not just peak speeds.
- Current timing: SSD prices are high in 2026 – if not urgent, wait for prices to drop.

## Extended 6: Display Selection

Display preference is subjective – I strongly recommend visiting a physical store to compare different panels with your own eyes.

**Personal preference**

Some users prefer high‑quality LCD panels (especially IPS or MiniLED backlit) because LCD has no OLED burn‑in risk, higher full‑screen brightness, and sharper text.

**Resolution vs. Refresh Rate**

Compared to high refresh rates (e.g., 120Hz), high resolution (e.g., 2560×1600 or 3840×2160) delivers crisper text and images, significantly improving productivity for office work, coding, and reading. 60Hz is sufficient for non‑competitive gaming. When budget is limited, prioritising resolution is a reasonable choice.

**Color Accuracy and Gamut Coverage**

Color accuracy (measured by Delta E) and gamut coverage are equally important. Color accuracy affects your judgement of colours – for example, you take a photo with your phone, edit it in Lightroom on your laptop; if the display is inaccurate, the colours you adjust may look completely wrong to others. Even for casual photo editing or watching movies, you want colours to be reasonably true. Gamut coverage determines how many colours the display can show – common standards include sRGB, DCI‑P3, etc. For most users, covering 100% sRGB is the baseline.

**Factory Calibration and Calibrators**

Modern thin‑and‑light laptops generally have decent out‑of‑box colour accuracy, but do not blindly trust "factory calibrated" claims. Factory calibration is often done on a few samples, and the reported Delta E is measured under specific brightness and mode – it does not guarantee your unit is the same. If you truly demand high colour accuracy (note: if you do not know what "colour accuracy" means, you probably do not need it for the foreseeable future), the most reliable method is to rent a colorimeter (e.g., Spyder or Calibrite) and calibrate your screen following a tutorial. Generate and load an ICC profile. Calibrate once a year.

## Extended 7: Buying Principles for Gaming and All‑rounder Laptops

For gaming laptops, a common core principle is: given acceptable performance release and a decent display, choose the cheapest one. Different brands with the same CPU/GPU can have vastly different sustained power limits (i.e., performance release). Check real cooling and stress tests from reviewers like 笔吧评测室. Only after confirming that performance release is not crippled should you compare prices.

For all‑rounder laptops, trying them in person is recommended – chassis thickness, cooling, and fan noise vary greatly between models.
