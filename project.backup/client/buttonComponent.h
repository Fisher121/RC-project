#include "ECS.h"
#include "positionComponent.h"

class buttonComponent : public Component
{
	private :
		bool state;
		positionComponent *pos;
	public:
		void init()
		{
			pos = &ent->getComponent<positionComponent>();
			state = false;
		}
		void Body()
		{
			std::cout<<"Hello\n";
		}
		void Update()
		{
			if(interface::mouseX >=pos->position.x && interface::mouseY >= pos->position.y && interface::mouseX <= pos->position.x + pos->scale*pos->width && interface::mouseY <=pos->position.y + pos->scale*pos->height )
			{
				state = !state;
				this->Body();
			}
		}
};
