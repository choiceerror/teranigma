#pragma once
#include "enemy.h"

class ballMonster : public enemy
{
public:
	ballMonster();
	~ballMonster();

	virtual HRESULT init(string enemyName, const char* imageName, float x, float y, int idX, int idY, int hp);
	virtual void release();
	virtual void update(float cameraX, float cameraY);
	virtual void render(float viewX, float viewY);

	void move();

public:
	//������ ������ ����
};

