/**
    Implements the LoadingScreen class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         LoadingScreen.cpp
    @brief        Implementation of the LoadingScreen class
*/
#include "LoadingScreen.hpp"
#include "VK.hpp"

SDL_STATUS_CODE LoadingScreen::loop() {


    SDL_Event e;

    while (!close) {

        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT) {

                closeMutex.lock();
                close = true;
                closeMutex.unlock();

            }

        }

        SDL_RenderCopy(
            renderer,
            background,
            NULL,
            NULL
            );

        SDL_RenderPresent(renderer);

    }

    clean();
    logger::log(EVENT_LOG, "Stopping loading screen thread...");

    return SDL_SC_SUCCESS;

}

LoadingScreen::LoadingScreen() {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

        std::string error = SDL_GetError();
        logger::log(ERROR_LOG, "SDL could not initialize! SDL Error: " + error);

    }

    window = SDL_CreateWindow(

        vk::TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1216,
        614,
        SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_BORDERLESS

    );

    renderer = SDL_CreateRenderer(

        window,
        -1,
        SDL_RENDERER_ACCELERATED

    );

    if (window == nullptr) {

        std::string error = SDL_GetError();
        logger::log(ERROR_LOG, "Could not create window: " + error);

    }

    imageSurface = IMG_Load("res/textures/loading_screen/infinity.jpg");
    if (imageSurface == NULL) {

        std::string error = SDL_GetError();
        logger::log(ERROR_LOG, "SDL could not load image! SDL Error: " + error);

    }

    background = SDL_CreateTextureFromSurface(renderer, imageSurface);

    SDL_SetWindowIcon(window, imageSurface);

}

SDL_STATUS_CODE LoadingScreen::clean() {

    SDL_FreeSurface(imageSurface);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);

    imageSurface = nullptr;

    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return SDL_SC_SUCCESS;

}
