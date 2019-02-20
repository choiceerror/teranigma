#include "stdafx.h"
#include "town.h"
#pragma warning(disable:4996)

town::town()
{
}


town::~town()
{
}

HRESULT town::init()
{
	setWindowsSize(WINSTARTX, WINSTARTY, GAMESIZEX, GAMESIZEY);

	IMAGEMANAGER->addFrameImage("bird", "image/����.bmp", 140, 160, 4, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("elle", "image/��.bmp", 100, 140, 4, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("elder", "image/���.bmp", 330, 40, 11, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("townHuman", "image/townHuman.bmp", 700, 250, 14, 5, true, RGB(255, 0, 255));
	IMAGEMANAGER->findImage("black")->setAlpahBlend(true);
	IMAGEMANAGER->findImage("townTile")->setAlpahBlend(true);
	IMAGEMANAGER->findImage("Ÿ�ϸ�4")->setAlpahBlend(true);

	SOUNDMANAGER->play("theTown", 1);

	_alphaValue = 255;
	_houseAlpha = 255;
	_bedAlpha = 0;

	_player = new player;
	_camera = new camera;
	_town = new townMap;
	_npcManager = new npcManager;
	_messageSpear = new messageSpear;

	_player->setTownManagerAddressLink(_town);
	_player->setnpcManagerAddressLink(_npcManager);
	_npcManager->setTownManagerAddressLink(_town);
	_player->setMessageManagerAddressLink(_messageSpear);
	_player->init(true);
	_town->init();
	_messageSpear->init();

	_npcManager->setBird();
	_npcManager->setElder();
	_npcManager->setElle();
	_npcManager->setTownHuman(TOWN_FISHMAN, 870, 870);
	_npcManager->setTownHuman(TOWN_GRANDFA, 200, 600);
	_npcManager->setTownHuman(TOWN_GRANDMA, 200, 400);
	_npcManager->init();

	_escape = RectMake(672, 1856 + 32, 128, 64);

	_camera->init(GAMESIZEX, GAMESIZEY, 1920, 1920);

	_worldTime = 0;
	_once = false;
	_worldMapIn = false;
	_fadeOut = true;
	_houseAlphaBlend = false;

	_house = RectMake(672, 1280, 352, 256);

	//_player->setPlayerPosX(704);
	//_player->setPlayerPosY(1792);

	
	playerSceneLoad();

	_player->setPlayerCurrentScene(PLAYERSCENE::TOWN);

	if (_player->getPlayerX() < 650 || _player->getPlayerX() > 1000 || _player->getPlayerY() < 1400 || _player->getPlayerY() > 1800)
	{
		_player->setPlayerDirection(UP);
		_player->setPlayerPosX(990);
		_player->setPlayerPosY(1408);
	}

	//_player->setTileCheck(false);
	return S_OK;
}

void town::release()
{
}

void town::update()
{
	_town->update();
	townIn();
	_camera->update(_player->getPlayerX(), _player->getPlayerY());
	_npcManager->update(2);
	_npcManager->aiBirdUpdate();
	worldMapIn();
	houseCollision();

	//==============�ؿ� �ۼ� ����===============
	_player->update(false, 2);
}

void town::render()
{
	_town->render(_camera->getCameraX(), _camera->getCameraY());
	_player->render(_camera->getCameraX(), _camera->getCameraY(), false);
	_npcManager->render(_camera->getCameraX(), _camera->getCameraY());

	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < 11; ++j)
		{
			IMAGEMANAGER->findImage("townTile")->alphaFrameRender(getMemDC(), 672 + 32 * j - _camera->getCameraX(), 1280 + 32 * i - _camera->getCameraY(), 1 + j, 4 + i, _houseAlpha);
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			IMAGEMANAGER->findImage("townTile")->alphaFrameRender(getMemDC(), 704 + 32 * j - _camera->getCameraX(), 1504 + 32 * i - _camera->getCameraY(), 2 + j, 11 + i, _houseAlpha);
			IMAGEMANAGER->findImage("Ÿ�ϸ�4")->alphaFrameRender(getMemDC(), 960 + 32 * j - _camera->getCameraX(), 1408 + 32 * i - _camera->getCameraY(), 21 + j, 5 + i, _bedAlpha);
		}
	}

	IMAGEMANAGER->findImage("black")->alphaRender(getMemDC(), _alphaValue);
	_messageSpear->render();

	//Rectangle(getMemDC(), _escape);
}

void town::setWindowsSize(int x, int y, int width, int height)
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

void town::worldMapIn()
{
	RECT temp;
	if (IntersectRect(&temp, &_player->getPlayerRc(), &_escape))
	{
		_worldMapIn = true;
	}

	if (_worldMapIn)
	{
		_player->setTileCheck(false);

		if (_fadeOut && _alphaValue < 255)
		{
			_alphaValue += 6;
		}

		if (!_fadeOut && _alphaValue < 255)
		{
			_alphaValue += 3;
		}

		if (_alphaValue > 255)
		{
			_alphaValue = 255;
		}


		_player->setPlayerDirection(DOWN);

		_player->setPlayerUnMove(true);

		_player->setPlayerPosY(_player->getPlayerY() + 3);

		if (!_once)
		{
			_worldTime = TIMEMANAGER->getWorldTime();
		}

		if (1.4f + _worldTime <= TIMEMANAGER->getWorldTime())
		{
			playerSceneSave();
			SOUNDMANAGER->stop("theTown");
			SCENEMANAGER->changeScene("worldMap");
		}


		_once = true;
	}
}

void town::playerSceneSave()
{
	int scene = (int)_player->getPlayerCurrentScene();

	HANDLE file;
	DWORD save;

	file = CreateFile("saveFile/playerScene.txt", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, &scene, sizeof(int), &save, NULL);

	CloseHandle(file);
}

void town::playerSceneLoad()
{
	HANDLE file;
	DWORD load;
	int scene;

	file = CreateFile("saveFile/playerScene.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, &scene, sizeof(int), &load, NULL);

	if (scene > 7)
	{
		scene = (int)PLAYERSCENE::TOWN;
	}

	_player->setPlayerCurrentScene((PLAYERSCENE)scene);

	if (_player->getPlayerCurrentScene() == PLAYERSCENE::WORLDMAP)
	{
		_player->setPlayerDirection(UP);
		_player->setPlayerPosX(704);
		_player->setPlayerPosY(1792);
	}

	if (_player->getPlayerCurrentScene() == PLAYERSCENE::TOWN)
	{
		_player->setPlayerPosX(990);
		_player->setPlayerPosY(1408);
	}

	CloseHandle(file);
}

void town::townIn()
{
	if (_fadeOut)
	{
		if (_fadeOut > 0)
		{
			_alphaValue -= 3;
		}

		if (_alphaValue < 0)
		{
			_alphaValue = 0;
			_fadeOut = false;
		}
	}
}

void town::houseCollision()
{
	RECT temp;

	if (IntersectRect(&temp, &_player->getPlayerRc(), &_house))
	{
		_houseAlphaBlend = true;
	}
	else
	{
		_houseAlphaBlend = false;
	}

	if (_houseAlphaBlend)
	{
		if (_houseAlpha > 0)
		{
			_houseAlpha -= 5;
		}

		if (_houseAlpha < 0)
		{
			_houseAlpha = 0;
		}

		if (_bedAlpha < 255)
		{
			_bedAlpha += 5;
		}

		if (_bedAlpha >= 255)
		{
			_bedAlpha = 255;
		}


	}
	else
	{
		if (_houseAlpha < 255)
		{
			_houseAlpha += 5;
		}

		if (_houseAlpha >= 255)
		{
			_houseAlpha = 255;
		}


		if (_bedAlpha > 0)
		{
			_bedAlpha -= 5;
		}

		if (_bedAlpha < 0)
		{
			_bedAlpha = 0;
		}
	}
}
