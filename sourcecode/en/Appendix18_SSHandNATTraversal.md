# Appendix 18: SSH and NAT Traversal (Intranet Penetration)

> This appendix explains what SSH is, how to use it, and how to access your computer from outside when it does not have a public IP address (often called "intranet penetration" or "NAT traversal"). It does not go deep into theory.  
> The main goal is to prevent you from being completely lost when you need to connect to your university's computing lab to run simulations.

## 18.1 What Is SSH?

**SSH (Secure Shell)** is an encrypted network protocol used to securely log into another computer remotely.

**Why SSH is needed**: In the past, people used telnet for remote login. However, telnet sends all data (including passwords) in plain text. Anyone who can capture network traffic in the middle can steal your password. SSH encrypts the entire communication – anyone in the middle sees only gibberish.

**What SSH can do**:
- Log into a remote server (replacing telnet)
- Securely transfer files (SFTP, SCP)
- Forward tunnels (push traffic from other, less secure protocols through an encrypted SSH channel)
- Run remote commands (`ssh user@host "ls -la"`)

**When you might use SSH at university**:
- Connecting to your department's Linux server to do assignments or run experiments
- Connecting to a university computing cluster (supercomputing centre)
- Connecting to a Raspberry Pi, NAS, or other device on your lab network
- Connecting from your dormitory to a lab computer (often combined with NAT traversal)

## 18.2 How SSH Differs from Other Protocols

| Protocol | Encryption | Typical use |
|----------|------------|--------------|
| **Telnet** | Plain text | Old‑style remote login (no longer recommended) |
| **SSH** | Encrypted | Remote command line, file transfer |
| **RDP** | Encrypted | Windows Remote Desktop (graphical interface) |
| **VNC** | Optional | Remote desktop – not encrypted by default |
| **VPN** | Encrypted | Whole network traffic goes through a tunnel |
| **HTTP** | Plain text | Websites |
| **HTTPS** | Encrypted | Websites (encrypted version) |

**Quick summary**: For command‑line remote access, use SSH. For a graphical Windows desktop, use RDP. To tunnel all network traffic from your whole computer, use a VPN.

## 18.3 Windows Built‑in OpenSSH

Windows 10 and 11 come with an OpenSSH client built in. You do not need to install third‑party software like PuTTY.

**Check if it is installed**: Settings → System → Optional features → search for "OpenSSH Client". If it is not there, click "Add a feature".

**Basic usage** (in CMD, PowerShell, or Windows Terminal):

```bash
ssh username@server_address
# For example: ssh root@192.168.1.100
# For example: ssh ubuntu@example.com -p 2222
```

The first time you connect, you will be prompted to confirm the server's fingerprint (its public key). Type `yes` and press Enter – after that, you will not be asked again.

**Common parameters**:
- `-p` to specify a port (default is 22)
- `-i` to specify a private key file (used for passwordless login)

**To exit an SSH session**: Type `exit` or press `Ctrl + D`.

## 18.4 Passwordless Login (SSH Keys)

Typing a password every time is annoying, and automation scripts cannot easily handle interactive password entry. You can set up passwordless login.

**Generate a key pair on your local computer**:

```bash
ssh-keygen -t ed25519
```

Press Enter all the way through (it is a good idea to set a password to protect your private key). This creates two files in the `~/.ssh/` directory:
- `id_ed25519`: your private key (never share this)
- `id_ed25519.pub`: your public key (this goes on the server)

**Upload the public key to the server**:

```bash
ssh-copy-id username@server_address
# For example: ssh-copy-id root@192.168.1.100
```

After entering your password once, subsequent connections will not require a password.

**Why ed25519?** It is faster, more secure, and uses shorter keys than RSA. Some older systems may not recognise it – in that case, use `-t rsa -b 4096`.

## 18.5 Aliases and the SSH Config File

If you connect to several servers, typing `ssh ubuntu@123.456.789.0 -p 2222 -i /path/to/key` every time is tedious. You can set up aliases in `~/.ssh/config`.

```bash
# Edit the config file
notepad ~/.ssh/config
```

Example content:

```
Host myvps
    HostName 123.456.789.0
    User ubuntu
    Port 2222
    IdentityFile ~/.ssh/id_ed25519

Host jumpserver
    HostName jump.example.com
    User admin
```

After that, you only need to type `ssh myvps` to connect.

## 18.6 NAT Traversal (Intranet Penetration): What If You Have No Public IP?

Most home broadband connections do not have a public IPv4 address. **University lab and dormitory networks** are typically the same – your computer does not have a standalone public IP, so people from outside cannot connect directly to you.

**What is NAT traversal?** NAT traversal (often called "intranet penetration") refers to techniques that allow external networks to access devices on an internal network. The principle is that your computer actively connects to a server that does have a public IP (a relay). Outside users then reach your computer through that relay server.

**University scenarios**:
- You are in your dorm and want to connect back to your dorm computer from outside (home, the lab, your internship company) to retrieve files.
- You are in the lab and want to connect from your dorm to the lab server to run code.
- You have created a simple webpage and want to temporarily show it to classmates, but you do not want to buy a server.

**FRP** is currently the most popular self‑hosted tunnelling tool. It is open source, free, written in Go, and supports TCP, UDP, HTTP, and HTTPS. However, you need your own server with a public IP (a VPS) to run the FRP server component.

If you do not want to buy your own server, you can use public services. Two recommendations:

- **Openfrp**: Has a free tier (speed‑limited to 12 Mbps, 2 tunnels). Supports TCP/UDP/TLS/HTTPS. Suitable for game hosting, remote desktop, temporary website testing.
- **cpolar**: Feature‑rich and well‑suited for many scenarios. Supports HTTP/HTTPS/TCP. The free tier provides a domain name that changes daily – good for temporary debugging.

Other public services (Ngrok, Natapp, SAKURA FRP, etc.) are also available – they work out of the box without requiring your own server.

> **Note**: Most domestic public services require real‑name verification. This is a compliance requirement.

## 18.7 University Lab SSH Jump Servers

Many university labs or supercomputing centres do not allow direct public SSH logins. Instead, you must first log into a **jump server (bastion host)** , and from there log into the internal lab server.

Typical workflow:
1. `ssh studentID@gateway.sjtu.edu.cn` (jump server)
2. On the jump server, `ssh 192.168.1.100` (internal lab server)

If you find two steps annoying, you can configure `ProxyJump` in your `~/.ssh/config`:

```
Host jumpserver
    HostName gateway.sjtu.edu.cn
    User studentID

Host labserver
    HostName 192.168.1.100
    User ubuntu
    ProxyJump jumpserver
```

After that, typing `ssh labserver` logs you directly into the lab server in one step.

## 18.8 Summary

| Scenario | Recommended solution |
|----------|----------------------|
| Remote server management | SSH key‑based passwordless login + config aliases |
| Access between computers on the same local network | Direct SSH using the internal IP address |
| Access a device without a public IP from outside | Openfrp / cpolar / self‑hosted FRP |
| Windows Remote Desktop (graphical) | Forward RDP through an SSH tunnel, or directly forward port 3389 using a NAT traversal tool |
| University jump server | Use `ProxyJump` in your SSH config |

SSH is capable of far more – port forwarding, proxy jumping, and other advanced features. However, for daily use, knowing `ssh user@host`, setting up passwordless login, and understanding how to do NAT traversal is already enough.