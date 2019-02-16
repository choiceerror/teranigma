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
	animation* ani;
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

	void move(); //�̵�

public:
	//������ ������
	vector<tagFireBullet> getVFireBullet() { return _vFireBullet; }
	vector<tagFireBullet>* setVFireBullet() { return &_vFireBullet; }

};

class bossBullet : public gameNode
{
private:
	enum class BULLETATTACKPATTERN
	{
		PATTERN_ONE,
		PATTERN_TWO,
	};

	vector<tagFireBullet> _vBossBullet;
	vector<tagFireBullet>::iterator _viBossBullet;

	const char* _imageName;
	int _range;
	int _bulletMax;
	animation* _tempAni;
	BULLETATTACKPATTERN _pattern;

public:
	HRESULT init(const char* imageName, float range, int bulletMax);
	void release();
	void update();
	void render(float cameraX, float cameraY);

	void bossFire(float x, float y, float angle, float speed); //�������� �Ѿ˹߻�

	void move(); //�̵�

	BULLETATTACKPATTERN getPattern() { return _pattern; }
	void setPattern(BULLETATTACKPATTERN pattern) { _pattern = pattern; }
};

