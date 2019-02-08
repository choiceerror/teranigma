#include "stdafx.h"
#include "dungeon.h"


dungeon::dungeon()
{
}


dungeon::~dungeon()
{
}

HRESULT dungeon::init()
{
	IMAGEMANAGER->addFrameImage("ballMonster", "image/enemy1.bmp", 64, 64, 4, 4, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("fireMonster", "image/enemy2.bmp", 90, 87, 5, 3, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("knightMonster", "image/enemy_3.bmp", 910, 800, 7, 8, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("boss", "image/����.bmp", 1048, 239, 4, 1, true, MAGENTA);

	_enemyManager = new enemyManager;
	_enemyManager->init();
	_enemyManager->setEnemy();

	return S_OK;
}

void dungeon::release()
{
	SAFE_DELETE(_enemyManager);
}

void dungeon::update()
{
	_enemyManager->update();
}

void dungeon::render()
{
	_enemyManager->render();
}
