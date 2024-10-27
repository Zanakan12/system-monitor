#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctime>

struct ProcessInfo {
    int pid;
    std::string command;
    float cpuUsage;     // Pourcentage d'utilisation CPU
    float memoryUsage;  // En Mo
};

std::vector<ProcessInfo> getProcesses() {
    std::vector<ProcessInfo> processes;
    DIR* dir = opendir("/proc");
    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
        if (isdigit(entry->d_name[0])) {
            int pid = std::stoi(entry->d_name);
            std::string statFile = "/proc/" + std::to_string(pid) + "/stat";
            std::ifstream file(statFile);
            std::string line;

            if (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string temp;
                ProcessInfo pInfo;
                pInfo.pid = pid;

                // Lire les champs nécessaires
                for (int i = 0; i < 24; ++i) {
                    iss >> temp; // Lire chaque champ
                    if (i == 1) {
                        pInfo.command = temp; // Nom du processus
                    } else if (i == 13) { // utime
                        long utime;
                        iss >> utime;
                        pInfo.cpuUsage = utime; // Utilisation CPU
                    } else if (i == 22) { // RSS (Resident Set Size)
                        long rss;
                        iss >> rss;
                        pInfo.memoryUsage = rss * 4.0 / 1024.0; // Convertir RSS à Mo
                    }
                }

                // Ajout des informations à la liste
                processes.push_back(pInfo);
            }
        }
    }

    closedir(dir);
    return processes;
}

void displaySystemInfo(sysinfo& info) {
    sysinfo(&info);

    std::cout << "Uptime: " << info.uptime << " seconds\n";
    std::cout << "Total RAM: " << info.totalram / (1024 * 1024) << " MB\n";
    std::cout << "Free RAM: " << info.freeram / (1024 * 1024) << " MB\n";
    std::cout << "Total Swap: " << info.totalswap / (1024 * 1024) << " MB\n";
    std::cout << "Free Swap: " << info.freeswap / (1024 * 1024) << " MB\n";
}

void displayProcesses(const std::vector<ProcessInfo>& processes, const sysinfo& info) {
    std::cout << std::left << std::setw(10) << "PID"
              << std::setw(20) << "Command"
              << std::setw(10) << "CPU (%)"
              << std::setw(10) << "Memory (%)"
              << std::endl;

    for (const auto& process : processes) {
        float memoryUsagePercent = (process.memoryUsage / (info.totalram / 1024)) * 100.0; // Pourcentage de mémoire
        std::cout << std::left << std::setw(10) << process.pid
                  << std::setw(20) << process.command
                  << std::setw(10) << std::fixed << std::setprecision(2) << process.cpuUsage
                  << std::setw(10) << std::fixed << std::setprecision(2) << memoryUsagePercent
                  << std::endl;
    }
}

int main() {
    sysinfo info;
    displaySystemInfo(info);
    auto processes = getProcesses();
    displayProcesses(processes, info);
    return 0;
}
