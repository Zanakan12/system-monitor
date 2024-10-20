#include <iostream>
#include <fstream>
#include <string>

std::string getProcessorInfo() {
    std::ifstream cpuinfoFile("/proc/cpuinfo");
    std::string line;
    std::string processorName;

    if (cpuinfoFile.is_open()) {
        while (std::getline(cpuinfoFile, line)) {
            if (line.find("model name") != std::string::npos) {
                // Récupère le nom du processeur
                processorName = line.substr(line.find(":") + 2);
                break; // Sortir de la boucle après avoir trouvé le nom du processeur
            }
        }
        cpuinfoFile.close();
    } else {
        std::cerr << "Impossible d'ouvrir le fichier /proc/cpuinfo" << std::endl;
    }

    return processorName;
}

int main() {
    std::string processor = getProcessorInfo();
    if (!processor.empty()) {
        std::cout << "Nom du processeur : " << processor << std::endl;
    } else {
        std::cout << "Nom du processeur non trouvé." << std::endl;
    }
    return 0;
}
