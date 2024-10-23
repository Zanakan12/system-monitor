#include "header.h"
#include <tuple>


// Fonction pour obtenir l'utilisation de la RAM
std::pair<float, std::pair<int, std::string>> getRamUsage() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        // Total de la RAM en GB
        float totalRam = info.totalram / (1024.0f * 1024.0f * 1024.0f);
        // RAM utilisée en GB
        float usedRam = (info.totalram - info.freeram) / (1024.0f * 1024.0f * 1024.0f);

        // Calcul du ratio utilisé (entre 0 et 1)
        float ramUsageRatio = usedRam / totalRam;

        // Texte "X GB / Y GB"
        std::ostringstream ramUsageText;
        ramUsageText << std::fixed << std::setprecision(2) << usedRam << " GB / " << totalRam << " GB";

        // Retourner le ratio et le texte
        return {ramUsageRatio, {static_cast<int>(totalRam), ramUsageText.str()}};
    }
    return {0.0f, {0, "0 GB / 0 GB"}}; // En cas d'erreur
}

std::pair<float, std::pair<float, std::string>> getSwapUsage() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        // Total de la SWAP en GB
        float totalSwap = info.totalswap / (1024.0f * 1024.0f * 1024.0f);
        
        // Debug pour afficher la valeur brute en bytes et en GB
        std::cout << "Total SWAP (bytes): " << info.totalswap << std::endl;
        std::cout << "Total SWAP (GB float): " << totalSwap << std::endl;

        // SWAP utilisée en GB
        float usedSwap = (info.totalswap - info.freeswap) / (1024.0f * 1024.0f * 1024.0f);

        // Calcul du ratio utilisé (entre 0 et 1)
        float swapUsageRatio = totalSwap > 0 ? usedSwap / totalSwap : 0.0f;

        // Générer une chaîne de texte "X GB / Y GB"
        std::ostringstream swapUsageText;
        swapUsageText << std::fixed << std::setprecision(2) << usedSwap << " GB / " << totalSwap << " GB";

        // Retourner le ratio, le total de la SWAP en GB (converti en int) et le texte
        return {swapUsageRatio, {totalSwap, swapUsageText.str()}};
    }
    return {0.0f, {0, "0 GB / 0 GB"}}; // En cas d'erreur
}



std::pair<float, std::pair<float, std::string>> getDiskUsage() {
    struct statvfs stat;
    if (statvfs("/", &stat) == 0) {
        // Total du disque en GB
        float totalDisk = (stat.f_blocks * stat.f_frsize) / (1024.0f * 1024.0f * 1024.0f);
        // Espace utilisé en GB
        float usedDisk = ((stat.f_blocks - stat.f_bfree) * stat.f_frsize) / (1024.0f * 1024.0f * 1024.0f);

        // Calcul du ratio utilisé (entre 0 et 1)
        float diskUsageRatio = usedDisk / totalDisk;

        // Texte "X GB / Y GB"
        std::ostringstream diskUsageText;
        diskUsageText << std::fixed << std::setprecision(2) << usedDisk << " GB / " << totalDisk << " GB";

        return {diskUsageRatio,{ totalDisk, diskUsageText.str()}};
    }
    return {0.0f ,{0,"0 GB / 0 GB"}}; // En cas d'erreur
}


std::vector<Process> getProcesses() {
    std::vector<Process> processes;
    DIR* dir = opendir("/proc");
    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
        // Si l'entrée est un dossier avec un nom numérique (ce qui signifie qu'il s'agit d'un processus)
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            int pid = std::stoi(entry->d_name);
            std::string statusFilePath = "/proc/" + std::to_string(pid) + "/status";
            
            // Lire le fichier /proc/[pid]/status pour obtenir des informations sur le processus
            std::ifstream statusFile(statusFilePath);
            if (statusFile) {
                Process process;
                process.pid = pid;
                std::string line;

                // Lire les lignes du fichier status pour extraire les informations nécessaires
                while (std::getline(statusFile, line)) {
                    if (line.find("Name:") == 0) {
                        process.name = line.substr(6);  // Nom du processus
                    } else if (line.find("State:") == 0) {
                        process.state = line.substr(7, 1);  // État du processus
                    } else if (line.find("VmSize:") == 0) {
                        std::istringstream iss(line.substr(8));
                        iss >> process.memUsage;  // Utilisation de la mémoire (en ko)
                    }
                }
                processes.push_back(process);
            }
        }
    }
    closedir(dir);
    return processes;
}
