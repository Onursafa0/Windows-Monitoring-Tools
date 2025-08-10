
# 🔑 Registry Activity Monitor

A Windows C++ program that monitors changes in the registry key:  
`HKEY_CURRENT_USER\Software` and logs detected changes to a text file.

---

## ✨ Features

- Monitors the `HKEY_CURRENT_USER\Software` key.
- Logs:
  - Key name changes
  - Attribute changes
  - Last set time updates
  - Security changes
- Writes entries with timestamps to `registry_log.txt`.
- Throttles logging to avoid flooding (minimum 5-second interval).

---

## ⚙ How It Works

1. Uses `RegNotifyChangeKeyValue` to watch for changes.
2. On change detection, logs the registry path and `"Change"` message.
3. Enforces a 5-second cooldown between logs.
4. Runs until manually stopped.

---

## 🛠 Build Instructions

### Using MSVC
```bash
cl /EHsc registry_monitor.cpp
```

### Using MinGW
```bash
g++ -o registry_monitor.exe registry_monitor.cpp -lAdvapi32
```

## ▶ Usage
```bash
registry_monitor.exe
```

- Logs are saved to `registry_log.txt` in the executable's directory.

- Stop with `Ctrl + C` in the console.


## ⚠ Notes
- Runs indefinitely until manually stopped.

- Ensure proper permissions for registry access and file writing.