#include "header.h"


    
    auto ramUsage = getRamUsage();
    float ramUsageRatio = ramUsage.first;
    int totalRamInt = ramUsage.second.first;
    std::string ramUsageText = ramUsage.second.second;

    MemoryInfo memInfo = getMemoryInfo();
    float swapUsageRatio = memInfo.swapUsageRatio;
    float totalSwap = memInfo.totalSwap;  // Note que tu convertis totalSwap en int ici
    std::string swapUsageText = memInfo.swapUsageText;


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


MemoryInfo getMemoryInfo() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    MemoryInfo memInfo = {0.0f, 0.0f, 0.0f, 0.0f, "0 GB / 0 GB", 0.0f, 0.0f, 0.0f, "0 GB / 0 GB"};

    if (!meminfo.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir /proc/meminfo." << std::endl;
        return memInfo; // Retourner des valeurs par défaut
    }

    while (std::getline(meminfo, line)) {
        // Lire les valeurs de RAM
        if (line.find("MemTotal:") == 0) {
            memInfo.totalRAM = std::stof(line.substr(line.find_first_of("0123456789")));
        } else if (line.find("MemAvailable:") == 0) {
            memInfo.availableRAM = std::stof(line.substr(line.find_first_of("0123456789")));
        }
        // Lire les valeurs de SWAP
        else if (line.find("SwapTotal:") == 0) {
            memInfo.totalSwap = std::stof(line.substr(line.find_first_of("0123456789")));
        } else if (line.find("SwapFree:") == 0) {
            memInfo.freeSwap = std::stof(line.substr(line.find_first_of("0123456789")));
        }
    }

    // Calcul de l'utilisation de la RAM
    memInfo.totalRAM /= 1024.0f;         // Convertir en Mo
    memInfo.availableRAM /= 1024.0f;     // Convertir en Mo
    float usedRAM = memInfo.totalRAM - memInfo.availableRAM;
    memInfo.ramUsageRatio = usedRAM / memInfo.totalRAM;

    std::ostringstream ramUsageText;
    ramUsageText << std::fixed << std::setprecision(2) << (usedRAM / 1024.0f) << " GB / " << (memInfo.totalRAM / 1024.0f) << " GB";
    memInfo.ramUsageText = ramUsageText.str();

    // Calcul de l'utilisation de SWAP
    memInfo.totalSwap /= 1024.0f;         // Convertir en Mo
    memInfo.freeSwap /= 1024.0f;          // Convertir en Mo
    memInfo.usedSwap = memInfo.totalSwap - memInfo.freeSwap;
    memInfo.swapUsageRatio = memInfo.usedSwap / memInfo.totalSwap;

    std::ostringstream swapUsageText;
    swapUsageText << std::fixed << std::setprecision(2) << (memInfo.usedSwap / 1024.0f) << " GB / " << (memInfo.totalSwap / 1024.0f) << " GB";
    memInfo.swapUsageText = swapUsageText.str();
    struct statvfs diskInfo;
    if (statvfs("/", &diskInfo) == 0) { // "/" pour le système de fichiers racine
        float totalDisk = diskInfo.f_blocks * diskInfo.f_frsize / (1024.0f * 1024.0f * 1024.0f); // En Go
        float freeDisk = diskInfo.f_bfree * diskInfo.f_frsize / (1024.0f * 1024.0f * 1024.0f);   // En Go
        float usedDisk = totalDisk - freeDisk;
        memInfo.totalDisk = totalDisk;
        memInfo.usedDisk = usedDisk;
        memInfo.diskUsageRatio = usedDisk / totalDisk;

        std::ostringstream diskUsageText;
        diskUsageText << std::fixed << std::setprecision(2) << usedDisk << " GB / " << totalDisk << " GB";
        memInfo.diskUsageText = diskUsageText.str();
    }
    return memInfo;
}


std::vector<Process> getProcesses() {
    DIR* dir = opendir("/proc");
    struct dirent* entry;
    std::vector<Process> processes;
    long clkTck = sysconf(_SC_CLK_TCK);  // Nombre de ticks par seconde
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long totalMemory = memInfo.totalram / 1024;  // Mémoire totale en Ko

    // Lire le uptime pour calculer le CPU
    double uptime;
    std::ifstream uptimeFile("/proc/uptime");
    if (uptimeFile) uptimeFile >> uptime;

    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            int pid = std::stoi(entry->d_name);
            std::string statusFilePath = "/proc/" + std::to_string(pid) + "/status";
            std::string statFilePath = "/proc/" + std::to_string(pid) + "/stat";

            // Lire le fichier /proc/[pid]/status pour obtenir des informations de base sur le processus
            std::ifstream statusFile(statusFilePath);
            if (statusFile) {
                Process process;
                process.pid = pid;
                std::string line;

                // Lire les lignes du fichier status pour extraire les informations
                while (std::getline(statusFile, line)) {
                    if (line.find("Name:") == 0) {
                        process.name = line.substr(6);
                    } else if (line.find("State:") == 0) {
                        process.state = line[7];
                    } else if (line.find("VmRSS:") == 0) {
                        std::istringstream iss(line.substr(8));
                        double memUsageKb;
                        iss >> memUsageKb;
                        process.memUsage = (memUsageKb / totalMemory) * 100.0;  // En pourcentage de la mémoire physique
                    }
                }

                // Lire le fichier /proc/[pid]/stat pour calculer l'utilisation du CPU
                std::ifstream statFile(statFilePath);
                if (statFile) {
                    std::string statLine;
                    std::getline(statFile, statLine);
                    std::istringstream statStream(statLine);
                    std::string token;
                    int field = 1;
                    long utime, stime, starttime;

                    // Extraire les valeurs utime, stime, starttime
                    while (statStream >> token) {
                        if (field == 14) utime = std::stol(token);
                        else if (field == 15) stime = std::stol(token);
                        else if (field == 22) starttime = std::stol(token);
                        field++;
                    }

                    // Calcul du temps total d'utilisation CPU du processus
                    double totalTime = (utime + stime) / static_cast<double>(clkTck);
                    double processUptime = uptime - (starttime / static_cast<double>(clkTck));
                    process.cpuUsage = (totalTime / processUptime) * 100.0;
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
    MemoryInfo memInfo = getMemoryInfo();

    // Affichage de l'utilisation de la RAM
    ImGui::Text("Physical Memory (RAM):");
    ImGui::ProgressBar(memInfo.ramUsageRatio, ImVec2(0.0f, 0.0f), memInfo.ramUsageText.c_str());
    ImGui::SameLine();
    ImGui::Text("RAM");
    ImGui::Text("0 Go");
    ImGui::SameLine(400);
    ImGui::Text("%.2f Go", memInfo.totalRAM / 1024.0f);
    ImGui::Text("");

    // Affichage de l'utilisation de la SWAP
    ImGui::Text("Virtual Memory (SWAP):");
    ImGui::ProgressBar(memInfo.swapUsageRatio, ImVec2(0.0f, 0.0f), memInfo.swapUsageText.c_str());
    ImGui::SameLine();
    ImGui::Text("VM");
    ImGui::Text("0 Go");
    ImGui::SameLine(400);
    ImGui::Text("%.2f Go", memInfo.totalSwap / 1024.0f);
    ImGui::Text("");

    // Affichage de l'utilisation du disque
    ImGui::Text("Disk Usage:");
    ImGui::ProgressBar(memInfo.diskUsageRatio, ImVec2(0.0f, 0.0f), memInfo.diskUsageText.c_str());
    ImGui::SameLine();
    ImGui::Text("Disk");
    ImGui::Text("0 Go");
    ImGui::SameLine(400);
    ImGui::Text("%.2f Go", memInfo.totalDisk); // Total disque en Go
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
            ImGui::Text("%.1f", calculateCpuUsage(process.pid));

            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%.1f", process.memUsage);
        }
        }
        ImGui::EndTable();
    }
    }
    ImGui::PopStyleColor(1);
    }