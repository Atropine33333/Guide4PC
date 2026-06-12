# Appendix 4: Quick Reference for Common Ports

>This chapter helps you identify the shape, purpose, speed, and power delivery of various ports on your laptop – so you don‘t buy the wrong cables or docking stations.
>The Must-Read part covers only what you need day‑to‑day (does it fit? can it do the job?). Detailed version tables are in Extended Reading – consult them when you need specifics.

## First, understand the unit conversion

Port speeds are often quoted in **Mbps** (megabits per second) or **Gbps** (gigabits per second), but file copy dialogs show **MB/s** (megabytes per second). The conversion is:

**8 Mbps = 1 MB/s**

Examples:
- USB 2.0 theoretical speed: 480 Mbps → about 60 MB/s (slower in practice)
- Gigabit Ethernet: 1000 Mbps → 125 MB/s

Just divide by 8 to get the theoretical MB/s (real‑world will be lower).

## 4.1 The Minimum You Need to Know

For most ports, if the shape matches, it will work. Only a few cases require checking versions.

- **USB**: Black or white plastic inside means slow (USB 2.0, about 30‑40 MB/s actual). Blue plastic or "SS" logo means fast (USB 3.x, several hundred MB/s actual).
- **Charging**: Only **Type‑C** ports, **with both your laptop and charger supporting PD (Power Delivery)**, can fast‑charge the laptop. Regular Type‑A ports cannot charge a laptop.
- **External display**: Both HDMI and Type‑C work. For 4K 60Hz or higher, prefer Type‑C or DisplayPort (DP).
- **Ethernet and headphone jack**: If the shape fits, plug it in.

## 4.2 USB Ports (Practical Guide)

### Shapes
- **Type‑A** (flat rectangle): Common on flash drives, mice. **Blue** plastic or "SS" logo means high speed.
- **Type‑C** (oval, reversible): The main port on new laptops. **You cannot tell speed by shape alone** – it could be slow USB 2.0 or very fast USB4. Check your laptop's specs.
- **Type‑B** (square / trapezoidal): Used on printers, scanners, some drive adapters. Insertion cycles are limited – keep it plugged in and only unplug the other end.
- **Mini / Micro USB**: Old devices, now obsolete.

### Power delivery (laptop charging)
- Only **Type‑C with PD** can charge your laptop.
- The port usually has a **PD** or lightning bolt logo next to it.
- Charger, cable, and laptop must all support the same or higher PD power level.

> Detailed USB version table (2.0 / 3.x / USB4) is in Extended Reading at the end of this appendix.

## 4.3 Display Ports

- **HDMI** (trapezoidal, flat): Connect to TVs, monitors, projectors.
  - If your 4K monitor shows only 30Hz (choppy motion), the cable or port may be an old version. Use a cable labelled "HDMI 2.0" or higher.
- **DP / Mini‑DP** (rectangular or small rectangle): High bandwidth, good for high resolution and high refresh rates. Common on desktop monitors and some laptops.
- **VGA** (blue trapezoid, 15 pins): For old projectors. New laptops don't have it – you'll need an **active** HDMI‑to‑VGA adapter.

**Simple rule**: For 4K 60Hz or higher, prefer **Type‑C to DP** or **DP directly**. HDMI can also do it, but check your hardware.

## 4.4 Network Port

**RJ45** (square with a clip): Wired Ethernet – stable and low latency. Thin‑and‑light laptops often omit it; use a Type‑C to RJ45 adapter.

## 4.5 Audio Port

**3.5mm headphone jack** (round): Almost all laptops keep this. Plug in headphones, mic, or headset.


## 4.6 Thunderbolt vs. USB4

- **Thunderbolt port**: Type‑C shape with a **⚡ lightning bolt** logo next to it. Without that logo, it‘s just regular USB.
- **Do you need Thunderbolt?** Only if you need to connect an **external GPU** or **two 4K displays simultaneously**. For daily office work, one monitor, or ordinary storage, regular USB 3.x or USB4 is enough.

> Detailed Thunderbolt vs. USB4 comparison table (bandwidth, power, daisy‑chaining) is in Extended Reading.

## 4.7 One‑Cable Display with Reverse Charging

"One‑cable connection": a single Type‑C cable between laptop and monitor that carries video, charges the laptop, and extends USB ports.

**Requirements**:
- Monitor supports PD output (at least 65W)
- Laptop supports PD charging
- Cable supports 5A current (100W or higher)

No side effects – your laptop‘s power management handles it safely.

## 4.8 TGX Port (Specific Laptop Models)

Looks like a slim Type‑C, labelled **TGX**. Only found on a few Lenovo ThinkBook models (14+/16+). Used to connect an external desktop graphics card. **Regular users can completely ignore this.**

## 4.9 Cable Advice

A poor‑quality cable will cripple your expensive devices. Common problems: slow charging, 4K display blackout or flickering, device not recognised.

**Recommendation**: Luxshare's 240W USB4 universal cable – supports PD 3.1 240W charging, USB4 40Gbps data, and 8K video. 

## 4.10 Memory Cards (SD / microSD)

Laptop card slots come in two shapes:
- **SD card** (full‑size, about the size of a postage stamp)
- **microSD / TF card** (tiny, used in phones and drones)

### Speed rating simplified
- Photos or normal video: buy a card labelled **C10** (10 inside a circle) or **U1**.
- 4K video recording: buy a card labelled **U3** or **V30**.
- Ignore other letters (A1/A2, V60/V90) unless you know you need them.

### Physical interface type
- Most thin‑and‑light laptops have **UHS‑I** slots (one row of gold contacts) – theoretical maximum about 100 MB/s.
- A few high‑end models have **UHS‑II** (two rows of contacts) – can do >300 MB/s.
- To tell: flip the card over and count the contact rows. Check your laptop‘s manual for what it supports.

### CFexpress (for professional cameras)
Regular users can ignore this completely. If you own a high‑end camera and need very fast continuous shooting or high‑bitrate video, check your camera's manual for the correct CFexpress type (A, B, or C) before buying a card.

## 4.11 SATA Port

SATA (Serial Advanced Technology Attachment) is an older interface standard for connecting hard drives. New laptops no longer include SATA ports, but you may encounter external drive enclosures, desktop hard drives, or drives pulled from old laptops.

### Shape and versions
A SATA connection has two parts:
- **Data connector** (L‑shaped, 7 pins): short flat connector, carries data.
- **Power connector** (longer, 15 pins): supplies power.

Common SATA versions:
- **SATA 3.0**: mainstream, speed 6 Gbps (about 550‑560 MB/s actual).
- SATA 2.0: 3 Gbps, older devices.
- All versions have identical physical connectors and are backwards compatible.

### Usage
- **2.5‑inch drives** (laptop HDDs, older SSDs): use SATA data + power.
- **3.5‑inch desktop drives**: also SATA, but require 12V power (2.5‑inch drives need only 5V). Therefore, external 3.5‑inch drives need a separate power supply.
- **Optical drives** (obsolete): also use SATA.

### Using an external SATA drive (USB‑to‑SATA adapter / enclosure)
If you have a spare SATA drive and want to use it as an external drive:
- Buy a **USB‑to‑SATA adapter cable** (often called "easy cable") or a **drive enclosure**.
- **Watch the power**: 2.5‑inch drives can run on USB power (5V). 3.5‑inch drives **must** use an adapter with a separate 12V power supply; otherwise they won‘t spin up.
- **Safe removal**: Always use "Safely Remove Hardware" in the Windows system tray, or shut down the computer before unplugging. Never unplug while the drive is reading or writing – this can damage the heads or create bad sectors.

### Insertion cycle limit
SATA connectors have a limited mating life (about 50‑100 cycles). If you frequently connect an external SATA drive, leave the SATA side of the adapter or enclosure permanently attached to the drive, and only unplug the USB side. This preserves the SATA connector.

---

# Extended Reading (Technical Details)

>The following sections contain detailed tables and advanced knowledge. Read them when you need specifics.

## Extended 1: USB Version Full Table

| USB Version | Alias | Max Speed | Theoretical Bandwidth (approx) | Common Use |
|-------------|-------|-----------|-------------------------------|-------------|
| USB 2.0 | Hi‑Speed | 480 Mbps | 60 MB/s | Keyboards, mice, old flash drives |
| USB 3.0 / 3.1 Gen1 | USB 3.0 | 5 Gbps | 500 MB/s | Normal flash drives, external HDDs |
| USB 3.1 Gen2 | USB 3.1 | 10 Gbps | 1.2 GB/s | Fast external SSDs |
| USB 3.2 Gen2x2 | USB 3.2 | 20 Gbps | 2.4 GB/s | High‑end external SSDs |
| USB4 | USB4 | 40 Gbps | 4 GB/s | Laptops, docks, displays |

**Identification**: USB 2.0 ports usually have black or white plastic inside. USB 3.x ports typically have blue plastic or logos like "SS" (SuperSpeed) or "10" (10Gbps). Type‑C ports cannot be identified by appearance alone.

## Extended 2: HDMI Version Quick Table

| HDMI Version | Max Resolution / Refresh | Bandwidth | Notes |
|--------------|--------------------------|-----------|-------|
| 1.4 | 4K 30Hz | 10.2 Gbps | Legacy |
| 2.0 | 4K 60Hz | 18 Gbps | Mainstream |
| 2.1 TMDS | 4K 60Hz | 18 Gbps | Essentially renamed 2.0 |
| 2.1 FRL | 4K 120Hz / 8K 60Hz | 48 Gbps | True 2.1 |

When buying an HDMI cable, look for "HDMI 2.1 certified" and explicit support for 4K 120Hz or 8K.

## Extended 3: Thunderbolt vs. USB4 Comparison

| Feature | Thunderbolt 3 | Thunderbolt 4 | Thunderbolt 5 | USB4 (baseline) |
|---------|---------------|---------------|---------------|------------------|
| Theoretical bandwidth | 40 Gbps | 40 Gbps | 80 Gbps (120/40 symmetric mode) | 40 Gbps (optional) |
| Minimum PCIe bandwidth | 16 Gbps | mandatory 32 Gbps | 64 Gbps | not guaranteed |
| Minimum video output | 1x 4K | mandatory 2x 4K | 2x 6K / 3x 4K | not guaranteed |
| Minimum port power | 9W | mandatory 15W | 15W+ | only 7.5W |
| Daisy‑chaining | supported | supported | supported | not guaranteed |
| Certification | Intel | Intel | Intel | none |

**Conclusion**: USB4 baseline is loose – vendors can make cut‑down versions (20Gbps, low power). Thunderbolt 4/5 enforce higher minimums. A Type‑C port without a lightning bolt logo is just regular USB.

## Extended 4: Memory Card Speed Class Full Table

| Logo | Name | Minimum Write Speed |
|------|------|---------------------|
| C2/C4/C6/C10 | Speed Class | 2/4/6/10 MB/s |
| U1 / U3 | UHS Speed Class | 10 / 30 MB/s |
| V10–V90 | Video Speed Class | 10–90 MB/s |
| A1 / A2 | Application Performance Class | random IOPS |

A card may show C10, U3, V30, A2 together – it meets all those minimums.

## Extended 5: SD Express and CFexpress

- **SD Express**: Brings PCIe bus to SD cards. Theoretical speed up to 985 MB/s (PCIe 3.0 x1) or 4 GB/s (PCIe 4.0 x2). However, as of 2026, very few laptops support it – ordinary users do not need it.
- **CFexpress**: For professional cameras, based on PCIe/NVMe. Three physically incompatible types: Type A (~2 GB/s), Type B (~4 GB/s), Type C (~8 GB/s). Always check your camera's manual before buying.

## Extended 6: SATA Versions and AHCI

SATA 3.0 has a theoretical speed of 6 Gbps, but due to 8b/10b encoding (each 10 bits transmitted contain 8 bits of real data), the effective bandwidth is about 4.8 Gbps, or roughly 560 MB/s. This is the ceiling for SATA – even the fastest SSD cannot exceed this speed when connected via SATA.

AHCI (Advanced Host Controller Interface) is the driver protocol for SATA drives. It was designed for mechanical hard drives and is not well optimised for the high queue depth and parallelism of SSDs. In contrast, NVMe (used by M.2 drives) fully unlocks SSD performance. This is why M.2 NVMe drives are much faster than SATA SSDs.

If you use a USB‑to‑SATA adapter, Windows will handle AHCI automatically – you don‘t need to worry about it.