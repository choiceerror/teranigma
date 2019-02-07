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
	_fireMonsterBullet = new fireMonsterBullet;
	_fireMonsterBullet->init("fireMonster", WINSIZEX, 10);

	return S_OK;
}

void enemyManager::release()
{
}

void enemyManager::update()
{
	//������Ʈ ���� �Լ�
	updateCollection();
	//���Ѿ� �߻��Լ�
	fireMonsterBulletFire();

}

void enemyManager::render()
{
	for (_viBallMonster = _vBallMonster.begin(); _viBallMonster != _vBallMonster.end(); _viBallMonster++)
	{
		(*_viBallMonster)->render((*_viBallMonster)->getViewX(), (*_viBallMonster)->getViewY());
	}
	for (_viKnightMonster = _vKnightMonster.begin(); _viKnightMonster != _vKnightMonster.end(); _viKnightMonster++)
	{
		(*_viKnightMonster)->render((*_viKnightMonster)->getViewX(), (*_viKnightMonster)->getViewY());
	}

	for (_viFireMonster = _vFireMonster.begin(); _viFireMonster != _vFireMonster.end(); _viFireMonster++)
	{
		(*_viFireMonster)->render((*_viFireMonster)->getViewX(), (*_viFireMonster)->getViewY());
	}

	//�Ҹ��� �Ѿ� ����
	for (int i = 0; i < _fireMonsterBullet->getVFireBullet().size(); i++)
	{
		_fireMonsterBullet->render((*_fireMonsterBullet->setVFireBullet())[i].x, (*_fireMonsterBullet->setVFireBullet())[i].y);
	}
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

	_fireMonsterBullet->update();
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

			fm->init("fire", "fireMonster", 100 + j * 100, 300 + i * 100, j, i);
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
			_fireMonsterBullet->fire(fireRc.left + (fireRc.right - fireRc.left) / 2, fireRc.top + (fireRc.bottom - fireRc.top) / 2, PI2, 1);
		}
	}
}
