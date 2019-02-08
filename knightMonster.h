#pragma once
#include "enemy.h"

class knightMonster : public enemy
{
public:
	knightMonster();
	~knightMonster();

	virtual HRESULT init(string enemyName, const char* imageName, float x, float y, int idX, int idY, int hp);
	virtual void release();
	virtual void update(float cameraX, float cameraY);
	virtual void render(float viewX, float viewY);

	void move();

	static void cbAttack(void* obj); //������ �ٸ�������� ���ư��� ���� �ݹ��Լ�

public:
	//������ ������ ����
	animation* getMotion() { return _enemy.motion; }
	void setMotion(animation* motion) { _enemy.motion = motion; }

	string getName() { return _enemy.name; }
};

