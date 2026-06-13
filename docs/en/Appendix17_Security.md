# Appendix 17: Privacy and Security

> This appendix introduces the basic principles of network communication (HTTPS, TCP, TLS), password managers, two‑factor authentication (2FA), and some easily overlooked privacy habits. It does not go deep into technical details, but it will help you understand "how data travels through the network" and "how to prevent your accounts from being stolen."

## 17.1 How Data Travels Through the Network: TCP, TLS, HTTPS

When you open a website (for example, `https://www.baidu.com`), three things happen behind the scenes: a connection is established, an encrypted channel is created, and data is transmitted.

### 17.1.1 TCP: Establishing a Reliable Connection

**TCP (Transmission Control Protocol)** is responsible for delivering data reliably from one end to the other. It first breaks the data into small packets, numbers each packet, sends them out, and waits for an acknowledgement before sending the next. If a packet is lost, TCP resends it.

Think of it this way: TCP is like sending a registered letter – each letter has a tracking number, the recipient must sign for it, and if it is lost, you send another copy. The advantage is reliability; the downside is that it is slower because of all the back‑and‑forth acknowledgements.

**The difference from UDP**: UDP is like sending a postcard – you drop it in the mailbox and forget about it, with no confirmation of receipt. The advantage is speed; the downside is that packets may be lost. Video calls and online games often use UDP (losing an occasional frame is tolerable, but high latency is not).

### 17.1.2 TLS: Locking the Data

**TLS (Transport Layer Security)** is the "S" in HTTPS. It adds an encryption layer on top of the TCP connection. The data you send is encrypted into ciphertext before it leaves your computer and is only decrypted when it reaches the server. Any intermediate node (router, ISP, Wi‑Fi hotspot) sees only gibberish and cannot read the content.

The TLS handshake works roughly like this:
1. Your computer and the server greet each other and agree on an encryption algorithm.
2. The server presents a digital certificate (to prove "I really am Baidu").
3. Both sides generate a shared session key (known only to the two of you).
4. All subsequent communication is encrypted with that key.

This is why it is relatively safe to log into your online banking or type passwords on public Wi‑Fi – the data is encrypted, so even if someone intercepts it, they cannot decrypt it.

### 17.1.3 HTTPS: HTTP + TLS

**HTTP (Hypertext Transfer Protocol)** is the protocol browsers and servers use to transmit web page content. However, HTTP transmits everything in plain text, meaning anyone can eavesdrop. **HTTPS** is HTTP with TLS encryption.

**How to tell if a website uses HTTPS**: There is a small padlock icon next to the address bar. Click on it to see the certificate information.

## 17.2 Password Management

### 17.2.1 Why You Need a Password Manager

Most people use the same password for every website. If just one of those websites has a database breach (which happens often), hackers will try that password on your email, WeChat, Alipay – a technique called **credential stuffing**.

A password manager helps you:
- Generate a strong, random password for each website (e.g., `xK9#mQ2$pL5@nR8`)
- Automatically fill in login forms (no need to remember or type)
- Sync your passwords across your phone and computer

### 17.2.2 Firefox Built‑in Password Manager

If you use Firefox, its built‑in password manager is sufficient for many people.

**Enable sync**: Register for a Firefox account, then turn on sync in the settings – your passwords, bookmarks, and history will be synced (encrypted) to your other devices (phone, another computer).

**View / manage passwords**: Settings → Privacy & Security → Saved Logins. Here you can view, delete, or export your passwords.

**Master Password**: It is strongly recommended to set a Master Password. By default, Firefox saves your passwords encrypted, but if you are logged into Windows, anyone who opens Firefox can see all your saved passwords. Setting a Master Password means you will need to enter it once per session when opening Firefox or viewing passwords. Path: Settings → Privacy & Security → Use a Master Password.

**Downside**: The Firefox password manager only works inside the Firefox browser. If you use other browsers or mobile apps, it cannot help you.

### 17.2.3 Cross‑Platform Password Managers

If you need something that works across browsers and apps, consider these:

- **Bitwarden**: Open source, free (premium is $10 per year). Supports Windows, macOS, Linux, iOS, Android, and has browser extensions for everything. You can also self‑host (not recommended for beginners). This is currently the safest choice for individual users.
- **KeePass**: Local storage only – the database file stays on your own machine. Very powerful, but the interface looks dated. You need to sync the database file manually (e.g., using cloud storage).
- **1Password**: Paid. You can read this [blog post](https://yishi.io/1password-complete-tutorial/) – it is very well written.

### 17.2.4 Things You Must Never Do

- ❌ Write your passwords on a sticky note and attach it to your monitor.
- ❌ Send password screenshots over WeChat or QQ.
- ❌ Use "remember password" in your browser without also setting a Master Password (anyone who sits down at your computer can see them).
- ❌ Use the same password for every website.

## 17.3 Two‑Factor Authentication (2FA)

### 17.3.1 What Is 2FA?

**2FA (Two‑Factor Authentication)** adds an extra layer of protection on top of your password. When you log in, besides entering your password, you also need to enter a dynamically generated 6‑digit verification code (usually changing every 30 seconds). Even if someone steals your password, they cannot log in without that code.

The most common form of 2FA is **TOTP (Time‑based One‑Time Password)** . The principle is: you and the server share a secret key, and you both generate a 6‑digit number based on the current time. Because time does not lie, both sides calculate the same number.

### 17.3.2 Microsoft Authenticator

Microsoft Authenticator is one of the better 2FA tools. It is free on mobile phones and supports cloud backup (requires a Microsoft account).

**Important: You must remove 2FA associations before switching phones**

This is a big trap many people fall into when they change phones.

Suppose you have used Microsoft Authenticator to set up 2FA for several accounts – Google, GitHub, Steam, your university email. You buy a new iPhone and use Apple's "quick transfer" to migrate your data. After the migration, Microsoft Authenticator is copied over, **but the 2FA secrets are not migrated**. TOTP secrets are stored in a special way that a simple "copy app data" does not include them.

To make matters worse, if your accounts are cross‑dependent – for example, you need a GitHub code to log into Google, and a Google code to log into GitHub – after switching phones you cannot log into either, creating a loop.

**The correct procedure**:
1. Before switching phones, log into every website that has 2FA enabled.
2. Go to the account security settings and **turn off 2FA** (or "remove this device").
3. Confirm that all accounts have been disassociated, then set up your new phone.
4. On the new phone, reinstall Microsoft Authenticator.
5. Re‑enable 2FA for each account one by one (scan the QR code or manually enter the secret key).

**What if you have already switched phones?** :
- Most websites provide **recovery codes** – when you first enabled 2FA, the site gave you a set of codes or words to write down or print. Use one of those recovery codes to log in, then re‑bind the 2FA.
- If you have lost the recovery codes as well, you must contact the website's customer support, prove your identity (ID card, photo holding your ID, etc.), and wait for them to remove 2FA for you. This process can take days or even weeks.
- Therefore, whenever you enable 2FA on any account, **store the recovery codes safely** (print them out, or save them somewhere other than that phone).

### 17.3.3 Which Accounts Are Worth Enabling 2FA On?

- **Email**: Your Gmail, Outlook, school email – because many sites send password reset links to your email. If your email is compromised, all your other accounts are at risk.
- **Apple ID / Google account / Microsoft account**: These are the keys to your phone.
- **Steam**: Your game library has real value, and recovering a stolen Steam account is a pain.
- **GitHub**: If you are a computer science student, your code repositories could be deleted or malicious code could be inserted – the consequences are severe.
- **Password manager**: Bitwarden, 1Password, etc. – if they support 2FA, enable it.

**Not recommended**: Small forum accounts that you almost never log into and would not care about if lost.

### 17.3.4 Backup Options for 2FA

Aside from Microsoft Authenticator, here are a few other choices:

- **2FAS**: Open source, supports iOS/Android, has cloud backup (via iCloud/Google Drive), clean interface. Recommended.
- **Aegis** (Android only): Open source, supports backup to a local file (encrypted and then stored in cloud storage), very powerful.

## 17.4 Some Easily Overlooked Privacy Habits

### 17.4.1 Do Not Let Your Browser Save Credit Card Information

Chrome/Edge will ask if you want to save your credit card information for auto‑fill later. It is convenient, but if someone opens your browser (or if your computer is remotely controlled), they can see your full card number, expiration date, and even the CVV (in some versions).

**Suggestion**: Do not save it. Typing 16 digits yourself is not that slow.

### 17.4.2 Public Wi‑Fi

Public Wi‑Fi in coffee shops, airports, and hotels can be malicious hotspots (someone sets up a fake hotspot named "Starbucks Wi-Fi"). Once you connect, they can capture your HTTP traffic.

**Protection measures**:
- Avoid typing passwords on public Wi‑Fi whenever possible.
- If you must use it, try to visit only HTTPS websites (the padlock icon will be green).
- The safest approach is to use your phone's hotspot or connect through your own proxy/VPN.

### 17.4.3 Screen Lock

Many people do not set a screen lock on their computer (in Windows, press `Win + L` to lock). In a dormitory, lab, or library, you walk away for a minute to get water, and someone else can sit down at your computer and read your WeChat, email, or thesis draft.

**Suggestions**:
- Windows: Settings → Accounts → Sign‑in options → add a PIN or password.
- Develop the habit: when you step away, press `Win + L`.

---

# Extended Reading

> The following sections cover HTTPS certificates, TLS handshake details, and some fundamental cryptographic concepts. They are more advanced. Come back when you need them.

## Extended 1: Digital Certificates and Certificate Authorities

The certificates used in HTTPS are issued by a **CA (Certificate Authority)** . Your operating system and browser come with a built‑in set of trusted CA root certificates (e.g., DigiCert, Let's Encrypt, GlobalSign). When you visit `https://www.baidu.com`, the server presents a certificate. The browser checks whether the certificate was issued by a trusted CA, whether the domain name on the certificate matches the domain you are visiting, and whether the certificate has expired. Only if all checks pass will the browser establish a TLS connection. This also explains why "downloading a root certificate from an unknown source" is dangerous – once you install a malicious CA certificate, an attacker can forge valid certificates for any website, and your browser will show it as "secure".

## Extended 2: TLS 1.2 vs. TLS 1.3

TLS 1.3 is faster by one round trip (RTT). TLS 1.2 requires two handshakes (about 2 RTT) before it can start transmitting data. TLS 1.3 streamlines the handshake (1 RTT, and even 0 RTT for session resumption). It is also more secure – it removes outdated cryptographic algorithms (such as RSA key exchange and CBC mode) and requires Forward Secrecy for key exchange. Most mainstream browsers and servers now support TLS 1.3.

## Extended 3: How Password Managers Encrypt Data

The password database stored by a password manager is encrypted (typically using AES‑256). The decryption key is derived from your master password. The master password is never stored on any server – services like Bitwarden and 1Password use a zero‑knowledge architecture: the server stores only the encrypted data blob; without your master password, even they cannot decrypt your vault. This is also why "forgetting your master password" is usually unrecoverable – there is no "forgot password" button.

## Extended 4: Time Synchronisation Issues with TOTP

TOTP relies on your device having an accurate clock. If your phone's time is off by more than 30 seconds from the standard time, the generated codes will be invalid. Enabling "auto‑set time" on your phone usually avoids this problem. If you find your codes are consistently rejected, go to your device settings and calibrate the time. Offline devices (e.g., hardware tokens) also have internal battery‑powered clocks that drift over the years and may need to be resynchronised.

## Extended 5: Storing Recovery Codes

The recovery codes (usually 8‑10 words or a string of numbers) that a website gives you when you enable 2FA are lifesavers. Storage recommendations:
- Print them out and keep them in your wallet (not recommended to store them on your computer desktop).
- Store them in your password manager (Bitwarden allows notes for 2FA recovery codes).
- Store them in an encrypted notes app (e.g., Standard Notes, Joplin).
- Do not store them in your phone's photo gallery (if your phone is lost, everything is lost).

## Extended 6: Phishing Sites and the HTTPS Misconception

**HTTPS ≠ the website is trustworthy**. Phishing sites can also obtain a free Let's Encrypt certificate and show a padlock icon in the address bar. Seeing a padlock only tells you that "communication with this website is encrypted", not that "this website is legitimate". How to tell: check the domain name spelling – is it `google.com` or `g00gle.com`? Make sure it is exactly the same domain you always use.