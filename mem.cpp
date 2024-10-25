#include "header.h"



    auto ramUsage = getRamUsage();
    float ramUsageRatio = ramUsage.first;
    int totalRamInt = ramUsage.second.first;
    std::string ramUsageText = ramUsage.second.second;

    auto swapData = getSwapUsage();
    float swapUsageRatio = swapData.first;
    float totalSwap = swapData.second.first;  // Note que tu convertis totalSwap en int ici
    std::string swapUsageText = swapData.second.second;

    auto diskData = getDiskUsage();
    float diskUsageRatio = diskData.first;
    float totaldisk = diskData.second.first;  // Note que tu convertis totaldisk en int ici
    std::string diskUsageText = diskData.second.second;



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
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    float totalSwap = 0.0f;
    float freeSwap = 0.0f;

    // Lire le fichier et extraire les informations sur la swap
    while (std::getline(meminfo, line)) {
        // Chercher SwapTotal
        if (line.find("SwapTotal:") == 0) {
            totalSwap = std::stof(line.substr(line.find_first_of("0123456789")));
            
        } 
        // Chercher SwapFree
        else if (line.find("SwapFree:") == 0) {
            freeSwap = std::stof(line.substr(line.find_first_of("0123456789")));
            
        }
        
        // Si nous avons trouvé les deux valeurs, sortir de la boucle
        if (totalSwap > 0 && freeSwap >= 0) {
            break;
        }
    }
    std::cout << "Debug: SwapTotal = " << totalSwap << " kB" << std::endl; // Debug
    std::cout << "Debug: SwapFree = " << freeSwap << " kB" << std::endl; // Debug
    // Vérifier si les valeurs ont été trouvées
    if (totalSwap <= 0) {
        std::cerr << "Erreur: SwapTotal non trouvé ou invalide." << std::endl;
        return {0.0f, {0.0f, "0 GB / 0 GB"}};
    }
    
    // Convertir en Go
    totalSwap /= 1024.0f; // Convertir totalSwap en Mo
    freeSwap /= 1024.0f;  // Convertir freeSwap en Mo
    float usedSwap = totalSwap - freeSwap; // Calculer la swap utilisée en Mo

    // Convertir en Go
    usedSwap /= 1024.0f; // Convertir usedSwap en Go

    // Calculer le ratio utilisé (entre 0 et 1)
    float swapUsageRatio = totalSwap > 0 ? usedSwap / totalSwap : 0.0f;

    // Générer une chaîne de texte "X GB / Y GB"
    std::ostringstream swapUsageText;
    swapUsageText << std::fixed << std::setprecision(2) << usedSwap << " GB / " << totalSwap << " GB";

    // Retourner le ratio, le total de la SWAP en GB et le texte
    return {swapUsageRatio, {totalSwap, swapUsageText.str()}};
}


MemoryInfo getMemoryInfo() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    MemoryInfo memInfo = {0.0f, 0.0f, 0.0f, 0.0f, "0 GB / 0 GB"};

    if (!meminfo.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir /proc/meminfo." << std::endl;
        return memInfo; // Retourner des valeurs par défaut
    }

    // Lire le fichier et extraire les informations sur la mémoire
    while (std::getline(meminfo, line)) {
        // Debug: afficher chaque ligne lue
        std::cout << "Debug: Ligne lue: " << line << std::endl;

        // Chercher SwapTotal
        if (line.find("SwapTotal:") == 0) {
            memInfo.totalSwap = std::stof(line.substr(line.find_first_of("0123456789")));
            std::cout << "Debug: SwapTotal = " << memInfo.totalSwap << " kB" << std::endl; // Debug
        } 
        // Chercher SwapFree
        else if (line.find("SwapFree:") == 0) {
            memInfo.freeSwap = std::stof(line.substr(line.find_first_of("0123456789")));
            std::cout << "Debug: SwapFree = " << memInfo.freeSwap << " kB" << std::endl; // Debug
        }

        // Si nous avons trouvé les deux valeurs, sortir de la boucle
        if (memInfo.totalSwap > 0 && memInfo.freeSwap >= 0) {
            break;
        }
    }

    // Vérifier si SwapTotal a été trouvé
    if (memInfo.totalSwap <= 0) {
        std::cerr << "Erreur: SwapTotal non trouvé ou invalide." << std::endl;
        return memInfo; // Retourner des valeurs par défaut
    }

    // Conversion en Mo
    memInfo.totalSwap /= 1024.0f; // Convertir totalSwap en Mo
    memInfo.freeSwap /= 1024.0f;  // Convertir freeSwap en Mo
    memInfo.usedSwap = memInfo.totalSwap - memInfo.freeSwap; // Calculer la swap utilisée en Mo

    // Vérifier si usedSwap est négatif et le corriger
    if (memInfo.usedSwap < 0) {
        memInfo.usedSwap = 0.0f; // Assurez-vous que usedSwap ne soit pas négatif
    }

    // Calculer le ratio d'utilisation de swap
    memInfo.swapUsageRatio = memInfo.totalSwap > 0 ? memInfo.usedSwap / memInfo.totalSwap : 0.0f;

    // Générer une chaîne de texte "X GB / Y GB"
    std::ostringstream swapUsageText;
    swapUsageText << std::fixed << std::setprecision(2) << (memInfo.usedSwap / 1024.0f) << " GB / " 
                   << (memInfo.totalSwap / 1024.0f) << " GB";
    memInfo.swapUsageText = swapUsageText.str();

    return memInfo;
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


void progresseBar(){
            ImGui::Text("Physical Memory (RAM):");
    ImGui::ProgressBar(ramUsageRatio, ImVec2(0.0f, 0.0f), ramUsageText.c_str());
    ImGui::SameLine();
    ImGui::Text("RAM");
    ImGui::Text("0 Go                                                    %d Go", totalRamInt);
    ImGui::Text("");

    ImGui::Text("Virtual Memory (SWAP):");
    ImGui::ProgressBar(swapUsageRatio, ImVec2(0.0f, 0.0f), swapUsageText.c_str());
    ImGui::SameLine();
    ImGui::Text("VM");
    ImGui::Text("0 Go                                                    %2.f Go", totalSwap);
    ImGui::Text("");
    
    ImGui::Text("Disk Usage:");
    ImGui::ProgressBar(diskUsageRatio, ImVec2(0.0f, 0.0f), diskUsageText.c_str());
    ImGui::SameLine();
    ImGui::Text("Disk");
    ImGui::Text("0 Go                                                    %2.f Go", totaldisk);
    ImGui::Text("");

    }

    void filterTable(){
        
    //Search bar
    ImGui::Separator();
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));            // Enlève la couleur de fond du header
    if (ImGui::CollapsingHeader("Process Table")) {
    static char searchQuery[64] = "";
    std::vector<Process> processes = getProcesses();
    ImGui::Text("");
    ImGui::SetCursorPosX((50 - 10) * 0.5f);
    ImGui::Text("Filter the process by name:");
    ImGui::SetCursorPosX((50 - 10) * 0.5f);
    ImGui::InputText("Search", searchQuery, IM_ARRAYSIZE(searchQuery));
    //table 
    ImGui::SetCursorPosX((50 - 10) * 0.5f);
    if (ImGui::BeginTable("ProcessesTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        // Définir les en-têtes de colonnes
        ImGui::TableSetupColumn("PID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("State");
        ImGui::TableSetupColumn("CPU (%)");
        ImGui::TableSetupColumn("Memory (Mb)");
        ImGui::TableHeadersRow();
        // Remplir le tableau avec les données des processus
        for (const Process& process : processes) {
            if (strstr(process.name.c_str(), searchQuery)) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", process.pid);

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", process.name.c_str());

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", process.state.c_str());

            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.2f", calculateCpuUsage(process.pid));

            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%.f", process.memUsage/100);
        }
        }
        ImGui::EndTable();
    }
    }
    ImGui::PopStyleColor(1);
    }