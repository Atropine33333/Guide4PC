# Appendix 14: Color

> This appendix introduces the basic concepts of color – gamut, color space, color accuracy, and calibration. If you only use your computer to watch videos, play games, and edit your own photos, you probably won't need this knowledge. But if you start doing design work, photo retouching, or video editing, and need to share your work with others or send it to print, understanding these concepts will help.

## 14.1 The Most Important Thing You Need to Know

**If you are not delivering final work to someone else (e.g., sending to a client, printing, submitting to a publication) – if you are just using your computer for games, watching videos, or editing your own photos – then whether your screen's colors are "accurate" simply does not matter. Use whatever looks comfortable to you.**

Color management is a rabbit hole. Unless you make a living from color (graphic design, retouching, video color grading, printing), you do not need to go deep. If you ever need it later, just knowing that it exists and knowing which direction to search is enough.

## 14.2 Gamut and Color Space

### 14.2.1 What Is Gamut?

**Gamut** is the range of colors that a device (a monitor, printer, projector) or a color standard can reproduce. The human eye can see many colors, but different devices can only cover a portion of them.

Common gamut standards:

| Gamut | Coverage (relative to human vision) | Main use | One‑sentence explanation |
|-------|--------------------------------------|----------|---------------------------|
| **sRGB** | About 35% | Web, ordinary monitors, Windows default | The most universal "safe" gamut. Almost every device displays it correctly. |
| **DCI-P3** | About 45% | Digital cinema, Apple devices, HDR video | Wider than sRGB, especially in reds and greens. |
| **Display P3** | About 45% | Apple displays | Based on DCI-P3, but with the same white point as sRGB for better compatibility. |
| **Adobe RGB** | About 50% | Professional photography, printing | Enhanced cyan and green to cover the CMYK print gamut. |
| **Rec. 709** | Almost identical to sRGB | HDTV | sRGB's "TV version" – slightly different white point and gamma. |
| **Rec. 2020** | About 75% | 4K/8K UHDTV | One of the widest gamuts available; almost no consumer display can fully cover it. |

### 14.2.2 Three Names, Essentially the Same Range

**sRGB, Rec.709, and Rec.601** have nearly identical color ranges. The differences:

- **sRGB**: For computer monitors, the web, consumer digital cameras
- **Rec.709**: For HDTV – white point is the same as sRGB, gamma curve is slightly different
- **Rec.601**: Standard for SDTV, now largely replaced by Rec.709

For ordinary users, you can treat them as the same thing – the "common small gamut". The vast majority of images and videos you see online are in sRGB or Rec.709.

### 14.2.3 OKLAB: Why It Exists

Traditional color spaces like sRGB and P3 are not mathematically "uniform". What does that mean? In sRGB, a numeric change of 10 might look large in dark areas but barely noticeable in bright areas. OKLAB is a newer color model designed so that **the same numeric change produces the same perceptual change** for the human eye.

OKLAB is mainly used inside color conversion algorithms (for example, converting an sRGB image to P3 without introducing a color cast). Ordinary users do not need to work with it directly, but you can remember: if you see a piece of software that supports OKLAB, it handles color more scientifically.

### 14.2.4 Why So Many Gamuts?

Different industries have different needs. Printing requires coverage of CMYK (cyan, magenta, yellow, black), hence Adobe RGB. Cinema needs a wider dynamic range and vivid reds/greens, hence DCI-P3. Television has evolved from SDTV to HDTV to UHDTV, giving us Rec.601, Rec.709, and Rec.2020.

For ordinary users, **sRGB is the safest**. If you create a very vivid image on a wide‑gamut monitor and send it to a friend who uses a normal sRGB monitor, they will see less saturated colors. Conversely, when someone sends you an sRGB image and you view it on a wide‑gamut monitor without proper color management, the colors will be incorrectly "stretched" and look oversaturated. This is why many designers switch their monitors to sRGB mode when editing.

## 14.3 Monitor Gamut Coverage

When buying a monitor, you will see specs like "99% sRGB coverage" or "95% DCI-P3 coverage". What do these numbers mean?

- **Gamut coverage**: What percentage of a given gamut this monitor can actually display. 99% sRGB means the monitor can show 99% of the colors inside the sRGB triangle; the remaining 1% cannot be shown.
- **Gamut volume**: The ratio of the monitor's total color area to the area of a reference gamut. It is possible to have "120% sRGB volume" – but that does not mean accurate color, because the extra 20% may be in entirely wrong directions.

**For ordinary users**: A monitor that covers 100% sRGB is already very good for daily use. Wider gamuts (P3, Adobe RGB) are not necessary unless you have a specific need (e.g., editing HDR video, doing print design).

## 14.4 Color Accuracy: ΔE (Delta E)

**ΔE (Delta E)** is a number that measures how "accurate" a color is. The smaller the ΔE, the smaller the difference between the displayed color and the standard reference color.

| ΔE range | Meaning |
|----------|---------|
| ΔE < 1 | Visually indistinguishable (professional grade) |
| ΔE < 2 | Very accurate – only professionals can spot differences with careful comparison |
| ΔE < 3 | Acceptable for ordinary users |
| ΔE > 5 | Color cast is clearly visible to the naked eye |

Monitor ΔE values are usually quoted as "average ΔE". For example, "average ΔE < 2" means most colors are fairly accurate, but some colors (e.g., deep blue) may be off by more.

**For ordinary users**: Do not obsess over ΔE. When you are gaming, you do not care whether "this red is the exact sRGB red". As long as colors are not wildly wrong (e.g., white looking pink), you are fine.

## 14.5 Calibration Devices and Calibration

If you truly need accurate color (for example, you have taken a design job and the client requires no color deviation), then you need **calibration**.

### 14.5.1 What Is a Calibration Device?

A **colorimeter** (or spectrophotometer) is a small, mouse‑sized piece of hardware. You attach it to your screen, it measures the colors your screen actually displays, and then it generates an **ICC profile** for your operating system. Once Windows loads this profile, it adjusts the output signal to make the screen more accurate.

Common calibration device brands:

- **Datacolor Spyder**: Common models include Spyder X, Spyder 5. Suitable for ordinary users and photography enthusiasts.
- **Calibrite (formerly X-Rite)** : More professional – e.g., ColorChecker Display, i1Display Pro. Many studios use these.

### 14.5.2 Calibration Steps

1. Connect the calibration device to your computer and install the accompanying software (or use the open‑source DisplayCAL).
2. Let your monitor warm up for at least 30 minutes (so colors stabilise).
3. In the software, set your targets: white point (usually D65, i.e., 6500K), brightness (120 cd/m² for a typical indoor environment), gamma (choose 2.2 for sRGB/Rec.709).
4. Attach the device to the center of your screen and run the measurement. The software will display a series of colors, and the device reads them one by one.
5. When the measurement finishes, the software generates an ICC profile and tells Windows to load it.
6. Take this with a grain of salt – I researched this information; I have never used one myself. If you really need calibration, it is best to ask the vendor for advice.

### 14.5.3 When Do You Need Calibration?

- **You just bought a new monitor**: Factory calibration is not always accurate, and it drifts over time. Professional users calibrate every 1–2 months.
- **You changed your graphics card or reinstalled your OS**: The system's color configuration may have been lost.
- **You notice two screens look different**: For example, your laptop screen and an external monitor clearly do not match.
- **You are sending work to print**: Printing has strict color requirements. Without calibration, the printed result may be wildly off.

### 14.5.4 When Do You NOT Need Calibration?

- **You only game and watch videos**: Not needed.
- **You use a built‑in laptop screen**: The hardware quality of laptop screens is limited. Calibration will not improve it much, and it is not worth the expense.
- **You occasionally edit photos, only to post on social media**: Not needed. No one on social media is comparing your photos against a color checker.
- **You do not own a calibration device**: You can rent one (available on platforms like Taobao for a few dozen RMB per day) or borrow from a friend. Buying one costs anywhere from a few hundred to over a thousand RMB – if you only need it once, renting is more economical.

## 14.6 When Color Management Is Important vs. When It Is Not

### 14.6.1 When You Must Take Color Management Seriously

- **Design work delivered to a client**: If a blue that looks great on your screen looks purple on the client's screen, your final payment may be at risk.
- **Printing**: A screen and a piece of paper are completely different media. Without color management, the printed output will look nothing like what you see on screen.
- **Video color grading delivered to a platform (Bilibili, YouTube, etc.)** : If you set the wrong color space, the platform will re‑encode the video and the colors will shift.
- **Product photography (Taobao, e‑commerce)** : If the color of a garment is inaccurate, customers will receive items that look different from the photos, leading to high return rates.

### 14.6.2 When You Can Ignore Color Management

- **Playing games**: Not needed.
- **Watching videos and movies**: Not needed. The video is already graded; you are seeing what the director intended (as long as your monitor is not terrible).
- **Editing your own photos, only to post on social media or keep on your phone**: Not needed. Your friends use different brands of phones, each with different screen colors – no matter how accurately you calibrate, the colors will look different on their screens. Relax.
- **Coding, writing documents, taking online classes**: Color has nothing to do with the content. Not needed.

### 14.6.3 One‑Sentence Summary

**If you are not delivering work to others – if it is just for your own enjoyment – do not bother with color management. Spending your energy on content is far more valuable than spending it on color accuracy.**

## 14.7 Windows Color Management: Why It Is More Complicated Than on Mac

Let me briefly explain this. Many people say "Mac colors are accurate, Windows colors are a mess". The reason lies in the different designs of the two systems:

- **macOS**: System‑level, mandatory color management. Regardless of the application (even old software that does not support color management), macOS maps color data to the display's actual gamut at the system level. Combined with Apple's factory‑calibrated screens, it is accurate out of the box.
- **Windows**: Applications are on their own. Programs that support color management (Photoshop, Lightroom, Chrome/Edge) handle colors correctly. Programs that do not support it (older games, old screenshot tools, some image viewers) simply send their color values directly to the screen. If you have a wide‑gamut screen, colors in those applications will appear oversaturated.

**This difference does not affect ordinary users.** But if you need accurate color and do not want to tinker, a Mac is indeed the more hassle‑free choice.

---

# Extended Reading

> The following sections involve mathematical calculations and deeper principles of color spaces. They are more advanced. Skim them first, and come back later when you need them.

## Extended 1: The CIE 1931 Chromaticity Diagram

In 1931, the International Commission on Illumination (CIE) defined the **CIE 1931 XYZ color space**, which forms the foundation of all modern colorimetry.

![CIE 1931 LAB](/img/CIE-1931_diagram_in_LAB_space.svg)

It is a horseshoe‑shaped diagram of human color perception. The x and y axes represent chromaticity (color, without brightness). Any gamut can be drawn as a triangle inside this horseshoe – the three vertices are the coordinates of that gamut's red, green, and blue primaries. sRGB, DCI-P3, and Rec.2020 are all different‑sized triangles inside this horseshoe.

## Extended 2: Gamut Coverage vs. Gamut Volume – The Mathematical Difference

- **Coverage**: The area of overlap between the monitor's gamut and the reference gamut, divided by the area of the reference gamut (a percentage). Coverage cannot exceed 100%.
- **Volume**: The total area of the monitor's gamut divided by the area of the reference gamut (a percentage). Volume can exceed 100% (meaning the monitor's gamut is wider than the reference).

A monitor might claim "120% sRGB volume", but if that extra 20% volume lies in wrong directions, the monitor may still be inaccurate when displaying sRGB content. High volume does not necessarily mean good – coverage is a better indicator of accuracy for a given gamut.

## Extended 3: CIE Lab vs. OKLAB

**CIE Lab** (also called CIELAB, L*a*b*) is a color space defined by the CIE in 1976. It was designed to be "perceptually uniform" – but later research found that it is still not very uniform, especially in the blue region. **OKLAB** (proposed in 2020) improves uniformity, particularly in blue and cyan areas. OKLAB is currently one of the better color spaces for color conversion algorithms (e.g., converting from P3 to sRGB). Adobe Photoshop, Blender, and OpenColorIO have started to support OKLAB.

## Extended 4: White Point and Color Temperature

The **white point** determines the coordinates of "white" on the chromaticity diagram. Common white points:

- **D65**: 6500K, corresponds to midday sunlight. sRGB, Rec.709, and Display P3 all use D65.
- **D50**: 5000K, slightly yellowish. The standard for the printing industry (because printing paper itself is not pure white).
- **Original DCI-P3 white point**: 6300K, slightly different from D65. Display P3 changed the white point to D65 for better compatibility with sRGB.

Color temperature is measured in Kelvin (K). Lower values look more yellow/red (warm), higher values look more blue (cool). The "warm", "standard", and "cool" settings on a TV adjust the white point.

## Extended 5: How Calibration Data Is Stored

The ICC profile generated by a calibration device is a file containing a **CLUT (Color Lookup Table)** . When Windows loads this profile, the graphics card applies the table when outputting signals: `actual displayed color = graphics card output × color conversion matrix`. This process requires GPU and OS support. Some older games may bypass color management entirely, making the calibration ineffective.

## Extended 6: Using a Wide‑Gamut Monitor for Everyday Work

If you have bought a wide‑gamut monitor (e.g., covering 95% DCI-P3) but mainly use Windows, here is some advice:

1. Switch your monitor to **sRGB mode** via its on‑screen display (OSD) menu. This makes the monitor internally compress the wide gamut down to sRGB, so all applications (including those that do not support color management) show correct colors.
2. If you truly need the wide gamut (e.g., for editing HDR video), enable color management inside your editing software, but switch back to sRGB mode for everyday use.
3. Alternatively, only enable wide‑gamut mode when you are certain you need it. Use sRGB mode the rest of the time – no fuss.