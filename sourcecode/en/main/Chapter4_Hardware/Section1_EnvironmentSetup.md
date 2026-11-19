# Section 1: Setting Up the Environment
  
> I will hammer on two things over and over in this section: use a normal browser, and when the network won't cooperate, put a proxy on (TUN mode recommended).  
> Worth noting: some of this has already come up in earlier chapters, and I won't repeat it – browsers and proxies get a fuller treatment in `Appendix 5` and `Appendix 6`, and Python has already been covered in Chapter 3 under main.


## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| myST account | ST's single account for its website – used to download tools, sign in to CubeMX, and activate licences |
| STM32CubeCLT | ST's official command-line toolset, with the compiler, build system, debug server, and flashing tools all installed |
| STM32CubeMX | The graphical configuration tool – pick the clocks, enable peripherals, and generate a CMake project skeleton |
| ST-LINK driver | The USB driver that lets Windows recognise the ST-LINK debug probe; without it you can neither flash nor debug |
| VSCode Profile | A set of mutually isolated "VSCode personalities": extensions, settings, and keybindings each mind their own business |
| STM32Cube for VS Code | ST's official extension; installing this one alone brings the whole pack – C/C++, CMake Tools, Cortex-Debug and the rest |
| Bundles Manager | The toolchain manager built into the ST extension; it downloads GCC, CMake, Ninja, and GDB Server in the background |
| TUN mode | A global-takeover mode in proxy software – lower-level than "system proxy", and much harder for any single program to "not see" |
| Build Analyzer | The build analyser in the CMake sidebar; if it has something to show, the project has been correctly recognised and configured |


## 1.1 Register a myST Account First – and in the *Right* Place

Later on, installing CubeMX will ask you to sign in; downloading the toolchain will ask you to sign in; some builds of the CubeCLT installer even want an account verified. **Get the account out of the way first, and the road ahead stays clear.**

Open your browser and go to ST's **English site**:

```
https://www.st.com/content/st_com/en.html
```

Note the `en` – this is not the Chinese site. The Chinese site sometimes routes the registration entry into a different flow, or simply stalls. **If ST is unstable from where you are, put the proxy on for this step.** My own experience: registration, sign-in, and activation are the steps most sensitive to network quality; the big downloads afterwards are comparatively rugged.

Fill in your email, name, and company (students: put your school; if you have none, `Individual` / `Student` will do), and ST will send you an email. **That email is the real door to an activated account** – not the "registration successful" notice on the sign-up page. Click the link inside, then set a username and password. If it isn't in your inbox, check spam first: ST's verification mail gets misjudged rather often.

**On browsers**: use Edge, Chrome, or Firefox. Do not use certain domestic browsers. CubeMX sign-in later hands the OAuth hop to the system default browser, which may fail to recognise a non-conforming domestic browser and open a built-in Chromium window instead – the result being a login page that crawls, a callback address that gets misread, or a plain white screen. You will then waste a great deal of time on "why can't CubeMX sign in", when the root cause was only ever the browser.

Once activation is done, you have a working myST account. **Don't rush to click "Download"** – the tools in the next section will manage all that for you once you sign in to CubeMX with this account.


## 1.2 Installing CubeCLT and CubeMX: All Defaults

### 1.2.1 CubeCLT

Download the Windows installer for STM32CubeCLT (`.exe`) from ST's website, then **right-click → Run as administrator**.

**Do not change the install path; the default is fine.** It is usually `C:\ST\STM32CubeCLT_x.xx.x\`. Don't move it to the D: drive, don't tuck it into some "Software" folder, and above all don't let the path acquire Chinese characters or spaces. The ST extension and Bundles Manager have detection logic keyed to the default location; changing the path won't make anything "cleaner", it will only make auto-discovery wander a few extra turns.

When the installer finishes, you will see subfolders named `CMake`, `Ninja`, `GNU-tools-for-Stm32`, `STLink-gdb-server`, and `STM32CubeProgrammer` under that default directory. **Leave them alone for now** – the ST extension will find them on its own.

### 1.2.2 CubeMX

Likewise, download the STM32CubeMX Windows installer from ST's website, and **run it with the default settings**. Version 6.11.0 or later is recommended, since we will want it to generate a CMake project.

Once it's installed, open CubeMX first. It will ask you to sign in to your myST account. **This sign-in very likely needs a proxy, and TUN mode is the recommendation.** "System proxy" mode is sometimes invisible to CubeMX's Java network stack, which shows up as "Check Connection" failing forever or the sign-in button just spinning. TUN mode takes over traffic at the operating-system level; CubeMX has no way around it.

If your network is already friendly to ST (HUST's campus network, for one – it deserves to be named), you can skip the proxy. But **to be safe, run TUN mode for sign-in and for the update downloads afterwards**, so you don't keep retrying something that was never supposed to cost time.

After a successful sign-in, CubeMX will offer to download a pile of update packages, device support packs (DFP), HAL libraries, and so on. **You can't use it without these updates**, so don't skip them. Keep the proxy on and let the download finish.


## 1.3 Installing the ST-LINK Driver

Neither the ST extension nor CubeCLT necessarily installs the USB driver for you. **Before your first ST-LINK connection, installing the driver by hand is the safest course.**

Search ST's website for **STSW-LINK009** – the Windows USB driver package for ST-LINK/V2, V2-1, and V3. Download it, extract it, run the `dpinst_x64.exe` inside (or whichever build matches your system), and follow the prompts. Afterwards **reboot once** so the driver takes effect.

> Some boards (Nucleo, Discovery) carry an on-board ST-LINK that Windows picks up with a basic driver the moment you plug them in – but that driver is not necessarily complete. Installing STSW-LINK009 by hand ensures both the debug interface and the virtual COM port get enumerated properly. If you are using a standalone ST-LINK V2 or V3, this step is **mandatory**.


## 1.4 Building a Dedicated STM32 Profile in VSCode

### 1.4.1 Why a Profile

If VSCode is only ever your STM32 editor, you can live without a Profile. But the moment you also write Python, front-end code, or firmware for other microcontrollers on the same machine, extensions and settings start to quarrel. The STM32 extension drags in CMake Tools, Cortex-Debug, C/C++, and their kin, which collide with same-named or same-purpose extensions you already have – overriding each other, and fighting over the status bar and the command palette.

What a Profile does is **environment isolation**: one Profile, one set of extensions, one set of settings. The entire ST suite you install inside the "STM32" Profile never contaminates the Profile you use for everyday Python.

### 1.4.2 Creating and **Activating** a Profile

Open VSCode and click the gear icon in the bottom-left corner:

![VSCode Profile](/img/4.1.4.2-1.png)

Click Profile, then choose Profiles in the sub-window that opens; the interface should look something like this:

![VSCode Profiles Manager](/img/4.1.4.2-2.png)

Click New Profile to create a new one, rename it to STM32 if you like, then click this button to activate it:

![VSCode Profile Activation](/img/4.1.4.2-3.png)

And with that, you have created a Profile named STM32 and activated it.


## 1.5 Install the ST Extension and Let It Pull the Whole Suite Down

**With the STM32 Profile active**, open the Extensions panel (`Ctrl+Shift+X`) and search for:

```
STM32Cube for Visual Studio Code
```

The publisher is STMicroelectronics – look for the butterfly logo. Click Install.

**Installing this one extension automatically brings in its dependencies – C/C++, CMake Tools, Cortex-Debug, and so on.** You don't need to search for them by hand, nor click Install on each. The previous edition had you install four plugins manually; that was redundant.

Installation takes a while. Once the ST extension is in place, it starts Bundles Manager in the background, which automatically downloads the toolchain components it judges you need – ARM GCC, CMake, Ninja, GDB Server, and so on. **Keep TUN mode on for this step** so the download runs smoothly. If your connection tends to drop during large downloads, close VSCode first and reopen it once the network is steady; Bundles Manager can usually resume.

**How do I know it's done?** One practical signal: **the bottom-right corner stops popping notifications and spinning**. While the extension works in the background, VSCode may show notices down there; when it falls quiet, you are essentially there. When it's finished you can close VSCode and open the project in the next section.

With the ST extension installed and Bundles Manager quiet, install the Python extension as well while you're at it.

This one is for the host-side software you will write yourself. The project's ultimate goal is a DSP sound card: the chip runs firmware, the computer runs the host application – tuning parameters, watching waveforms, flashing firmware, running automated tests. All of that is written in Python. You don't want to be switching back to the default Profile later just to install one extension.

**How to install Python itself, how to create a virtual environment, how to configure the extension – Chapter 3, Section 1 already covered all of it from beginning to end. Go back and follow it; I won't repeat it here.** The one reminder: **the Python extension also belongs inside the `STM32` Profile**, not back in the default one. The whole point of a Profile is that "everything in this window belongs to the STM32 project"; flipping between host scripts and firmware code in one window is far less trouble than keeping two VSCode windows open.

If you intend to keep host scripts in a `tools/` or `host/` subfolder of the project, put the virtual environment there too, and remember to add `.venv/` to `.gitignore` – also covered earlier, so I won't expand on it.


## 1.6 Generating a CMake Project with CubeMX

Open CubeMX (already signed in to myST). `File → New Project`, search for `STM32G474CEx`, select the chip, and start the project.

The goal in this section is not functionality, so nothing needs configuring; in principle the series you pick doesn't matter. But I don't know whether the H7 series differs from the others, so to be safe – and since this is only a trial run – I suggest you pick the STM32G474CEx part and start there.

Go to the `Project Manager` page:

- `Project Name`: English, no spaces – `blink_test`, say.
- `Project Location`: a directory with **no Chinese characters and no spaces**.
- `Toolchain / IDE`: choose **CMake**.

> One more aside: I suggest cultivating good project-management habits. You could create a folder called `STProjects` on your D: drive, for instance, to hold every program you write for ST microcontrollers. It pays off in many ways – when you want to reuse some code you won't have to hunt for it, and everything stays tidy.

Click `GENERATE CODE`. CubeMX will produce a complete project with a `CMakeLists.txt`.


## 1.7 Open the Project in the STM32 Profile and Let It "Recognise Itself"

Open the project folder you just generated in VSCode (`File → Open Folder`, down to the `blink_test` level). Confirm the current Profile is still `STM32`.

Once it's open, the ST extension may pop a prompt in the bottom-right asking whether to configure this CMake project, or it may simply start configuring in the background. **Let it run.** The extension is doing the whole sequence on your behalf: pick a Kit, run CMake Configure, generate the debug configuration.

**How do you tell it succeeded?** Restart VSCode, open the project, and look at the activity bar on the left:

- If the **CMake icon** appears, click into it, and **Build Analyzer** shows something (a target, a source-file list, or build-artifact information), then the project has been correctly recognised and configured by CMake Tools.
- If the CMake sidebar never appears, or appears empty, that usually means the project has not been recognised as a CMake project, or no Kit has been selected yet.

**At this step you do not need to hand-edit paths in `launch.json`, and you do not need to write flashing commands into `tasks.json`.** Once the extension recognises the project, it generates a working debug configuration pointing at the GDB Server and toolchain it downloaded itself. Those paths are managed by the ST extension and adjust themselves when you upgrade CubeCLT or the extension – precisely where "let the extension manage it" beats "hard-coded absolute paths".


## 1.8 Building and Debugging: Close the Loop

Click **Build** (the hammer icon) in the CMake sidebar or the bottom status bar, or run `CMake: Build` from the command palette. On a successful build, an `.elf` file appears under `build/`.

Press `Ctrl+Shift+D` to open Run and Debug; the dropdown at the top should already hold a debug configuration generated by the ST extension (its name may be `Debug` or `STM32 Debug`). Select it and press `F5`.

If all is well, the program is downloaded into the chip, halts at `main`, and the variables and peripheral-register panels appear on the left. **If the peripheral-register panel is empty while breakpoints and stepping work normally**, the SVD file simply wasn't associated – that doesn't stop you validating the toolchain in this section; we will deal with it in later chapters.

And with that, the loop – **"register an account → install the tools → create a Profile → install the extension → generate a project → build → debug"** – is closed.


## 1.9 Common Issues

> Fair warning: this Q&A is certainly not exhaustive. If you hit something it doesn't answer, go ask DeepSeek – ideally through the DeepSeek plugin inside VSCode – and let it sort the problem out, because environment troubles are always torture.
> If you can't tell what you're doing, my advice is to ask DeepSeek rather than wreck your own environment.

**Q1: I finished registering a myST account, but the verification email never arrives.**

Check spam first – ST's verification mail is misjudged with no small probability. If it isn't in the spam folder either, check whether the address you used is one of the domestic free mail services (especially those with a corporate-domain suffix); some mail paths drop messages silently. Resend once with Gmail, Outlook, or a university address. If it still fails, **turn the proxy on and click "Resend verification email" once more**. It isn't that ST won't send it – some segment of the path is timing out.

One more caution: the "registration successful" notice on the sign-up page **is not the same thing as an activated account**. What actually activates it is the link in the email. Until you have clicked that link, CubeMX will reject your account at sign-in.


**Q2: "Check Connection" in CubeMX keeps failing, yet I can open ST's website in a browser just fine.**

That is a **proxy mode** problem, not a broken network. CubeMX is Java-based, and its network stack often fails to pick up "system proxy" mode. Switch to **TUN mode** and let the proxy take over traffic at the operating-system level – CubeMX has no way around it.


**Q3: When signing in to CubeMX, the browser opens a frozen window, or the login page is blank white.**

**Change browsers.** Edge, Chrome, Firefox – any of them; not a domestic one. CubeMX sign-in is an OAuth redirect, which invokes the system default browser; certain domestic browsers open a built-in Chromium window in "compatibility mode" to catch that callback, and the symptoms are slow loading, a misparsed callback address, or a plain white screen.

To change the default browser: Windows Settings → Apps → Default apps → Web browser. Afterwards **close CubeMX and reopen it** so it invokes the default browser afresh.


**Q4: I installed the ST extension in the default Profile and created the STM32 Profile only afterwards – now the extension is gone after switching.**

You did it in the wrong order. **Create the Profile first, activate it, then install the extension.** Extensions live inside a Profile: whichever Profile you were in when you installed it, that is the one that owns it.

The remedy: switch back to the Profile where you installed it and uninstall the ST extension; switch to the STM32 Profile and install it again. Uninstall-and-reinstall is less trouble than "migrating extensions", and an install only takes a few minutes.


**Q5: The ST extension is installed, but Bundles Manager keeps downloading and the bottom-right corner has been spinning for ages.**

Perfectly normal – it is fetching GCC, CMake, Ninja, and GDB Server, several hundred MB in total. **Run TUN mode and let it finish**; don't close VSCode midway. If the network is unstable and the download breaks, reopen VSCode; Bundles Manager can usually resume.

The signal that it's done: **the bottom-right corner stops popping notifications and spinning**. Once it goes quiet, you can close VSCode and move on.


**Q6: I opened the project generated by CubeMX, but the CMake sidebar never appears – or it appears and is empty inside.**

Check three things, in order:

1. **Are you actually in the project root?** `File → Open Folder` must land on the `blink_test` level, not its parent directory. CMake Tools only recognises a project when the root of the opened folder contains `CMakeLists.txt`.
2. **Is the current Profile STM32?** With the wrong Profile, neither the ST extension nor CMake Tools is present, so of course nothing responds.
3. **Restart VSCode once.** CMake Tools occasionally needs a restart before it refreshes its view of the project. A small quirk of its own rather than a bug, but annoying all the same.

If all three check out and it still won't work, open the command palette and run `CMake: Configure`, then read what the Output panel complains about. The messages are usually blunt; "compiler not found", for instance, means the Kit is wrong.

**Environment setup is maddening; if it still isn't clear, go ask DeepSeek**


**Q7: The build fails, saying `arm-none-eabi-gcc` cannot be found.**

In all likelihood the **Kit is wrong**, or Bundles Manager hasn't finished downloading the toolchain. First confirm the download in Q5 has completed, then run `CMake: Select a Kit` from the command palette and pick the entry whose name contains STM32CubeCLT or arm-none-eabi.

If you can't tell what you're doing, my advice is to ask DeepSeek rather than wreck your own environment.

If no such entry exists in the Kit list at all, the ST extension hasn't found CubeCLT. Confirm CubeCLT was installed at the **default path** (`C:\ST\STM32CubeCLT_*`) and has not been moved to another drive. The extension's detection logic is keyed to the default path; changing it makes auto-discovery take detours.


**Q8: After F5 it hangs at "Launching", or reports `Error: Failed to launch GDB Server`.**

Check in order:

1. **Is the ST-LINK driver installed?** Back to Section 1.3: confirm STSW-LINK009 is installed and that you have rebooted since.
2. **Is the chip locked?** If an earlier program remapped the SWD pins to ordinary GPIO, the debug interface is switched off. Run a full-chip erase with `STM32_Programmer_CLI`, then reconnect.
3. **Is the USB cable a charge-only one?** It sounds foolish, but people genuinely run into this. Swap in a cable you know can carry data.


**Q9: I hand-edited the paths in `launch.json`, and then the ST extension updated and every path broke.**

That is the side effect of hand-writing absolute paths. The right posture is to **take the configuration the ST extension generates as your base**, make the smallest possible edits only where a field genuinely needs adjusting (a wrong chip model, a missing SVD path), and leave paths to the extension.

If you have already made a mess of it, the simplest fix is to delete `.vscode/launch.json` and let the ST extension regenerate one. Re-importing the project works too (ST extension icon → `Import CMake Project`).


**Q10: My machine already has CMake and Ninja on it – will installing CubeCLT make them fight?**

**Not with a default install.** The CMake and Ninja that ship with CubeCLT are referenced only inside STM32 projects, through a Kit or the extension; the CMake already on your system is still the `cmake` in your terminal. The two sets keep to themselves.


## 1.10 What You Did in This Section

1. Registered a myST account on ST's **English site**, completed activation through the email link, and learned what sign-in and activation demand of your proxy.
2. Installed CubeCLT and CubeMX as administrator, at the **default paths** – without touching PATH, and without manually relocating anything.
3. Installed the **STSW-LINK009** ST-LINK USB driver by hand, and rebooted.
4. Created and **activated** a separate Profile named `STM32` in VSCode, isolating extensions and settings.
5. Installed only one extension in the STM32 Profile – **STM32Cube for Visual Studio Code** – and let it pull in C/C++, CMake Tools, Cortex-Debug and the other dependencies, with Bundles Manager downloading the toolchain in the background. Then added the **Python extension** inside the `STM32` Profile, in preparation for the host software to come.
6. Generated an STM32G474CEx project in CubeMX with **CMake** as the toolchain.
7. Opened the project in the VSCode STM32 Profile and waited for the ST extension and CMake to recognise it; the criterion for "recognised" was **whether Build Analyzer in the CMake sidebar has content**.
8. Closed the loop – Build → Debug → halting at `main` – without hand-writing a single absolute path in `launch.json`.

## 1.11 A Few Personal Indulgences

![VSCode Interface](/img/4.1.11-1.png)

This is my current VSCode interface, and I find it handsome. According to my thoroughly unscientific research, the following claim is pure personal conjecture: a handsome interface makes writing code or writing prose more pleasurable, and therefore makes the work more enjoyable to do.

If you also think it looks reasonably handsome, the configuration below is yours to try.

Install the `Catppuccin for VSCode` extension; it offers four elegant colour schemes. The theme I use is Catppuccin Latte, a light theme – with plenty of ambient light, I find a light theme tires my eyes less.

The font I use is `Maple Mono NF CN`, an excellent typeface. I am especially fond of its italics, and its Chinese glyphs are beautiful, harmonious, and properly monospaced. I recommend it.

I use a font size of 16; that hardly matters – bigger or smaller is up to you.

The default interface colours, however, lean rather too cool, so I adjusted them. Below is the configuration; take what you need and put it into `settings.json`.

```json

    "editor.fontSize": 16,
    "editor.fontFamily": "'Maple Mono NF CN', 'JetBrains Mono', 'Fira Code', Menlo, Consolas, monospace",
    "terminal.integrated.fontFamily": "monospace",
    "workbench.colorTheme": "Catppuccin Latte",
    "workbench.colorCustomizations": {
        "[Catppuccin Latte]": {
            "editor.background": "#ececec",
            "activityBar.background": "#ececec",
            "sideBar.background": "#ececec",
            "tab.activeBackground": "#ececec",
            "panel.background": "#ececec",
            "terminal.background": "#ececec",
            "editorGroupHeader.tabsBackground": "#ececec",
            
            "editorGutter.background": "#ececec",
            "minimap.background": "#ececec",
            
            "editor.lineHighlightBorder": "#00000000",
            "editorOverviewRuler.border": "#00000000",
            "sideBar.border": "#00000000",
            "panel.border": "#00000000",
            "editorGroup.border": "#00000000",
            "activityBar.border": "#00000000",
            "editorGroupHeader.tabsBorder": "#00000000",
            "statusBar.border": "#00000000",
            "tab.border": "#00000000",

            "titleBar.activeBackground": "#dcd3e8",
            "titleBar.activeForeground": "#333333",
            "statusBar.background": "#dcd3e8",
            "statusBar.foreground": "#333333",
            "statusBar.noFolderBackground": "#dcd3e8",
            "statusBar.debuggingBackground": "#dcd3e8"
        }
    }
    
```