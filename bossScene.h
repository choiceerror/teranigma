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
	GOLD_BULLET,
	BLUE_BULLET,
	RED_BULLET,
};

enum ATTACK_PATTERN
{
	ATTACK_ONE,
	ATTACK_TWO,
	ATTACK_THREE,
	ATTACK_SHOCK, //��ũ����
	ATTACK_NONE,
};

struct tagShockAttack
{
	RECT rc;
	float x, y;
	image* image;
	animation* ani;
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

	tagShockAttack* _shockAttack;

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
	float _attackWorldTime[5];

	int _rndAttack[4]; //3���߿� �Ѱ�����Ÿ��
	int _rndAttackType[3]; //1�� �����߿� �ٸ��� ����Ÿ��.

	float _moveSpeed;
	int _rndMove;
	float _bulletSpeed[3];

public:
	bossScene();
	~bossScene();

	HRESULT init();
	void release();
	void update();
	void render();

	void bossAppear(); //���� ����
	void movePattern(); //���� ��������
	void attackPattern(); //���� �Ѿ˰�������
	void shockAttack(); //�������

	void setWindowsSize(int x, int y, int width, int height);
};

