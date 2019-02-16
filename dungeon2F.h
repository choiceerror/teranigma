#pragma once
#include "gameNode.h"
#include "enemyManager.h"
#include "player.h"
#include "dungeonMap.h"
#include "camera.h"
#include "ItemManager.h"

class dungeon2F : public gameNode
{
private:
	enemyManager* _enemyManager;
	player* _player;
	camera* _camera;

	dungeonMap* _dungeon2F;
	ItemManager* _itemManager;

	int _rndItemDrop; //������ �������� ����Ұ�
	int _rndItemTypeDrop; //������ Ÿ�� �������� ����Ұ�.
public:
	dungeon2F();
	~dungeon2F();

	HRESULT init();
	void release();
	void update();
	void render();

	void itemRandomDrop(); //���� �����۵��
	void playerItemGet(); //�÷��̾� ������ ��� �Լ�

	void setWindowsSize(int x, int y, int width, int height);
};

