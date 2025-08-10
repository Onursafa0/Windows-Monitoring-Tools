#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <tlhelp32.h>  // Required for process snapshot and entry functions

// Logger class: Used to log process activities
class Logger {
public:
    // Function to log process activities
    void LogProcessActivity(DWORD processId, const std::string& action) {
        // Open process_log.txt in append mode
        std::ofstream logFile("process_log.txt", std::ios::app);
        if (logFile.is_open()) {
            // Write process ID and performed action to the log file
            logFile << "Process ID: " << processId << ", Action: " << action << std::endl;
            logFile.close();  // Close the file
        } else {
            std::cerr << "Failed to open process activity log file." << std::endl;
        }
    }
};

// MonitorProcesses function: Used to monitor running processes
void MonitorProcesses(Logger& logger) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot." << std::endl;
        return;
    }

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(entry);

    // Retrieve the first process
    if (Process32First(hSnapshot, &entry)) {
        do {
            // Log process activity via Logger
            logger.LogProcessActivity(entry.th32ProcessID, "Detected");
        } while (Process32Next(hSnapshot, &entry));  // Retrieve the next process
    }

    CloseHandle(hSnapshot);  // Close snapshot
}

int main() {
    Logger logger;  // Create an instance of the Logger class

    while (true) {
        MonitorProcesses(logger);  // Continuously monitor processes
        Sleep(1000);  // Wait for 1 second before checking again
    }

    return 0;
}
