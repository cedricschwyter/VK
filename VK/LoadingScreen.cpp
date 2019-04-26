/**
	Implements the LoadingScreen class

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		LoadingScreen.cpp
	@brief		Implementation of the LoadingScreen class
*/
#include "LoadingScreen.hpp"
#include "VK.hpp"

SDL_STATUS_CODE LoadingScreen::loop() {


	SDL_Event e;
	SDL_Rect rect;
	rect.x = 100;
	rect.y = 500;
	rect.w = 400;
	rect.h = 20;

	while (!close) {

		while (SDL_PollEvent(&e)) {

			if (e.type == SDL_QUIT) {

				vk::engine.closeLoadingScreen.lock();
				close = true;
				vk::engine.closeLoadingScreen.unlock();

			}

		}

		SDL_Rect rectProgress;
		rectProgress.x = 100;
		rectProgress.y = 500;
		rectProgress.w = static_cast< int >(vk::engine.loadingProgress * 400);
		rectProgress.h = 20;

		SDL_RenderCopy(
			renderer,
			background,
			NULL,
			NULL
			);

		SDL_SetRenderDrawColor(
			renderer,
			255,
			255,
			255,
			SDL_ALPHA_OPAQUE
			);

		SDL_RenderFillRect(renderer, &rect);

		SDL_SetRenderDrawColor(
			renderer,
			150,
			82,
			104,
			SDL_ALPHA_OPAQUE
			);

		SDL_RenderDrawRect(renderer, &rect);
		SDL_RenderFillRect(renderer, &rectProgress);
		SDL_RenderPresent(renderer);

	}

	clean();

	return SDL_SC_SUCCESS;

}

LoadingScreen::LoadingScreen() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

		std::string error = SDL_GetError();
		logger::log(ERROR_LOG, "SDL could not initialize! SDL Error: " + error);

	}

	window = SDL_CreateWindow(

		vk::engine.TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1300,
		500,
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

	imageSurface = SDL_LoadBMP("res/textures/loading_screen/vulkan.bmp");
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
	SDL_Quit();

	return SDL_SC_SUCCESS;

}
