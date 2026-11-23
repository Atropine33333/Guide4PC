# 第1节：WSL2

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 内核 | 操作系统的核心，负责管理硬件、进程、内存、文件系统 |
| Linux内核 | Linux操作系统的内核，由林纳斯·托瓦兹在1991年发起 |
| 发行版 | 在内核基础上打包各种软件形成的完整系统，比如Ubuntu |
| Ubuntu 26.04 | Ubuntu的最新长期支持版本，代号Resolute |
| WSL2 | Windows Subsystem for Linux 2，Windows里跑Linux的方式 |
| 虚拟机平台 | Windows里的虚拟化功能，WSL2依赖它运行 |
| Hyper-V | Windows专业版/教育版自带的完整虚拟化平台 |
| apt | Ubuntu的包管理器，负责安装、更新、卸载软件 |
| 软件源 | apt下载软件包的服务器地址，国内常用阿里云镜像加速 |

---

## 1.1 为什么需要Linux

你在大学里学的课程——操作系统、计算机网络、编译原理、并行计算——它们的实验代码，几乎都默认在Linux环境下运行。

原因大致有三条：

- 服务器跑的是Linux。你写的代码最终要部署到服务器上，在本地用Linux开发可以避免环境不一致导致的问题。
- 超算中心跑的是Linux。做科研、跑仿真、调模型，你得用SSH连上去，在命令行里操作。
- 开源工具链首选Linux。gcc、clang、meson、git、python这些工具在Linux上的体验最顺畅，文档最多，遇到问题最容易找到答案。

WSL2是目前在Windows上获得Linux环境门槛最低、体验最顺的方式。它不需要单独分区，不需要忍受虚拟机卡顿，不需要每次等Linux慢慢启动。

---

## 1.2 什么是Linux内核

严格来说，“Linux”这个词指的不是一个完整的操作系统，而是操作系统最底层的那个部分——**内核**。

内核是操作系统的心脏。它管理着：

- **CPU**：决定哪个程序在什么时候运行、运行多久。
- **内存**：给每个程序分配地址空间，防止它们互相踩踏。
- **文件系统**：管理硬盘上的数据怎么存、怎么读、怎么索引。
- **设备驱动**：让键盘、鼠标、网卡、显卡这些硬件能被程序使用。
- **网络**：管理网络数据的收发、连接、路由。

你打开Ubuntu终端输入命令，那个叫Bash的程序并不是内核。它跑在用户空间，通过内核提供的系统调用来完成读写文件、创建进程、收发网络数据等操作。

1991年，林纳斯·托瓦兹在赫尔辛基大学读书时发布了Linux内核的第一个版本。当时他21岁。三十多年后，Linux内核成了世界上最大的开源项目，全世界超过90%的服务器在跑它，Android手机在跑它，绝大多数超级计算机在跑它。

你不需要现在就懂内核的实现，但需要知道一件事：**WSL2里跑的就是这个内核**，只不过微软自己维护了一个专门适配Windows虚拟化环境的版本。

---

## 1.3 开启Windows功能

在Windows 11上跑WSL2，需要开启两个Windows功能：**虚拟机平台**和**适用于Linux的Windows子系统**。

打开“启用或关闭Windows功能”：
- 按`Win + R`，输入`optionalfeatures`，回车
- 或者直接在开始菜单搜索“启用或关闭Windows功能”

在弹出的窗口中，勾选以下两项：
- **虚拟机平台**（Virtual Machine Platform）
- **适用于Linux的Windows子系统**（Windows Subsystem for Linux）

**关于Hyper-V**：如果你用的是Windows 11专业版或教育版，在同一个窗口里会看到“Hyper-V”这个选项。勾上它不会有什么坏处，WSL2不依赖它但也不冲突。如果你用的是家庭版，压根没有这个选项，不用管。这个功能开不开都不影响WSL2的正常使用。

确认后，Windows会开始安装这些功能，完成后提示重启。**必须重启**，否则后续操作会失败。

---

## 1.4 安装PowerShell 7

Windows自带的PowerShell版本是5.1（在Windows终端里叫“Windows PowerShell”）。我们需要安装PowerShell 7，它在WSL集成方面体验更好。

用**管理员身份**打开Windows终端，然后打开一个**Windows PowerShell**选项卡（就是系统自带那个），执行：

```powershell
winget install --id Microsoft.PowerShell --source winget
```

安装完成后，关掉Windows终端再重新打开，新建选项卡时应该能看到“PowerShell 7”的选项（图标通常是深蓝色的）。之后的命令都在**PowerShell 7**选项卡里执行，不要用“Windows PowerShell”那个选项卡。

---

## 1.5 安装Ubuntu 26.04

在PowerShell 7选项卡中执行：

```powershell
wsl --install -d Ubuntu-26.04
```

等待下载和安装完成。Ubuntu 26.04是2026年4月发布的长期支持版本，代号Resolute。本书所有命令和操作都在这个版本上验证。

安装完成后，从开始菜单启动Ubuntu 26.04，或者在PowerShell中执行`wsl -d Ubuntu-26.04`。

首次启动会提示创建用户名和密码：
- 用户名不要用`root`。root是系统管理员账户，日常操作使用自己的用户名。
- 输入密码时屏幕上没有任何显示（星号也没有），这是正常行为，照着敲就行。

---

## 1.6 配置WSL网络

WSL的网络有一些默认行为在实际使用中不太友好：IP地址频繁变动、DNS解析偶尔失败、内存不释放。推荐使用一个叫**WSL Settings**的图形化工具来配置。

安装WSL Settings：
- 在Microsoft Store中搜索“WSL Settings”并安装
- 或者访问其GitHub仓库获取最新版本

打开WSL Settings，在**网络（Network）** 栏中做以下设置：

- **网络模式**：设置为`mirrored`（镜像模式）。WSL的IP会和Windows保持一致，从局域网其他设备访问WSL里跑的服务不需要额外配端口转发。
- **启用主机地址环回**（Host Address Loopback）：开启。允许WSL访问Windows本地监听的端口。
- **启用自动代理**：开启。WSL里的命令行工具（curl、apt、git等）会自动使用Windows的代理设置。
- **启用DNS隧道**：开启。WSL的DNS请求被转发给Windows解析，避免校园网或运营商劫持DNS导致解析失败。
- **启用尽力DNS解析**：开启。DNS解析失败时自动尝试其他方式，提高稳定性。

在**可选功能（Experimental）** 中找到**内存回收**，设置为`gradual`（渐进式）。WSL空闲时会温和地释放占用的内存还给Windows，避免越用越卡。

配置完成后，**重启Windows终端**（不是重启电脑，关掉终端窗口再打开就行）。重启之后，新建终端时应该能看到一个名叫“Ubuntu-26.04”的可选发行版。

---

## 1.7 更换软件源并更新系统

Ubuntu默认的软件源服务器在海外，国内访问速度慢。需要换成国内镜像。

### 1.7.1 备份原有配置

在Ubuntu终端中执行：

```bash
sudo cp /etc/apt/sources.list.d/ubuntu.sources /etc/apt/sources.list.d/ubuntu.sources.bak
```

这条命令把原始软件源配置文件复制一份作为备份。如果后面改出问题，可以执行下面的命令恢复：

```bash
sudo cp /etc/apt/sources.list.d/ubuntu.sources.bak /etc/apt/sources.list.d/ubuntu.sources
```

### 1.7.2 编辑软件源配置

Ubuntu 26.04的软件源配置文件路径是`/etc/apt/sources.list.d/ubuntu.sources`。用vim打开：

```bash
sudo vim /etc/apt/sources.list.d/ubuntu.sources
```

如果你不熟悉vim，可以用nano代替：

```bash
sudo nano /etc/apt/sources.list.d/ubuntu.sources
```

用vim的话，操作步骤如下：
1. 按方向键下，将光标移动到注释区域之外
2. 输入`13dd`，删除原有的13行配置
3. 按`o`键，在下方新建一行，进入插入模式
4. 将下面内容复制进去（右键粘贴，或者`Shift+Insert`）：

```
Types: deb
URIs: https://mirrors.aliyun.com/ubuntu/
Suites: resolute resolute-updates resolute-backports
Components: main restricted universe multiverse
Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg

Types: deb
URIs: https://mirrors.aliyun.com/ubuntu/
Suites: resolute-security
Components: main restricted universe multiverse
Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg
```

5. 按`Esc`退出插入模式
6. 输入`:w`保存
7. 输入`:q`退出

使用nano的话更直观：粘贴内容后按`Ctrl+O`保存，再按`Ctrl+X`退出。

### 1.7.3 更新软件包列表

换源完成后，执行：

```bash
sudo apt update
```

这会从阿里云镜像拉取最新的软件包列表。接下来执行：

```bash
sudo apt full-upgrade -y
```

`full-upgrade`会更新所有已安装的软件包到最新版本，包括内核和系统组件。`-y`表示自动确认所有更新提示，不需要手动按`Y`。

第一次更新可能需要几分钟到十几分钟，取决于网络速度。

---

## 1.8 这一节你做了什么

停下来回顾一下。这一节完成了以下工作：

1. 在Windows功能中开启了虚拟机平台和适用于Linux的Windows子系统
2. 安装了PowerShell 7，并确认在Windows终端中使用PowerShell 7选项卡
3. 安装了Ubuntu 26.04
4. 用WSL Settings配置了网络模式、DNS、代理和内存回收
5. 把Ubuntu的软件源换成了阿里云镜像
6. 更新了所有系统软件包

现在你的WSL2 Ubuntu已经是一个网络通畅、源已换好、系统最新的开发环境了。下一步，我们会安装编译器、构建工具和Python环境，把开发工具链补齐。