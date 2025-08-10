
# 📄 Windows Monitoring Tools (C++)

A collection of **four standalone C++ Windows monitoring utilities**, each focusing on a different aspect of system activity.  
These tools are lightweight, use native Windows APIs, and log detected events to text files.

---

## 📦 Included Tools

| Tool | Description | Monitors | Log File |
|------|-------------|----------|----------|
| [📂 Directory Activity Monitor](directory_monitor/) | Tracks file & folder changes in real time | File creation, deletion, modification, renaming | `file_log.txt` |
| [🌐 TCP Network Connection Monitor](network_monitor/) | Lists active TCP connections | Local/remote IP, ports, connection state | `network_log.txt` |
| [⚙️ Process Activity Monitor](process_monitor/) | Lists running processes periodically | Process IDs | `process_log.txt` |
| [🔑 Registry Activity Monitor](registry_monitor/) | Watches registry changes | Key changes under `HKEY_CURRENT_USER\Software` | `registry_log.txt` |

---

## 🛠 Build Requirements
- Windows OS
- C++17 or later
- **Compiler:** MinGW or MSVC
- **Libraries:**
  - `user32` (for Directory Monitor)
  - `ws2_32` & `iphlpapi` (for Network Monitor)
  - `Advapi32` (for Registry Monitor)

---

## 📄 Build Instructions
Each tool has its own `README.md` inside its folder with specific build commands.  

**Example using MinGW:**
```bash
g++ -std=c++17 -o directory_monitor directory_monitor.cpp -luser32
```
## ▶ Usage
Each application runs as a standalone executable.
Run from the command line:

**Directory Monitor**
```bash
directory_monitor.exe "C:\Path\To\Monitor"
```

**Network Monitor**
```bash
network_monitor.exe
```

**Process Monitor**
```bash
process_monitor.exe
```

**Registry Monitor**
```bash
registry_monitor.exe
```

## ⚠ Notes
- All tools run until manually stopped (usually `Ctrl + C`).

- Logs are saved in the same folder as the executable.

- Administrator rights may be required for some monitoring tasks.
