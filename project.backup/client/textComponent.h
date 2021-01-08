#include "ECS.h"
#include "TextureManager.h"

class textComponent : public Component
{
	private:
		positionComponent* pos;
		SDL_Texture* tex;
		SDL_Rect rect;
		bool move = false;
	public:
		textComponent(const char* text, int size,bool moving)
		{
			tex = TextureManager::LoadText(text,size);
			move = moving;
		}
		~textComponent()
		{
			SDL_DestroyTexture(tex);
		}
		void init()
		{
			pos = &ent->getComponent<positionComponent>();
			rect.x = pos->position.x;
			rect.y = pos->position.y;
			rect.w = pos->width*pos->scale;
			rect.h = pos->height*pos->scale;
		}
		void Update()
		{
			if(move)
			{
				rect.x = static_cast<int>(pos->position.x);
                		rect.y = static_cast<int>(pos->position.y) - interface::camera.y;
                		rect.w = pos->width*pos->scale;
                		rect.h = pos->height*pos->scale;
			}
		}
		void Draw()
		{
				TextureManager::DrawText(tex, rect);
		}
};
