#pragma once
#include "gameNode.h"
#include "enemyManager.h"
#include "player.h"
#include "dungeonMap.h"
#include "camera.h"
#include "ItemManager.h"
#include "ClockFadeOut.h"


class dungeon : public gameNode
{
private:
	enemyManager* _enemyManager;
	player* _player;
	camera* _camera;
	ClockFadeOut* _clockFade;

	dungeonMap* _dungeon;
	ItemManager* _itemManager;

	int _rndItemDrop; //������ �������� ����Ұ�
	int _rndItemTypeDrop; //������ Ÿ�� �������� ����Ұ�.

public:
	dungeon();
	~dungeon();

	HRESULT init();
	void release();
	void update();
	void render();

	void itemRandomDrop(); //���� �����۵��
	void playerItemGet(); //�÷��̾� ������ ��� �Լ�

	void setWindowsSize(int x, int y, int width, int height);


};

