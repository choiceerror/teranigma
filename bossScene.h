#pragma once
#include "gameNode.h"
#include "enemyManager.h"
#include "player.h"
#include "dungeonMap.h"
#include "camera.h"
#include "ItemManager.h"
#include "fireMonsterBullet.h"

enum ONCE //�ѹ��� �ޱ�����.
{
	LINEAR_APPEAR,
	CAMERA_LINEAR,
	BOSS_APPEAR,
	WORLDTIME,
};

enum class BULLET_PATTERN
{
	NONE,
	GOLD_BULLET,
	BLUE_BULLET,
	RED_BULLET,
};

enum ATTACK_PATTERN
{
	ATTACK_NONE,
	ATTACK_ONE,
	ATTACK_TWO,
	ATTACK_THREE,

};

class bossScene : public gameNode
{
private:
	enemyManager* _enemyManager;
	player* _player;
	camera* _camera;

	dungeonMap* _dungeonBossMap;
	ItemManager* _itemManager;
	bossBullet* _bossBullet;
	BULLET_PATTERN _bulletPattern; //�Ѿ� ������ ����
	ATTACK_PATTERN _attackPattern; //��������

	float _worldTime;
	bool _isBossAppear; //���� ������� �Ǻ����� �Ұ�
	bool _isOnce[4]; //������Ʈ�κп� �ѹ��� �ޱ�����.
	
	POINTFLOAT _appearGoal;
	float _goalAngle;
	float _elapsedTime;
	float _goalDistance;
	float _appearSpeed; //���� ���ǵ�

	POINTFLOAT _moveGoal[4];

	float _goalX;
	float _goalY;
	float _moveWorldTime;
	float _rndMoveWorldTime;
	float _attackWorldTime[4];
	float _bulletWorldTime;

	int _rndAttack; //3���߿� �Ѱ�����Ÿ��
	int _rndAttackType; //1�� �����߿� �ٸ��� ����Ÿ��.

	float _moveSpeed;
	int _rndMove;
	float _bulletSpeed;

public:
	bossScene();
	~bossScene();

	HRESULT init();
	void release();
	void update();
	void render();

	void bossAppear(); //���� ����
	void movePattern(); //���� ��������
	void attackPattern(); //���� ��������

	void setWindowsSize(int x, int y, int width, int height);
};

