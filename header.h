// To make sure you don't declare the function more than once by including the header multiple times.
#ifndef header_H
#define header_H


#include <cstdio>
#include <string>
#include <array>
#include <memory>      // Pour std::shared_ptr
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
extern int fps;
extern int fps;
extern bool animate;
extern float scalemax;
extern float temperatureData[100]; // Array to store temperature values
extern int temperatureIndex;
// student TODO : system stats
string CPUinfo();
const char *getOsName();
const char* getComputerName();
// student TODO : memory and processes
int getActiveProcessCount();
std::string getProcessorInfo();
std::string readFile(const std::string& filePath);
std::string getCPUInfo();
std::string getFanSpeed();
std::string getProcessorTemperature();
// student TODO : network

#endif
