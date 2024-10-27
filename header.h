// To make sure you don't declare the function more than once by including the header multiple times.
#ifndef header_H
#define header_H


#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <iomanip>
#include <unistd.h>  // Pour sysconf(_SC_CLK_TCK)
#include <algorithm>
#include <sstream>
#include <cstdio>
#include <string>
#include <array>
#include <memory>
#include <thread> 
#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif
#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include <iostream>
#include <cmath>
// lib to read from file
#include <fstream>
// for the name of the computer and the logged in user
#include <unistd.h>
#include <limits.h>
// this is for us to get the cpu information
// mostly in unix system
// not sure if it will work in windows
#include <cpuid.h>
// this is for the memory usage and other memory visualization
// for linux gotta find a way for windows
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
// for time and date
#include <ctime>
// ifconfig ip addresses
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>

using namespace std;

struct CPUStats
{
    long long int user;
    long long int nice;
    long long int system;
    long long int idle;
    long long int iowait;
    long long int irq;
    long long int softirq;
    long long int steal;
    long long int guest;
    long long int guestNice;
};

// processes `stat`
struct Proc
{
    int pid;
    string name;
    char state;
    long long int vsize;
    long long int rss;
    long long int utime;
    long long int stime;
};

struct ProcessCpuInfo {
    long utime;
    long stime;
    long starttime;
};


struct IP4
{
    char *name;
    char addressBuffer[INET_ADDRSTRLEN];
};

struct Networks
{
    vector<IP4> ip4s;
};

struct TX
{
    long long bytes;
    long long packets;
    long long errs;
    long long drop;
    long long fifo;
    long long frame;
    long long compressed;
    long long multicast;
};

struct RX
{
    long long bytes;
    long long packets;
    long long errs;
    long long drop;
    long long fifo;
    long long colls;
    long long carrier;
    long long compressed;
};
struct NetworkUsage {
    float rx_gb;
    float tx_gb;
};
struct Process {
    int pid;
    std::string name;
    std::string state;
    float cpuUsage;  // Tu peux ajouter une logique plus tard pour calculer l'usage CPU
    float memUsage;  // Tu peux ajouter une logique plus tard pour calculer l'usage mémoire
};

struct NetworkInterface {
    std::string name;
    std::string ipAddress;
    RX rxStats;
    TX txStats;
};

struct MemoryInfo {
    float totalSwap;
    float freeSwap;
    float usedSwap;
    float swapUsageRatio;
    std::string swapUsageText;
    float totalRAM;
    float availableRAM;
    float ramUsageRatio;
    std::string ramUsageText;
    float totalDisk;         // Taille totale du disque en Go
    float usedDisk;          // Espace disque utilisé en Go
    float diskUsageRatio;    // Ratio d'utilisation du disque
    std::string diskUsageText; // Texte formaté pour l'utilisation du disque
};

struct DiskInfo {
    std::string mountPoint;
    float totalSpace;
    float freeSpace;
    float usedSpace;
    float usageRatio;
    std::string usageText;
};

extern int fps;
extern bool animate;
extern float scalemax;
extern float Data[100]; // Array to store temperature values
extern int Index;
extern std::string item1;
extern float item2;
extern int item3;
extern std::string graphTitle;
extern std::vector<float> cpuTemperatureData; // Déclaration externe sans initialisation
extern std::vector<float> fanSpeedData;       // Déclaration externe sans initialisation
extern int cpuTemperatureIndex;                // Déclaration externe sans initialisation
extern int fanSpeedIndex;  
extern std::ostringstream stream;
// student TODO : system stats
const char *getOsName();
const char* getComputerName();
// student TODO : memory and processes
int getActiveProcessCount();
std::string getProcessorInfo();
std::string readFile(const std::string& filePath);
std::string getTemperature();
float getFanSpeed(int fanNumber);
std::string getCpuTemperature();
std::string getFan1Speed();
float getCpuUsage();
ProcessCpuInfo getProcessCpuInfo(int pid);
double getSystemUptime();
double calculateCpuUsage(int pid);
std::vector<Process> getProcesses();
std::pair<float, std::pair<int, std::string>> getRamUsage();
std::pair<float, std::pair<float, std::string>> getSwapUsage();
void displaySysInfo();
// student TODO : network
void dateTime();
std::string getIPAddress(const std::string& interface);
void displayTXStatsTable();
void displayRXStatsTable();
std::vector<NetworkInterface> getNetworkInterfaces();
std::vector<NetworkInterface> getNetworkStats();
void readSpecificNetworkDev(const std::string& interface);
void DisplayNetworkUsage();
void displayTabBar();
void progresseBar();
void filterTable();
MemoryInfo getMemoryInfo();
#endif
