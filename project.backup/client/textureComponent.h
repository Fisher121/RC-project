#pragma once
#include "ECS.h"
#include <SDL2/SDL.h>
#include "TextureManager.h"

class textureComponent :public Component {
private:
	positionComponent* pos;
	SDL_Texture* tex;
	SDL_Rect imgRect, destRec;

public:
	SDL_RendererFlip texFlip = SDL_FLIP_NONE;
	textureComponent(const char* filename)
	{
		setTex(filename);
	}
	void setTex(const char* filename)
	{
		tex = TextureManager::LoadTexture(filename);
	}
	~textureComponent()
	{
		SDL_DestroyTexture(tex);
	}
	void init()
	{
		imgRect.x = 0;
		imgRect.y = 0;
		pos = &ent->getComponent<positionComponent>();
		imgRect.w = pos->width;
		imgRect.h = pos->height;
		
	}
	void Update()
	{
		destRec.x = static_cast<int>(pos->position.x);
		destRec.y = static_cast<int>(pos->position.y) - interface::camera.y;
		destRec.w = pos->width*pos->scale;
		destRec.h = pos->height*pos->scale;

	}
	void changeTexture(const char* file)
	{
		tex = TextureManager::LoadTexture(file);
	}
	void Draw() {
		TextureManager::DrawTexture(tex, imgRect, destRec,texFlip);
	}
};
