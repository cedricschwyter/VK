/**
	Declares the LoadingScreen class

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		LoadingScreen.hpp
	@brief		Declaration of the LoadingScreen class
*/
#pragma once
#include <SDL.h>
#undef main		// fixes SDL2 error: Entry point not defined

#include <iostream>
#include <mutex>

#include "SDL_STATUS_CODE.cpp"
#include "Logger.hpp"

class LoadingScreen
{
public:

	bool			close				= false;
	std::mutex		closeMutex;

	/**
		Contains the main loop for the loading screen window

		@return		Returns SDL_SC_SUCCESS on success
	*/
	SDL_STATUS_CODE loop(void);

	/**
		Default Constructor
	*/
	LoadingScreen(void);

private:

	SDL_Window*			window;
	SDL_Surface*		imageSurface;
	SDL_Texture*		background;
	SDL_Renderer*		renderer;

	/**
		Cleans allocated resources and terminates SDL2

		@return		Returns SDL_SC_SUCCESS on success
	*/
	SDL_STATUS_CODE clean(void);

};

