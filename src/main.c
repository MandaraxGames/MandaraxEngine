#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <SDL3/SDL.h>

// Your custom entry point defined in build.bat [/ENTRY:RawEntryPoint]
void RawEntryPoint(void) {
    // 1. Initialize SDL Video subsystem
    // We use SDL_SetMainReady() because we aren't using SDL's macro-main
    SDL_SetMainReady();
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // SDL3 returns false on failure. Win32 ExitProcess is our "return"
        ExitProcess(1);
    }

    // 2. Create a Window
    SDL_Window* window = SDL_CreateWindow("No-CRT SDL3", 640, 480, 0);
    if (!window) {
        SDL_Quit();
        ExitProcess(1);
    }

    // 3. Simple Event Loop
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }
        
        // Use SDL's built-in delay to keep CPU usage down
        SDL_Delay(16); 
    }

    // 4. Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    // Explicitly exit the process
    ExitProcess(0);
}