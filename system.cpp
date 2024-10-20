#include "header.h"

// get cpu id and information, you can use `proc/cpuinfo`
string CPUinfo()
{
    char CPUBrandString[0x40];
    unsigned int CPUInfo[4] = {0, 0, 0, 0};

    // unix system
    // for windoes maybe we must add the following
    // __cpuid(regs, 0);
    // regs is the array of 4 positions
    __cpuid(0x80000000, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
    unsigned int nExIds = CPUInfo[0];

    memset(CPUBrandString, 0, sizeof(CPUBrandString));

    for (unsigned int i = 0x80000000; i <= nExIds; ++i)
    {
        __cpuid(i, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);

        if (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }
    string str(CPUBrandString);
    return str;
}

// getOsName, this will get the OS of the current computer
const char *getOsName()
{
#ifdef _WIN32
    return "Windows 32-bit";
#elif _WIN64
    return "Windows 64-bit";
#elif __APPLE__ || __MACH__
    return "Mac OSX";
#elif __linux__
    return "Linux";
#elif __FreeBSD__
    return "FreeBSD";
#elif __unix || __unix__
    return "Unix";
#else
    return "Other";
#endif
}

// getComputerName, will give the computer Name in return
const char* getComputerName() {
#ifdef _WIN32
    static char computerName[256];
    DWORD size = sizeof(computerName);
    if (GetComputerNameA(computerName, &size)) {
        return computerName;
    } else {
        return "Unknown Computer";
    }
#else
    static char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return hostname;
    } else {
        return "Unknown Hostname";
    }
#endif
}


// Procesor information
std::string getProcessorInfo() {
    std::string info;

#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            info = "Processor architecture: x64 (AMD or Intel)";
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            info = "Processor architecture: x86";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            info = "Processor architecture: ARM";
            break;
        default:
            info = "Unknown processor architecture";
            break;
    }
#else
    std::ifstream cpuInfo("/proc/cpuinfo");
    std::string line;

    if (cpuInfo.is_open()) {
        while (std::getline(cpuInfo, line)) {
            if (line.find("model name") != std::string::npos) {
                info = line.substr(line.find(":") + 2);
                break;
            }
        }
        cpuInfo.close();
    } else {
        info = "Could not open /proc/cpuinfo.";
    }

    if (info.empty()) {
        info = "Processor information not found.";
    }
#endif
    return info;
}



   #include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif

int getActiveProcessCount() {
    int running = 0;

#ifdef _WIN32
    // Windows implementation
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_PROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return -1; // Indicate an error
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hProcessSnap, &pe32)) {
        do {
            running++; // Count each process
        } while (Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);

#else
    // Linux implementation
    DIR* dir = opendir("/proc");
    struct dirent* entry;

    if (dir == nullptr) {
        return -1; // Indicate an error
    }

    while ((entry = readdir(dir)) != nullptr) {
        if (isdigit(entry->d_name[0])) {  // Check if the directory name is a PID (a running process)
            running++; // Count each process
        }
    }
    closedir(dir);
#endif

    return running; // Return total number of active processes
}

std::string readFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return "N/A";
    
    std::string line;
    std::getline(file, line);
    return line;
}

// Function to get CPU information
std::string getCPUInfo()
{
    return readFile("/proc/cpuinfo"); // Simplified for the example, you'd need to parse the info
}

// Function to get temperature
std::string getTemperature()
{
    return readFile("/sys/class/thermal/thermal_zone1/temp");
}

// Function to get fan speed
std::string getFanSpeed()
{
    return readFile("/sys/class/hwmon/hwmon0/fan1_input");
}

void updateTemperatureData(float newTemperature)
{
    temperatureData[temperatureIndex] = newTemperature;
    temperatureIndex = (temperatureIndex + 1) % 100; // Circular buffer
}

float getFanSpeed(int fanNumber) {
    std::string path = "/sys/class/hwmon/hwmon0/fan" + std::to_string(fanNumber) + "_input"; // Assure-toi que le numéro de hwmon et fan est correct
    std::ifstream file(path);
    float speed = 0.0f;

    if (file.is_open()) {
        file >> speed;
        file.close();
    }

    return speed; 
}