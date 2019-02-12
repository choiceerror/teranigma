#pragma once
#include "enemy.h"

class ballMonster : public enemy
{
private:
	bool _isOne;
public:
	ballMonster();
	~ballMonster();

	virtual HRESULT init(string enemyName, const char* imageName, float x, float y, int idX, int idY, int hp);
	virtual void release();
	virtual void update();
	virtual void render(float cameraX, float cameraY);

	void move();

public:
	//접근자 설정자 모음

};

