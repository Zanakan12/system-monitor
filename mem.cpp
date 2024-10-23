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

std::pair<float, std::string> getSwapUsage() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        // Total de la SWAP en GB
        float totalSwap = info.totalswap / (1024.0f * 1024.0f * 1024.0f);
        // SWAP utilisée en GB
        float usedSwap = (info.totalswap - info.freeswap) / (1024.0f * 1024.0f * 1024.0f);

        // Calcul du ratio utilisé (entre 0 et 1)
        float swapUsageRatio = usedSwap / totalSwap;

        // Générer une chaîne de texte "X GB / Y GB"
        std::ostringstream swapUsageText;
        swapUsageText << std::fixed << std::setprecision(2) << usedSwap << " GB / " << totalSwap << " GB";

        return {swapUsageRatio, swapUsageText.str()};
    }
    return {0.0f, "0 GB / 0 GB"}; // En cas d'erreur
}


std::pair<float, std::string> getDiskUsage() {
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

        return {diskUsageRatio, diskUsageText.str()};
    }
    return {0.0f, "0 GB / 0 GB"}; // En cas d'erreur
}