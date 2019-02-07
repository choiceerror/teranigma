#pragma once
#include "gameNode.h"
#include "ballMonster.h"
#include "fireMonster.h"
#include "knightMonster.h"
#include "fireMonsterBullet.h"

class enemyManager : public gameNode
{
private:
	//������ ����
	vector<ballMonster*> _vBallMonster;
	vector<ballMonster*>::iterator _viBallMonster;

	//���̾� ���� ����
	vector<fireMonster*> _vFireMonster;
	vector<fireMonster*>::iterator _viFireMonster;

	//����Ʈ ���� ����
	vector<knightMonster*> _vKnightMonster;
	vector<knightMonster*>::iterator _viKnightMonster;

	//���̾���� �Ѿ�
	fireMonsterBullet* _fireMonsterBullet;
public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void updateCollection(); //������Ʈ ���� �Լ�
	void setEnemy(); //���ʹ� ����
	void fireMonsterBulletFire(); //���̾���� �Ѿ� �߻��� �Լ�

public:
	//������ ������ ����
	vector<ballMonster*> getVBallMonster() {return _vBallMonster;}
	vector<ballMonster*>* setVBallMonster() {return &_vBallMonster;}

	vector<fireMonster*> getVFireMonster() {return _vFireMonster;}
	vector<fireMonster*>* setVFireMonster() {return &_vFireMonster;}

	vector<knightMonster*> getVKnightMonster() {return _vKnightMonster;}
	vector<knightMonster*>* setVKnightMonster() {return &_vKnightMonster;}
};

