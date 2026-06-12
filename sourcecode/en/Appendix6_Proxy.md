# Appendix 6: Proxy

> This appendix explains the differences between proxy, VPN, and related concepts, as well as how to use common proxy tools. No specific service providers are recommended.

## 6.1 Relationship Between Concepts

**Proxy**: An intermediary network service. Your device sends requests to a proxy server, which accesses the target website on your behalf and returns the result. Proxies can bypass network restrictions, but they usually only work for specific applications (e.g., browsers).

**VPN (Virtual Private Network)** : Creates an encrypted tunnel between your device and the target server. All network traffic (including system background traffic, UDP, etc.) goes through the tunnel. VPNs operate at the network layer and affect the entire system.

**Ladder**: A colloquial term for circumvention tools, including both proxies and VPN services.

**Airport**: Slang for a commercial proxy service provider – literally “the place where you get a ladder”. Users purchase a plan and receive a subscription link, which they import into a client.

To put it simply: proxy and VPN are technical implementations, while “ladder” and “airport” are community slang.

## 6.2 Clash and Its Variants

Clash is a rule‑based proxy core that supports multiple protocols (HTTP, SOCKS5, Shadowsocks, VMess, etc.). It has no graphical interface on its own and needs a GUI front‑end.

Main variants:

| Name | Description |
|------|-------------|
| **Clash (original)** | The earliest core, now unmaintained. |
| **Clash Premium** | A closed‑source version by the original author, added features like TUN mode, also discontinued. |
| **Clash Meta (mihomo)** | An active fork maintained by the open‑source community, with more features and the most commonly used today. It is described separately later. |
| **Clash Verge Rev** | A cross‑platform GUI front‑end (Windows/macOS/Linux) based on the Clash Meta core. Clean interface, suitable for daily use. |
| **Clash for Windows (CFW)** | Once the most popular Windows GUI, now discontinued and no longer recommended. |
| **ClashX / ClashX Pro** | GUI front‑ends for macOS. |
| **ClashMetaForAndroid (CMFA)** | Android GUI based on the mihomo core. |

For ordinary users, simply use **Clash Verge** (desktop) and **ClashMetaForAndroid** (Android).

## 6.3 YAML Configuration Files

The core of Clash is a configuration file (usually `config.yaml`) written in YAML format. The subscription link you get from an airport is essentially this file.

Basic structure of a configuration file:

```yaml
# Proxy server list
proxies:
  - name: "Hong Kong Node"
    type: ss
    server: example.com
    port: 12345
    cipher: aes-256-gcm
    password: "password"
  - name: "Japan Node"
    type: vmess
    server: example2.com
    port: 443
    uuid: "xxxx-xxxx"
    alterId: 0
    cipher: auto

# Proxy groups (policy groups)
proxy-groups:
  - name: "PROXY"
    type: select          # manual selection
    proxies:
      - "Hong Kong Node"
      - "Japan Node"
  - name: "Auto Select"
    type: url-test        # automatic speed test, picks fastest
    url: "http://www.gstatic.com/generate_204"
    interval: 300
    proxies:
      - "Hong Kong Node"
      - "Japan Node"

# Rules
rules:
  - "DOMAIN-SUFFIX,google.com,PROXY"
  - "DOMAIN-KEYWORD,youtube,PROXY"
  - "IP-CIDR,192.168.0.0/16,DIRECT"
  - "GEOIP,CN,DIRECT"
  - "MATCH,PROXY"
```

**How to use**:
1. Obtain a subscription link (usually `https://xxx.com/link/xxxx`) or download a `config.yaml` file directly from your airport.
2. In Clash Verge, click “Import” → “Import from URL” and paste the link; or manually place the config file into the config directory.
3. Select a proxy group (e.g., choose a node in “PROXY”), then enable “System Proxy” or “TUN Mode”.

**Key fields**:
- `proxies`: List of nodes, each with type, address, port, encryption, etc.
- `proxy-groups`: Policy groups. `type: select` means manual selection; `type: url-test` automatically picks the fastest; `type: fallback` switches in order.
- `rules`: Matching rules. They are evaluated from top to bottom; the first match applies. `DOMAIN-SUFFIX` matches domain suffixes, `IP-CIDR` matches IP ranges, `GEOIP` matches geographic location, and `MATCH` is the catch‑all rule.

## 6.4 Shadowrocket

Shadowrocket is a proxy client for iOS, supporting protocols such as SSR, VMess, Trojan, and also subscriptions.

**How to use**:
1. Purchase from the App Store (requires a US region account), about $2.99.
2. Open the app, tap the “+” in the top‑right corner → choose “Subscribe”.
3. Paste the subscription link provided by your airport and save.
4. Go back to the home screen and tap the switch to connect. The first time you do this, you will be prompted to add a VPN configuration – allow it.
5. You can adjust routing rules via “Configuration” → “Rules”.

## 6.5 Mihomo

Mihomo (formerly Clash Meta) is the core maintained by the Clash open‑source community and is the foundation of all active Clash clients today. It fixes many issues from the original Clash and adds new protocols (such as WireGuard, Hysteria) and new features.

Mihomo itself is a command‑line program; you do not need to run it separately. GUIs like Clash Verge and ClashMetaForAndroid all bundle the mihomo core. If you need to run it manually, download the binary for your platform from GitHub and start it with:

```bash
./mihomo -f config.yaml
```

## 6.6 System Proxy Mode vs. TUN Mode

Clash provides two ways to route traffic through the proxy.

**System Proxy Mode**:
- Configures an HTTP/SOCKS5 proxy address in your system network settings (usually `127.0.0.1:7890`).
- Only programs that “respect the system proxy” will go through the proxy. Most browsers and some applications do; but games, command‑line tools, and UDP traffic (e.g., DNS, voice chat) usually do not.
- Pros: Lightweight, good compatibility. Cons: Many types of traffic bypass the proxy.

**TUN Mode**:
- Creates a virtual network interface. The operating system routes all traffic (TCP, UDP, ICMP) to this interface, where Clash processes it and forwards accordingly.
- Essentially a global proxy that works without any program having to cooperate. Games, Spotify clients, and system DNS will all go through the proxy.
- How to enable: In Clash Verge, go to “Settings” → install the TUN service (requires administrator privileges) → turn on the TUN Mode switch.
- Cons: Slightly higher resource usage than system proxy; may affect local network access (you may need to configure rules to allow LAN traffic).

In general, use system proxy if you only browse the web. If you play games or use software that does not respect the system proxy, enable TUN Mode.

## 6.7 Shizuku Mode on Android

Shizuku is an Android tool that allows ordinary apps to execute system‑level commands via ADB or root privileges, granting high permissions without root.

In **ClashMetaForAndroid (CMFA)** , enabling TUN Mode normally requires root or Shizuku. Shizuku mode lets CMFA use TUN on non‑rooted phones.

**How to use**:
1. Install the Shizuku app (from GitHub or CoolAPK).
2. Activate Shizuku as instructed (needs ADB or root):
   - Connect to a computer and run `adb shell sh /sdcard/Android/data/moe.shizuku.privileged.api/files/start.sh`
   - Or activate via wireless debugging (Android 11+).
3. Open CMFA → Settings → TUN Settings → choose “Shizuku Mode”.
4. After granting permissions, TUN Mode will work normally.

If your phone is already rooted, you can directly select “Root Mode” and skip Shizuku.

---

# Extended Reading

> The following sections cover proxy protocols and underlying mechanisms. They are more advanced. It is recommended to skim them first – just be aware these concepts exist. When you run into a specific problem later, you can search again using the keywords you learned here.

## Extended 1: Common Proxy Protocols

- **HTTP Proxy**: Only proxies HTTP traffic, transmits in plain text without encryption. Rarely used alone today.
- **SOCKS5 Proxy**: Proxies TCP and UDP traffic, does not inspect content, can be used with other encryption protocols. Clash’s mixed port offers both HTTP and SOCKS5.
- **Shadowsocks (SS)** : Encrypted SOCKS5 proxy, lightweight, weak obfuscation; already recognized by some ISPs.
- **ShadowsocksR (SSR)** : A derivative of SS with added obfuscation plugins, now gradually obsolete.
- **VMess**: The core protocol of V2Ray, strong encryption, supports dynamic ports.
- **Trojan**: Mimics HTTPS traffic, good camouflage.
- **Hysteria**: A QUIC (UDP)‑based protocol designed for aggressive speed boosting, works well on high‑packet‑loss networks.
- **WireGuard**: A next‑generation VPN protocol with kernel‑level implementation, now also supported by Mihomo.

## Extended 2: Detailed Clash Rule Types

- `DOMAIN,`: Exact full domain name match.
- `DOMAIN-SUFFIX,`: Matches domain suffix, e.g., `google.com` matches `www.google.com` and `mail.google.com`.
- `DOMAIN-KEYWORD,`: Matches a keyword appearing in the domain name.
- `IP-CIDR,`: Matches IPv4 address ranges.
- `IP-CIDR6,`: Matches IPv6 address ranges.
- `GEOIP,`: Uses the MaxMind database to determine the country of an IP address.
- `PROCESS-NAME,` (TUN mode only): Matches by process name.
- `SRC-IP-CIDR,`: Matches by source IP address (used for sharing proxy on a LAN).
- `MATCH,`: Catch‑all rule; must be placed last.

## Extended 3: Virtual Network Interface Principle of TUN Mode

TUN mode creates a virtual layer‑3 network interface (e.g., `tun0`) inside the operating system. The Clash core reads raw IP packets from this interface, parses the destination address, and decides – based on rules – whether to forward the request through a proxy or directly. For requests that need to go through a proxy, Clash encapsulates the original packet into a proxy protocol (e.g., Shadowsocks) and sends it to the remote server. The remote server decapsulates it, accesses the target, and sends back the response, which is then re‑encapsulated and returned. Because TUN mode requires full decapsulation and re‑encapsulation of packets, its CPU consumption is slightly higher than system proxy mode.

## Extended 4: IPv6 and Proxies

Many airport nodes do not support IPv6. If your device has a public IPv6 address and you visit an IPv6 website, improper rule configuration may either cause the traffic to go direct (accessible) or go through a proxy (which might fail if the node doesn’t support IPv6). It is recommended to set `ipv6: false` in your configuration file, or explicitly handle IPv6 ranges with `IP-CIDR6` rules.

## Extended 5: DNS Split Routing

Clash supports split DNS routing, meaning it uses different DNS servers depending on the domain name. A typical configuration:
- Domestic domains go to a domestic DNS (e.g., 114.114.114.114), returning domestic CDN addresses.
- Blocked domains go to the proxy node’s own DNS (or a trusted DNS such as 1.1.1.1) to avoid DNS poisoning.
- `fallback-filter` can detect whether a DNS result points to a foreign IP, providing further protection against poisoning.

Example configuration snippet:
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