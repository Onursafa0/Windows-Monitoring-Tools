# 🌐 TCP Network Connection Monitor

A Windows C++ console application that retrieves and logs the **current active TCP connections** on the system.  
It uses the Windows IP Helper API (`GetTcpTable2`) to gather connection details such as local and remote addresses, ports, and TCP states, and writes them into `network_log.txt`.

---

## ✨ Features

- Retrieves **all active TCP connections**.
- Displays:
  - Local address and port
  - Remote address and port
  - Connection state (e.g., LISTEN, ESTABLISHED, TIME_WAIT)
- Appends results to a log file without overwriting previous entries.
- Works on Windows Vista and later (`_WIN32_WINNT >= 0x0600`).

---

## ⚙ How It Works

1. **Initialize Winsock** – Prepares Winsock (good practice even if no sockets are created).
2. **Call `GetTcpTable2`** – Retrieves TCP connection data.
3. **Log Connections** – Saves addresses, ports, and states to `network_log.txt`.
4. **Clean Up** – Frees memory and shuts down Winsock.

---

## 📄 Example Log Output
```bash
Active TCP Connections:
Local Address:Port  Remote Address:Port  State
192.168.1.10:51234  93.184.216.34:443    ESTABLISHED
192.168.1.10:51235  93.184.216.34:80     TIME_WAIT
0.0.0.0:135         0.0.0.0:0            LISTEN
```

## 🛠 Build Instructions
### Using MSVC

```bash
cl /EHsc /W4 /D_WIN32_WINNT=0x0600 network_monitor.cpp /link ws2_32.lib iphlpapi.lib
```

### Using MinGW

```bash
g++ -std=c++17 -D_WIN32_WINNT=0x0600 network_monitor.cpp -lws2_32 -liphlpapi -o network_monitor.exe
```

## ▶ Usage

```bash
network_monitor.exe
```

The program will:

- Retrieve current TCP connections

- Append them to `network_log.txt`

- Wait for you to press Enter before exiting