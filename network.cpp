#include "header.h"


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


