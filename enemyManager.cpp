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
	_fireMonsterBullet->init("fireMonster", WINSIZEX / 2, 10);
	_fireBulletSpeed = 5;

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
	char str[128];

	sprintf_s(str, "targetDistance : %f", (_vKnightMonster.front()->getTargetDistance()));
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
	//�Ҹ��� �Ѿ� ������
	for (int i = 0; i < _fireMonsterBullet->getVFireBullet().size(); i++)
	{
		_fireMonsterBullet->render(_fireMonsterBullet->getVFireBullet()[i].viewX, _fireMonsterBullet->getVFireBullet()[i].viewY);
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

//���ʹ� ���ݵ��� �÷��̾� �浹�� �Լ�
void enemyManager::enemyAttackPlayerCollision()
{
	//�÷��̾� ��Ʈ �ӽÿ����� ����� �浹 ����� ������
	for (_viBallMonster = _vBallMonster.begin(); _viBallMonster != _vBallMonster.end(); _viBallMonster++)
	{
		//���Ϳ� �÷��̾�� �Ÿ��� ������.
		(*_viBallMonster)->setTargetDistance(getDistance((*_viBallMonster)->getX(), (*_viBallMonster)->getY(), _x, _y));
		RECT temp;
		if (IntersectRect(&temp, &(*_viBallMonster)->getRangeRect(), &_playerRc))
		{
			(*_viBallMonster)->setMoveType(FOLLOW_MOVE_TYPE); //�浹�ϸ� ���󰡴� Ÿ������
			(*_viBallMonster)->setState(BALLMONSTER_STATE_MOVE); //�׸��� ��ӿ�����.
			(*_viBallMonster)->setTargetAngle(getAngle((*_viBallMonster)->getX(), (*_viBallMonster)->getY(), _x, _y)); //���Ϳ� �÷��̾�� ���� ������.
			switch ((*_viBallMonster)->getState())
			{
			case BALLMONSTER_STATE_IDLE:
			case BALLMONSTER_STATE_MOVE:

				//�Ÿ��� 10���� Ŭ���� �Ѿƿ�.
				if ((*_viBallMonster)->getTargetDistance() > 10)
				{
					//�����ʰ��� �浹�����϶��� ���������� �Ѿƿ���
					if (((*_viBallMonster)->getTargetAngle() < (PI / 180) * 25 && (*_viBallMonster)->getTargetAngle() > 0) ||
						((*_viBallMonster)->getTargetAngle() > (PI / 180) * 335 && (*_viBallMonster)->getTargetAngle() < PI2))
					{
						(*_viBallMonster)->setDirection(BALLMONSTER_DIRECTION_RIGHT);
					}
					//�������� �浹�����϶��� ���� �Ѿƿ���
					else if ((*_viBallMonster)->getTargetAngle() > (PI / 180) * 65 && (*_viBallMonster)->getTargetAngle() < (PI / 180) * 115)
					{
						(*_viBallMonster)->setDirection(BALLMONSTER_DIRECTION_UP);
					}
					//���ʰ���
					else if ((*_viBallMonster)->getTargetAngle() > (PI / 180) * 155 && (*_viBallMonster)->getTargetAngle() < (PI / 180) * 205)
					{
						(*_viBallMonster)->setDirection(BALLMONSTER_DIRECTION_LEFT);
					}
					//�Ʒ�����
					else if ((*_viBallMonster)->getTargetAngle() > (PI / 180) * 245 && (*_viBallMonster)->getTargetAngle() < (PI / 180) * 295)
					{
						(*_viBallMonster)->setDirection(BALLMONSTER_DIRECTION_DOWN);
					}
				}
				break;
			}
		}
		//�Ÿ��� �־����� �ٽ� �⺻������Ÿ������
		else if((*_viBallMonster)->getTargetDistance() < 320)
		{
			(*_viBallMonster)->setMoveType(BASIC_MOVE_TYPE);
		}
	}

	//�Ҹ���
	for (int i = 0; i < _vFireMonster.size(); i++)
	{
		RECT temp;
		//�Ÿ� �����ְ�
		_vFireMonster[i]->setTargetDistance(getDistance(_vFireMonster[i]->getX(), _vFireMonster[i]->getY(), _x, _y));

		if (IntersectRect(&temp, &_vFireMonster[i]->getRangeRect(), &_playerRc))
		{
			//���� ������
			_vFireMonster[i]->setTargetAngle(getAngle(_vFireMonster[i]->getX(), _vFireMonster[i]->getY(), _x, _y));
			_vFireMonster[i]->setMoveType(FOLLOW_MOVE_TYPE);
			_vFireMonster[i]->setState(FIREMONSTER_STATE_MOVE);
			switch (_vFireMonster[i]->getState())
			{
			case FIREMONSTER_STATE_IDLE:
			case FIREMONSTER_STATE_MOVE:
				if ((_vFireMonster[i]->getTargetAngle() < (PI / 180) * 25 && _vFireMonster[i]->getTargetAngle() > 0) ||
					(_vFireMonster[i]->getTargetAngle() > (PI / 180) * 335 && _vFireMonster[i]->getTargetAngle() < PI2))
				{
					_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_RIGHT);
				}
				else if (_vFireMonster[i]->getTargetAngle() > (PI / 180) * 65 && _vFireMonster[i]->getTargetAngle() < (PI / 180) * 115)
				{
					_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_UP);
				}
				else if (_vFireMonster[i]->getTargetAngle() > (PI / 180) * 155 && _vFireMonster[i]->getTargetAngle() < (PI / 180) * 205)
				{
					_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_LEFT);
				}
				else if (_vFireMonster[i]->getTargetAngle() > (PI / 180) * 245 && _vFireMonster[i]->getTargetAngle() < (PI / 180) * 295)
				{
					_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_DOWN);
				}
				//�Ÿ��� ��������� ����
				if (_vFireMonster[i]->getTargetDistance() < 500)
				{
					fireMonsterBulletFire(i); //���Ѿ� �߻�
				}
				break;
			}
		}
		else if (_vFireMonster[i]->getTargetDistance() < 700)
		{
			_vFireMonster[i]->setMoveType(BASIC_MOVE_TYPE);
		}

	}

	//��� ����
	for (int i = 0; i < _vKnightMonster.size(); i++)
	{
		RECT temp;
		//���� �÷��̾�� �Ÿ� ������.
		_vKnightMonster[i]->setTargetDistance(getDistance(_vKnightMonster[i]->getX(), _vKnightMonster[i]->getY(), _x, _y));
		//������Ʈ�� �浹�ϸ�
		if (IntersectRect(&temp, &_vKnightMonster[i]->getRangeRect(), &_playerRc))
		{
			//���� �÷��̾�� ���� ������.
			_vKnightMonster[i]->setTargetAngle(getAngle(_vKnightMonster[i]->getX(), _vKnightMonster[i]->getY(), _x, _y));
			_vKnightMonster[i]->setMoveType(FOLLOW_MOVE_TYPE); //���󰡴� Ÿ������.
			switch (_vKnightMonster[i]->getState())
			{
			case KNIGHTMONSTER_STATE_IDLE:
			case KNIGHTMONSTER_STATE_MOVE:

				if ((_vKnightMonster[i]->getTargetAngle() < (PI / 180) * 25 && _vKnightMonster[i]->getTargetAngle() > 0) ||
					(_vKnightMonster[i]->getTargetAngle() > (PI / 180) * 335 && _vKnightMonster[i]->getTargetAngle() < PI2))
				{
					_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_RIGHT);
				}
				else if (_vKnightMonster[i]->getTargetAngle() > (PI / 180) * 65 && _vKnightMonster[i]->getTargetAngle() < (PI / 180) * 115)
				{
					_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_UP);
				}
				else if (_vKnightMonster[i]->getTargetAngle() > (PI / 180) * 155 && _vKnightMonster[i]->getTargetAngle() < (PI / 180) * 205)
				{
					_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_LEFT);
				}
				else if (_vKnightMonster[i]->getTargetAngle() > (PI / 180) * 245 && _vKnightMonster[i]->getTargetAngle() < (PI / 180) * 295)
				{
					_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_DOWN);
				}

				//�Ÿ��� ��������� ����
				if (_vKnightMonster[i]->getTargetDistance() < 150)
				{
					_vKnightMonster[i]->setState(KNIGHTMONSTER_STATE_ATTACK);
				}
				else _vKnightMonster[i]->setState(KNIGHTMONSTER_STATE_MOVE); //�Ÿ��� ���־����� ��������.

				break;
			}
		}
		//�����Ÿ� ������ �ٽ� �⺻������ Ÿ������
		else if (_vKnightMonster[i]->getTargetDistance() < 500)
		{
			_vKnightMonster[i]->setMoveType(BASIC_MOVE_TYPE);
		}
	}
}

//���Ѿ� �߻�
void enemyManager::fireMonsterBulletFire(int i)
{
	//���̾� ���Ͱ� �� �Ѿ��� ī��Ʈ �߻簡 true���
	if (_vFireMonster[i]->fireBulletCountFire() == true)
	{
		RECT fireRc; //�Ҹ��� ��Ʈ
		fireRc = _vFireMonster[i]->getRect();

		//�Ҹ��Ͱ� �Ѿ��� ���⺰�� �����.
		for (int i = 0; i < 4; i++)
		{
			_fireMonsterBullet->fire(fireRc.left + (fireRc.right - fireRc.left) / 2, fireRc.top + (fireRc.bottom - fireRc.top) / 2, PI + i * (PI / 2), _fireBulletSpeed);
		}
	}
	
	//���̾���� �Ѿ� �浹
	for (int i = 0; i < _fireMonsterBullet->getVFireBullet().size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp, &_fireMonsterBullet->getVFireBullet()[i].rc, &_playerRc))
		{
			(*_fireMonsterBullet->setVFireBullet())[i].isCollision = true;
		}
	}
}

//���ʹ̵��� �����Լ�
void enemyManager::enemyDead()
{
}

//���ʹ̵��� �������� �������� �Լ�
void enemyManager::enemyRespon()
{
}
