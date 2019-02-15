#pragma once
#include "gameNode.h"
#include "enemyManager.h"
#include "player.h"
#include "dungeonMap.h"
#include "camera.h"
#include "ItemManager.h"


class dungeon : public gameNode
{
private:
	enemyManager* _enemyManager;
	player* _player;
	camera* _camera;

	dungeonMap* _dungeon;
	ItemManager* _itemManager;


	DWORD* _attribute;

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

