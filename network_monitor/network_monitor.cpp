#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iphlpapi.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

// The NetworkMonitor class is used to monitor TCP connections
class NetworkMonitor {
public:
    // This function is used to monitor TCP connections
    void MonitorTcpConnections() {
        // Initialize Winsock (good practice even if GetTcpTable2 doesn't strictly require sockets creation)
        WSADATA wsaData;
        int wsaErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsaErr != 0) {
            std::cerr << "WSAStartup failed: " << wsaErr << std::endl;
            return;
        }

        PMIB_TCPTABLE2 pTcpTable = nullptr;
        ULONG ulSize = 0;
        DWORD dwRetVal = 0;

        // First call to determine the required buffer size
        dwRetVal = GetTcpTable2(nullptr, &ulSize, TRUE);
        if (dwRetVal != ERROR_INSUFFICIENT_BUFFER) {
            // If it returns NO_ERROR with nullptr that's surprising; normal path is ERROR_INSUFFICIENT_BUFFER
            if (dwRetVal == NO_ERROR && ulSize > 0) {
                // continue
            } else {
                std::cerr << "GetTcpTable2 failed to get buffer size. Error: " << dwRetVal << std::endl;
                WSACleanup();
                return;
            }
        }

        // Allocate required memory
        pTcpTable = (MIB_TCPTABLE2*)malloc(ulSize);
        if (pTcpTable == nullptr) {
            std::cerr << "Error: Memory allocation failed." << std::endl;
            WSACleanup();
            return;
        }

        // Retrieve the table
        dwRetVal = GetTcpTable2(pTcpTable, &ulSize, TRUE);
        if (dwRetVal == NO_ERROR) {
            LogTcpConnections(pTcpTable);
        } else {
            std::cerr << "Error: Failed to retrieve TCP table. Error: " << dwRetVal << std::endl;
        }

        if (pTcpTable) {
            free(pTcpTable);
            pTcpTable = nullptr;
        }

        WSACleanup();
    }

private:
    // Function to log TCP connections
    void LogTcpConnections(PMIB_TCPTABLE2 pTcpTable) {
        std::ofstream logfile("network_log.txt", std::ios_base::app); // Append mode for existing file
        if (!logfile.is_open()) {
            std::cerr << "Unable to open network_log.txt for writing." << std::endl;
            return;
        }

        logfile << "Active TCP Connections:" << std::endl;
        logfile << std::left << std::setw(22) << "Local Address:Port" << std::setw(22) << "Remote Address:Port" << std::setw(12) << "State" << std::endl;

        for (DWORD i = 0; i < pTcpTable->dwNumEntries; i++) {
            MIB_TCPROW2& row = pTcpTable->table[i];

            std::string localIp = ConvertAddress(row.dwLocalAddr);
            std::string remoteIp = ConvertAddress(row.dwRemoteAddr);

            // Ports are stored in network byte order in the low-order 16 bits of the DWORD
            unsigned short localPort = ntohs((unsigned short)row.dwLocalPort);
            unsigned short remotePort = ntohs((unsigned short)row.dwRemotePort);

            logfile << std::left << std::setw(16) << (localIp + ":" + std::to_string(localPort))
                    << std::setw(20) << (remoteIp + ":" + std::to_string(remotePort))
                    << std::setw(12) << TcpStateToString(row.dwState)
                    << std::endl;
        }

        logfile << std::endl;
        logfile.close();
        std::cout << "TCP connections have been saved to network_log.txt." << std::endl;
    }

    // Function to convert a DWORD address to a string (IPv4)
    std::string ConvertAddress(DWORD addr) {
        // MIB_TCPROW2.dwLocalAddr is in network byte-order; inet_ntoa expects in_addr with S_addr in network byte order
        in_addr in;
        in.S_un.S_addr = addr;
        const char* ip = inet_ntoa(in);
        if (ip)
            return std::string(ip);
        else
            return std::string("0.0.0.0");
    }

    // Convert TCP state to readable string
    std::string TcpStateToString(DWORD state) {
        switch (state) {
            case MIB_TCP_STATE_CLOSED: return "CLOSED";
            case MIB_TCP_STATE_LISTEN: return "LISTEN";
            case MIB_TCP_STATE_SYN_SENT: return "SYN_SENT";
            case MIB_TCP_STATE_SYN_RCVD: return "SYN_RCVD";
            case MIB_TCP_STATE_ESTAB: return "ESTABLISHED";
            case MIB_TCP_STATE_FIN_WAIT1: return "FIN_WAIT1";
            case MIB_TCP_STATE_FIN_WAIT2: return "FIN_WAIT2";
            case MIB_TCP_STATE_CLOSE_WAIT: return "CLOSE_WAIT";
            case MIB_TCP_STATE_CLOSING: return "CLOSING";
            case MIB_TCP_STATE_LAST_ACK: return "LAST_ACK";
            case MIB_TCP_STATE_TIME_WAIT: return "TIME_WAIT";
            case MIB_TCP_STATE_DELETE_TCB: return "DELETE_TCB";
            default: return "UNKNOWN";
        }
    }
};

// Main function
int main() {
    NetworkMonitor monitor;
    monitor.MonitorTcpConnections();

    // Wait to prevent the console from closing immediately
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}
