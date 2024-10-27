#include "header.h"

std::string getIPAddress(const std::string& interface) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("Socket creation failed");
        return "";
    }

    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ-1);

    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
        close(fd);
        return "N/A";
    }

    close(fd);

    struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifr.ifr_addr;
    return inet_ntoa(ipaddr->sin_addr);
}


// Function to display RX stats in ImGui table
void displayRXStatsTable() {
    std::vector<NetworkInterface> interfaces = getNetworkStats(); // Correction ici

    // Begin an ImGui table for RX stats
    if (ImGui::BeginTable("RX Interface", 9)) { // Nombre de colonnes mis à jour à 8
        // Set up headers for the RX table
        ImGui::TableSetupColumn("Interface");
        ImGui::TableSetupColumn("bytes");
        ImGui::TableSetupColumn("Packets");
        ImGui::TableSetupColumn("Errs");
        ImGui::TableSetupColumn("Drop");
        ImGui::TableSetupColumn("Fifo");
        ImGui::TableSetupColumn("Colls");
        ImGui::TableSetupColumn("Carrier");
        ImGui::TableSetupColumn("Compressed");
        


        ImGui::TableHeadersRow();

        for (const auto& iface : interfaces) {
            // Fill in the RX row
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text(iface.name.c_str());
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.rxStats.bytes);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.rxStats.packets);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.rxStats.errs);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.rxStats.drop);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.rxStats.fifo);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.rxStats.colls);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.rxStats.carrier);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.rxStats.compressed);
            
            
            
        }
        // End the RX table
        ImGui::EndTable();
    }
}


// Function to display TX stats in ImGui table
void displayTXStatsTable() {
    // Obtenir toutes les interfaces réseau et leurs statistiques
    std::vector<NetworkInterface> interfaces = getNetworkStats();

    // Débuter un tableau ImGui pour les statistiques TX
    if (ImGui::BeginTable("TX Interface", 9)) { // Utiliser 8 colonnes comme défini
        // Configurer les en-têtes pour le tableau TX
        ImGui::TableSetupColumn("Interface");
        ImGui::TableSetupColumn("Bytes");
        ImGui::TableSetupColumn("Packets");
        ImGui::TableSetupColumn("Errors");
        ImGui::TableSetupColumn("Dropped");
        ImGui::TableSetupColumn("FIFO");
        ImGui::TableSetupColumn("Frame");
        ImGui::TableSetupColumn("Compressed");
        ImGui::TableSetupColumn("Multicast");

        ImGui::TableHeadersRow();

        // Remplir le tableau avec les statistiques TX pour chaque interface
        for (const auto& iface : interfaces) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text(iface.name.c_str());
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.txStats.bytes);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.txStats.packets);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.txStats.errs);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.txStats.drop);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.txStats.fifo);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.txStats.frame);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.txStats.compressed);
            ImGui::TableNextColumn(); ImGui::Text("%llu", iface.txStats.multicast);
        }

        // Fin du tableau TX
        ImGui::EndTable();
    }
}



// Fonction pour obtenir toutes les interfaces réseau disponibles
std::vector<NetworkInterface> getNetworkInterfaces() {
    std::ifstream file("/proc/net/dev");
    std::string line;
    std::vector<NetworkInterface> interfaces;

    // Ignorer les deux premières lignes d'en-tête
    std::getline(file, line);
    std::getline(file, line);

    // Lire chaque interface réseau
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string iface;

        // Lecture du nom de l'interface et nettoyage
        iss >> iface;
        iface = iface.substr(0, iface.find(":"));

        // Obtenir l'adresse IP de l'interface
        std::string ipAddress = getIPAddress(iface);

        // Ajouter l'interface et son adresse IP au vecteur
        interfaces.push_back({iface, ipAddress});
    }

    return interfaces;
}
std::vector<NetworkInterface> getNetworkStats() {
    std::vector<NetworkInterface> interfaces;
    std::ifstream file("/proc/net/dev");

    if (!file.is_open()) {
        std::cerr << "Unable to open /proc/net/dev" << std::endl;
        return interfaces;
    }

    std::string line;
    std::getline(file, line); // Ignore the header
    std::getline(file, line); // Ignore the header

    while (std::getline(file, line)) {
        NetworkInterface iface;
        std::istringstream iss(line);
        std::string ifaceName;

        iss >> ifaceName; // Lire le nom de l'interface
        iface.name = ifaceName.substr(0, ifaceName.length() - 1); // Enlever le ':' à la fin

        // Lire les statistiques RX
        iss >> iface.rxStats.bytes >> iface.rxStats.packets >> iface.rxStats.errs >>
            iface.rxStats.drop >> iface.rxStats.fifo >> iface.rxStats.colls >> iface.rxStats.carrier >>
            iface.rxStats.compressed;

        // Lire les statistiques TX
        iss >> iface.txStats.bytes >> iface.txStats.packets >> iface.txStats.errs >>
            iface.txStats.drop >> iface.txStats.fifo >> iface.txStats.frame  >> iface.txStats.compressed>> iface.txStats.multicast ;

        interfaces.push_back(iface);
    }
    

    file.close();
    return interfaces;
}

void dateTime(){
    time_t t;
    time(&t);
    // Convertir en un format lisible
    struct tm *localTime = localtime(&t);
    // Formater la date et l'heure en une chaîne de caractères
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", localTime);
    ImGui::Text( buffer);
}


// Fonction pour convertir les bytes en Go, Mo, Ko
std::string formatBytes(long long bytes) {
    float kb = bytes / 1024.0f;
    float mb = kb / 1024.0f;
    float gb = mb / 1024.0f;

    std::ostringstream out;
    if (gb >= 1.0f) {
        out << std::fixed << std::setprecision(2) << gb << " GB";
    } else if (mb >= 1.0f) {
        out << std::fixed << std::setprecision(2) << mb << " MB";
    } else {
        out << std::fixed << std::setprecision(2) << kb << " KB";
    }
    return out.str();
}


void DisplayNetworkUsage() {
    std::vector<NetworkInterface> networkInterfaces = getNetworkStats();

    if (ImGui::BeginTabBar("Network Usage")) {
        // Onglet Received (RX)
        if (ImGui::BeginTabItem("Recieved")) {
            for (const auto& iface : networkInterfaces) {
                float rx_gb = iface.rxStats.bytes / (1024.0f * 1024.0f * 1024.0f);
                float progress = rx_gb / 2.0f; // Limite de 2 Go
                std::string displayText = formatBytes(iface.rxStats.bytes);
                ImGui::Text((iface.name + ":").c_str());
                ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f), displayText.c_str());
                ImGui::SameLine();
                ImGui::Text("2 GO");
            }
            ImGui::EndTabItem();
        }

        // Onglet Transmissed (TX)
        if (ImGui::BeginTabItem("Transmissed")) {
            for (const auto& iface : networkInterfaces) {
                float tx_gb = iface.txStats.bytes / (1024.0f * 1024.0f * 1024.0f);
                float progress = tx_gb / 2.0f; // Limite de 2 Go
                std::string displayText = formatBytes(iface.txStats.bytes);
                ImGui::Text((iface.name + ":").c_str());
                ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f), displayText.c_str());
                ImGui::SameLine();
                ImGui::Text("2 GO");
            }
            ImGui::EndTabItem();
        }
        ImGui::Text("rappel la conversion est parfaite,\n la commande ifconfig ne fait pas de conversion.\n");
        ImGui::EndTabBar();
    }
}
