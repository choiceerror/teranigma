#pragma once
#include "item.h"

class Potion : public item
{
private:
	int _hpUpPoint;

public:
	Potion();
	~Potion();

	virtual HRESULT init(const char* imageName, float x, float y, int frameX, int frameY, int hpUpPoint);
	virtual void release();
	virtual void update();
	virtual void render(float cameraX,float cameraY);


};

