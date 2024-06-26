#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <SDL3/SDL_vulkan.h>

#include "engine.hpp"
#include <cmath>

struct AppContext {
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_bool app_quit = SDL_FALSE;
};

int SDL_Fail(){
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return -1;
}

int SDL_AppInit(void** appstate, int argc, char* argv[]) {
    // init the library, here we make a window, so we only need the Video capabilities.
    if (SDL_Init(SDL_INIT_VIDEO)){
        return SDL_Fail();
    }

    // create a window
    SDL_Window* window = SDL_CreateWindow("2g43s", 640, 360, SDL_WINDOW_RESIZABLE);
    if (!window){
        return SDL_Fail();
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer){
        return SDL_Fail();
    }

    // print some information about the window
    SDL_ShowWindow(window);
    {
        int width, height, bbwidth, bbheight;
        SDL_GetWindowSize(window, &width, &height);
        SDL_GetWindowSizeInPixels(window, &bbwidth, &bbheight);
        SDL_Log("Window size: %ix%i", width, height);
        SDL_Log("Backbuffer size: %ix%i", bbwidth, bbheight);
        if (width != bbwidth){
            SDL_Log("This is a highdpi environment.");
        }
    }

    // set up the application data
    *appstate = new AppContext{
            window,
            renderer,
    };

    SDL_Log("Application started successfully!");

    return 0;
}

//App events manager
int SDL_AppEvent(void *appstate, const SDL_Event* event) {
    auto* app = (AppContext*)appstate;

    if (event->type == SDL_EVENT_QUIT) {
        app->app_quit = SDL_TRUE;
    }

    return 0;
}

//Main app loop
int SDL_AppIterate(void *appstate) {
    auto* app = (AppContext*)appstate;


    auto time = SDL_GetTicks() / 1000.0f;
    auto red = (std::sin(time) + 1) / 2.0 * 255;

    // draw a color
    SDL_SetRenderDrawColor(app->renderer, 0, red, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(app->renderer);
    SDL_RenderPresent(app->renderer);

    return app->app_quit;
}

//Quit app and destroy all the things
void SDL_AppQuit(void* appstate) {
    auto* app = (AppContext*)appstate;
    if (app) {
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        delete app;
    }

    SDL_Quit();
    SDL_Log("Application quit successfully!");
}