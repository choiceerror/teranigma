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

	RECT _temp;
	RECT _dungeonDown;
	RECT _dungeonUp;

	int _rndItemDrop; //������ �������� ����Ұ�
	int _rndItemTypeDrop; //������ Ÿ�� �������� ����Ұ�.

	float _worldTime;
	float _once;
	int _alphaValue;
	bool _changeScene;

public:
	dungeon();
	~dungeon();

	HRESULT init();
	void release();
	void update();
	void render();

	void dungeonChange();
	void playerSceneSave();
	void playerSceneLoad();

	void itemRandomDrop(); //���� �����۵��
	void playerItemGet(); //�÷��̾� ������ ��� �Լ�

	void setWindowsSize(int x, int y, int width, int height);
};

