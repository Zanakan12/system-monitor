#include "header.h"



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
    DIR* dir = opendir("/proc");
    struct dirent* entry;
    std::vector<Process> processes;
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



ProcessCpuInfo getProcessCpuInfo(int pid) {
    std::string statFile = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream file(statFile);
    ProcessCpuInfo info = {0, 0, 0};
    
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << statFile << std::endl;
        return info;
    }

    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);

    std::string temp;
    int field = 1;
    while (iss >> temp) {
        if (field == 14) {
            info.utime = std::stol(temp); // 14th field is utime
        } else if (field == 15) {
            info.stime = std::stol(temp); // 15th field is stime
        } else if (field == 22) {
            info.starttime = std::stol(temp); // 22nd field is starttime
            break; // We don't need more fields after starttime
        }
        field++;
    }
    file.close();
    return info;
}

double getSystemUptime() {
    std::ifstream file("/proc/uptime");
    double uptime = 0.0;
    
    if (file.is_open()) {
        file >> uptime;
        file.close();
    } else {
        std::cerr << "Could not open /proc/uptime" << std::endl;
    }
    
    return uptime;
}

double calculateCpuUsage(int pid) {
    ProcessCpuInfo pInfo = getProcessCpuInfo(pid);
    long hertz = sysconf(_SC_CLK_TCK); // Get system clock ticks per second
    double uptime = getSystemUptime();

    long total_time = pInfo.utime + pInfo.stime;
    double seconds = uptime - (pInfo.starttime / hertz);

    if (seconds > 0.0) {
        return 100.0 * ((total_time / (double)hertz) / seconds);
    } else {
        return 0.0;
    }
}

std::string getIPv4Addresses() {
    std::string line;
    std::string ipv4Addresses;

    // Utiliser la commande 'ip addr' pour obtenir les informations réseau
    system("ip -4 addr show > ip_output.txt");
    
    // Lire le fichier temporaire contenant les résultats
    std::ifstream ipFile("ip_output.txt");
    
    if (ipFile.is_open()) {
        while (std::getline(ipFile, line)) {
            // Filtrer et stocker uniquement les lignes contenant 'inet' (adresses IPv4)
            if (line.find("inet ") != std::string::npos) {
                ipv4Addresses += line + "\n"; // Ajouter la ligne à la chaîne de résultats
            }
        }
        ipFile.close();
    } else {
        return "Erreur lors de l'ouverture du fichier temporaire"; // Retourner une erreur
    }

    return ipv4Addresses.empty() ? "Aucune adresse IPv4 trouvée" : ipv4Addresses; // Retourner un message si aucune adresse n'est trouvée
}
