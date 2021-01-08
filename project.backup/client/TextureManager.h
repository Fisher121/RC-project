#pragma once
#include "interface.h"
class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* filename);
	static void DrawTexture(SDL_Texture* tex, SDL_Rect imgRect, SDL_Rect destRec,SDL_RendererFlip flip);
	static void DrawText(SDL_Texture* tex,SDL_Rect rect);
	static SDL_Texture* LoadText(const char* text,int size);
};
