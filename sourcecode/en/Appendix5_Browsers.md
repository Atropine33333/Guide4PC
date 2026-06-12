# Appendix 5: Web Browsers

> This appendix covers basic browser usage, search techniques, how to ask questions effectively, and commonly used extensions.

## 5.1 Which Browser to Choose

Windows comes with Microsoft Edge, which is built on Chromium and offers good compatibility. You can use it as is.

Firefox is also a solid choice.

Please **avoid** any Chinese-branded browsers such as 360 Extreme Browser or 2345 Browser.

## 5.2 Multi‑Threaded Downloading: FDM

The built‑in downloader in browsers is single‑threaded. Large files download slowly, and if the connection drops, you may have to restart from scratch. Multi‑threaded tools split a file into chunks and download them simultaneously, which is faster and supports resuming interrupted downloads.

**FDM (Free Download Manager)** is recommended. It is free, open‑source, and ad‑free. During installation, enable the browser integration plugin. No manual configuration is needed. When you download a large file (several hundred MB or more), FDM will be invoked automatically.

[FDM Download Link](https://www.freedownloadmanager.org/)

## 5.3 Search Engines

**Bing** is recommended. It is accessible from within China and shows fewer ads than Baidu. If you have access to Google, its English search results are more accurate.

## 5.4 Effective Search Tips

Do not type full sentences. Extract and use only the core keywords.

Examples:
- Bad: `Why does my mouse stop moving after I plug in a USB drive`
- Better: `USB mouse stops working after plugging in a USB drive`

Useful search operators:

| Operator | Purpose | Example |
|----------|---------|---------|
| `site:` | Restrict to a specific website | `site:reddit.com laptop power supply` |
| `filetype:` | Restrict to a file format | `filetype:pdf calculus textbook` |
| `"double quotes"` | Exact phrase match | `"how to install Python on Windows"` |
| `-` minus sign | Exclude a word | `apple -fruit` |

When searching for an error message, copy the complete error code (e.g., `0x80070005`) rather than describing what happened – it works much better.

## 5.5 Judging Whether Information Is True or False

- **Check the source**: Prioritize official sources and reputable media. Be cautious with unknown self‑media accounts.
- **Check the date**: Outdated information may no longer be valid.
- **Cross‑validate**: Compare different sources.
- **Look at comments**: If many commenters disagree or point out factual errors, the content is likely unreliable.
- **Watch for clickbait headlines**: Words like “shocking,” “urgent,” “watch before deletion” often signal hype or misinformation.

For academic purposes, use Google Scholar or your university library’s databases. The references in English Wikipedia articles can serve as a starting point.

## 5.6 How to Ask Questions and Netiquette

When asking for help, include as much of the following as possible:
- **The full error message** (text or screenshot)
- **Your environment**: OS version, software version, hardware model, etc.
- **Log files**: Many programs and games generate `.log` files in their installation folder or under `%appdata%`. Copy the last few dozen lines.

Example (game crashing to desktop):
> **Problem**: Game goes black and crashes on startup  
> **Environment**: Windows 11 22H2, graphics driver 610.47, game version 1.2.3  
> **Error message**: `Fatal error: DXGI_ERROR_DEVICE_REMOVED`  
> **Last lines of the log**: [paste here]  
> **Already tried**: Updated drivers, verified game file integrity

Basic netiquette:
- Try to find the answer yourself first. If you can’t, then ask.
- Describe the problem clearly and mention what you have already tried.
- After receiving help, report back the outcome so others can learn.
- Do not send private messages for public questions – ask in public forums/channels.

## 5.7 Useful Browser Extensions

Extensions should be installed from the official stores (Edge Add‑ons, Chrome Web Store, Firefox Add‑ons).

### 5.7.1 Ad Blocking: uBlock Origin

Open‑source, lightweight, and blocks most ads and trackers. For small websites or personal blogs that you visit often, if the ads are not intrusive, you can disable uBlock (click the extension icon → the power icon turns gray) or add the site to the allowlist. Those sites rely on ad revenue to stay running.

### 5.7.2 Cat Catch

Sniffs out the real addresses of audio and video on a page and lets you download them. When a page has media playing, the extension icon shows a number; click it and choose what to download. Useful for recording online lectures, background music, etc.

### 5.7.3 LeechBlock NG

Limits the time you can spend on certain websites. You can set a maximum daily allowance or specific allowed time windows. Once the limit is reached, access is automatically blocked. Helps control time spent on social media or video sites.

### 5.7.4 Tampermonkey

A user‑script manager. You can install scripts from GreasyFork (greasyfork.org) to auto‑fill forms, remove ads, and add custom functionality.

**Security reminders**:
- Do not install scripts from unknown sources, especially those promising “free VIP” or “shopping bot”.
- Before installing, look at the script code. If it requests many external permissions (e.g., `@connect *`), do not install it.
- Do not install extensions from outside the official stores, especially “free VPN” or “web accelerator” tools.

---

# Extended Reading

> The following sections go into browser internals and technical details. They are more advanced. It is recommended to skim them first – just be aware these concepts exist. When you run into a specific problem later, you can search again using the keywords you learned here.
> If something puzzles you or you want to dive deeper, I suggest looking up [MDN](https://developer.mozilla.org) and [W3Schools](https://www.w3schools.com).

## Extended 1: Browser Engine

The browser engine (rendering engine) turns HTML, CSS, and JavaScript code into a visible page.

- **Blink**: Used by Chrome, Edge, Opera, Brave, and others. Currently the highest market share.
- **Gecko**: Used by Firefox, completely open‑source.
- **WebKit**: Used by Safari. Blink originally forked from WebKit.

Ordinary users do not need to worry about these differences. Very rarely, an old website may say “Please use Chrome” – but Edge will actually work fine too. If a site looks broken, try switching browsers.

## Extended 2: Web Page Structure (HTML, CSS, JavaScript)

A web page is built from three languages:

- **HTML**: Defines content and structure. The browser parses it into a DOM tree.
- **CSS**: Defines style and layout – colors, fonts, positioning, etc.
- **JavaScript**: Defines interactive behavior – button clicks, carousels, form validation, etc.
  - **TypeScript**: A superset of JavaScript that adds type annotations. It needs to be compiled into JavaScript to run.
  - **ES6**: The 2015 version of JavaScript, introducing arrow functions, classes, modules, etc. All modern browsers support it.

**Responsive design**: Uses CSS media queries (`@media`) to automatically adapt to different screen widths. Press F12 → click the device simulation icon (a small phone) to see how a site looks on mobile.

**Progressive enhancement**: Start with basic content (plain HTML) accessible on all devices, then add styles and interactions on top. This works better for older devices and users with disabilities.

## Extended 3: Response Headers and Status Codes

When a browser requests a page, the server returns response headers. Press F12 → Network tab → click any request to see them.

Common status codes:
- **200**: Success
- **301**: Permanent redirect
- **302** (or 307): Temporary redirect
- **403**: Forbidden
- **404**: Page not found
- **500**: Internal server error

## Extended 4: User‑Agent (UA)

A string the browser sends to the server, containing browser name, version, and operating system. The server can use the UA to return different versions (e.g., mobile site vs. desktop site).

Press F12 → device simulation (choose a phone model), then refresh the page – you are now pretending to be a mobile device. You can also install a UA switcher extension. Do **not** use UA spoofing to scrape website data; always follow the `robots.txt` rules.

## Extended 5: Cookie, localStorage, IndexedDB

Three ways browsers store data locally:

| Name | Capacity | Lifetime | Sent with requests | Main use |
|------|----------|----------|--------------------|-----------|
| Cookie | 4KB | Can set expiration | Yes | Login state, user tracking |
| localStorage | 5–10MB | Permanent (unless manually cleared) | No | User preferences, theme settings |
| IndexedDB | Hundreds of MB | Permanent | No | Offline cache for large structured data |

- **Cookie**: Stores a session ID. If you clear cookies, you will be logged out.
- **localStorage**: Simple API, good for small settings.
- **IndexedDB**: Supports indexes and transactions, but the API is more complex. Many PWAs use it to cache data.

View and manage: F12 → Application → Storage section.

## Extended 6: Chrome/Edge Flags and Firefox about:config

Entry points for experimental browser features:
- Chrome/Edge: Type `chrome://flags` or `edge://flags` in the address bar.
- Firefox: Type `about:config` in the address bar, then click “Accept the Risk”.

If changes cause problems, click “Reset all” to restore defaults.

## Extended 7: Main Panels of Developer Tools (F12)

- **Elements**: View and edit HTML/CSS (temporary, local only).
- **Console**: Shows JavaScript errors and warnings; you can also execute JS code.
- **Sources**: View source files and set breakpoints to debug JS.
- **Network**: Records all network requests – status codes, timings, request/response data.
- **Application**: Manage cookies, localStorage, IndexedDB, and Service Workers.
- **Performance**: Record performance data to analyze lag or jank.
- **Lighthouse**: Provides a comprehensive score for performance, accessibility, SEO, etc.

## Extended 8: Service Worker and PWA

**Service Worker**: A script that runs in the background of the browser. It can intercept network requests, manage caches, and send push notifications – even when the page is not open.

**PWA (Progressive Web App)**: Uses Service Workers and other technologies to give a web app a native‑like experience – it can be installed on the desktop, work offline, and receive push notifications. When you see an install icon (a computer screen with a down arrow) in the Chrome or Edge address bar, you can click it to install the site as a desktop app.

## Extended 9: CORS (Cross‑Origin Resource Sharing)

The browser’s security policy, the “same‑origin policy”, prevents a web page from making requests to a different domain. If a page legitimately needs to request data from another domain, that target server must include the `Access-Control-Allow-Origin` header in its response.

When developing local web pages, you often run into CORS errors. To fix them, either configure the backend to send the proper headers or use a proxy. Understanding CORS helps you understand why certain API calls from a web page might fail.

## Extended 10: WebFonts, Browser Cache, and WebAssembly (Wasm)

### WebFonts

Fonts used on websites (e.g., Noto Sans, Source Han Sans) are often not installed on the operating system, so they must be downloaded from the server. These font files (`.woff2`, `.woff`, `.ttf`) are called WebFonts.

- A website declares which font files to download using the `@font-face` rule in CSS.
- The browser downloads them and caches them locally, so they don’t need to be downloaded again on subsequent visits.
- Common formats: **WOFF2** (high compression, used by modern browsers), **WOFF** (for older browser compatibility), **TTF** (uncompressed, rarely used).

If text on a site flickers or appears as squares (gibberish), WebFonts might have failed to load. Press F12 → Network tab → filter by “Font” to see if font requests return 404.

### Browser Cache

The HTTP cache stores downloaded resources (images, CSS, JS, fonts, etc.) on the local disk. When you revisit the same page, resources are read from the local disk instead of being requested from the server again, which speeds up loading significantly.

**Cache mechanism in a nutshell**:
- When a server returns a resource, it includes headers like `Cache-Control` or `Expires` that tell the browser how long it may cache the resource.
- For example, `Cache-Control: max-age=86400` means the resource is valid for 24 hours; the browser will use the cached copy without even asking the server during that time.
- After the cache expires, the browser sends a “conditional request” using `If-Modified-Since` or `ETag` to ask the server whether the resource has changed. If it hasn’t, the server returns `304 Not Modified` and the browser continues using the cached copy.

**Manually controlling the cache**:
- Press `Ctrl+F5` to force a refresh that re‑downloads everything from the server.
- Press F12 → Network tab → check “Disable cache” to force fresh downloads during debugging.
- Clear the browser cache: Settings → Privacy and security → Clear browsing data → choose “Cached images and files”.

### WebAssembly (Wasm)

WebAssembly is a binary instruction format that runs in the browser. Its goal is near‑native execution speed while maintaining safety. Languages like C, C++, Rust, and Go can be compiled into Wasm and then called from JavaScript inside a web page.

**Why do we need Wasm?**  
Traditional JavaScript is interpreted (though modern JS engines use JIT compilation), but for computationally intensive tasks (video encoding/decoding, image processing, game physics, cryptography) it may still not be fast enough. Wasm offers a lower‑level, pre‑compiled format that executes significantly faster.

**A simple Wasm example**

Here’s how to write a C function that adds two numbers, compile it to Wasm, and call it from HTML.

**Step 1: Write C code (`add.c`)**
```c
// Export an addition function
int add(int a, int b) {
    return a + b;
}
```

**Step 2: Compile to Wasm**  
After installing the Emscripten toolchain, run:
```bash
emcc add.c -o add.wasm -s EXPORTED_FUNCTIONS="['_add']" -s EXPORTED_RUNTIME_METHODS="['ccall']" --no-entry
```
This generates an `add.wasm` file.

**Step 3: Load and call it from HTML**
```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Wasm Addition Example</title>
</head>
<body>
    <script>
        // Load the wasm module
        fetch('add.wasm')
            .then(response => response.arrayBuffer())
            .then(bytes => WebAssembly.instantiate(bytes))
            .then(results => {
                // Call the exported add function
                const result = results.instance.exports._add(3, 5);
                console.log('3 + 5 =', result);  // Outputs 8
                document.body.innerHTML = '<p>3 + 5 = ' + result + '</p>';
            });
    </script>
</body>
</html>
```

**Real‑world use cases**:
- **Figma**: The graphics rendering engine is implemented in Wasm.
- **AutoCAD Web**: The browser version of the CAD software.
- **Video / image processing**: For example, ffmpeg compiled to Wasm enables online transcoding.
- **Game engines**: Unity and Unreal Engine can export to Wasm for web deployment.

Ordinary users do not need to interact with Wasm directly. But knowing about it helps you understand why certain web apps run so fast and why your CPU usage might suddenly spike – the page could be running a heavy Wasm computation task.