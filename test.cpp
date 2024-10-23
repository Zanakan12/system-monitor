#include <iostream>
#include <cstdlib>  // Pour utiliser system()
#include <fstream>
#include <string>

int main() {
    std::string line;
    
    // Utiliser la commande 'ip addr' pour obtenir les informations réseau
    std::cout << "Adresses IPv4 des interfaces réseau :\n";
    
    // Appel de la commande 'ip' et redirection de la sortie vers un fichier temporaire
    system("ip -4 addr show > ip_output.txt");
    
    // Lire le fichier temporaire contenant les résultats
    std::ifstream ipFile("ip_output.txt");
    
    if (ipFile.is_open()) {
        while (std::getline(ipFile, line)) {
            // Filtrer et afficher uniquement les lignes contenant 'inet' (adresses IPv4)
            if (line.find("inet ") != std::string::npos) {
                std::cout << line << std::endl;
            }
        }
        ipFile.close();
    } else {
        std::cerr << "Erreur lors de l'ouverture du fichier temporaire" << std::endl;
    }
    
    return 0;
}
