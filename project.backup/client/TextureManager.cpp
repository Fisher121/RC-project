#include "TextureManager.h"
#include <SDL2/SDL_ttf.h>
SDL_Texture* TextureManager::LoadTexture(const char* filename)
{
	SDL_Surface* tempSurface = IMG_Load(filename);
	if (!tempSurface)
	{
		std::cout << "ERROR ON IMG LOAD..." << filename <<' '<< SDL_GetError<<std::endl;
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(interface::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}

void TextureManager::DrawTexture(SDL_Texture* tex,SDL_Rect imgRect,SDL_Rect destRec, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(interface::renderer, tex, &imgRect, &destRec,0,NULL,flip);
}
SDL_Texture* TextureManager::LoadText(const char* text,int size)
{
	SDL_Surface* surfaceMessage;
	TTF_Font* Sans = TTF_OpenFont("assets/fonts/OpenSans-Light.ttf", size);
	if(!Sans)
		printf("Error at font: %s\n",TTF_GetError());

        SDL_Color color = {0,0,0};
	if(Sans !=NULL)
        	surfaceMessage = TTF_RenderText_Solid(Sans, text, color);
	if( surfaceMessage == NULL )
    	{
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    	}
	
        SDL_Texture* Message = SDL_CreateTextureFromSurface(interface::renderer, surfaceMessage);
	if( Message == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
	SDL_FreeSurface(surfaceMessage);
	
	return Message;
}
void TextureManager::DrawText(SDL_Texture* tex,SDL_Rect rect)
{
	SDL_RenderCopy(interface::renderer, tex, NULL, &rect);
}
