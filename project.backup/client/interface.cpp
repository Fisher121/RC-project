#include "interface.h"
#include "ECS.h"
#include "textureComponent.h"
#include "buttonComponent.h"
#include "textComponent.h"

SDL_Renderer* interface::renderer;
SDL_Rect interface::camera;
SDL_Event interface::event;
int interface::mouseX = -1;
int interface::mouseY = -1;
manager man;

interface::interface(int w,  int h)
{
	width = w;
	height = h;
	state = true;
	camera ={0,0,w,h};
}
int interface::init(const char* title)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);
		if(window == NULL)
		{
			std::cout<<"Eroare la crearea ferestrei!\n";
			state = false;
		}
		renderer = SDL_CreateRenderer(window,-1,0);
		if(renderer == NULL)
		{
			std::cout<<"Eroare la crearea rendererului!\n";
			state = false;
		}
		else
		{
			SDL_SetRenderDrawColor(renderer,255,255,255,255);
		}
		if(TTF_Init() < 0) {
 		   std::cout << "Couldn't initialize TTF lib: " << TTF_GetError() << std::endl;
		   state = false;
		}
	}
	else
	{
		std::cout<<"ERROR AT INIT EVERYTHING\n";
		state = false;
	}
	Client.init();
	/*
	std::vector<str> bookVector;
	Client.getBook("1",bookVector);
	int count=0;
	char out[77];
	bzero(out,77);
	for(auto &c : bookVector)
		for(int i=0;i<=strlen(c.buffer)-1;i++)
			if(count < 75 && c.buffer[i] !='\n')
			{
				out[count] = c.buffer[i];
				count ++;
			}
			else
			{
				out[count] = '\n';
				std::cout<<out;
				bzero(out,77);
				count = 0;
			}
			*/
	this->loadHome();
}
void interface::loadHome()
{
	int w=width*0.9,h=height*0.03,s = 2;
	Entity *button = man.createEntity();
	button->addComponent<positionComponent>(w,h,32,32,s);
	button->addComponent<textureComponent>("assets/button.png");
	button->addComponent<buttonComponent>();
	button->addGroup(groupButton);

	Entity *text = man.createEntity();
	text->addComponent<positionComponent>(100,100,200,200,1);
	text->addComponent<textComponent>("text",200,0);
	text->addGroup(groupButton);


}
void interface::eventHandler()
{
	SDL_PollEvent(&event);
	switch(event.type)
	{
		case SDL_QUIT:
			state = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT && press == false)
				{
					SDL_GetMouseState(&mouseX,&mouseY);
					press = true;
				}
			break;	
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT)
                                {
                                        press = false;
                                }

			break;
	}
}
void interface::loop()
{
	this->eventHandler();
	man.refresh();
	man.Update();
	mouseX = -1;
	mouseY = -1;
}
bool interface::getState()
{
	return state;
}
auto& background(man.getGroup(interface::groupBackground));
auto& buttons(man.getGroup(interface::groupButton));
auto& texts(man.getGroup(interface::groupText));

void interface::render()
{
	SDL_RenderClear(renderer);

	for(auto& t : background)
		t->Draw();
	for(auto& t : buttons)
		t->Draw();
	for(auto& t : buttons)
                t->Draw();
	SDL_RenderPresent(renderer);
}

void interface::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
