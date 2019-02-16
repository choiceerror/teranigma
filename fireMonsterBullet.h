#pragma once
#include "gameNode.h"

struct tagFireBullet
{
	image* image;
	RECT rc;
	RECT tileCollisionRc;
	float x, y;
	int idX, idY;
	float angle;
	float radius;
	float speed;
	float fireX, fireY;
	float viewX, viewY;
	bool isCollision;
	animation* fireBulletAni;
	POINT tileIndex[8]; //Ÿ���ε��� �Ǻ�
};

class fireMonsterBullet : public gameNode
{
private:
	vector<tagFireBullet> _vFireBullet;
	vector<tagFireBullet>::iterator _viFireBullet;

	const char* _imageName;
	float _range; //�Ÿ�
	int _fireBulletMax; 

public:
	fireMonsterBullet();
	~fireMonsterBullet();

	HRESULT init(const char* imageName, float range, int fireBulletMax);
	void release();
	void update();
	void render(float cameraX, float cameraY);

	void fire(float x, float y, float angle, float speed); //���̾���� �Ѿ˹߻�
	void bossBulletFire(float x, float y, float angle, float speed);
	
	void move(); //�̵�

public:
	//������ ������
	vector<tagFireBullet> getVFireBullet() {return _vFireBullet;}
	vector<tagFireBullet>* setVFireBullet() {return &_vFireBullet;}

	
};

