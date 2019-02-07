#pragma once
#include "enemy.h"
enum MOVETYPE
{
	BASIC_MOVE_TYPE, //�⺻ �����ӻ���
	FOLLOW_MOVE_TYPE, //���󰡴� ������ ����
};

class ballMonster : public enemy
{
private:
	int _rndDirection; //�������� ������ȯ
	float _directionTime, _directionWorldTime;
	float _moveAngle;
	int _moveType;
public:
	ballMonster();
	~ballMonster();

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

	void setX(float x) { _enemy.x = x; }
	void setY(float y) { _enemy.y = y; }
	void setViewX(float viewX) { _enemy.viewX = viewX; }
	void setViewY(float viewY) { _enemy.viewY = viewY; }

	RECT getRangeRect() { return _enemy.rangeRc; }
	ENEMYDIRECTION getDirection() { return _enemy.direction; }
	void setDirection(ENEMYDIRECTION direction) { _enemy.direction = direction; }
	float getMoveAngle() {return _moveAngle;}
};

