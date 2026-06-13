# Appendix 15: Audio and Video

> This appendix introduces basic concepts and common tools for audio and video processing, including codecs, container formats, bitrate, OBS recording settings, and video editing software choices. The extended reading section covers FFmpeg command‑line usage and deeper technical concepts.

## 15.1 Codecs

An **encoder** compresses raw audio or video data into a specific format. A **decoder** decompresses that data back into a playable signal.

### 15.1.1 Video Codecs

| Name | Common encoders | One‑sentence explanation |
|------|----------------|---------------------------|
| **H.264 / AVC** | `libx264` (software), `h264_nvenc` (NVIDIA), `h264_qsv` (Intel) | Best compatibility – almost every device can play it. |
| **H.265 / HEVC** | `libx265` (software), `hevc_nvenc` (NVIDIA), `hevc_qsv` (Intel) | Same quality as H.264 but smaller file size. |
| **AV1** | `libsvtav1`, `libaom-av1` (software) | Even higher compression, good for streaming. |

**Note**: `hevc_nvenc` requires an NVIDIA GPU; `hevc_qsv` requires an Intel iGPU. Not every computer has them.

**Software encoding vs. hardware encoding**:
- Software encoding (`libx264`, `libx265`, `libsvtav1`): Better quality, smaller files, but uses the CPU heavily and is slow. Good for archiving and high‑quality encodes.
- Hardware encoding (`hevc_nvenc`, `hevc_qsv`): Very fast (5‑10× faster than software), but slightly lower quality at the same bitrate. Good for screen recording, live streaming, and fast exports.

### 15.1.2 Audio Codecs

| Name | Common encoder | One‑sentence explanation |
|------|----------------|---------------------------|
| **AAC** | `aac` | The most universal audio format today – almost every video uses it. |
| **MP3** | `libmp3lame` | Older format, good compatibility, but worse quality than AAC at the same bitrate. |
| **FLAC** | `flac` | Lossless compression, large file size – good for archiving and listening to music. |
| **Opus** | `libopus` | Newer technology, excellent quality and compression; some older devices do not support it. |

## 15.2 Container Formats

A **container format** is the "box" that holds video, audio, and subtitles. The same H.265 video can be put into an `.mp4` or into an `.mkv`.

| Container | Characteristics |
|-----------|-----------------|
| **MP4** | Most universal – almost every device plays it. Downside: usually can only hold one video track, one audio track, and one subtitle track. |
| **MKV** | Very powerful – can hold multiple audio tracks, multiple subtitle tracks, and chapter information. If damaged, only the last few seconds are corrupted (the rest remains recoverable). |
| **MOV** | Apple's QuickTime format, also supports multiple tracks. Good for exchanging files between Apple devices and Windows. |

## 15.3 Bitrate

Bitrate is the amount of data used per second of video, measured in **kbps** (kilobits per second) or **Mbps** (megabits per second). Higher bitrate → better quality → larger file.

- **CBR (Constant Bitrate)** : The bitrate stays the same throughout. Suitable for live streaming.
- **VBR (Variable Bitrate)** : Simple scenes get a lower bitrate; complex scenes get a higher bitrate. Better quality at the same average bitrate.
- **CRF (Constant Quality)** : The encoder automatically allocates bitrate to keep the quality roughly constant for every frame. The most hassle‑free option for everyday encoding.

## 15.4 OBS Recording Settings

### 15.4.1 Basic Settings

- **Output mode**: Advanced → set output to "Output to file"
- **Container format**: Choose **MKV** (reason: if OBS crashes or your computer loses power during recording, MKV will only corrupt the last few seconds; MP4 will become completely unreadable)
- **Encoder**:
  - If you have both an Intel iGPU and a discrete GPU → choose **QuickSync HEVC** (uses the iGPU to encode, does not compete with your games for GPU resources)
  - If you have only an NVIDIA GPU → choose **NVENC HEVC**
  - If you have only an AMD GPU → choose **AMD AMF HEVC** (good luck)

### 15.4.2 Recommended Settings

| Resolution | Bitrate | Frame rate | Keyframe interval | B‑frames |
|------------|---------|------------|-------------------|----------|
| 1080p | 35 Mbps | 60 fps | 3 seconds | 3 |
| 4K | 90 Mbps | 60 fps | 3 seconds | 3 |

Games lack motion blur – every frame is a sharp instant capture. When the camera moves quickly, details can break up easily. Therefore, game recording requires higher bitrates than normal video.

**About B‑frames**: B‑frames (bidirectional predicted frames) improve compression efficiency but increase encoding latency. Setting them to 3 is a good balance between quality and compatibility.

### 15.4.3 Administrator Privileges

For some full‑screen games, if OBS cannot capture the game window, **right‑click the OBS icon and select "Run as administrator"**.

## 15.5 Editing Software

### 15.5.1 Adobe Suite

- **Premiere Pro**: Video editing – one of the industry standards.
- **After Effects**: Visual effects, compositing, motion graphics.
- **Audition**: Audio editing, noise reduction, mixing.

Adobe software is subscription‑based. You can download a trial version from Creative Cloud, then – **without opening the software** – immediately run Adobe GenP to activate it. I am not providing download links. Search GitHub or relevant forums for the steps. This touches on copyright issues, so please use your own judgement.

### 15.5.2 DaVinci Resolve

A professional editing and color grading tool from Blackmagic Design. **The free version is already extremely powerful** – editing, color grading (used by Hollywood movies), Fairlight audio, and Fusion effects all integrated. If you care about color accuracy, this is often a better choice than Premiere.

### 15.5.3 Open‑Source Alternatives

If you do not want to pay for software and do not want to deal with piracy, these three open‑source tools are worth trying.

#### Clypra

Clypra is a modern open‑source video editor built with Tauri, React, TypeScript, Rust, and FFmpeg, released under the MIT license. Its goal is to replicate the paid features of CapCut's international version for free. Because it does not use Electron (which consumes lots of memory), it starts faster and uses fewer resources than similar tools.

**Core features**:
- Multi‑track professional timeline with drag‑and‑drop editing and frame‑level trimming
- Real‑time audio waveform display for easy beat‑matching
- Filmstrip preview for quick content navigation
- Text subtitles and titles with custom fonts
- Project management and auto‑save, with 100 levels of undo/redo
- Supports MP4, MOV, MKV, AVI (video) and MP3, WAV, AAC (audio)
- Cross‑platform: Windows, macOS, Linux

**Not yet implemented**: AI auto‑subtitles, transition/effect libraries, colour grading, green screen keying (these are under development).

> Who it is for: **Short‑form video creators, beginners, people who do not want to pay for CapCut**. Basic editing is already usable.

**Website**: Search GitHub for `AIEraDev/Clypra`

#### Audacity

Audacity is a long‑established, easy‑to‑use open‑source multi‑track audio editor and recorder, released under GPLv3. It runs on Windows, macOS, and Linux.

**Core features**:
- Multi‑track editing with unlimited tracks
- Import/export of WAV, AIFF, MP3, OGG, FLAC, and more (can support MOV, M4A, etc. via FFmpeg)
- High‑quality recording (up to 24‑bit / 96 kHz)
- Built‑in effects: noise reduction, EQ, compressor, limiter, and more
- Vocal remover (using phase cancellation to separate vocals from accompaniment)
- Supports VST2/VST3, LV2, and Audio Unit plugins
- Nyquist scripting for custom audio processing (using a Lisp dialect)

> Who it is for: **Podcasters, music hobbyists, recording beginners, people who need occasional audio cleanup**. It is the "Notepad of audio editors" – not fancy, but it can do a bit of everything.

**Website**: `audacityteam.org`

#### Ardour

Ardour is a free and open‑source digital audio workstation (DAW) that competes with Pro Tools, Logic, and other professional software. It runs on macOS, Windows, and Linux. The current version is 9.0.

**Core features**:
- Recording and MIDI composition: multi‑channel simultaneous recording (24+ channels), unlimited tracks and busses
- Non‑linear editing: non‑destructive editing with unlimited undo/redo
- Professional mixing: floating‑point mixing architecture, sends and returns, automation
- Extensive plugin support: AudioUnit, LV2, VST2/VST3, LADSPA
- Highly flexible signal routing: any channel to any other channel
- Video sync: sample‑accurate synchronisation, suitable for film and game sound design
- Advanced MIDI editing (new in 8.0): chord editing, quantisation, velocity maps, etc.
- Clip Launching mode (new in 7.0): Ableton Live‑style loop playback

**System requirements**: 2 GB RAM recommended, ~600 MB disk space.

> Who it is for: **Music producers, heavy‑duty podcasters, film/game audio professionals, anyone who needs serious depth in audio**. The learning curve is steep, but its depth is unmatched by Audacity.

**Website**: `ardour.org`

### 15.5.4 Comparison and Recommendations

| Software | Main use | Complexity | Paid equivalent | Recommended for |
|----------|----------|------------|-----------------|------------------|
| **Clypra** | Video editing | Medium | CapCut | Short‑form creators, beginners |
| **Audacity** | Audio editing / recording | Low | Adobe Audition (basic parts) | Podcasters, voice recording, quick fixes |
| **Ardour** | Full‑featured DAW | High | Pro Tools, Logic | Music production, professional mixing |

## 15.6 HDR and Color Management (Recap)

From Appendix 14, we know that color management on Windows is finicky. HDR video editing is even more so:

- **HDR10**: The baseline HDR standard, 10‑bit colour depth, static metadata. Supported by most HDR monitors.
- **Dolby Vision**: A more advanced standard with dynamic metadata (per‑frame brightness information). Requires licensing on both the hardware and software sides.
- **HLG**: An HDR standard for broadcast TV. No metadata needed and is backwards‑compatible with SDR displays.

If you have serious HDR editing needs, **a MacBook Pro is currently the most hassle‑free choice** (XDR screen, 1600 nits peak brightness, system‑level colour management). The time cost of wrestling with HDR on Windows can far exceed your expectations.

---

# Extended Reading

> The following sections cover FFmpeg command‑line usage, the inner workings of I/P/B frames, encoder tuning, and hardware detection. Come back when you need them.  
> I will cover FFmpeg in more depth in the main book – stay tuned.

## Extended 1: FFmpeg Command‑Line Tool

FFmpeg is an open‑source command‑line audio and video processing tool. Almost every audio/video software uses it under the hood. Converting formats, cutting clips, burning in subtitles – all can be done with a single command.

### Installation

Download the Windows pre‑built package from [ffmpeg.org](https://ffmpeg.org/download.html). Extract it, then add the `bin` folder to your system PATH. Open a command prompt and type `ffmpeg -version` to confirm it works.

### Common Commands

**Format conversion**:

```bash
ffmpeg -i input.mkv output.mp4
```

**Video cutting** (start at 1 minute 30 seconds, take 45 seconds):

```bash
# Fast cut (may not be frame‑accurate, but fast)
ffmpeg -ss 00:01:30 -i input.mp4 -t 45 -c copy output.mp4

# Accurate cut (re‑encodes, slower but precise)
ffmpeg -i input.mp4 -ss 00:01:30 -t 45 -c:v libx264 -c:a aac output.mp4
```

> Parameter explanation: `-ss` start time, `-t` duration, `-c copy` do not re‑encode (copy the streams directly). Putting `-ss` before `-i` performs a fast seek (based on keyframes); putting it after performs an accurate seek (decodes every frame).

**Encode to HEVC (software)** :

```bash
ffmpeg -i input.mp4 -c:v libx265 -crf 23 -c:a aac output.mp4
```

**Use NVIDIA hardware encoding** (if your GPU supports it):

```bash
ffmpeg -i input.mp4 -c:v hevc_nvenc -crf 23 -c:a aac output.mp4
```

**Use Intel iGPU hardware encoding** (if your iGPU supports it):

```bash
ffmpeg -i input.mp4 -c:v hevc_qsv -crf 23 -c:a aac output.mp4
```

**Extract audio**:

```bash
ffmpeg -i input.mp4 -vn -c:a aac -b:a 192k output.aac
```

`-vn` means "no video stream"; `-b:a` sets the audio bitrate.

**Download an m3u8 video (online stream)** :

```bash
ffmpeg -i "https://example.com/stream.m3u8" -c copy output.mp4
```

### Batch Processing

Here is a simple Windows batch script (`.bat`):

```batch
@echo off
for %%f in (*.mp4) do (
    ffmpeg -i "%%f" -c:v libx265 -crf 23 -c:a aac "%%~nf_hevc.mp4"
)
```

For more complex tasks, use PowerShell 7 (which supports `ForEach-Object -Parallel` for parallel processing) or Python with `subprocess`. The power of FFmpeg is that – apart from tasks that require real‑time preview (e.g., colour grading) – almost everything else (rough cutting, transcoding, concatenation, filtering) can be done with it.

## Extended 2: I‑frames, P‑frames, B‑frames – Detailed Explanation

Video compression does not store every frame as a full image. Instead, it stores the **differences** between frames.

- **I‑frame (Intra‑frame)** : Stores a complete frame. Largest size, fastest to decode. These are the "full images" in the video. An I‑frame appears every so often (controlled by the "keyframe interval" setting).
- **P‑frame (Predicted frame)** : Stores only the differences from the **previous** frame. Medium size. Decoding requires referring back to a previous I‑frame or P‑frame.
- **B‑frame (Bidirectional frame)** : Stores differences from both the **previous** frame and the **next** frame. Smallest size, highest compression, but slowest to decode.

**Why cutting at a non‑I‑frame causes corruption**: P‑frames and B‑frames only store "changes", not full images. If playback starts at a P‑frame or B‑frame, the decoder does not know what the previous frame looked like, so it guesses – resulting in broken blocks on the screen. When cutting without re‑encoding, cuts can only be made at I‑frames. To cut at an arbitrary frame, re‑encoding is required.

## Extended 3: Encoder Speed Presets

The `-preset` parameter for x264/x265 ranges from `ultrafast` to `placebo`. Slower presets give better quality (at the same bitrate). For daily use, `medium` or `slow` is fine. Beyond `slower`, the gains are marginal.

- `ultrafast`: Extremely fast encoding, low quality, large files
- `medium`: Default, balances speed and quality
- `slow`: About 50% slower than medium, saves 5‑10% bitrate
- `veryslow`: Very slow, diminishing returns

For NVENC, the `-preset` options range from `p1` (fastest) to `p7` (slowest, best quality). Default is `p4`. At `p7`, quality improves noticeably, but encoding speed drops significantly.

## Extended 4: AV1 Encoders Compared

- **libaom-av1**: The reference AV1 encoder. Best quality, but extremely slow (encoding 1 second of video can take minutes to tens of minutes).
- **libsvtav1**: Developed by Intel. Much faster (hundreds of times faster than libaom) and still delivers very good quality. For everyday AV1 encoding, use libsvtav1.

## Extended 5: 2‑pass Encoding

The first pass analyses the video, recording the complexity of each frame. The second pass then allocates bitrate accordingly – giving more bits to complex scenes and fewer bits to simple scenes. This is useful when you need to hit an exact file size (e.g., a file size limit for uploading to a website). For daily use, CRF is sufficient; 2‑pass is not needed.

## Extended 6: Dolby Audio Formats – Details

- **AC‑3**: The older Dolby Digital audio format. The DVD era standard. Supports 5.1 channels, typical bitrate 448k or 640k.
- **E-AC‑3 (Dolby Digital Plus)** : Dolby's lossy surround sound format. Supports 5.1 and 7.1 channels. Commonly used by streaming platforms (Netflix, Disney+, etc.). Typical bitrate 192k–768k. File extension may be `.ec3`.
- **TrueHD**: Dolby's lossless surround sound format. Common on Blu‑ray discs. Large file size; requires HDMI or eARC for direct passthrough. Supports up to 7.1 channels.

## Extended 7: Hardware Encoder Support Detection

There is a tool on GitHub called **HwCodecDetect** that detects which hardware encoders and decoders your computer supports, and up to what resolution.

```bash
pip install hwcodecdetect
hwcodecdetect
```

It produces a report listing support for NVENC, QSV, AMF, VAAPI, VideoToolbox, and other interfaces.

## Extended 8: Pitfalls of FFmpeg Stream Copy

`-c copy` does not re‑encode – it is very fast and lossless. However, when cutting video, if the start point is not an I‑frame, FFmpeg will start copying from the **previous** I‑frame. This means the resulting clip will start a few hundred milliseconds earlier than you intended.

How to handle this:
- Accept the small error (usually less than half a second, often acceptable)
- Or re‑encode just the beginning part: first cut a rough range with `-c copy`, then re‑encode the head section accurately

```bash
# Rough cut first (starts at an I‑frame)
ffmpeg -ss 00:01:30 -i input.mp4 -t 60 -c copy temp.mp4
# Then accurately re‑encode the first second
ffmpeg -i temp.mp4 -ss 00:00:00 -t 45 -c:v libx264 -c:a aac output.mp4
```

## Extended 9: FFmpeg Filter Chain Example

```bash
# Scale + crop + add text watermark
ffmpeg -i input.mp4 -vf "scale=1280:720,crop=1200:600:40:40,drawtext=text='Watermark':x=10:y=10:fontsize=24" output.mp4
```