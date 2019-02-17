#include "stdafx.h"
#include "bossScene.h"


bossScene::bossScene()
{
}


bossScene::~bossScene()
{
}

HRESULT bossScene::init()
{
	setWindowsSize(WINSTARTX, WINSTARTY, GAMESIZEX, GAMESIZEY);

	_enemyManager = new enemyManager;
	_player = new player;
	_camera = new camera;
	_dungeonBossMap = new dungeonMap;
	_itemManager = new ItemManager;

	_enemyManager->setPlayerMemoryAddressLink(_player);
	_player->setEnemyManagerAddressLink(_enemyManager);
	_player->setMapManagerAddressLink(_dungeonBossMap);
	_enemyManager->setDungeonMapAddressLink(_dungeonBossMap);

	_dungeonBossMap->setDungeonFloor(DUNGEON_FLOOR::BOSS_FLOOR);
	_dungeonBossMap->init(_dungeonBossMap->getDungeonFloor());
	_player->init();
	_enemyManager->init();
	_itemManager->init();
	_enemyManager->setEnemy();
	_player->setPlayerPosX(1280 / 2 - 50);
	_player->setPlayerPosY(2500);

	_camera->init(GAMESIZEX, GAMESIZEY, 1280, 2560);

	_worldTime = TIMEMANAGER->getWorldTime();
	_isBossAppear = false;

	SOUNDMANAGER->addSound("bossSceneBGM", "sound/bossFight.mp3", true, true);

	return S_OK;
}

void bossScene::release()
{
}

void bossScene::update()
{
	_camera->update(_player->getPlayerX(), _player->getPlayerY());
	_player->update(false, 1);
	_enemyManager->update();
	_itemManager->update();
	bossAppear();
	movePattern();
}

void bossScene::render()
{
	char str[128];
	sprintf_s(str, "%f", _enemyManager->getVBoss()[0]->getX());

	TextOut(getMemDC(), 100, 80, str, strlen(str));

	sprintf_s(str, "%f", _enemyManager->getVBoss()[0]->getY());

	TextOut(getMemDC(), 100, 100, str, strlen(str));
	if (_isBossAppear == true || _isOnce[BOSS_APPEAR] == true)
	{
		_enemyManager->render(_camera->getCameraX(), _camera->getCameraY());
	}
	_dungeonBossMap->render(_camera->getCameraX(), _camera->getCameraY());
	_itemManager->render(_camera->getCameraX(), _camera->getCameraY());
	_player->render(_camera->getCameraX(), _camera->getCameraY());
}

void bossScene::bossAppear()
{
	//������ �÷��̾�� �Ÿ� ������.
	//_enemyManager->getVBoss()[0]->setTargetDistance(getDistance(_player->getPlayerX(), _player->getPlayerY(), _enemyManager->getVBoss()[0]->getX(), _enemyManager->getVBoss()[0]->getY()));

	if (_player->getPlayerY() <= 900 && _isOnce[BOSS_APPEAR] == false)
	{
		_isBossAppear = true;
		SOUNDMANAGER->play("bossSceneBGM");
		_isOnce[BOSS_APPEAR] = true;
	}

	if (_isBossAppear == true) //��������
	{
		_goal.x = GAMESIZEX / 2 + 70;
		_goal.y = 650;

		_elapsedTime = TIMEMANAGER->getElapsedTime();

		//������� ������ �����ְ�
		_goalAngle = getAngle(_enemyManager->getVBoss()[0]->getX(), _enemyManager->getVBoss()[0]->getY(), _goal.x, _goal.y);
		//������� �Ÿ� ������.
		_goalDistance = getDistance(_enemyManager->getVBoss()[0]->getX(), _enemyManager->getVBoss()[0]->getY(), _goal.x, _goal.y);
		if (_isOnce[LINEAR] == false)
		{
			//�żӽ� ��������
			_moveSpeed = _goalDistance * (_elapsedTime / 6.0f);
			_worldTime = TIMEMANAGER->getWorldTime();
		}
		_isOnce[LINEAR] = true;

		//8�������� ������.
		if (8.0f + _worldTime >= TIMEMANAGER->getWorldTime())
		{
			_enemyManager->getVBoss()[0]->setX(_enemyManager->getVBoss()[0]->getX() + cosf(_goalAngle) * _moveSpeed);
			_enemyManager->getVBoss()[0]->setY(_enemyManager->getVBoss()[0]->getY() + -sinf(_goalAngle) * _moveSpeed);
		}

		if (_goalDistance <= 50 && _isOnce[CAMERA_LINEAR] == false)
		{
			_camera->linearKeepMove(_goal.x, _goal.y + 50, 2, 100000);
			_isOnce[CAMERA_LINEAR] = true; //ī�޶� �ѹ��� �ޱ�����.
			_isBossAppear = false; //������������������ ���� �����ϴ°� ����.
		}
	}


}

void bossScene::movePattern()
{

	for (int i = 0; i < 10; i++)
	{
		_moveGoal[i].x = RND->getRandomFloat(300.f, 1000.f);
	}
	if (_isBossAppear == false)
	{
		switch (_enemyManager->getVBoss()[0]->getState())
		{
		case BOSS_STATE_MOVE:
			if (_enemyManager->getVBoss()[0]->getDirection() == BOSS_DIRECTION_LEFT)
			{
				_enemyManager->getVBoss()[0]->setMoveAngle()
				_enemyManager->getVBoss()[0]->setX()
			}
		}
	}
}

void bossScene::setWindowsSize(int x, int y, int width, int height)
{
	RECT winRect;

	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;
	winRect.bottom = height;

	AdjustWindowRect(&winRect, WINSTYLE, false);

	//���������� Ŭ���̾�Ʈ ���� ũ�� ������ �Ѵ�
	SetWindowPos(_hWnd, NULL, x, y,
		(winRect.right - winRect.left),
		(winRect.bottom - winRect.top),
		SWP_NOZORDER | SWP_NOMOVE);

}
