#include "header.h"




float getRamUsage() {
    std::ifstream file("/proc/meminfo");
    std::string line;
    float totalMemory = 0.0f, freeMemory = 0.0f, availableMemory = 0.0f;

    while (std::getline(file, line)) {
        if (line.find("MemTotal:") == 0) {
            sscanf(line.c_str(), "MemTotal: %f", &totalMemory);
        }
        if (line.find("MemAvailable:") == 0) {
            sscanf(line.c_str(), "MemAvailable: %f", &availableMemory);
        }
    }

    file.close();

    float usedMemory = totalMemory - availableMemory;
    return usedMemory / totalMemory;  // Retourne le pourcentage utilisé
}

float getSwapUsage() {
    std::ifstream file("/proc/meminfo");
    std::string line;
    float totalSwap = 0.0f, freeSwap = 0.0f;

    while (std::getline(file, line)) {
        if (line.find("SwapTotal:") == 0) {
            sscanf(line.c_str(), "SwapTotal: %f", &totalSwap);
        }
        if (line.find("SwapFree:") == 0) {
            sscanf(line.c_str(), "SwapFree: %f", &freeSwap);
        }
    }

    file.close();

    float usedSwap = totalSwap - freeSwap;
    return usedSwap / totalSwap;  // Retourne le pourcentage utilisé
}

float getDiskUsage(const char* path) {
    struct statvfs buffer;
    if (statvfs(path, &buffer) != 0) {
        return -1;  // Erreur
    }

    float totalSize = buffer.f_blocks * buffer.f_frsize;
    float freeSize = buffer.f_bfree * buffer.f_frsize;
    float usedSize = totalSize - freeSize;

    return usedSize / totalSize;  // Retourne le pourcentage utilisé
}