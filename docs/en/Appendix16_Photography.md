# Appendix 16: Photography

> This appendix introduces basic photography concepts – white balance, ISO, shutter speed, aperture, focal length, dynamic range – as well as professional camera apps for phones, the usefulness of RAW format, and some simple post‑processing ideas. Even if you only take casual snapshots, a little extra knowledge never hurts.

## 16.1 The Exposure Triangle: ISO, Shutter Speed, Aperture

The brightness of a photo is determined by three things: ISO (sensitivity), shutter speed, and aperture. They are like three valves on a water tap, together controlling how much light reaches the camera.

**ISO (sensitivity)**

ISO is how sensitive the camera sensor is to light. A lower number (e.g., ISO 100) means less sensitivity, cleaner and sharper image quality. A higher number (e.g., ISO 3200) means more sensitivity – you can take brighter photos in very dark conditions – but at the cost of noise (grain) appearing in the image.

> **Everyday advice**: Use low ISO (100–400) when light is good for the best quality. Only raise ISO when it is genuinely dark.

**Shutter speed**

Shutter speed is the duration of exposure. A fast shutter speed (e.g., 1/1000 second) freezes moving subjects (birds in flight, racing cars). A slow shutter speed (e.g., 1/30 second or slower) captures motion blur (light trails from cars at night, silky water).

> **Safe shutter speed**: When shooting handheld, do not go slower than roughly "1 / (equivalent focal length)". For a 50mm lens, stay above 1/50 second, otherwise camera shake will blur the photo. Phone lenses have shorter equivalent focal lengths (around 24–28mm), so a safe shutter speed is around 1/30 second.

**Aperture**

Aperture is the size of the opening in the lens. It controls how much light enters and also affects depth of field (background blur).

- Wide aperture (small number, e.g., f/1.8) → more light, more background blur. Good for portraits and close‑ups.
- Narrow aperture (large number, e.g., f/8) → less light, everything in focus. Good for landscapes and group photos.

> **Aperture on phones**: The main cameras on most phones have a fixed aperture – you cannot change it. The "portrait mode" on phones does not adjust aperture; it uses software to simulate blur. So you do not need to worry about aperture values on a phone – just understand the concept.

**How the three work together**

A photo with the same overall brightness can be taken with many different combinations of settings. For example: wide aperture + fast shutter speed = narrow aperture + slow shutter speed. Choose based on what you need – want background blur? Use a wide aperture. Want everything sharp? Use a narrow aperture. Want to freeze motion? Use a fast shutter speed. Want light trails? Use a slow shutter speed.

## 16.2 White Balance

White balance (WB) makes sure that white objects look white in a photo, regardless of the colour of the light illuminating them.

Our eyes are very clever: under a yellow desk lamp, we still see a white piece of paper as white – our brain automatically compensates. A camera does not. Without adjusting white balance, a photo of white paper taken under yellow light will look yellow-ish.

Cameras usually have Auto White Balance (AWB), which works well in most situations. But if you find your photos looking too yellow or too blue, you can manually select a WB mode (daylight, cloudy, tungsten, fluorescent, etc.) when shooting, or adjust the temperature slider afterwards in software like Lightroom.

## 16.3 Focal Length and Equivalent Focal Length

**Focal length** (measured in mm) determines how wide or zoomed‑in your view is. Smaller numbers → wider view (wide‑angle). Larger numbers → narrower view, bringing distant objects closer (telephoto).

- Ultra‑wide (14–24mm): landscapes, architecture – grand, sweeping views.
- Standard (35–50mm): roughly the field of view of the human eye – street photography, everyday scenes.
- Telephoto (70mm and above): distant subjects – birds, people, the moon.

**Equivalent focal length**: Phone sensors and standalone camera sensors (mirrorless/DSLR) have different sizes. A lens marked "24mm" on a phone will show a different field of view than a 24mm lens on a full‑frame camera. To make comparisons easy, the industry uses "full‑frame equivalent focal length". You do not need to memorise the formula – just remember that a phone's main camera is typically around 24–28mm equivalent (wide‑angle), and what phone makers call "telephoto" might be around 70mm equivalent or longer.

## 16.4 Minimum Focus Distance

Every lens has a limit to how close you can get to a subject and still focus. If you move too close, the camera will refuse to focus – you have exceeded the **minimum focus distance**.

Macro photography (flower stamens, insects) requires a lens that can focus very close. Ordinary lenses have minimum focus distances of a few tens of centimetres. If you want to try macro on your phone, check if it has a "macro mode" or "super macro", or buy a clip‑on macro lens.

## 16.5 Focus Peaking

**Focus peaking** is a manual focus aid. When you turn the focus ring, areas that are in focus are highlighted with coloured dots (red, yellow, or white). The densest area of dots is where the sharpest focus lies.

**When to use it**: When shooting macro, starry skies, or still life (e.g., product photography), autofocus may hunt or miss. Switch to manual focus, turn the ring while watching the peaking overlay, and stop when the dots are brightest – much more reliable than relying on your eye alone. Most mirrorless cameras and some phone pro modes support this.

## 16.6 Dynamic Range

**Dynamic range** is the range between the brightest and darkest parts that a camera can capture at the same time.

A high‑dynamic‑range scene: a portrait against a bright sky. The sky is very bright, the face is much darker. If the camera's dynamic range is insufficient, either the sky will be blown out (pure white) or the face will be too dark (details lost).

**The "HDR mode" on phones** does this: the camera takes several exposures in quick succession (one for the highlights, one for the shadows, etc.) and merges them into a single photo where both the bright sky and the dark face are visible. Modern computational photography often runs this process automatically in the background – you do not need to turn on an HDR switch.

**When to use it**: Sunrises and sunsets, backlit portraits, scenes with bright windows and dark interiors – any time the contrast is very high, HDR (if not already automatic) will help significantly.

## 16.7 Professional Camera Apps for Phones

The built‑in camera app on most phones hides most controls; you just press the shutter button. If you want manual control over ISO, shutter speed, white balance, and focus, and want to shoot RAW, install a third‑party camera app.

**On iPhone: Halide Mark III**

Halide is a well‑known professional camera app. The latest version is Mark III.

- Manual control over all parameters (ISO, shutter speed, white balance, focus)
- Can shoot RAW (also supports Process Zero, which bypasses Apple's automatic processing)
- Now includes a Photo Lab editor for editing RAW photos

**Price**: I am not sure – I have never used an Apple device. (But a friend of mine uses Halide often.)

**On Android: ProShot, Open Camera**

Android's openness allows for more third‑party camera apps.

- **ProShot**: DSLR‑style interface, manual control over ISO, shutter speed, white balance, focus, RAW support. Considered one of the best manual camera apps on Android. Paid.
- **Open Camera**: Open source, free, powerful. Supports manual controls, RAW, HDR, noise reduction, and more. If you do not want to spend money, this is the best choice – completely sufficient.
- **Camera FV‑5**: Another classic manual camera app, supports long exposure and RAW. Paid.

**When to use these apps**: Shooting landscapes (manual focus to infinity), capturing light trails (slow shutter speed manually), shooting RAW for later editing – any time you want full control over your photo.

## 16.8 RAW Format

### 16.8.1 What Is RAW?

By default, phones and cameras save photos as JPG (or HEIF). These are finished products – white balance, sharpening, noise reduction, and colour style are all baked in. There is very little room for editing afterwards.

RAW is the unprocessed data straight from the sensor. It retains all the brightness and colour information as it was captured, without any processing. White balance can be changed freely, exposure can be pulled back by several stops, shadow details can be brightened – the editing flexibility is enormous.

### 16.8.2 Why Does RAW Look Grey and Flat?

When you first open a RAW photo on your computer or phone, it often looks greyish, low‑contrast, and desaturated. This is normal. It is not "broken". It is because the RAW file has not gone through the camera's internal colour and contrast processing. You need to use software like Lightroom, Photoshop, or another RAW processor to "develop" it – adjust exposure, contrast, saturation, curves, etc. This process is called **demosaicing** or **developing**.

### 16.8.3 High Megapixel Counts on Phones and RAW

Phones today boast 48MP, 64MP, or even 108MP sensors, but the RAW you get is often only 12MP or 16MP. This is not the phone lying to you – it is how the sensors work.

Most high‑megapixel phone sensors use **Quad Bayer** (or even 16‑cell) designs. In normal shooting, adjacent pixels are merged into larger "super‑pixels" to get better light capture and image quality. A 48MP sensor, when quad‑merged, outputs a 12MP photo. Even when you shoot RAW, many phones output the merged 12MP RAW (for example, iPhone's ProRAW defaults to 12MP from the merged output). This is not a bad thing – a 12MP merged RAW usually has better image quality than a full 48MP RAW, because individual pixels on a tiny sensor are very small and prone to noise.

If you truly need full resolution RAW (e.g., for large prints), look in your camera app for an option like "Full resolution RAW" or "Unbinned RAW". But in most cases, the 12MP merged RAW is perfectly adequate.

## 16.9 RAW vs. Straight‑out‑of‑Camera JPG

| Format | File size | Editing flexibility | Best for |
|--------|-----------|---------------------|----------|
| **JPG / HEIF** | Small | Very limited – small brightness adjustments maybe, but white balance is mostly fixed | Everyday snapshots, social media, when you do not plan to edit |
| **RAW** | Large | Huge – exposure can be pulled ±2 stops, white balance can be changed freely | When you want to do serious editing, colour grading, printing, selling photos |

**Conclusion**: For casual photos, JPG is fine. But if you think "I might want to edit this later", or if the scene has very high contrast (backlighting, night scenes), shoot a RAW copy as well – disk space is cheap, but a missed moment is gone forever.

## 16.10 Colour Grading – Ideas and Tools

**Lightroom (LR)**

Lightroom is the most mainstream and beginner‑friendly photo editing software. It is available on mobile and desktop. A Creative Cloud Photography plan (which includes Photoshop) gives you access. The mobile version's basic features (exposure, contrast, saturation, curves) are free.

**A simple editing workflow (using Lightroom mobile as an example)** :

1. Import the RAW (LR will automatically enter the "develop" mode)
2. In the "Brightness" section, adjust exposure, contrast, highlights, and shadows (pull down the bright sky, lift the shadow details)
3. In the "Colour" section, adjust white balance (temperature, tint) and saturation
4. Apply a preset or manually adjust the curve (an S‑curve is a simple way to increase contrast and add "pop")
5. Finally, add a little sharpening and noise reduction – do not overdo it

**Two important reminders**:

- **Your screen colour needs to be trustworthy**: You spend time carefully adjusting colours, only to find that your own screen was off – and the photo looks completely different (often grey and flat) on someone else's phone. This is a classic beginner trap. If your computer or phone screen seems too warm or too cool (compare it with another device), go into its settings and turn off "vivid mode", "dynamic colour enhancement", or similar options. The safest approach is to trust a device you know is accurate (like an iPad) or a friend's phone.
- **Mobile LR presets ≠ desktop LR presets**: The same preset file can look different when applied in Lightroom mobile versus Lightroom desktop – because the two use different colour spaces and rendering engines. If you tweak a preset on your phone and want to use it on your computer, test it side‑by‑side first and be ready to make small adjustments.

**Everyone has their own style**

There is no single "correct" way to colour grade. Some people like rich, saturated, punchy colours. Others prefer muted, cool, desaturated tones. Some only work in black and white. It is up to you. Look a lot (at good photos, at colour charts), edit a lot (pull curves, adjust HSL sliders), and over time you will develop your own feel.

---

# Extended Reading

> The following sections go into deeper technical details. Come back when you need them.

## Extended 1: Technical Principle of Pixel Binning

Quad Bayer sensors arrange four same‑colour pixels in a 2×2 array. In normal shooting, these four pixels are merged into one "super‑pixel", outputting a lower‑resolution image. However, each super‑pixel has four times the light‑gathering area of a single tiny pixel, resulting in much higher signal‑to‑noise ratio. When high resolution is genuinely needed (e.g., in excellent light), the sensor can perform a remosaic to output the full 48MP. Some phones also use different gain (ISO) levels on the different sub‑pixels and merge them later, improving dynamic range in a single shot (a kind of "HDR‑in‑one").

## Extended 2: Fine‑tuning White Balance

Auto white balance can be fooled by a scene dominated by a single colour (for example, if you stand in front of a large red wall). If your photos have a colour cast, you can manually set a grey point: in Lightroom or another RAW processor, use the white balance eyedropper to click on an area that should be neutral grey or white (e.g., a piece of concrete, a white wall, a white piece of paper). The software will correct the global colour temperature based on that sample.

## Extended 3: Dynamic Range vs. Latitude

**Dynamic range** is the range between the brightest and darkest light levels that a sensor can capture in a single exposure. **Latitude** is how much you can later brighten underexposed areas or darken overexposed areas in a RAW file without introducing excessive noise or artefacts. They are related but not identical. A sensor with high dynamic range usually has good latitude – you can lift shadows by 2–3 stops without ruining the image.

## Extended 4: How Computational Photography Affects RAW

Modern phones' RAW files are no longer "pure" RAW in the traditional sense. iPhone ProRAW, for example, starts with RAW data but then incorporates multi‑frame information from Deep Fusion (it has already merged several exposures at different brightness levels in the background). The benefit is that ProRAW's dynamic range in challenging scenes far exceeds what a traditional RAW would have. The cost is that it already carries some "computational" fingerprints – it is not the rawest of raw data. Android manufacturers' "RAW" implementations vary – some are also multi‑frame DNGs. If you truly want the most unprocessed sensor output, you may need to use a third‑party app and disable all multi‑frame enhancements.

## Extended 5: Colour Space Settings in Lightroom

Lightroom's working space on the desktop is typically ProPhoto RGB (very large). However, for photos you export to share with others, you should convert to sRGB – the most universal colour gamut, which almost every screen can display correctly. If you edit a photo on a wide‑gamut screen, then export it as ProPhoto RGB or Adobe RGB and send it via WeChat or upload it to a website, the colours will appear dull and grey on most viewers' devices. **Remember to select sRGB when exporting.**