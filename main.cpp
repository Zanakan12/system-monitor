#include "header.h"


/*
NOTE : You are free to change the code as you wish, the main objective is to make the
       application work and pass the audit.

       It will be provided the main function with the following functions :

       - `void systemWindow(const char *id, ImVec2 size, ImVec2 position)`
            This function will draw the system window on your screen
       - `void memoryProcessesWindow(const char *id, ImVec2 size, ImVec2 position)`
            This function will draw the memory and processes window on your screen
       - `void networkWindow(const char *id, ImVec2 size, ImVec2 position)`
            This function will draw the network window on your screen
*/

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h> // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h> // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h> // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h> // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE      // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h> // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE        // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h> // Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif
#include <sstream>
#include <iomanip> // Pour std::setprecision
#include <string>
#include <cstdlib> // Pour atoi

#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib> // Pour atoi

// Déclaration globale
const int maxDataPoints = 100;  // Taille maximale du tableau
float Data[maxDataPoints] = {0}; // Tableau pour stocker les données
int Index = 0;                   // Index circulaire pour les données
int dataCount = 0;               // Compteur pour le nombre de points de données
int fps = 60;                    // Taux de rafraîchissement
bool animate = true;             // Animation activée
float scalemax = 100.0f;         // Échelle maximale
std::string item1;               // Utilisation de la variable item1
float item2;
std::string graphTitle;
std::ostringstream stream;

// systemWindow, affichage des informations système
void systemWindow(const char *id, ImVec2 size, ImVec2 position) {
    ImGui::Begin(id);
    ImGui::SetWindowSize(id, size);
    ImGui::SetWindowPos(id, position);

    ImGui::Text("Operating system: %s", getOsName());
    ImGui::Text("Computer name: %s", getComputerName());
    ImGui::Text("Logged in user: %s", getenv("USER") ? getenv("USER") : getenv("USERNAME"));
    ImGui::Text("Number of working processes: %d", getActiveProcessCount());
    ImGui::Text("Processor: %s", getProcessorInfo().c_str());

    ImGui::Separator();

    if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None)) {

        if (ImGui::BeginTabItem("CPU")) {
            usleep(1000000 / fps);
            // Réinitialiser les données à chaque changement d'onglet
            if (dataCount == 0) {
                std::fill(std::begin(Data), std::end(Data), 0.0f); // Ne pas effacer si déjà rempli
            }

            item1 = "CPU";
            float cpuUsage = getCpuUsage();
            if (cpuUsage >= 0) {
                //std::cout << "Utilisation du CPU : " << cpuUsage << "%" << std::endl; // debug
            }
            item2 = Data[Index] = cpuUsage;
            Index = (Index + 1) % maxDataPoints;

            if (dataCount < maxDataPoints) {
                dataCount++;  // Augmenter le nombre de points disponibles
            }

            // Réinitialisation du flux
            stream.str(""); // Réinitialise le flux
            stream << std::fixed << std::setprecision(2) << item2;
            graphTitle = item1 + ": " + stream.str() + "%";

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Fan")) {
            usleep(1000000 / fps);
            // Réinitialiser les données à chaque changement d'onglet
            if (dataCount == 0) {
                std::fill(std::begin(Data), std::end(Data), 0.0f); // Ne pas effacer si déjà rempli
            }

            item1 = "Fan";
            ImGui::Text("Fan status :");
            ImGui::Text("");
            if (getFan1Speed() == "0") {
                ImGui::Text("Status : Disable");
            } else {
                ImGui::Text("Status : Enable");
            }
            ImGui::Text("Speed: %s RPM", getFan1Speed().c_str());

            int number = atoi(getFan1Speed().c_str());
            ImGui::Text("Level : %d ", number / 1000);

            item2 = Data[Index] = float(number)*100/4900; // specifique param for this computer.
            Index = (Index + 1) % maxDataPoints;

            if (dataCount < maxDataPoints) {
                dataCount++;  // Augmenter le nombre de points disponibles
            }

            // Réinitialisation du flux
            stream.str(""); // Réinitialise le flux
            stream << std::fixed << std::setprecision(2) << item2*4900/100;
            graphTitle = item1 + ": " + getFan1Speed().c_str() + " RPM";

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Thermal")) {
            // Réinitialiser les données à chaque changement d'onglet
            if (dataCount == 0) {
                std::fill(std::begin(Data), std::end(Data), 0.0f); // Ne pas effacer si déjà rempli
            }
            float temperature = std::stof(getTemperature()) / 1000.0f; // Conversion en Celsius
            ImGui::Text("Temperature : %.f°C", temperature);
            item1 = "Temperature";
            item2 = Data[Index] = temperature;
            Index = (Index + 1) % maxDataPoints;

            if (dataCount < maxDataPoints) {
                dataCount++;  // Augmenter le nombre de points disponibles
            }

            // Réinitialisation du flux
            stream.str(""); // Réinitialise le flux
            stream << std::fixed << std::setprecision(2) << item2;
            graphTitle = item1 + ": " + stream.str() + "°C";

            ImGui::EndTabItem();
        }

        ImGui::Checkbox("Animate", &animate);
        if (animate) {
            ImGui::SliderInt("FPS", &fps, 1, 60);
            ImGui::SliderFloat("Scale Max", &scalemax, 1, 100);
            // Affichage du graphique avec les données accumulées
            usleep(1000000 / fps);
            ImGui::PlotLines(item1.c_str(), Data, dataCount, Index, graphTitle.c_str(), 0.0f, scalemax, ImVec2(0, 180));
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

// memoryProcessesWindow, display information for the memory and processes information
void memoryProcessesWindow(const char *id, ImVec2 size, ImVec2 position)
{
    ImGui::Begin(id);
    ImGui::SetWindowSize(id, size);
    ImGui::SetWindowPos(id, position);
    
    
    std::pair<float, std::string> ramData = getRamUsage();
    std::pair<float, std::string> swapData = getSwapUsage();
    std::pair<float, std::string> diskData = getDiskUsage();

    ImGui::Text("Physical Memory (RAM):");
    ImGui::ProgressBar(ramData.first, ImVec2(0.0f, 0.0f), ramData.second.c_str());
    ImGui::Text("0 Go                                                 %f Go", ramData.second);

    ImGui::Text("Virtual Memory (SWAP):");
    ImGui::ProgressBar(swapData.first, ImVec2(0.0f, 0.0f), swapData.second.c_str());

    
    ImGui::Text("Disk Usage:");
    ImGui::ProgressBar(diskData.first, ImVec2(0.0f, 0.0f), diskData.second.c_str());
    
    

    ImGui::End();
}

// network, display information network information
void networkWindow(const char *id, ImVec2 size, ImVec2 position)
{
    ImGui::Begin(id);
    ImGui::SetWindowSize(id, size);
    ImGui::SetWindowPos(id, position);
    // student TODO : add code here for the network information
    ImGui::ShowDemoWindow();
    ImGui::End();
}

// Main code
int main(int, char **)
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char *name) { return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // render bindings
    ImGuiIO &io = ImGui::GetIO();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // background color
    // note : you are free to change the style of the application
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        {
            ImVec2 mainDisplay = io.DisplaySize;
            memoryProcessesWindow("== Memory and Processes ==",
                                  ImVec2((mainDisplay.x / 2) - 20, (mainDisplay.y / 2) + 30),
                                  ImVec2((mainDisplay.x / 2) + 10, 10));
            // --------------------------------------
            systemWindow("== System ==",
                         ImVec2((mainDisplay.x / 2) - 10, (mainDisplay.y / 2) + 30),
                         ImVec2(10, 10));
            // --------------------------------------
            networkWindow("== Network ==",
                          ImVec2(mainDisplay.x - 20, (mainDisplay.y / 2) - 60),
                          ImVec2(10, (mainDisplay.y / 2) + 50));
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
