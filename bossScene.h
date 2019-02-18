#pragma once
#include "gameNode.h"
#include "enemyManager.h"
#include "player.h"
#include "dungeonMap.h"
#include "camera.h"
#include "ItemManager.h"

enum ONCE //�ѹ��� �ޱ�����.
{
	LINEAR_APPEAR,
	CAMERA_LINEAR,
	BOSS_APPEAR,
	WORLDTIME,

};

class bossScene : public gameNode
{
private:
	enemyManager* _enemyManager;
	player* _player;
	camera* _camera;

	dungeonMap* _dungeonBossMap;
	ItemManager* _itemManager;

	float _worldTime;
	bool _isBossAppear; //���� ������� �Ǻ����� �Ұ�
	bool _isOnce[4]; //������Ʈ�κп� �ѹ��� �ޱ�����.
	
	POINTFLOAT _goal;
	float _goalAngle;
	float _elapsedTime;
	float _goalDistance;
	float _appearSpeed; //���� ���ǵ�

	POINTFLOAT _moveGoal[4];

	float _goalX;
	float _goalY;
	float _moveWorldTime;
	float _rndMoveWorldTime[2];

	float _moveSpeed;
	int _rndMove;


public:
	bossScene();
	~bossScene();

	HRESULT init();
	void release();
	void update();
	void render();

	void bossAppear(); //���� ����
	void movePattern(); //���� ��������

	void setWindowsSize(int x, int y, int width, int height);
};

