# 第1节：WSL2

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| Linux | 一种操作系统内核，和Windows是两套不同的东西 |
| 发行版 | 在Linux内核基础上打包了各种工具软件的完整系统，比如Ubuntu |
| Ubuntu | 最流行的Linux发行版之一，本书所有命令都在Ubuntu 24.04上验证 |
| WSL2 | Windows Subsystem for Linux 2，Windows里跑Linux的一个方式 |
| 虚拟机 | 用软件模拟出一台完整的电脑，里面可以装另一个操作系统 |
| Hyper-V | Windows自带的虚拟化技术，WSL2和虚拟机都依赖它 |
| 镜像网络 | WSL2的一种网络模式，让WSL的IP和Windows保持一致 |
| DNS隧道 | 让WSL通过Windows去解析域名，解决某些网络环境下DNS不通的问题 |

---

## 1.1 为什么需要Linux

如果你的大学比较不错，不使用老掉牙的vc6.0作为IDE，那么你在大学里学的CS课程——操作系统、计算机网络、编译原理、并行计算——它们的实验代码，几乎都默认在Linux环境下运行。

我大致可以给出如下原因：

- 绝大多数服务器跑的是Linux。你写的代码最终要部署到服务器上，在本地用Linux开发可以避免“在我电脑上能跑”这种尴尬。
- 超算中心跑的是Linux。如果你以后做科研、跑仿真、调模型，你得用SSH连上去，在命令行里操作。
- 开源工具链首选Linux。这些工具在Linux上的体验最顺畅，文档最多，遇到问题最容易找到答案。

你当然可以在Windows上用虚拟机跑Linux，也可以在macOS上折腾。但WSL2是目前在Windows上获得Linux环境门槛最低、体验最顺的方式。它不需要你单独分一个分区，不需要你在虚拟机里忍受卡顿，不需要你每次开机等Linux慢慢启动。

WSL2不是虚拟机，但也离不开虚拟化技术。下面解释它们的关系。

---

## 1.2 安装WSL2

以下步骤在Windows 11上执行。

**第一步：开启必要的Windows功能**

以管理员身份打开PowerShell或Windows Terminal，执行：

```powershell
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
```

第一条命令开启WSL功能。第二条命令开启虚拟机平台。WSL2需要虚拟化技术支持才能运行完整的Linux内核。

执行完后**重启电脑**。

**第二步：将WSL2设为默认版本**

重启后，以管理员身份执行：

```powershell
wsl --set-default-version 2
```

这之后安装的所有Linux发行版都会默认使用WSL2。如果要在WSL1和WSL2之间切换，可以用`wsl --set-version <发行版名> 1`或`2`。

**第三步：安装Ubuntu 24.04**

在终端执行：

```powershell
wsl --install -d Ubuntu-24.04
```

安装完成后，从开始菜单启动Ubuntu 24.04，或者在终端执行`wsl -d Ubuntu-24.04`。

**首次启动**：会提示你创建用户名和密码。注意：
- 用户名不要用`root`。`root`是系统管理员账户，日常操作用自己的用户名。
- 输入密码时不显示任何字符（星号也没有）。这是正常的，照着敲就行。

**验证安装**：

```bash
wsl -l -v
```

输出应该类似：

```
  NAME                   STATE           VERSION
* Ubuntu-24.04           Running         2
```

星号`*`表示默认发行版。当你只输入`wsl`不加参数时，进入的就是带星号的这个。

---

## 1.3 WSL2与虚拟机的区别

很多人会问：WSL2不就是虚拟机吗？

答案是：它用了虚拟机的技术（Hyper-V），但体验不像虚拟机。

**虚拟机（比如VirtualBox、VMware）做了什么**：

用软件模拟出一整套硬件——CPU、内存、硬盘、网卡、显卡、声卡、USB控制器——然后在上面装一个完整的操作系统。这个操作系统不直接接触你电脑的真实硬件，它看到的都是虚拟机软件模拟出来的东西。

结果就是：启动慢、吃内存、图形性能差、文件共享麻烦。当然隔离性好，但对你日常开发来说，隔离性换来的代价太大了。

**WSL2做了什么**：

WSL2也跑在一个轻量级虚拟机上（基于Hyper-V），但这个虚拟机有几个特殊之处：

- **没有完整硬件模拟**：它只虚拟了运行Linux内核必需的硬件，其他设备（显卡、声卡、USB）直接走Windows的驱动。
- **内核是特制的**：微软自己维护了一份Linux内核，针对WSL场景做了大量优化，尤其是文件系统和网络部分。
- **和Windows无缝集成**：`\\wsl.localhost`里直接看到WSL的文件系统，Windows进程和WSL进程可以互相通信。

**什么时候用虚拟机**：你需要完整的图形桌面环境（比如必须用Linux桌面）、需要直接访问USB设备、需要跑多个独立隔离的环境。

**什么时候用WSL2**：你需要Linux命令行、需要在Linux环境下编译代码。

---

## 1.4 配置文件：.wslconfig

WSL2默认的网络配置在有些场景下不太好用。最常见的问题：

- WSL的IP地址和Windows不在同一个网段，想从局域网其他设备访问WSL里跑的服务（比如Jupyter、HTTP Server）需要额外配置端口转发。
- DNS解析偶尔会失败，表现为WSL里能ping通IP但ping不通域名。
- WSL2长时间运行后会占用大量内存（因为Linux会把空闲内存用作缓存，不会主动释放）。

这些问题可以通过一个配置文件解决。

在`%UserProfile%`目录下（也就是`C:\Users\你的用户名\`）新建一个文件，名叫`.wslconfig`。注意文件名以点开头，没有扩展名。

写入以下内容：

```ini
[wsl2]
networkingMode=mirrored
dnsTunneling=true
firewall=true
autoProxy=true
autoMemoryReclaim=gradual
```

**每一项的作用**：

`networkingMode=mirrored`
：镜像网络模式。开启后，WSL的IP地址和Windows完全一样，所有网络接口被“镜像”到WSL内部。好处是：从局域网其他设备可以直接访问WSL里跑的服务，不需要配端口转发。WSL里的应用绑定的端口会自动在Windows的同一个端口上监听。

`dnsTunneling=true`
：DNS隧道。WSL内部的DNS请求被转发给Windows，由Windows去解析域名。绕过WSL直接发DNS包可能遇到的各种网络问题（比如某些校园网劫持DNS）。打开这个之后，WSL里执行`apt update`、`git clone`、`curl`等操作时，域名解析会更稳定。

`firewall=true`
：让Windows防火墙对WSL的流量生效。默认情况下WSL的流量绕过Windows防火墙，这在某些企业或校园网络环境下可能被网络策略拦截。打开后WSL的出入流量受Windows防火墙规则管理。

`autoProxy=true`
：自动继承Windows的系统代理设置。如果你在Windows里开了代理工具（比如Clash、v2ray），WSL里的命令行工具（curl、apt、git等）会自动使用这个代理，不需要在WSL里单独配置。

`autoMemoryReclaim=gradual`
：自动内存回收，模式为渐进式。WSL2空闲时，后台会逐步释放Linux内核占用的内存还给Windows。默认是`disabled`。打开后可以避免WSL越用越占内存最后需要手动`wsl --shutdown`才能释放的问题。`gradual`表示温和地回收，避免对正在运行的程序造成性能冲击。还有一个值是`dropcache`，更激进，但可能导致首次访问文件时变慢。

**如何生效**：修改`.wslconfig`后，需要完全关闭WSL才能让新配置生效。在PowerShell中执行：

```powershell
wsl --shutdown
```

然后重新启动WSL（打开Ubuntu终端或执行`wsl`）。配置就会加载。

**验证**：重启后执行`ip addr`，可以看到网络接口的配置和之前不一样了。具体差异取决于你的网络环境，不用太纠结。

---

## 1.5 本节验证动作

完成以下检查，确认WSL2环境就绪：

1. **确认版本**：`wsl -l -v`显示Ubuntu 24.04的VERSION是2。
2. **确认能联网**：在WSL终端里执行`ping -c 4 www.baidu.com`，应该能收到回复。
3. **确认文件系统正常**：执行`ls /`，应该能看到`bin`、`home`、`usr`等目录。执行`ls /mnt/c/`，应该能看到你的Windows C盘内容。

如果第2步ping不通，先检查`.wslconfig`里`dnsTunneling=true`和`autoProxy=true`是否写对，然后执行`wsl --shutdown`重试。

---

## 1.6 可能遇到的问题

**安装WSL时提示“需要更新WSL内核”**
：去微软官方搜索“WSL2内核更新包”，下载安装后重试。

**启动Ubuntu时卡在“Installing...”很久**
：网络问题。关掉代理工具试试，或者换个时间段。

**执行wsl --shutdown后Ubuntu启动变慢**
：第一次启动会重建一些缓存，正常。再启动一次就好了。

**配置文件不生效**
：确认文件名是`.wslconfig`（不是`.wslconfig.txt`）。确认文件放在`%UserProfile%`目录下（`C:\Users\你的用户名\`），不是在WSL里面。修改后记得`wsl --shutdown`。

