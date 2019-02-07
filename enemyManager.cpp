#include "stdafx.h"
#include "enemyManager.h"


enemyManager::enemyManager()
{
}


enemyManager::~enemyManager()
{
}

HRESULT enemyManager::init()
{
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_radius = 50;
	_playerRc = RectMake(_x, _y, 100, 100);

	_fireMonsterBullet = new fireMonsterBullet;
	_fireMonsterBullet->init("fireMonster", WINSIZEX, 10);

	return S_OK;
}

void enemyManager::release()
{
	SAFE_DELETE(_fireMonsterBullet);
	for (_viBallMonster = _vBallMonster.begin(); _viBallMonster != _vBallMonster.end(); _viBallMonster++)
	{
		SAFE_DELETE((*_viBallMonster));
	}
	for (_viFireMonster = _vFireMonster.begin(); _viFireMonster != _vFireMonster.end(); _viFireMonster++)
	{
		SAFE_DELETE((*_viFireMonster));
	}
	for (_viKnightMonster = _vKnightMonster.begin(); _viKnightMonster != _vKnightMonster.end(); _viKnightMonster++)
	{
		SAFE_DELETE((*_viKnightMonster));
	}
}

void enemyManager::update()
{
	//������Ʈ ���� �Լ�
	updateCollection();
	//���Ѿ� �߻��Լ�
	fireMonsterBulletFire();

	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_x -= 3;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_x += 3;
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_y -= 3;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		_y += 3;
	}
	_playerRc = RectMake(_x, _y, 100, 100);


	for (_viBallMonster = _vBallMonster.begin(); _viBallMonster != _vBallMonster.end(); _viBallMonster++)
	{
		RECT temp;
		if (IntersectRect(&temp, &(*_viBallMonster)->getRangeRect(), &_playerRc))
		{
			switch ((*_viBallMonster)->getDirection())
			{
			case BALLMONSTER_LEFT_IDLE:
			case BALLMONSTER_LEFT_MOVE:
			case BALLMONSTER_RIGHT_IDLE:
			case BALLMONSTER_RIGHT_MOVE:
			case BALLMONSTER_UP_IDLE:
			case BALLMONSTER_UP_MOVE:
			case BALLMONSTER_DOWN_IDLE:
			case BALLMONSTER_DOWN_MOVE:
				
				//if (getDistance(_playerRc.left, _playerRc.top, (*_viBallMonster)->getX(), (*_viBallMonster)->getY()) < _x + (*_viBallMonster)->getX())
				//{
		
				//	(*_viBallMonster)->setY(getAngle(_playerRc.left, _playerRc.top, (*_viBallMonster)->getX(), (*_viBallMonster)->getY()));
				//	(*_viBallMonster)->setX(getAngle(_playerRc.left, _playerRc.top, (*_viBallMonster)->getX(), (*_viBallMonster)->getY()));
				//	
				//}
				//(*_viBallMonster)->setX((*_viBallMonster)->getX() - 3);
				//getAngle(_playerRc.left, _playerRc.top, (*_viBallMonster)->getX(), (*_viBallMonster)->getY());
				break;
			}
		}

	}

}

void enemyManager::render()
{


	//������ ������
	for (_viBallMonster = _vBallMonster.begin(); _viBallMonster != _vBallMonster.end(); _viBallMonster++)
	{
		(*_viBallMonster)->render((*_viBallMonster)->getViewX(), (*_viBallMonster)->getViewY());
	}
	//���̾���� ������
	for (_viFireMonster = _vFireMonster.begin(); _viFireMonster != _vFireMonster.end(); _viFireMonster++)
	{
		(*_viFireMonster)->render((*_viFireMonster)->getViewX(), (*_viFireMonster)->getViewY());
	}
	//����Ʈ���� ������
	for (_viKnightMonster = _vKnightMonster.begin(); _viKnightMonster != _vKnightMonster.end(); _viKnightMonster++)
	{
		(*_viKnightMonster)->render((*_viKnightMonster)->getViewX(), (*_viKnightMonster)->getViewY());
	}

	//�Ҹ��� �Ѿ� ����
	for (int i = 0; i < _fireMonsterBullet->getVFireBullet().size(); i++)
	{
		_fireMonsterBullet->render(_fireMonsterBullet->getVFireBullet()[i].viewX, _fireMonsterBullet->getVFireBullet()[i].viewY);
	}
	Rectangle(getMemDC(), _playerRc);
}

//������Ʈ ����
void enemyManager::updateCollection()
{
	for (_viBallMonster = _vBallMonster.begin(); _viBallMonster != _vBallMonster.end(); _viBallMonster++)
	{
		(*_viBallMonster)->update(0, 0);
	}
	for (_viKnightMonster = _vKnightMonster.begin(); _viKnightMonster != _vKnightMonster.end(); _viKnightMonster++)
	{
		(*_viKnightMonster)->update(0, 0);
	}
	for (_viFireMonster = _vFireMonster.begin(); _viFireMonster != _vFireMonster.end(); _viFireMonster++)
	{
		(*_viFireMonster)->update(0, 0);
	}

	_fireMonsterBullet->update(0, 0);
}

//���ʹ� ����
void enemyManager::setEnemy()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ballMonster* bm;
			bm = new ballMonster;

			bm->init("ball", "ballMonster", 100 + j * 100, 100 + i * 100, j, i);
			_vBallMonster.push_back(bm);

			knightMonster* km;
			km = new knightMonster;

			km->init("knight", "knightMonster", 100 + j * 100, 200 + i * 100, j, i);
			_vKnightMonster.push_back(km);

			fireMonster* fm;
			fm = new fireMonster;

			fm->init("fire", "fireMonster", 100 + j * 100, 500 + i * 100, j, i);
			_vFireMonster.push_back(fm);
		}
	}
}

//���Ѿ� �߻�
void enemyManager::fireMonsterBulletFire()
{
	for (_viFireMonster = _vFireMonster.begin(); _viFireMonster != _vFireMonster.end(); _viFireMonster++)
	{
		//���̾� ���Ͱ� �� �Ѿ��� ī��Ʈ �߻簡 true���
		if ((*_viFireMonster)->fireBulletCountFire() == true)
		{
			RECT fireRc; //�Ҹ��� ��Ʈ
			fireRc = (*_viFireMonster)->getRect();
			//�Ҹ��ͷκ��� ���Ѿ� �߻�
			_fireMonsterBullet->fire(fireRc.left + (fireRc.right - fireRc.left) / 2, fireRc.top + (fireRc.bottom - fireRc.top) / 2, getAngle(fireRc.left, fireRc.top, _playerRc.left, _playerRc.top), 3);
		}
	}
}
