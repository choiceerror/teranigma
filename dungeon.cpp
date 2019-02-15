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
	setWindowsSize(WINSTARTX, WINSTARTY, GAMESIZEX, GAMESIZEY);


	IMAGEMANAGER->addFrameImage("ballMonster", "image/enemy1.bmp", 128, 128, 4, 4, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("fireMonster", "image/enemy2.bmp", 180, 174, 5, 3, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("knightMonster", "image/enemy3.bmp", 1820, 1600, 7, 8, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("boss", "image/����.bmp", 1048, 239, 4, 1, true, MAGENTA);
	EFFECTMANAGER->addEffect("deadEffect", "image/deadEffect2.bmp", 1496, 110, 136, 110, 5, 0.17f, 1000);
	_enemyManager = new enemyManager;
	_player = new player;
	_camera = new camera;
	_dungeon = new dungeonMap;
	_itemManager = new ItemManager;

	_enemyManager->setPlayerMemoryAddressLink(_player);
	_player->setEnemyManagerAddressLink(_enemyManager);
	_player->setMapManagerAddressLink(_dungeon);
	_enemyManager->setDungeonMapAddressLink(_dungeon);

	_dungeon->init();
	_player->init();
	_enemyManager->init();
	_itemManager->init();
	_enemyManager->setEnemy();

	if (_dungeon->getDungeonFloor() == DUNGEON_FLOOR::FIRST_FLOOR)
	{
		_camera->init(GAMESIZEX, GAMESIZEY, GAMESIZEX, 3200);
	}
	else if (_dungeon->getDungeonFloor() == DUNGEON_FLOOR::SECOND_FLOOR)
	{
		_camera->init(GAMESIZEX, GAMESIZEY, 2240, 1600);
	}


	return S_OK;
}

void dungeon::release()
{
	//SAFE_DELETE(_enemyManager);
	//SAFE_DELETE(_player);
	//SAFE_DELETE(_dungeon);
	//SAFE_DELETE(_camera);
}

void dungeon::update()
{
	_camera->update(_player->getPlayerX(), _player->getPlayerY());
	_player->update(false, 1);
	_enemyManager->update();
	_itemManager->update();
	itemRandomDrop();
	playerItemGet();
}

void dungeon::render()
{
	_dungeon->render(_camera->getCameraX(), _camera->getCameraY());
	_enemyManager->render(_camera->getCameraX(), _camera->getCameraY());
	_itemManager->render(_camera->getCameraX(), _camera->getCameraY());
	_player->render(_camera->getCameraX(), _camera->getCameraY());

	//char str[100];
	//sprintf_s(str, "%d", _player->getInventory()->getPotionCount());
	//TextOut(getMemDC(), 120, 120, str, strlen(str));

}

void dungeon::itemRandomDrop()
{
	for (int i = 0; i < _enemyManager->getVEnemyDeadPoint().size(); i++)
	{
		_rndItemDrop = RND->getRandomInt(0, 300);
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

void dungeon::playerItemGet()
{
	RECT temp;
	//��������
	for (int i = 0; i < _itemManager->getVGlod().size(); i++)
	{
		if (IntersectRect(&temp, &_player->getPlayerRc(), &_itemManager->getVGlod()[i]->getItemRect()))
		{
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

void dungeon::setWindowsSize(int x, int y, int width, int height)
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
