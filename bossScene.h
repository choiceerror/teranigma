#pragma once
#include "gameNode.h"
#include "enemyManager.h"
#include "player.h"
#include "dungeonMap.h"
#include "camera.h"
#include "ItemManager.h"

enum ONCE //�ѹ��� �ޱ�����.
{
	LINEAR,
	CAMERA_LINEAR,
	BOSS_APPEAR,
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
	bool _isOnce[3]; //������Ʈ�κп� �ѹ��� �ޱ�����.
	
	POINTFLOAT _goal;
	float _goalAngle;
	float _elapsedTime;
	float _goalDistance;
	float _moveSpeed;

	POINTFLOAT _moveGoal[10];
	float _moveGoalAngle;

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

