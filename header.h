// To make sure you don't declare the function more than once by including the header multiple times.
#ifndef header_H
#define header_H


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
    int bytes;
    int packets;
    int errs;
    int drop;
    int fifo;
    int frame;
    int compressed;
    int multicast;
};

struct RX
{
    int bytes;
    int packets;
    int errs;
    int drop;
    int fifo;
    int colls;
    int carrier;
    int compressed;
};

struct Process {
    int pid;
    std::string name;
    std::string state;
    float cpuUsage;  // Tu peux ajouter une logique plus tard pour calculer l'usage CPU
    float memUsage;  // Tu peux ajouter une logique plus tard pour calculer l'usage mémoire
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
std::string getIPv4Addresses();
std::pair<float, std::pair<int, std::string>> getRamUsage();
std::pair<float, std::pair<float, std::string>> getDiskUsage();
std::pair<float, std::pair<float, std::string>> getSwapUsage();
// student TODO : network

#endif
