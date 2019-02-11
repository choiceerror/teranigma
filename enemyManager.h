#pragma once
#include "gameNode.h"
#include "ballMonster.h"
#include "fireMonster.h"
#include "knightMonster.h"
#include "fireMonsterBullet.h"

//���� ����
enum DUNGEON_FLOOR
{
	FIRST_FLOOR, //1��
	SECOND_FLOOR, //2��
	THIRD_FLOOR, //3��
};

class player;

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

	player* _player;

	DUNGEON_FLOOR _dungeonFloor; //���� ����
	int _fireBulletSpeed;

public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void updateCollection(); //������Ʈ ���� �Լ�
	void enemyDraw(); //���ʹ̵� ������ �����Լ�
	void setEnemy(); //���ʹ� ����
	void enemyAttackPlayerCollision(); //���ʹ̵��� ������ �÷��̾�� �浹�� �Լ�
	void fireMonsterBulletFire(int i); //���̾���� �Ѿ� �߻��� �Լ�
	void enemyDead(); //���ʹ̵��� ���� �Լ�
	void enemyRespon(); //���ʹ̵��� �������� �������� �Լ�

public:
	//������ ������ ����
	vector<ballMonster*> getVBallMonster() {return _vBallMonster;}
	vector<ballMonster*>* setVBallMonster() {return &_vBallMonster;}

	vector<fireMonster*> getVFireMonster() {return _vFireMonster;}
	vector<fireMonster*>* setVFireMonster() {return &_vFireMonster;}

	vector<knightMonster*> getVKnightMonster() {return _vKnightMonster;}
	vector<knightMonster*>* setVKnightMonster() {return &_vKnightMonster;}

	void setPlayerMemoryAddressLink(player* player) {_player = player;}

};

