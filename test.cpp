#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <array>
#include <memory>      // Pour std::shared_ptr
#include <thread>      // Pour std::this_thread
#include <chrono>      // Pour std::chrono

std::string getProcessorTemperature() {
    std::array<char, 128> buffer;
    std::string result;
    std::string command = "sensors | grep 'Core 0'"; // Change 'Core 0' si nécessaire

    // Exécute la commande et capture la sortie
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        std::cerr << "Erreur : impossible d'exécuter la commande." << std::endl;
        return "Erreur.";
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

int main() {
    while (true) {
        std::string temperature = getProcessorTemperature();
        std::cout << "Température du processeur : " << temperature << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Attendre 2 secondes avant la prochaine lecture
    }
    return 0;
}
