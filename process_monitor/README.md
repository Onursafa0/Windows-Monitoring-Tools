
# ⚙️ Process Activity Monitor

A simple C++ tool for monitoring and logging **active processes** on a Windows system.  
It uses the Windows Tool Help API to take process snapshots and record the process IDs along with their detection.

---

## ✨ Features

- Continuously monitors running processes in **real time**.
- Logs process ID and a detection message into `process_log.txt`.
- Uses the Windows Tool Help API for process enumeration.
- Lightweight and easy to use.

---

## ⚙ How It Works

1. Creates a snapshot of all running processes.
2. Iterates through the snapshot and logs each process ID with `"Detected"`.
3. Appends results to `process_log.txt`.
4. Repeats every 1 second.

---

## 📄 Example Log Output
```bash
Process ID: 4321, Action: Detected
Process ID: 9132, Action: Detected
Process ID: 2500, Action: Detected
```

## 🛠 Build Instructions
### Using MinGW
```bash
g++ process_monitor.cpp -o process_monitor.exe -static
```

### Using MSVC
```bash
cl process_monitor.cpp
```

## ▶ Usage
```bash
process_monitor.exe
```

## ⚠ Notes
- Logs all running processes every second — log file can grow quickly.

- Administrator privileges may be required for some processes.

- The "`Detected`" action is static but can be modified for more detailed events.
