# 📂 Directory Activity Monitor

A simple C++ program for **real-time monitoring of file and directory changes** on Windows.  
It uses the Windows API function `ReadDirectoryChangesW` to detect file creation, deletion, modification, and renaming events, and logs them to a text file.

---

## ✨ Features

- Monitors a specified directory **and all its subdirectories**.
- Detects:
  - File creation
  - File deletion
  - File modification
  - File renaming (old and new names)
- Logs activities to `file_log.txt` in a human-readable format.

---

## ⚙ How It Works

1. Uses `CreateFileW` to open a handle to the target directory.
2. Uses `ReadDirectoryChangesW` to listen for file system events.
3. Converts event data to readable file paths and actions.
4. Writes activity details into `file_log.txt`.

---

## 📄 Example Log Output
```bash
File Path: C:\TestFolder\example.txt, Action: Created
File Path: C:\TestFolder\example.txt, Action: Modified
File Path: C:\TestFolder\example.txt, Action: Deleted
```

## 🛠 Build Instructions
### Requirements:

- Windows OS

- C++17 or later

- MinGW or Visual Studio (MSVC) compiler

### Compile with MinGW (g++)

```bash
g++ -std=c++17 -o directory_monitor directory_monitor.cpp -luser32
```

### Compile with Visual Studio (MSVC)

```bash
cl /EHsc directory_monitor.cpp
```

## ▶ Usage

```bash
directory_monitor.exe <directory_to_monitor>
```

### Example

```bash
directory_monitor.exe "C:\Users\YourName\Documents"
```

## ⚠ Notes
- The program runs until manually stopped `(Ctrl + C)`.

- Ensure the directory path is accessible with the required permissions.

- `file_log.txt` will be created in the same directory as the executable.