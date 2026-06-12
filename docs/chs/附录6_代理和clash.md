# 附录六：代理

> 本附录解释代理、VPN等概念的区别，以及常见代理工具的使用方法。不涉及任何具体服务商推荐。

## 6.1 几个概念的关系

**代理（Proxy）**：一种网络中转服务。你的设备把请求发给代理服务器，服务器代为访问目标网站，再将结果返回给你。代理可以绕过网络限制，但通常只针对特定应用（如浏览器）。

**VPN（虚拟专用网络）**：在设备和目标服务器之间建立一条加密隧道，所有网络流量（包括系统后台、UDP等）都经过隧道转发。VPN 工作在网络层，影响整个系统。

**梯子**：对翻墙工具的俗称，包括代理和 VPN 服务。

**机场**：对商业代理服务提供商的俗称，即“提供梯子的地方”。用户购买套餐后获得订阅链接，导入客户端即可使用。

简单理解：代理和 VPN 是技术实现方式，梯子和机场是圈内俗称。

## 6.2 Clash 及其版本

Clash 是一个基于规则的代理内核，支持多种协议（HTTP、SOCKS5、Shadowsocks、VMess 等）。它本身没有图形界面，需要搭配 GUI 前端使用。

主要版本：

| 名称 | 说明 |
|------|------|
| **Clash（原版）** | 最早的内核，已停止维护。 |
| **Clash Premium** | 原版作者开发的闭源版本，增加了 TUN 模式等特性，也已停更。 |
| **Clash Meta（mihomo）** | 开源社区接手的活跃分支，功能更完善，目前最常用。后文单独介绍。 |
| **Clash Verge Rev** | 跨平台 GUI 前端（Windows/macOS/Linux），基于 Clash Meta 内核，界面简洁，适合日常使用。 |
| **Clash for Windows（CFW）** | 曾是最流行的 Windows GUI，已停止维护，不再推荐。 |
| **ClashX / ClashX Pro** | macOS 上的 GUI 前端。 |
| **ClashMetaForAndroid（CMFA）** | 安卓端 GUI，基于 mihomo 内核。 |

对于普通用户，直接用 **Clash Verge**（桌面）和 **ClashMetaForAndroid**（安卓）即可。

## 6.3 YAML 配置文件

Clash 的核心是配置文件（通常为 `config.yaml`），用 YAML 格式书写。从机场获取的订阅链接本质上就是这个文件。

配置文件的基本结构：

```yaml
# 代理节点列表
proxies:
  - name: "香港节点"
    type: ss
    server: example.com
    port: 12345
    cipher: aes-256-gcm
    password: "password"
  - name: "日本节点"
    type: vmess
    server: example2.com
    port: 443
    uuid: "xxxx-xxxx"
    alterId: 0
    cipher: auto

# 代理组（策略组）
proxy-groups:
  - name: "PROXY"
    type: select          # 手动选择
    proxies:
      - "香港节点"
      - "日本节点"
  - name: "自动选择"
    type: url-test        # 自动测速选最快
    url: "http://www.gstatic.com/generate_204"
    interval: 300
    proxies:
      - "香港节点"
      - "日本节点"

# 规则
rules:
  - "DOMAIN-SUFFIX,google.com,PROXY"
  - "DOMAIN-KEYWORD,youtube,PROXY"
  - "IP-CIDR,192.168.0.0/16,DIRECT"
  - "GEOIP,CN,DIRECT"
  - "MATCH,PROXY"
```

**如何使用**：
1. 从机场获取订阅链接（通常是 `https://xxx.com/link/xxxx`）或直接下载 `config.yaml`。
2. 在 Clash Verge 中，点击“导入” → “从 URL 导入”，粘贴链接；或手动将配置文件放入配置目录。
3. 选择要使用的代理组（如 PROXY 选一个节点），开启“系统代理”或“TUN 模式”。

**几个关键字段**：
- `proxies`：节点列表，每个节点有类型、地址、端口、加密方式等。
- `proxy-groups`：策略组。`type: select` 表示手动选；`type: url-test` 自动选最快；`type: fallback` 按顺序切换。
- `rules`：匹配规则。从上到下匹配，第一个匹配的生效。`DOMAIN-SUFFIX` 匹配域名后缀，`IP-CIDR` 匹配 IP 段，`GEOIP` 匹配地理位置，`MATCH` 为兜底规则。

## 6.4 Shadowrocket（小火箭）

Shadowrocket 是 iOS 平台上的代理客户端，支持 SSR、VMess、Trojan 等协议，也支持订阅。

**使用方法**：
1. 在 App Store（需美区账号）购买，约 $2.99。
2. 打开 App，点击右上角“+” → 类型选择“Subscribe”（订阅）。
3. 粘贴机场提供的订阅链接，保存。
4. 回到首页，点击开关连接。首次使用会提示添加 VPN 配置，允许即可。
5. 可以通过“配置” → “规则”调整分流规则。

## 6.5 Mihomo

Mihomo（原 Clash Meta）是 Clash 开源社区维护的内核，也是目前所有活跃 Clash 客户端的内核基础。它修复了原版 Clash 的许多问题，增加了新协议（如 WireGuard、Hysteria）和新特性。

Mihomo 本身是命令行程序，无需单独使用。Clash Verge、ClashMetaForAndroid 等 GUI 都内置了 mihomo 内核。如果你需要手动运行，可以从 GitHub 下载对应平台的二进制文件，然后用命令行启动：

```bash
./mihomo -f config.yaml
```

## 6.6 系统代理模式与 TUN 模式

Clash 提供了两种让流量走代理的方式。

**系统代理模式**：
- 在系统网络设置中配置 HTTP/SOCKS5 代理地址（通常是 `127.0.0.1:7890`）。
- 只有“遵守系统代理”的程序才会走代理。大多数浏览器、部分应用程序会遵守；但游戏、命令行工具、UDP 流量（如 DNS、在线语音）通常不遵守。
- 优点：轻量，兼容性好。缺点：很多流量不经过代理。

**TUN 模式**：
- 创建一个虚拟网卡，操作系统将所有流量（包括 TCP、UDP、ICMP）都路由到这张网卡，由 Clash 处理后转发。
- 相当于全局代理，无需程序主动配合。游戏、Spotify 客户端、系统自带 DNS 都会走代理。
- 开启方法：在 Clash Verge 中进入“设置” → 安装 TUN 服务（需要管理员权限）→ 开启 TUN 模式开关。
- 缺点：比系统代理消耗稍多资源，可能影响局域网访问（需配置规则放行局域网流量）。

一般来说，只浏览网页用系统代理即可；如果需要玩游戏或使用不遵守系统代理的软件，再开启 TUN 模式。

## 6.7 安卓上的 Shizuku 模式

Shizuku 是一个安卓工具，允许普通应用以 ADB 或 root 权限执行系统级命令，无需 root 也能获得部分高权限。

在 **ClashMetaForAndroid（CMFA）** 中，开启 TUN 模式通常需要 root 权限或 Shizuku。Shizuku 模式可以让 CMFA 在非 root 手机上正常使用 TUN。

**使用方法**：
1. 安装 Shizuku App（从 GitHub 或酷安下载）。
2. 按照 Shizuku 提示激活（需要 ADB 或 root）：
   - 连接电脑执行 `adb shell sh /sdcard/Android/data/moe.shizuku.privileged.api/files/start.sh`
   - 或通过无线调试激活（Android 11+）。
3. 打开 CMFA → 设置 → TUN 设置 → 选择“Shizuku 模式”。
4. 授予权限后即可正常使用 TUN 模式。

如果手机已经 root，可以直接选择“Root 模式”，无需 Shizuku。

---

# 补充阅读

> 以下内容涉及代理协议和底层机制，难度较高。建议先快速扫一眼，知道有这些概念即可。后续遇到具体问题时，再用关键词搜索回来查阅。

## 补充1：常见代理协议

- **HTTP 代理**：只代理 HTTP 流量，明文传输，不加密。目前已很少单独使用。
- **SOCKS5 代理**：代理 TCP 和 UDP 流量，不解析内容，可配合其他加密协议。Clash 的混合端口（Mixed Port）同时提供 HTTP 和 SOCKS5。
- **Shadowsocks（SS）**：加密 SOCKS5 代理，轻量、混淆能力弱，已被部分运营商识别。
- **ShadowsocksR（SSR）**：SS 的衍生版，增加混淆插件，现已逐渐淘汰。
- **VMess**：V2Ray 的核心协议，加密能力强，支持动态端口。
- **Trojan**：模拟 HTTPS 流量，伪装性好。
- **Hysteria**：基于 QUIC（UDP）的协议，主打暴力加速，适合丢包率高的网络。
- **WireGuard**：新一代 VPN 协议，内核级实现，Mihomo 也已支持。

## 补充2：Clash 规则类型详解

- `DOMAIN,`：精确匹配完整域名。
- `DOMAIN-SUFFIX,`：匹配域名后缀，如 `google.com` 匹配 `www.google.com`、`mail.google.com`。
- `DOMAIN-KEYWORD,`：匹配域名包含的关键词。
- `IP-CIDR,`：匹配 IPv4 地址段。
- `IP-CIDR6,`：匹配 IPv6 地址段。
- `GEOIP,`：基于 MaxMind 数据库判断 IP 归属国家。
- `PROCESS-NAME,`（仅 TUN 模式）：按进程名匹配。
- `SRC-IP-CIDR,`：按来源 IP 匹配（用于局域网共享代理）。
- `MATCH,`：兜底规则，必须放在最后。

## 补充3：TUN 模式的虚拟网卡原理

TUN 模式在操作系统内创建一个虚拟三层网络接口（如 `tun0`）。Clash 内核从该接口读取原始 IP 数据包，解析目标地址后根据规则决定转发给代理还是直连。对于需要走代理的请求，Clash 将原始数据包封装成代理协议（如 Shadowsocks）发送给远端服务器，远端服务器拆包后访问目标，再将响应封装返回。TUN 模式需要对数据包进行完整解包和重封包，因此 CPU 消耗略高于系统代理。

## 补充4：IPv6 与代理

许多机场节点不支持 IPv6。如果你的设备有公网 IPv6 地址，访问 IPv6 网站时，如果规则没有正确配置，可能导致直连（能访问）或走代理（可能因节点不支持而失败）。建议在配置文件中设置 `ipv6: false` 或规则中明确处理 IPv6 地址段（`IP-CIDR6`）。

## 补充5：DNS 分流

Clash 支持 DNS 分流，即根据域名使用不同 DNS 服务器。常见配置：
- 国内域名用国内 DNS（如 114.114.114.114），返回国内 CDN 地址。
- 被墙域名用代理节点自带的 DNS（或可信 DNS，如 1.1.1.1），避免 DNS 污染。
- `fallback-filter` 可以检测 DNS 结果是否为国外 IP，进一步防止污染。

配置文件示例：
```yaml
dns:
  enable: true
  listen: 0.0.0.0:53
  default-nameserver:
    - 114.114.114.114
  nameserver:
    - https://dns.alidns.com/dns-query
  fallback:
    - https://cloudflare-dns.com/dns-query
  fallback-filter:
    geoip: true
    geoip-code: CN
```