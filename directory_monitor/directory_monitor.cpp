#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>

class Logger {
public:
    // Used to log file activity.
    void LogFileActivity(const std::string& filePath, const std::string& action) {
        // Open "file_log.txt" in append mode.
        std::ofstream logFile("file_log.txt", std::ios::app);
        if (logFile.is_open()) {
            // Write the file path and performed action to the log file.
            logFile << "File Path: " << filePath << ", Action: " << action << std::endl;
            logFile.close();
        } else {
            // If the log file cannot be opened, display an error message.
            std::cerr << "Failed to open log file for file activity." << std::endl;
        }
    }
};

// Monitors the given directory and logs file activities.
void MonitorDirectory(const std::string& directoryPath, Logger& logger) {
    // Create a handle for the directory.
    // Convert std::string to std::wstring
    int wchars_num = MultiByteToWideChar(CP_UTF8, 0, directoryPath.c_str(), -1, NULL, 0);
    std::wstring wDirectoryPath(wchars_num, 0);
    MultiByteToWideChar(CP_UTF8, 0, directoryPath.c_str(), -1, &wDirectoryPath[0], wchars_num);

    HANDLE hDir = CreateFileW(
        wDirectoryPath.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        // If the handle cannot be created, display an error message.
        std::cerr << "Failed to open directory for monitoring." << std::endl;
        return;
    }

    // Create a large buffer for monitoring.
    char buffer[4096];
    DWORD bytesReturned;
    BOOL result;

    // Continuously read directory changes in a loop.
    while (true) {
        result = ReadDirectoryChangesW(
            hDir,
            &buffer,
            sizeof(buffer),
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES |
            FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_LAST_ACCESS |
            FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY,
            &bytesReturned,
            NULL,
            NULL
        );

        if (!result) {
            // If ReadDirectoryChangesW fails, show the error code and break the loop.
            std::cerr << "ReadDirectoryChangesW error: " << GetLastError() << std::endl;
            break;
        }

        if (bytesReturned == 0) {
            // If the number of bytes read is zero, display an error message and break.
            std::cerr << "ReadDirectoryChangesW returned zero bytes." << std::endl;
            break;
        }

        // Store the read information in the FILE_NOTIFY_INFORMATION structure.
        FILE_NOTIFY_INFORMATION* fni = (FILE_NOTIFY_INFORMATION*)buffer;
        do {
            // Retrieve the file name by converting from Unicode.
            std::wstring fileName(fni->FileName, fni->FileNameLength / sizeof(WCHAR));
            std::string fileNameStr(fileName.begin(), fileName.end());
            std::string action;

            // Determine the action text based on the file action.
            switch (fni->Action) {
                case FILE_ACTION_ADDED:
                    action = "Created";
                    break;
                case FILE_ACTION_REMOVED:
                    action = "Deleted";
                    break;
                case FILE_ACTION_MODIFIED:
                    action = "Modified";
                    break;
                case FILE_ACTION_RENAMED_OLD_NAME:
                    action = "Renamed (old name)";
                    break;
                case FILE_ACTION_RENAMED_NEW_NAME:
                    action = "Renamed (new name)";
                    break;
                default:
                    action = "Unknown";
                    break;
            }

            // Use the Logger class to log the file activity.
            logger.LogFileActivity(directoryPath + "\\" + fileNameStr, action);

            // If the offset to the next entry is zero, break the loop.
            if (fni->NextEntryOffset == 0)
                break;

            // Move to the next FILE_NOTIFY_INFORMATION structure.
            fni = (FILE_NOTIFY_INFORMATION*)((LPBYTE)fni + fni->NextEntryOffset);

        } while (true);
    }

    // Close the directory handle.
    CloseHandle(hDir);
}

// Main function, takes the directory to monitor as a parameter.
int main(int argc, char* argv[]) {
    // Check the number of arguments.
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <directory_to_monitor>" << std::endl;
        return 1;
    }

    // Get the directory to monitor.
    std::string directoryToMonitor = argv[1];
    // Create an instance of the Logger class.
    Logger logger;
    // Start monitoring the directory and logging.
    MonitorDirectory(directoryToMonitor, logger);

    // Display a message indicating the program has finished.
    std::cout << "Program finished. Press any key to continue..." << std::endl;
    // Wait for a key press before closing.
    std::cin.get();
    return 0;
}
