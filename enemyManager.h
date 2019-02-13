#pragma once
#include "gameNode.h"
#include "ballMonster.h"
#include "fireMonster.h"
#include "knightMonster.h"
#include "fireMonsterBullet.h"
#include "dungeonMap.h"

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

	dungeonMap* _dungeonMap; //������

	DUNGEON_FLOOR _dungeonFloor; //���� ����
	int _fireBulletSpeed;
	float _moveWorldTime;

	RECT _attackRect; //������ ���ݷ�Ʈ

public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render(float cameraX, float cameraY);

	void updateCollection(); //������Ʈ ���� �Լ�
	void enemyDraw(float cameraX, float cameraY); //���ʹ̵� ������ �����Լ�
	void setEnemy(); //���ʹ� ����
	void enemyAI(); //���ʹ̵��� AI
	void playerAttackEnemyCollision(); //�÷��̾� ���ݿ� ���ʹ̵��� �����Լ�
	void fireMonsterBulletFire(int i); //���̾���� �Ѿ� �߻��� �Լ�
	void enemyDead(); //���ʹ̵��� ���� �Լ�
	void enemyRespon(); //���ʹ̵��� �������� �������� �Լ�
	void tileCheckObjectCollision(); //Ÿ�ϰ��� �浹 �Լ�

public:
	//������ ������ ����
	vector<ballMonster*> getVBallMonster() { return _vBallMonster; }
	vector<ballMonster*>* setVBallMonster() { return &_vBallMonster; }

	vector<fireMonster*> getVFireMonster() { return _vFireMonster; }
	vector<fireMonster*>* setVFireMonster() { return &_vFireMonster; }

	vector<knightMonster*> getVKnightMonster() { return _vKnightMonster; }
	vector<knightMonster*>* setVKnightMonster() { return &_vKnightMonster; }

	void setPlayerMemoryAddressLink(player* player) { _player = player; }
	void setDungeonMapAddressLink(dungeonMap* map) { _dungeonMap = map; }

};

