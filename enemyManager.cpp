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
	char str[128];

	
	sprintf_s(str, "angle : %f", (_vBallMonster.back()->getTargetDistance()));
	TextOut(getMemDC(), 300, 100, str, strlen(str));

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

			bm->init("ball", "ballMonster", 500 + j * 100, 100 + i * 100, j, i, 100);
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
	for (_viBallMonster = _vBallMonster.begin(); _viBallMonster != _vBallMonster.end(); _viBallMonster++)
	{
		(*_viBallMonster)->setTargetDistance(getDistance((*_viBallMonster)->getX(), (*_viBallMonster)->getY(), _x, _y));
		RECT temp;
		if (IntersectRect(&temp, &(*_viBallMonster)->getRangeRect(), &_playerRc))
		{
			(*_viBallMonster)->setMoveType(FOLLOW_MOVE_TYPE); //�浹�ϸ� ���󰡴� Ÿ������
			//(*_viBallMonster)->setState(BALLMONSTER_STATE_MOVE);

			switch ((*_viBallMonster)->getState())
			{
			case BALLMONSTER_STATE_IDLE:
			case BALLMONSTER_STATE_MOVE:

				(*_viBallMonster)->setTargetAngle(getAngle((*_viBallMonster)->getX(), (*_viBallMonster)->getY(), _x, _y));

				
				
				if ((*_viBallMonster)->getTargetDistance() > 5)
				{
					if (((*_viBallMonster)->getTargetAngle() < (PI / 180) * 25 && (*_viBallMonster)->getTargetAngle() >= 0) ||
						((*_viBallMonster)->getTargetAngle() > (PI / 180) * 335 && (*_viBallMonster)->getTargetAngle() <= (PI / 180) * 360))
					{
						(*_viBallMonster)->setDirection(BALLMONSTER_DIRECTION_RIGHT);
					}
					else if ((*_viBallMonster)->getTargetAngle() < (PI / 180) * 115 && (*_viBallMonster)->getTargetAngle() > (PI / 180) * 65)
					{
						(*_viBallMonster)->setDirection(BALLMONSTER_DIRECTION_UP);
					}
					else if ((*_viBallMonster)->getTargetAngle() > (PI / 180) * 155 && (*_viBallMonster)->getTargetAngle() < (PI / 180) * 205)
					{
						(*_viBallMonster)->setDirection(BALLMONSTER_DIRECTION_LEFT);
					}
					else if ((*_viBallMonster)->getTargetAngle() > (PI / 180) * 245 && (*_viBallMonster)->getTargetAngle() < (PI / 180) * 295)
					{
						(*_viBallMonster)->setDirection(BALLMONSTER_DIRECTION_DOWN);
					}
				}
				else
				{
					(*_viBallMonster)->setState(BALLMONSTER_STATE_IDLE);
				}
				

				break;
			}
		}
	
		else if((*_viBallMonster)->getTargetDistance() < 160)
		{
			(*_viBallMonster)->setMoveType(BASIC_MOVE_TYPE);
		}

	}

	//for (int i = 0; i < _vKnightMonster.size(); i++)
	//{
	//	RECT temp;
	//	if (IntersectRect(&temp, &_vKnightMonster[i]->getRangeRect(), &_playerRc))
	//	{
	//		switch (_vKnightMonster[i]->getState())
	//		{
	//		case KNIGHTMONSTER_STATE_ATTACK:
	//			_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DOWN_ATTACK);
	//			break;

	//			_vKnightMonster[i]->setDirection(KNIGHTMONSTER_UP_ATTACK);
	//			break;

	//			if (_vKnightMonster[i]->getRangeRect().left <= _playerRc.right && _vKnightMonster[i]->getRangeRect().right > _playerRc.left)
	//			_vKnightMonster[i]->setDirection(KNIGHTMONSTER_LEFT_ATTACK);
	//			break;

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
