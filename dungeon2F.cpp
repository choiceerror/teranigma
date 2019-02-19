#include "stdafx.h"
#include "dungeon2F.h"


dungeon2F::dungeon2F()
{
}


dungeon2F::~dungeon2F()
{
}

HRESULT dungeon2F::init()
{
	setWindowsSize(WINSTARTX, WINSTARTY, GAMESIZEX, GAMESIZEY);

	_enemyManager = new enemyManager;
	_player = new player;
	_camera = new camera;
	_dungeon2F = new dungeonMap;
	_itemManager = new ItemManager;
	_clockFade = new ClockFadeOut;

	_enemyManager->setPlayerMemoryAddressLink(_player);
	_player->setEnemyManagerAddressLink(_enemyManager);
	_player->setMapManagerAddressLink(_dungeon2F);
	_enemyManager->setDungeonMapAddressLink(_dungeon2F);

	_dungeon2F->setDungeonFloor(DUNGEON_FLOOR::SECOND_FLOOR);
	_dungeon2F->init(_dungeon2F->getDungeonFloor());
	_player->init();
	_enemyManager->init();
	_itemManager->init();
	_enemyManager->setEnemy();
	_clockFade->init();

	_camera->init(GAMESIZEX, GAMESIZEY, 2240, 1600);

	_clockFade->setClockFadeOut(false);
	_clockFade->setClockFadeIn(true);

	return S_OK;
}

void dungeon2F::release()
{
	SAFE_DELETE(_enemyManager);
	SAFE_DELETE(_player);
	SAFE_DELETE(_dungeon2F);
	SAFE_DELETE(_camera);
	SAFE_DELETE(_itemManager);
	SAFE_DELETE(_clockFade);
}

void dungeon2F::update()
{
	_camera->update(_player->getPlayerX(), _player->getPlayerY());
	_enemyManager->update();
	_itemManager->update();
	itemRandomDrop();
	playerItemGet();
	_clockFade->update();



	//==============�ؿ� �ۼ� ����===============
	_player->update(false, 1);
}

void dungeon2F::render()
{
	_dungeon2F->render(_camera->getCameraX(), _camera->getCameraY());
	_enemyManager->render(_camera->getCameraX(), _camera->getCameraY());
	_itemManager->render(_camera->getCameraX(), _camera->getCameraY());
	_player->render(_camera->getCameraX(), _camera->getCameraY(), true);
	_clockFade->render();
}

void dungeon2F::playerSceneSave()
{
	HANDLE file;
	DWORD save;

	file = CreateFile("saveFile/playerScene.txt", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	_player->setPlayerCurrentScene(PLAYERSCENE::DUNGEON_2F);

	int scene = (int)_player->getPlayerCurrentScene();

	WriteFile(file, &scene, sizeof(int), &save, NULL);

	CloseHandle(file);
}

void dungeon2F::playerSceneLoad()
{
	HANDLE file;
	DWORD load;

	int scene;

	file = CreateFile("saveFile/playerScene.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, &scene, sizeof(int), &load, NULL);

	if (scene > 7)
	{
		scene = (int)PLAYERSCENE::DUNGEON_1F;
	}

	_player->setPlayerCurrentScene((PLAYERSCENE)scene);

	if (_player->getPlayerCurrentScene() == PLAYERSCENE::DUNGEON_1F)
	{
		//_playerWorldMap->setPlayerX(520.f);
		//_playerWorldMap->setPlayerY(896.f);
	}

	if (_player->getPlayerCurrentScene() == PLAYERSCENE::BOSS)
	{
		//_playerWorldMap->setPlayerX(1472.f);
		//_playerWorldMap->setPlayerY(1276.f);
	}

	CloseHandle(file);
}

void dungeon2F::itemRandomDrop()
{
	for (int i = 0; i < _enemyManager->getVEnemyDeadPoint().size(); i++)
	{
		_rndItemDrop = RND->getRandomInt(0, 200);
		_rndItemTypeDrop = RND->getRandomInt(0, 100);

		//40��Ȯ��
		if (_rndItemDrop >= 0 && _rndItemDrop <= 40)
		{
			_itemManager->dropGold(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y);
		}
		//20��Ȯ��
		else if (_rndItemDrop > 40 && _rndItemDrop <= 60)
		{
			//40�� Ȯ��
			if (_rndItemTypeDrop >= 60 && _rndItemTypeDrop <= 100)
			{
				_itemManager->dropPotion(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, POTIONTYPE::SMALL);
			}
			//30�� Ȯ��
			else if (_rndItemTypeDrop >= 30 && _rndItemTypeDrop < 60)
			{
				_itemManager->dropPotion(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, POTIONTYPE::MIDDLE);
			}
			//20��Ȯ��
			else if (_rndItemTypeDrop >= 10 && _rndItemTypeDrop < 30)
			{
				_itemManager->dropPotion(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, POTIONTYPE::BIG);
			}
			//10��Ȯ���� �ȳ���
			else
			{
				_itemManager->dropPotion(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, POTIONTYPE::NONE);
			}
		}
		//20��Ȯ��
		else if (_rndItemDrop > 60 && _rndItemDrop <= 80)
		{
			//40�� Ȯ��
			if (_rndItemTypeDrop >= 60 && _rndItemTypeDrop <= 100)
			{
				_itemManager->dropAccessory(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, ACCESSORYTYPE::LEEF);
			}
			//30�� Ȯ��
			else if (_rndItemTypeDrop >= 30 && _rndItemTypeDrop < 60)
			{
				_itemManager->dropAccessory(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, ACCESSORYTYPE::RED_SCARF);
			}
			//20��Ȯ��
			else if (_rndItemTypeDrop >= 10 && _rndItemTypeDrop < 30)
			{
				_itemManager->dropAccessory(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, ACCESSORYTYPE::TALISMAN);
			}
			//10��Ȯ���� �ȳ���
			else
			{
				_itemManager->dropAccessory(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, ACCESSORYTYPE::NONE);
			}
		}
		else if (_rndItemDrop > 80 && _rndItemDrop <= 100)
		{
			//40�� Ȯ��
			if (_rndItemTypeDrop >= 60 && _rndItemTypeDrop <= 100)
			{
				_itemManager->dropArmor(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, ARMORTYPE::HOOD);
			}
			//30�� Ȯ��
			else if (_rndItemTypeDrop >= 30 && _rndItemTypeDrop < 60)
			{
				_itemManager->dropArmor(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, ARMORTYPE::ICE_ARMOR);
			}
			//20��Ȯ��
			else if (_rndItemTypeDrop >= 10 && _rndItemTypeDrop < 30)
			{
				_itemManager->dropArmor(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, ARMORTYPE::IRON_ARMOR);
			}
			//10��Ȯ���� �ȳ���
			else
			{
				_itemManager->dropArmor(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, ARMORTYPE::NONE);
			}
		}
		else if (_rndItemDrop > 100 && _rndItemDrop <= 120)
		{
			//40�� Ȯ��
			if (_rndItemTypeDrop >= 60 && _rndItemTypeDrop <= 100)
			{
				_itemManager->dropWeapon(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, WEAPONTYPE::FIRE_SPEAR);
			}
			//30�� Ȯ��
			else if (_rndItemTypeDrop >= 30 && _rndItemTypeDrop < 60)
			{
				_itemManager->dropWeapon(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, WEAPONTYPE::ICE_SPEAR);
			}
			//20��Ȯ��
			else if (_rndItemTypeDrop >= 10 && _rndItemTypeDrop < 30)
			{
				_itemManager->dropWeapon(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, WEAPONTYPE::IRON_SPEAR);
			}
			//10��Ȯ���� �ȳ���
			else
			{
				_itemManager->dropWeapon(_enemyManager->getVEnemyDeadPoint()[i].x, _enemyManager->getVEnemyDeadPoint()[i].y, WEAPONTYPE::NONE);
			}
		}

		//������ ��ӻ������� �ʱ����ؼ� ����.
		_enemyManager->setVEnemyDeadPoint()->erase(_enemyManager->setVEnemyDeadPoint()->begin() + i);
	}

}

void dungeon2F::playerItemGet()
{
	RECT temp;
	//��������
	for (int i = 0; i < _itemManager->getVGlod().size(); i++)
	{
		if (IntersectRect(&temp, &_player->getPlayerRc(), &_itemManager->getVGlod()[i]->getItemRect()))
		{
			_player->setPlayerMoney(_player->getPlayerMoney() + _itemManager->getVGlod()[i]->getGoldNum());
			_itemManager->getVGlod()[i]->setItemIsLive(false);
		}
	}

	//���Ǿ�����
	for (int i = 0; i < _itemManager->getVPotion().size(); i++)
	{
		if (IntersectRect(&temp, &_player->getPlayerRc(), &_itemManager->getVPotion()[i]->getItemRect()))
		{
			_player->getInventory()->pickUpPotion(_itemManager->getVPotion()[i]->getTagPotion());
			_itemManager->getVPotion()[i]->setItemIsLive(false);
		}
	}

	//�Ǽ��縮
	for (int i = 0; i < _itemManager->getVAccessory().size(); i++)
	{
		if (IntersectRect(&temp, &_player->getPlayerRc(), &_itemManager->getVAccessory()[i]->getItemRect()))
		{
			_player->getInventory()->pickUpAccessory(_itemManager->getVAccessory()[i]->getTagAccessory());
			_itemManager->getVAccessory()[i]->setItemIsLive(false);
		}
	}

	//��
	for (int i = 0; i < _itemManager->getVArmor().size(); i++)
	{
		if (IntersectRect(&temp, &_player->getPlayerRc(), &_itemManager->getVArmor()[i]->getItemRect()))
		{
			_player->getInventory()->pickUpArmor(_itemManager->getVArmor()[i]->getTagArmor());
			_itemManager->getVArmor()[i]->setItemIsLive(false);
		}
	}

	//����
	for (int i = 0; i < _itemManager->getVWeapon().size(); i++)
	{
		if (IntersectRect(&temp, &_player->getPlayerRc(), &_itemManager->getVWeapon()[i]->getItemRect()))
		{
			_player->getInventory()->pickUpWeapon(_itemManager->getVWeapon()[i]->getTagWeapon());
			_itemManager->getVWeapon()[i]->setItemIsLive(false);
		}
	}
}

void dungeon2F::setWindowsSize(int x, int y, int width, int height)
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
