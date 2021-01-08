#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include "client.h"
#include "ECS.h"
#include "positionComponent.h"
class interface
{
	private:
		int width, height;
		SDL_Window* window;
		bool state;
		client Client;
		int press = false;
	public:
		interface(int w,int h);
		~interface(){};
		bool getState();
		int init(const char* titlu);
		void loop();
		void render();
		void clean();
		void loadHome();
		void loadLogin();
		void eventHandler();
		static SDL_Renderer* renderer;
		static SDL_Rect camera;
		static SDL_Event event;
		static int mouseX;
		static int mouseY;
		enum groupLabels : std::size_t
		{
			groupButton,
			groupBackground,
			groupText,
		};
};
