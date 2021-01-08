#pragma once
#include "ECS.h"
#include "vector2D.h"
class positionComponent : public Component {
public:
	vector2D position;
	int height = 32;
	int width = 32;
	int scale = 1;
	positionComponent()
	{
		position.Zero();
	}	
	positionComponent(int posx,int posy, int height, int width, int sc)
	{
		position.x = posx;
		position.y = posy;
		scale = sc;
		this->height = height;
		this->width = width;
	}
	positionComponent(float parx, float pary)
	{
		position.x = parx;
		position.y = pary;
	}

	positionComponent(float parx, float pary, int h, int w, int sc)
	{
		position.x = parx;
		position.y = pary;
		height = h;
		width = w;
		scale = sc;
	}
};
