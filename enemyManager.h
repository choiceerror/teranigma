#pragma once
#include "gameNode.h"
#include "ballMonster.h"
#include "fireMonster.h"
#include "knightMonster.h"
#include "fireMonsterBullet.h"
#include "dungeonMap.h"
#include "boss.h"


enum ENEMYINFO
{
	BALLMONSTER_HP = 50,
	FIREMONSTER_HP = 30,
	KNIGHTMONSTER_HP = 100,
	BOSS_HP = 2000,
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

	//���� ����
	vector<boss*> _vBoss;
	vector<boss*>::iterator _viBoss;

	//���̾���� �Ѿ�
	fireMonsterBullet* _fireMonsterBullet;

	vector<POINTFLOAT> _vEnemyDeadPoint;

	player* _player;

	dungeonMap* _dungeonMap; //������

	int _fireBulletSpeed;

	RECT _playerAttackRc; //������ ���ݷ�Ʈ

	int _rndMove[3];

	bool _isOnce;
	RECT _objectRc; //���͵� ������ �������� �ӽ÷�Ʈ

public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render(float cameraX, float cameraY);

	void updateCollection(); //������Ʈ ���� �Լ�
	void drawAll(float cameraX, float cameraY); //������ �����Լ�
	void setEnemy(); //���ʹ� ����
	void enemyAI(); //���ʹ̵��� AI
	void playerAttackEnemyCollision(); //�÷��̾� ���ݿ� ���ʹ̵��� �����Լ�
	void fireMonsterBulletFireCollision(int num); //���̾���� �Ѿ� �߻��ϰ� �浹�� �Լ�
	void enemyDead(); //���ʹ̵��� ���� �Լ�
	void tileCheckObjectCollision(); //Ÿ�ϰ��� �浹 �Լ�
	void enemyRespon(); //���ʹ̵��� �������� �������� �Լ�


public:
	//������ ������ ����
	vector<ballMonster*> getVBallMonster() { return _vBallMonster; }
	vector<ballMonster*>* setVBallMonster() { return &_vBallMonster; }

	vector<fireMonster*> getVFireMonster() { return _vFireMonster; }
	vector<fireMonster*>* setVFireMonster() { return &_vFireMonster; }

	vector<knightMonster*> getVKnightMonster() { return _vKnightMonster; }
	vector<knightMonster*>* setVKnightMonster() { return &_vKnightMonster; }

	vector<boss*> getVBoss() { return _vBoss; }
	vector<boss*>* setVBOss() { return &_vBoss; }

	vector<POINTFLOAT> getVEnemyDeadPoint() { return _vEnemyDeadPoint; }
	vector<POINTFLOAT>* setVEnemyDeadPoint() { return &_vEnemyDeadPoint; }
	
	fireMonsterBullet* getFireMonsterBullet() { return _fireMonsterBullet; }

	void setPlayerMemoryAddressLink(player* player) { _player = player; }
	void setDungeonMapAddressLink(dungeonMap* map) { _dungeonMap = map; }

};

