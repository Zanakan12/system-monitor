#include "header.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

// Fonction pour obtenir la température du CPU à partir de /proc (exemple basique)
float getCPUTemperature() {
    std::ifstream file("/sys/class/thermal/thermal_zone0/temp");
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier de température\n";
        return 0.0f;
    }
    float temp;
    file >> temp;
    file.close();
    return temp / 1000.0f; // Diviser par 1000 si la valeur est en millidegrés
}

int main() {
    // Stocker les températures
    std::vector<float> temps(100, 0.0f); // On garde les 100 dernières mesures
    
    // Boucle principale du programme
    while (true) {
        // Obtenir la température actuelle
        float currentTemp = getCPUTemperature();
        
        // Ajouter la nouvelle température à la fin du tableau, en supprimant la plus ancienne
        temps.erase(temps.begin());
        temps.push_back(currentTemp);
        
        // Afficher l'interface ImGui
        ImGui::Begin("Température du CPU");
        ImGui::PlotLines("Température", temps.data(), temps.size());
        ImGui::End();
        
        // Rafraîchir l'interface graphique (cette partie dépend de ton implémentation globale)
        // Exemple: ImGui::Render();
    }
    
    return 0;
}
