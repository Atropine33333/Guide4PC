# 第1节：环境搭建
  
> 我会在这节里反复强调两件事：用正常浏览器，网络不行就挂代理（建议 TUN 模式）。  
> 值得注意，有一些东西我已经在之前的章节里出现过了，我不在赘述，比如关于浏览器和代理我在 `附录5` 和 `附录6` 均有更细致的讲述，关于 Python 我在 main 下面的 Chapter3 已经讲述过了。


## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| myST 账户 | ST 官网的统一账户，下载工具、登录 CubeMX、激活许可证都用它 |
| STM32CubeCLT | ST 官方的命令行工具集，装齐编译器、构建系统、调试服务器和烧录工具 |
| STM32CubeMX | 图形化配置工具，选时钟、开外设，生成 CMake 工程骨架 |
| ST-LINK 驱动 | 让 Windows 认出 ST-LINK 调试器的 USB 驱动，不装就烧不进去、调不了 |
| VSCode Profile | 一套互相隔离的“VSCode 人格”：扩展、设置、快捷键各管各的，互不污染 |
| STM32Cube for VS Code | ST 官方扩展，装它一个就等于装齐 C/C++、CMake Tools、Cortex-Debug 等一整包 |
| Bundles Manager | ST 扩展内置的工具链管理器，在后台自动下载 GCC、CMake、Ninja、GDB Server |
| TUN 模式 | 代理软件的一种全局接管模式，比“系统代理”更底层，不容易被某个软件“看不见” |
| Build Analyzer | CMake 侧边栏里的构建分析器；它能显示东西，说明工程已经被正确识别和配置 |


## 1.1 先注册 myST 账户，而且要在“对的地方”注册

后面装 CubeMX 要登录，下载工具链要登录，甚至某些版本的 CubeCLT 安装包也需要账户验证。**先把账户搞定，后面一路畅通。**

打开浏览器，访问 ST 官网的**英文站** ：

```
https://www.st.com/content/st_com/en.html
```

注意是 `en`，不是中文站。中文站有时注册入口会导向不同的流程，或者干脆卡住。**如果你所在的网络访问 ST 不稳定，注册这一步就挂上代理。** 我个人的经验是：注册、登录、激活这几步对网络质量最敏感，后面下载大文件反而相对“皮实”。

填完邮箱、姓名、公司（学校填学校，没有就填 Individual / Student 即可）之后，ST 会给你发一封邮件。**这封邮件才是真正“激活账户”的入口**，不是注册页面上那个“注册成功”的提示。点邮件里的链接，进去设置用户名和密码。如果收件箱里找不到，先翻垃圾邮件——ST 的验证邮件有时会被误判。

**关于浏览器**：请用 Edge、Chrome 或 Firefox。不要用某些国产浏览器。因为后面 CubeMX 登录会调用系统默认浏览器做 OAuth 跳转，可能识别不到并不规范的国产浏览器，于是会打开一个内置 Chromium 窗口，结果就是登录页加载缓慢、回调地址识别错误、或者干脆白屏。你会在“为什么 CubeMX 登录不了”上浪费大量时间，而根因只是浏览器。

激活完成之后，你有一个可用的 myST 账户了。**先不要急着去点“下载”**，下一节的工具用这个账户登录 CubeMX 之后自然会帮你管理。


## 1.2 安装 CubeCLT 和 CubeMX：全部默认

### 1.2.1 CubeCLT

从 ST 官网下载 STM32CubeCLT 的 Windows 安装包（`.exe`），**右键 → 以管理员身份运行**。

**安装路径不要改，默认就好。** 默认路径通常是 `C:\ST\STM32CubeCLT_x.xx.x\`。不要挪到 D 盘，不要塞进某个“Software”文件夹，更不要带中文或空格。ST 的扩展和 Bundles Manager 对默认路径有识别逻辑，改路径不会让它“更干净”，只会让自动发现多绕几个弯。

安装完成后，你会在默认目录下看到 `CMake`、`Ninja`、`GNU-tools-for-Stm32`、`STLink-gdb-server`、`STM32CubeProgrammer` 这些子文件夹。**先不用管它们**，ST 扩展之后会自己找到。

### 1.2.2 CubeMX

同样从官网下载 STM32CubeMX 的 Windows 安装包，**运行并保持默认设置**。版本建议 6.11.0 及以上，因为我们要用它生成 CMake 工程。

装完之后先打开 CubeMX。它会提示你登录 myST 账户。**登录这一步大概率需要代理，而且建议用 TUN 模式。** “系统代理”模式有时候会被 CubeMX 的 Java 网络栈认不到，表现是“Check Connection”永远失败，或者登录按钮转圈。TUN 模式在操作系统层面接管流量，CubeMX 想绕都绕不过去。

如果你的网络本身对 ST 很友好（比如 HUST 校园网，值得点名表扬），可以不挂。但**保险起见，登录和后续的更新下载都挂上 TUN 代理**，省得在一个本来不该花时间的地方反复试。

登录成功之后，CubeMX 会提示下载一大堆更新包、器件支持包（DFP）、HAL 库等等。**这一步不更新是用不了的**，不要跳过。继续挂着代理，等它下载完。


## 1.3 安装 ST-LINK 驱动

ST 扩展和 CubeCLT 不一定替你装好 USB 驱动。**在第一次连接 ST-LINK 之前，手动装一遍驱动是最稳妥的做法。**

去 ST 官网搜索 **STSW-LINK009**，这是 ST-LINK/V2、V2-1、V3 的 Windows USB 驱动包。下载、解压，运行里面的 `dpinst_x64.exe`（或对应你系统的版本），按提示完成安装。装完**重启一次电脑**，让驱动生效。

> 有些板子（Nucleo、Discovery）的板载 ST-LINK 在插上时 Windows 会自动装一个基础驱动，但那个驱动不一定完整。手动装 STSW-LINK009 可以确保调试接口和虚拟串口都被正确枚举。如果你用的是独立的 ST-LINK V2 或 V3，这一步是**必须**的。


## 1.4 在 VSCode 里建一个 STM32 专用 Profile

### 1.4.1 为什么需要 Profile

如果你只拿 VSCode 写 STM32，不建 Profile 也能活。但只要你电脑上还写 Python、写前端、写别的单片机，扩展和设置就会开始打架。STM32 扩展装上来之后会带进 CMake Tools、Cortex-Debug、C/C++ 这一套，它们会和你已有的同名或同功能扩展互相覆盖、抢状态栏、抢命令面板。

Profile 做的就是**环境隔离**：一个 Profile 一套扩展、一套设置。你在“STM32” Profile 里装的 ST 全家桶，不会污染你日常写 Python 的那个 Profile。

### 1.4.2 创建并**激活** Profile

打开 VSCode，在左下角找到一个齿轮图标，点开

![VSCode Profile](/img/4.1.4.2-1.png)

点击 Profile，然后在打开的子窗口内选择 Profiles，打开，它的界面应该类似这样

![VSCode Profiles Manager](/img/4.1.4.2-2.png)

点击 New Profile 创建一个新的 Profile 并不妨修改其名称为 STM32，点击这个按钮激活它

![VSCode Profile Activation](/img/4.1.4.2-3.png)

如此，你就创建了一个名为 STM32 的 Profile 并进行了激活。


## 1.5 装 ST 扩展，让它自己把全家桶拉下来

在 **STM32 Profile 激活的状态下**，打开扩展面板（`Ctrl+Shift+X`），搜索：

```
STM32Cube for Visual Studio Code
```

发布者是 STMicroelectronics，认准蝴蝶标志。点击安装。

**这一个扩展安装下去，C/C++、CMake Tools、Cortex-Debug 等依赖会自动被一并安装**。你不需要手动去搜它们，也不需要挨个点安装。上一版让你手动装四个插件，是多余的。

安装过程会比较漫长。ST 扩展装完之后，会在后台启动 Bundles Manager，自动下载它认为你需要的工具链组件——ARM GCC、CMake、Ninja、GDB Server 等等。**这一步继续挂 TUN 代理**，让下载顺畅跑完。如果你的网络在下载大文件时容易断，可以先关掉 VSCode，等网络稳定了再打开它，Bundles Manager 通常能续传。

**我怎么知道装好了没有？** 一个实用的信号是：**右下角不再持续弹窗或转圈**。扩展在后台干活的时候，VSCode 右下角可能会有通知，等它安静下来，基本就到位了。装完之后可以关掉 VSCode，下一节再打开工程。

装完 ST 扩展、等 Bundles Manager 安静下来之后，顺手把 Python 扩展也装上。

这一步是给你自己写上位机用的。这个项目的最终目标是做一块 DSP 声卡，芯片跑固件，电脑跑上位机：调参数、看波形、刷固件、做自动化测试，这些活全用 Python 写。你不希望到时候为了装一个扩展再切回默认 Profile 折腾一遍。

**Python 本体怎么装、虚拟环境怎么建、扩展怎么配置，第三章第一节已经从头到尾讲过一遍了，这里直接回去照做，不重复。** 唯一要提醒的是：**Python 扩展同样要装在 `STM32` 这个 Profile 里**，不要装回默认 Profile——Profile 的意义就是“这个窗口里的一切都属于 STM32 项目”，上位机脚本和固件代码在同一个窗口里切换，比开两个 VSCode 窗口省事得多。

如果你打算把上位机脚本放在工程目录下的 `tools/` 或 `host/` 子文件夹里，虚拟环境也建在那里，`.gitignore` 记得把 `.venv/` 加进去——这个前面也讲过，不再展开。


## 1.6 用 CubeMX 生成一个 CMake 工程

打开 CubeMX（已经登录过 myST 了）。`File → New Project`，搜索 `STM32G474CEx`，选中芯片，开始项目。

这一节的目标不是做功能，所以什么都不需要配置，按理说你选择什么系列都不影响，但是我并不清楚 H7 系列和其他系列是否有什么不同，所以保险起见，反正是尝试，我建议你选择 STM32G474CEx 这个系列，然后开始。

到 `Project Manager` 页面：

- `Project Name`：英文，无空格，比如 `blink_test`。
- `Project Location`：**无中文、无空格**的目录。
- `Toolchain / IDE`：选择**CMake**。

> 另说一句，我建议养成项目管理的好习惯，比如你可以在 D盘 下创建一个叫 STProjects 的文件夹，用于存放你所有使用 ST 单片机的程序，这会给你很多好处，比如你想去复用某些代码不会找不到，还有这样管理很清晰。

点击 `GENERATE CODE`。CubeMX 会生成一个带 `CMakeLists.txt` 的完整工程。


## 1.7 在 STM32 Profile 下打开工程，等它“自己识别”

用 VSCode 打开刚才生成的工程文件夹（`File → Open Folder`，选到 `blink_test` 这一层）。确认当前 Profile 仍然是 `STM32`。

打开之后，ST 扩展可能会在右下角弹出提示，问你要不要配置这个 CMake 工程，或者直接开始后台 Configure。**让它跑。** 这是 ST 扩展在替你做“Kit 选择 + CMake Configure + 生成调试配置”这一整套事情。

**怎么判断成功了？** 重启一次 VSCode，打开这个工程，看左侧活动栏：

- 如果出现了 **CMake 图标**，点进去，在 **Build Analyzer** 里能看到东西（比如 target、源文件列表、或者构建产物信息），说明工程已经被 CMake Tools 正确识别和配置了。
- 如果 CMake 侧边栏没出现，或者里面空空如也，通常说明工程还没被识别为 CMake 工程，或者 Kit 还没选好。

**到这一步，你不需要手动去改 `launch.json` 的路径，也不需要写 `tasks.json` 的烧录命令。** ST 扩展在识别工程之后会自动生成一套能用的调试配置，指向它自己下载的 GDB Server 和工具链。这些路径由 ST 扩展管理，升级 CubeCLT 或扩展时它会自己调整——这正是“让扩展自己管”比“手写绝对路径”省心的地方。


## 1.8 编译和调试：确认闭环

在 CMake 侧边栏或底部状态栏点击 **Build**（锤子图标），或者在命令面板执行 `CMake: Build`。编译成功后，`build/` 目录下会出现 `.elf` 文件。

按 `Ctrl+Shift+D` 打开“运行和调试”，顶部下拉菜单里应该已经有一个 ST 扩展生成的调试配置（名字可能叫 `Debug` 或 `STM32 Debug`）。选中它，按 `F5`。

如果一切正常，程序会下载进芯片、停在 `main`，左侧出现变量和外设寄存器面板。**如果外设寄存器面板是空的，但断点和单步正常**，说明 SVD 文件没有关联上——这不影响你这一节验证工具链，后面的章节再处理。

到这里，**“注册账户 → 装工具 → 建 Profile → 装扩展 → 生成工程 → 编译 → 调试”** 的闭环就走完了。


## 1.9 常见问题

> 注意这个 QA 肯定覆盖不全面，所以遇到什么问题找不到的我建议你去问大肥鱼，最好是使用 VSCode 内的 deepseek 插件，然后让她帮你解决，因为环境问题总是折磨人的。
> 如果你看不懂在干什么，我建议你问大肥鱼，别给自己环境折腾坏了。

**Q1：ST 账户注册完了，但验证邮件死活收不到。**

先翻垃圾邮件——ST 的验证邮件被误判的概率不低。如果垃圾箱里也没有，检查你填的邮箱是不是某些国内免费邮箱（尤其是带企业域名后缀的那种），部分邮件链路上会静默丢信。换 Gmail、Outlook 或学校邮箱重发一次。如果还不行，**挂上代理再点一次“重新发送验证邮件”**。不是 ST 不发，是链路上某一段超时了。

另外注意：注册页面上那个“注册成功”的提示**不等于账户激活**。真正激活的是邮件里那个链接。没点过链接之前，你的账户在 CubeMX 登录时会被拒绝。


**Q2：CubeMX 登录时“Check Connection”一直失败，但我用浏览器能正常打开 ST 官网。**

这是**代理模式**的问题，不是你网络坏了。CubeMX 是基于 Java 的，它的网络栈对“系统代理”模式经常识别不到。换 **TUN 模式**，让代理在操作系统层面接管流量，CubeMX 想绕都绕不过去。


**Q3：CubeMX 登录时，浏览器弹出一个卡死的窗口，或者登录页白屏。**

**换浏览器。** Edge、Chrome、Firefox 都行，别用国产浏览器。CubeMX 登录走的是 OAuth 跳转，会调用系统默认浏览器；某些国产浏览器会以“兼容模式”开一个内置 Chromium 窗口去接这个回调，表现就是加载慢、回调地址识别错误、或者直接白屏。

改默认浏览器的方法：Windows 设置 → 应用 → 默认应用 → Web 浏览器。改完之后**关掉 CubeMX 重新打开**，让它重新调一次默认浏览器。


**Q4：在默认 Profile 里装了 ST 扩展，后来才建 STM32 Profile，结果切过去扩展不见了。**

顺序反了。**先建 Profile、先激活、再装扩展**。扩展是装在 Profile 里的，你装的时候在哪个 Profile，它就在哪个 Profile。

补救办法：切回你装扩展的那个 Profile，把 ST 扩展卸载；切到 STM32 Profile，重新装一次。卸载重装比“迁移扩展”省事，扩展装一次也就几分钟。


**Q5：ST 扩展装完了，Bundles Manager 一直在下载，右下角转了半天。**

这很正常，它在下 GCC、CMake、Ninja、GDB Server，加起来几百 MB。**挂 TUN 代理让它下完**，别中途关 VSCode。如果网络不稳、断了，重新打开 VSCode，Bundles Manager 通常能续传。

判断“下完了”的信号：**右下角不再持续弹窗或转圈**。安静下来之后，可以关掉 VSCode，进入下一步。


**Q6：打开 CubeMX 生成的工程，CMake 侧边栏一直不出现，或者出现了但里面是空的。**

按顺序排查三件事：

1. **当前打开的是不是工程根目录**。`File → Open Folder` 要选到 `blink_test` 那一层，不是它的父目录。CMake Tools 只在“打开的文件夹根目录有 `CMakeLists.txt`”时才认这个工程。
2. **当前 Profile 是不是 STM32**。切错了 Profile，ST 扩展和 CMake Tools 都不在，当然没反应。
3. **重启一次 VSCode**。CMake Tools 偶尔需要一次重启才会刷新对工程的识别。这是它的小毛病，不算 bug，但确实烦。

三件事都确认过还不行，打开命令面板执行 `CMake: Configure`，看 Output 面板里报什么错。错误信息通常很直白，比如“找不到编译器”就是 Kit 没选对。

**环境配置很恼人，还不明白去问大肥鱼**


**Q7：编译报错，说 `arm-none-eabi-gcc` 找不到。**

大概率是 **Kit 没选对**，或者 ST 扩展的 Bundles Manager 还没把工具链下完。先确认 Q5 的下载跑完了，然后在命令面板执行 `CMake: Select a Kit`，选名字里带 STM32CubeCLT 或 arm-none-eabi 的那一项。

如果你看不懂在干什么，我建议你问大肥鱼，别给自己环境折腾坏了。

如果 Kit 列表里根本没有这一项，说明 ST 扩展没找到 CubeCLT。确认 CubeCLT 是**默认路径**安装的（`C:\ST\STM32CubeCLT_*`），没有挪到别的盘。ST 扩展对默认路径有识别逻辑，改路径会让自动发现多绕弯。


**Q8：F5 之后卡在 “Launching”，或者报 `Error: Failed to launch GDB Server`。**

按顺序查：

1. **ST-LINK 驱动装了没有**。回到 1.3 节，确认 STSW-LINK009 装完并重启过。
2. **芯片是不是被锁死了**。之前如果跑过程序把 SWD 引脚复用成了普通 GPIO，调试接口就被关了。用 `STM32_Programmer_CLI` 执行一次全片擦除，再重新连接。
3. **USB 线是不是只能充电的那种**。这个听起来很蠢，但是真有人是这样的，请换一根确认能传数据的线。


**Q9：手动改了 `launch.json` 的路径，后来 ST 扩展更新，路径全失效了。**

这是“手写绝对路径”的副作用。正确的姿势是**让 ST 扩展生成的配置作为基础**，只在确实需要调字段（比如芯片型号写错、补 SVD 路径）的时候做最小修改，路径交给扩展管理。

如果已经改乱了，最简单的办法是删掉 `.vscode/launch.json`，让 ST 扩展重新生成一份。重新导入工程也行（ST 扩展图标 → `Import CMake Project`）。


**Q10：我电脑上本来就有 CMake 和 Ninja，装 CubeCLT 会不会打架？**

**默认安装就不会。** CubeCLT 自带的 CMake 和 Ninja 只在 STM32 工程里通过 Kit 或扩展被引用，你系统里原来那份 CMake 仍然是你终端里的 `cmake`。两套各干各的。


## 1.10 本节你做了什么

1. 在 ST **英文官网** 注册了 myST 账户，通过邮件链接完成激活，知道了登录和激活阶段对代理的需求。
2. 以管理员身份、**默认路径**安装了 CubeCLT 和 CubeMX，不碰 PATH，不手动改安装位置。
3. 手动安装了 **STSW-LINK009** ST-LINK USB 驱动，并重启电脑。
4. 在 VSCode 里创建并**激活**了一个名为 `STM32` 的独立 Profile，实现了扩展和设置的隔离。
5. 在 STM32 Profile 下只安装了 **STM32Cube for Visual Studio Code** 一个扩展，让它自动拉齐 C/C++、CMake Tools、Cortex-Debug 等依赖，并由 Bundles Manager 在后台下载工具链。在 `STM32` Profile 下补装了 **Python 扩展**，为后续写上位机做准备
6. 用 CubeMX 生成了一个 Toolchain 为 **CMake** 的 STM32G474CEx 工程。
7. 在 VSCode 的 STM32 Profile 下打开工程，等待 ST 扩展和 CMake 自动识别；**以 CMake 侧边栏的 Build Analyzer 是否有内容**作为“识别成功”的判据。
8. 完成了 Build → Debug → 停在 `main` 的闭环，不需要手写 `launch.json` 的绝对路径。

## 1.11 一些私货

![VSCode Interface](/img/4.1.11-1.png)

这是我现在的 VSCode 界面，我觉得这很美观，据我的非常不科学研究表明，以下论断纯属我个人臆断，一个美观的界面有助于提高写代码或者写作的愉悦性，进而让你做起来更加享受。

如果你也认为这个界面还是比较美观的，下面这套配置文件可供你尝试。

安装 `Catppuccin for VSCode` 扩展，它提供了四套优雅的配色方案。我选用的主题是 Catppuccin Latte，这是一个浅色主题，我认为在环境光充足的情况下，浅色主题让我更不容易视疲劳。

我使用的字体为 `Maple Mono NF CN` 这是个优秀的字体，我尤其喜欢她的斜体，而且她的中文字符美观协调等宽，我推荐这个字体。

我使用的字号为 16 号，这个无所谓，大了小了看你自己。

然而，默认的界面配色有些过于冷了，所以我对其进行了配置。以下是配置文件，你可以按需取用放到 `settings.json` 里。

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