#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>

// Logger class: Used to log registry activities
class Logger {
public:
    // Function to log registry activities
    void LogRegistryActivity(const std::string& registryPath, const std::string& action) {
        // Open registry_log.txt in append mode
        std::ofstream logFile("registry_log.txt", std::ios::app);
        if (logFile.is_open()) {
            // Write registry path and performed action to the log file
            logFile << "Registry Path: " << registryPath << ", Action: " << action << std::endl;
            logFile.close();  // Close the file
        } else {
            std::cerr << "Failed to open registry activity log file." << std::endl;
        }
    }
};

// MonitorRegistry function: Used to monitor registry changes
void MonitorRegistry(Logger& logger) {
    HKEY hKey;
    // Open the "Software" key under HKEY_CURRENT_USER
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software", 0, KEY_NOTIFY, &hKey) != ERROR_SUCCESS) {
        std::cerr << "Failed to open registry key for monitoring." << std::endl;
        return;
    }

    auto lastLogTime = std::chrono::steady_clock::now();  // Last log time
    const auto minLogInterval = std::chrono::seconds(5);  // Minimum log interval of 5 seconds

    while (true) {
        // Set notification to wait for registry changes
        if (RegNotifyChangeKeyValue(hKey, TRUE, REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_ATTRIBUTES |
            REG_NOTIFY_CHANGE_LAST_SET | REG_NOTIFY_CHANGE_SECURITY, NULL, FALSE) != ERROR_SUCCESS) {
            std::cerr << "Failed to set registry change notification." << std::endl;
            break;
        }

        auto currentTime = std::chrono::steady_clock::now();  // Get current time
        // Check minimum log interval
        if (currentTime - lastLogTime >= minLogInterval) {
            // Log activity when a change is detected
            logger.LogRegistryActivity("HKEY_CURRENT_USER\\Software", "Change");
            lastLogTime = currentTime;  // Update last log time
        }

        // Wait for a short time to prevent high CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    RegCloseKey(hKey);  // Close registry key
}

int main() {
    Logger logger;
    MonitorRegistry(logger);  // Start monitoring registry
    return 0;
}
