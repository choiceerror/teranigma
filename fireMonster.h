#pragma once
#include "enemy.h"
class fireMonster : public enemy
{
public:
	fireMonster();
	~fireMonster();

	virtual HRESULT init(string enemyName, const char* imageName, float x, float y, int idX, int idY);
	virtual void release();
	virtual void update(float cameraX, float cameraY);
	virtual void render(float viewX, float viewY);

	void move();

public:
	//������ ������ ����
	float getX() { return _enemy.x; }
	float getY() { return _enemy.y; }
	float getViewX() { return _enemy.viewX; }
	float getViewY() { return _enemy.viewY; }
};

