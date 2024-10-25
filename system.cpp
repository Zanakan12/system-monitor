#include "header.h"

// Déclaration globale
const int maxDataPoints = 100;  // Taille maximale du tableau
float Data[maxDataPoints] = {0}; // Tableau pour stocker les données
int Index = 0;                   // Index circulaire pour les données
int dataCount = 0;               // Compteur pour le nombre de points de données
int fps = 60;                    // Taux de rafraîchissement
bool animate = true;             // Animation activée
float scalemax = 100.0f;         // Échelle maximale
std::string item1;               // Utilisation de la variable item1
float item2;
std::string graphTitle;
std::ostringstream stream;


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

// Function to get temperature
std::string getTemperature()
{
    return readFile("/sys/class/thermal/thermal_zone0/temp");
}

void updateTemperatureData(float newTemperature)
{
    Data[Index] = newTemperature;
    Index = (Index + 1) % 100; // Circular buffer
}



std::string execCommand(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string getFan1Speed() {
    // Exécute la commande sensors et capture la sortie
    std::string output = execCommand("sensors");
    
    // Recherche la ligne contenant les informations de fan1
    std::istringstream ss(output);
    std::string line;
    
    while (std::getline(ss, line)) {
        // Cherche une ligne contenant 'fan1'
        if (line.find("CPU ") != std::string::npos) {
            // Extraire la valeur numérique avant "rpm"
            std::string::size_type pos = line.find(":");
            if (pos != std::string::npos) {
                std::string speed = line.substr(pos + 1); // Récupère tout ce qui est après ':'
                speed.erase(0, speed.find_first_not_of(" \t")); // Supprime les espaces initiaux
                
                // Trouver et extraire les chiffres avant "rpm"
                std::string::size_type rpmPos = speed.find("RPM");
                if (rpmPos != std::string::npos) {
                    speed = speed.substr(0, rpmPos); // Garde tout avant "rpm"
                    speed.erase(speed.find_last_not_of(" \t") + 1); // Supprime les espaces finaux
                   // std::cout << "Info fan1 trouvée : " << speed << std::endl;
                    return speed;
                }
            }
        }
    }
    
    return "N/A";  // Retourne "N/A" si fan1 n'est pas trouvé
}


float getCpuUsage() {
    // Ouvrir le fichier /proc/stat
    std::ifstream file("/proc/stat");
    if (!file.is_open()) {
        std::cerr << "Erreur lors de l'ouverture de /proc/stat" << std::endl;
        return -1.0f;
    }

    std::string line;
    std::getline(file, line); // Lire la première ligne

    // Séparer la ligne en mots
    std::istringstream ss(line);
    std::string cpu;
    std::vector<long long> values;
    ss >> cpu; // Ignorer le premier mot "cpu"

    long long value;
    while (ss >> value) {
        values.push_back(value);
    }

    // Calculer les valeurs d'utilisation
    long long total = 0;
    long long idle = values[3]; // idle est le 4ème champ
    for (long long val : values) {
        total += val;
    }

    // Utilisation du CPU = (total - idle) / total * 100
    float usage = 100.0f * (total - idle) / total;
    return usage;
}


void displaySysInfo(){
    ImGui::Text("Operating system: %s", getOsName());
    ImGui::Text("Computer name: %s", getComputerName());
    ImGui::Text("Logged in user: %s", getenv("USER") ? getenv("USER") : getenv("USERNAME"));
    ImGui::Text("Number of working processes: %d", getActiveProcessCount());
    ImGui::Text("Processor: %s", getProcessorInfo().c_str());

}

void displayTabBar(){
        if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("CPU")) {
            usleep(1000000 / fps);
            // Réinitialiser les données à chaque changement d'onglet
            if (dataCount == 0) {
                std::fill(std::begin(Data), std::end(Data), 0.0f); // Ne pas effacer si déjà rempli
            }

            item1 = "CPU";
            float cpuUsage = getCpuUsage();
            if (cpuUsage >= 0) {
                //std::cout << "Utilisation du CPU : " << cpuUsage << "%" << std::endl; // debug
            }
            item2 = Data[Index] = cpuUsage;
            Index = (Index + 1) % maxDataPoints;

            if (dataCount < maxDataPoints) {
                dataCount++;  // Augmenter le nombre de points disponibles
            }

            // Réinitialisation du flux
            stream.str(""); // Réinitialise le flux
            stream << std::fixed << std::setprecision(2) << item2;
            graphTitle = item1 + ": " + stream.str() + "%";

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Fan")) {
            usleep(1000000 / fps);
            // Réinitialiser les données à chaque changement d'onglet
            if (dataCount == 0) {
                std::fill(std::begin(Data), std::end(Data), 0.0f); // Ne pas effacer si déjà rempli
            }

            item1 = "Fan";
            ImGui::Text("Fan status :");
            ImGui::Text("");
            if (getFan1Speed() == "0") {
                ImGui::Text("Status : Disable");
            } else {
                ImGui::Text("Status : Enable");
            }
            ImGui::Text("Speed: %s RPM", getFan1Speed().c_str());

            int number = atoi(getFan1Speed().c_str());
            ImGui::Text("Level : %d ", number / 1000);

            item2 = Data[Index] = float(number)*100/4900; // specifique param for this computer.
            Index = (Index + 1) % maxDataPoints;

            if (dataCount < maxDataPoints) {
                dataCount++;  // Augmenter le nombre de points disponibles
            }

            // Réinitialisation du flux
            stream.str(""); // Réinitialise le flux
            stream << std::fixed << std::setprecision(2) << item2*4900/100;
            graphTitle = item1 + ": " + getFan1Speed().c_str() + " RPM";

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Thermal")) {
            // Réinitialiser les données à chaque changement d'onglet
            if (dataCount == 0) {
                std::fill(std::begin(Data), std::end(Data), 0.0f); // Ne pas effacer si déjà rempli
            }
            float temperature = std::stof(getTemperature()) / 1000.0f; // Conversion en Celsius
            ImGui::Text("Temperature : %.f°C", temperature);
            item1 = "Temperature";
            item2 = Data[Index] = temperature;
            Index = (Index + 1) % maxDataPoints;

            if (dataCount < maxDataPoints) {
                dataCount++;  // Augmenter le nombre de points disponibles
            }

            // Réinitialisation du flux
            stream.str(""); // Réinitialise le flux
            stream << std::fixed << std::setprecision(2) << item2;
            graphTitle = item1 + ": " + stream.str() + "°C";

            ImGui::EndTabItem();
        }

        ImGui::Checkbox("Animate", &animate);
        if (animate) {
            ImGui::SliderInt("FPS", &fps, 1, 60);
            ImGui::SliderFloat("Scale Max", &scalemax, 1, 100);
            // Affichage du graphique avec les données accumulées
            usleep(1000000 / fps);
            ImGui::PlotLines(item1.c_str(), Data, dataCount, Index, graphTitle.c_str(), 0.0f, scalemax, ImVec2(0, 180));
        }

        ImGui::EndTabBar();
    }

    }
