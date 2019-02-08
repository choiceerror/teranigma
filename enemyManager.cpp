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

	_playerRc = RectMakeCenter(_x, _y, 100, 100);

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

	//���ʹ� ���� �÷��̾� �浹�Լ�
	enemyAttackPlayerCollision();

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
	_playerRc = RectMakeCenter(_x, _y, 100, 100);

}

void enemyManager::render()
{
	//������ ���� �Լ�
	enemyDraw();
	//�Ҹ��� �Ѿ� ������
	for (int i = 0; i < _fireMonsterBullet->getVFireBullet().size(); i++)
	{
		_fireMonsterBullet->render(_fireMonsterBullet->getVFireBullet()[i].viewX, _fireMonsterBullet->getVFireBullet()[i].viewY);
	}
	Rectangle(getMemDC(), _playerRc);
}

//������Ʈ ����
void enemyManager::updateCollection()
{
	//�Ҹ��� ������Ʈ
	for (_viBallMonster = _vBallMonster.begin(); _viBallMonster != _vBallMonster.end(); _viBallMonster++)
	{
		(*_viBallMonster)->update(0, 0);
	}
	//���̾���� ������Ʈ
	for (_viFireMonster = _vFireMonster.begin(); _viFireMonster != _vFireMonster.end(); _viFireMonster++)
	{
		(*_viFireMonster)->update(0, 0);
	}
	//����Ʈ���� ������Ʈ
	for (_viKnightMonster = _vKnightMonster.begin(); _viKnightMonster != _vKnightMonster.end(); _viKnightMonster++)
	{
		(*_viKnightMonster)->update(0, 0);
	}
	//���̾���� �Ѿ� ������Ʈ
	_fireMonsterBullet->update(0, 0);
}

//������ ����
void enemyManager::enemyDraw()
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

			bm->init("ball", "ballMonster", 100 + j * 100, 100 + i * 100, j, i, 100);
			_vBallMonster.push_back(bm);

			fireMonster* fm;
			fm = new fireMonster;

			fm->init("fire", "fireMonster", 100 + j * 100, 500 + i * 100, j, i, 200);
			_vFireMonster.push_back(fm);


		}
	}
	knightMonster* km;
	km = new knightMonster;

	km->init("knight", "knightMonster", 1000, 500, 0, 0, 300);
	_vKnightMonster.push_back(km);
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

//���ʹ� ���ݵ��� �÷��̾� �浹�� �Լ�
void enemyManager::enemyAttackPlayerCollision()
{
	//���̾���� �Ѿ� �浹
	for (int i = 0; i < _fireMonsterBullet->getVFireBullet().size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp, &_fireMonsterBullet->getVFireBullet()[i].rc, &_playerRc))
		{
			(*_fireMonsterBullet->setVFireBullet())[i].isCollision = true;
		}
	}

	//�÷��̾� ��Ʈ �ӽÿ����� ����� �浹 �����
	//for (_viBallMonster = _vBallMonster.begin(); _viBallMonster != _vBallMonster.end(); _viBallMonster++)
	//{
	//	RECT temp;
	//	if (IntersectRect(&temp, &(*_viBallMonster)->getRangeRect(), &_playerRc))
	//	{
	//		(*_viBallMonster)->setMoveType(FOLLOW_MOVE_TYPE); //�浹�ϸ� ���󰡴� Ÿ������
	//		switch ((*_viBallMonster)->getDirection())
	//		{
	//		case BALLMONSTER_LEFT_IDLE:
	//		case BALLMONSTER_LEFT_MOVE:
	//		case BALLMONSTER_RIGHT_IDLE:
	//		case BALLMONSTER_RIGHT_MOVE:
	//		case BALLMONSTER_UP_IDLE:
	//		case BALLMONSTER_UP_MOVE:
	//		case BALLMONSTER_DOWN_IDLE:
	//		case BALLMONSTER_DOWN_MOVE:

	//			if ((*_viBallMonster)->getRangeRect().left <= _playerRc.right)
	//			{

	//				(*_viBallMonster)->setX((*_viBallMonster)->getX() + cosf(getAngle((*_viBallMonster)->getX(), (*_viBallMonster)->getY(), _x, _y)));
	//				(*_viBallMonster)->setY((*_viBallMonster)->getY() + -sinf(getAngle((*_viBallMonster)->getX(), (*_viBallMonster)->getY(), _x, _y)));
	//			}

	//			break;
	//		}
	//	}
	//	else
	//	{
	//		(*_viBallMonster)->setMoveType(BASIC_MOVE_TYPE);
	//	}

	//}

	//for (int i = 0; i < _vKnightMonster.size(); i++)
	//{
	//	RECT temp;
	//	if (IntersectRect(&temp, &_vKnightMonster[i]->getRangeRect(), &_playerRc))
	//	{
	//		switch (_vKnightMonster[i]->getDirection())
	//		{
	//		case KNIGHTMONSTER_DOWN_IDLE:
	//		case KNIGHTMONSTER_DOWN_MOVE:
	//			_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DOWN_ATTACK);
	//			break;
	//		case KNIGHTMONSTER_UP_IDLE:
	//		case KNIGHTMONSTER_UP_MOVE:
	//			_vKnightMonster[i]->setDirection(KNIGHTMONSTER_UP_ATTACK);
	//			break;
	//		case KNIGHTMONSTER_LEFT_IDLE:
	//		case KNIGHTMONSTER_LEFT_MOVE:
	//			if (_vKnightMonster[i]->getRangeRect().left <= _playerRc.right && _vKnightMonster[i]->getRangeRect().right > _playerRc.left)
	//			_vKnightMonster[i]->setDirection(KNIGHTMONSTER_LEFT_ATTACK);
	//			break;
	//		case KNIGHTMONSTER_RIGHT_IDLE:
	//		case KNIGHTMONSTER_RIGHT_MOVE:
	//			if (_vKnightMonster[i]->getRangeRect().right >= _playerRc.left && _vKnightMonster[i]->getRangeRect().left < _playerRc.right)
	//			_vKnightMonster[i]->setDirection(KNIGHTMONSTER_RIGHT_ATTACK);
	//			break;
	//		
	//		}
	//	}
	//}
}

//���ʹ̵��� �����Լ�
void enemyManager::enemyDead()
{
}

//���ʹ̵��� �������� �������� �Լ�
void enemyManager::enemyRespon()
{
}
