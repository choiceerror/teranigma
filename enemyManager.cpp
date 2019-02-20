#include "stdafx.h"
#include "enemyManager.h"
#include "player.h"


enemyManager::enemyManager()
{
}


enemyManager::~enemyManager()
{
}

HRESULT enemyManager::init()
{
	_fireMonsterBullet = new fireMonsterBullet;
	_goldBullet = new bossBullet;
	_blueBullet = new bossBullet;
	_redBullet = new bossBullet;
	_rgbBullet = new bossBullet;
	_shockAttack = new tagShockAttack;

	//====================== ���� ���� =======================
	_goldBullet->init("bossBullet", GAMESIZEY, 1);
	_blueBullet->init("bossBullet", GAMESIZEY, 1);
	_redBullet->init("bossBullet", GAMESIZEY, 1);
	_rgbBullet->init("bossBullet", GAMESIZEY, 1);

	_worldTime = TIMEMANAGER->getWorldTime();
	_moveWorldTime = TIMEMANAGER->getWorldTime();
	_isBossAppear = false;

	for (int i = 0; i < 4; i++)
	{
		_moveGoal[i].x = 400 + i * 150;
		_moveGoal[i].y = 600 + i * 80;
	}
	_goalX = GAMESIZEX / 2 + 70;
	_goalY = 650;
	_moveSpeed = 20.f;
	_bulletPattern = BULLET_PATTERN::GOLD_BULLET; //ó���� ����Ѿ˷�
	_attackPattern = ATTACK_PATTERN::ATTACK_ONE;

	for (int i = 0; i < 3; i++)
	{
		_bulletSpeed[i] = 7;
	}

	//��������Ʈ 
	_shockAttack->image = IMAGEMANAGER->findImage("shockAttack");
	_shockAttack->rc = RectMakeCenter(_shockAttack->x, _shockAttack->y, _shockAttack->image->getFrameWidth(), _shockAttack->image->getFrameHeight());
	int rightShock[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("shock", "rightShock", "shockAttack", rightShock, 8, 5, true);

	int leftShock[] = { 15, 14, 13, 12, 11, 10, 9, 8 };
	KEYANIMANAGER->addArrayFrameAnimation("shock", "leftShock", "shockAttack", leftShock, 8, 5, true);

	_shockAttack->ani = KEYANIMANAGER->findAnimation("shock", "rightShock");
	//====================== ���� ���� =======================

	_fireMonsterBullet->init("fireMonster", GAMESIZEX / 2, 10);
	_fireBulletSpeed = 5;
	_objectRc = RectMake(80, 355, 160, 125);
	return S_OK;
}

void enemyManager::release()
{
	SAFE_DELETE(_fireMonsterBullet);
	SAFE_DELETE(_goldBullet);
	SAFE_DELETE(_blueBullet);
	SAFE_DELETE(_redBullet);
	SAFE_DELETE(_rgbBullet);
	SAFE_DELETE(_shockAttack);
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
	SAFE_DELETE(_vBoss[0]);
}

void enemyManager::update()
{
	//������Ʈ ���� �Լ�
	updateCollection();

	if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::FIRST_FLOOR || _dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::SECOND_FLOOR)
	{
		//���ʹ̵��� Ÿ�ϰ���
		tileCheckObjectCollision();
	}

	if (_player->getPlayerDeath() == false || _player->getPlayerLevelUP() == false)
	{
		//���ʹ� AI �Լ�
		enemyAI();
	}

	//�������϶���
	if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::BOSS_FLOOR && _vBoss[0]->getCurrentHP() > 0)
	{
		//���� ����
		bossAppear();
		//���� ��������
		bossMovePattern();
		//���� ��������
		bossAttackPattern();

		if (_bulletPattern == BULLET_PATTERN::GOLD_BULLET || _bulletPattern == BULLET_PATTERN::BLUE_BULLET || _bulletPattern == BULLET_PATTERN::RED_BULLET)
		{
			_goldBullet->update();
			_blueBullet->update();
			_redBullet->update();
		}
		else if (_bulletPattern == BULLET_PATTERN::RGB_BULLET)
		{
			_rgbBullet->update();
		}
	}

	//�÷��̾� ���ݿ� ���ʹ̵��� ���� �Լ�
	playerAttackEnemyCollision();

	//���ʹ̵��� �����Լ�
	enemyDead();
}

void enemyManager::render(float cameraX, float cameraY)
{
	//Rectangle(getMemDC(), _objectRc);
	//Rectangle(getMemDC(), _playerAttackRc);
	Rectangle(getMemDC(), _shockAttack->rc.left - cameraX, _shockAttack->rc.top - cameraY, _shockAttack->rc.right - cameraX, _shockAttack->rc.bottom - cameraY);
	Rectangle(getMemDC(), _shockAttack->rc2.left - cameraX, _shockAttack->rc2.top - cameraY, _shockAttack->rc2.right - cameraX, _shockAttack->rc2.bottom - cameraY);
	//������ ���� �Լ�
	drawAll(cameraX, cameraY);

	//char str[128];

	//for (int i = 0; i < _vKnightMonster.size(); i++)
	{
		//sprintf_s(str, "distance : %f", _vKnightMonster[i]->getTargetDistance());
		//TextOut(getMemDC(), 100, 200 + i * 20, str, strlen(str));

		//sprintf_s(str, "idX : %d", _vKnightMonster[i]->getIdX());
		//TextOut(getMemDC(), 100, 400 + i * 20, str, strlen(str));


		//sprintf_s(str, "idY : %d", _vKnightMonster[i]->getIdY());
		//TextOut(getMemDC(), 100, 600 + i * 20, str, strlen(str));

		//sprintf_s(str, "state : %d", _vKnightMonster[i]->getState());
		//TextOut(getMemDC(), 100, 400 + i * 20, str, strlen(str));

		//sprintf_s(str, "viewY : %f", _vKnightMonster[i]->getViewY());
		//TextOut(getMemDC(), 100, 300 + i * 20, str, strlen(str));

		//sprintf_s(str, "x : %f", _vKnightMonster[i]->getX());
		//TextOut(getMemDC(), 300, 100 + i * 20, str, strlen(str));

		//sprintf_s(str, "hp : %d", _vKnightMonster[i]->getCurrentHP());
		//TextOut(getMemDC(), 300, 200 + i * 20, str, strlen(str));
	}

	//for (int i = 0; i < _vBallMonster.size(); i++)
	{
		//sprintf_s(str, "targetDistance : %f", _vBallMonster[i]->getTargetDistance());
		//TextOut(getMemDC(), 600, 100 + i * 20, str, strlen(str));

		//sprintf_s(str, "ballviewX : %f", _vBallMonster[i]->getViewX());
		//TextOut(getMemDC(), 600, 200 + i * 20, str, strlen(str));

		//sprintf_s(str, "ballviewY : %f", _vBallMonster[i]->getViewY());
		//TextOut(getMemDC(), 600, 300 + i * 20, str, strlen(str));

		//sprintf_s(str, "speed : %f", _vBallMonster[i]->getSpeed());
		//TextOut(getMemDC(), 800, 100 + i * 20, str, strlen(str));

		//sprintf_s(str, "bally : %f", _vBallMonster[i]->getY());
		//TextOut(getMemDC(), 800, 200 + i * 20, str, strlen(str));

		//sprintf_s(str, "isAttack : %d", _vBallMonster[i]->getIsAttack());
		//TextOut(getMemDC(), 100, 200 + i * 20, str, strlen(str));
	}

	//for (int i = 0; i < _vFireMonster.size(); i++)
	//{
	//	sprintf_s(str, "isAttack : %d", _vFireMonster[i]->getIsAttack());
	//	TextOut(getMemDC(), 600, 100 + i * 20, str, strlen(str));
	//}
}

//������Ʈ ����
void enemyManager::updateCollection()
{
	if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::FIRST_FLOOR || _dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::SECOND_FLOOR)
	{
		//�Ҹ��� ������Ʈ
		for (_viBallMonster = _vBallMonster.begin(); _viBallMonster != _vBallMonster.end(); _viBallMonster++)
		{
			(*_viBallMonster)->update();
		}
		//���̾���� ������Ʈ
		for (_viFireMonster = _vFireMonster.begin(); _viFireMonster != _vFireMonster.end(); _viFireMonster++)
		{
			(*_viFireMonster)->update();
		}
		//����Ʈ���� ������Ʈ
		for (_viKnightMonster = _vKnightMonster.begin(); _viKnightMonster != _vKnightMonster.end(); _viKnightMonster++)
		{
			(*_viKnightMonster)->update();
		}
		//���̾���� �Ѿ� ������Ʈ
		_fireMonsterBullet->update();
	}
	else if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::BOSS_FLOOR && _vBoss[0]->getCurrentHP() > 0)
	{
		//���� ������Ʈ
		_vBoss[0]->update();
	}

}

//������ ����
void enemyManager::drawAll(float cameraX, float cameraY)
{
	if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::FIRST_FLOOR || _dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::SECOND_FLOOR)
	{
		//������ ������
		for (_viBallMonster = _vBallMonster.begin(); _viBallMonster != _vBallMonster.end(); _viBallMonster++)
		{
			(*_viBallMonster)->render(cameraX, cameraY);
		}
		//���̾���� ������
		for (_viFireMonster = _vFireMonster.begin(); _viFireMonster != _vFireMonster.end(); _viFireMonster++)
		{
			(*_viFireMonster)->render(cameraX, cameraY);
		}
		//����Ʈ���� ������
		for (_viKnightMonster = _vKnightMonster.begin(); _viKnightMonster != _vKnightMonster.end(); _viKnightMonster++)
		{
			(*_viKnightMonster)->render(cameraX, cameraY);
		}
		//�Ҹ��� �Ѿ� ������
		for (int i = 0; i < _fireMonsterBullet->getVFireBullet().size(); i++)
		{
			_fireMonsterBullet->render(cameraX, cameraY);
		}
	}
	else if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::BOSS_FLOOR && _vBoss[0]->getCurrentHP() > 0)
	{
		if (_isBossAppear == true || _isOnce[BOSS_APPEAR] == true)
		{
			//���� ������
			_vBoss[0]->render(cameraX, cameraY);
		}
	}
}

//���ʹ� ����
void enemyManager::setEnemy()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ballMonster* bm;
			bm = new ballMonster;

			fireMonster* fm;
			fm = new fireMonster;
			if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::FIRST_FLOOR)
			{
				bm->init("ball", "ballMonster", GAMESIZEX / 4 + j * 300, 300 + i * 200, j, i, BALLMONSTER_HP);
				_vBallMonster.push_back(bm);

				fm->init("fire", "fireMonster", GAMESIZEX / 3 + j * 200, 500 + i * 300, j, i, FIREMONSTER_HP);
				_vFireMonster.push_back(fm);
			}
			else if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::SECOND_FLOOR)
			{
				bm->init("ball", "ballMonster", 200 + j * 700, 200 + i * 800, j, i, BALLMONSTER_HP);
				_vBallMonster.push_back(bm);

				fm->init("fire", "fireMonster", 200 + j * 680, 460 + i * 300, j, i, FIREMONSTER_HP);
				_vFireMonster.push_back(fm);

				knightMonster* km;
				km = new knightMonster;

				km->init("knight", "knightMonster", 2240 / 3 + j * 300, 1600 / 2 + i * 300, j, i, KNIGHTMONSTER_HP);
				_vKnightMonster.push_back(km);
			}
		}
	}

	//�ؿ��� ����
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::FIRST_FLOOR)
			{
				ballMonster* bm;
				bm = new ballMonster;

				bm->init("ball", "ballMonster", GAMESIZEX / 4 + j * 300, 1500 + i * 500, j, i, BALLMONSTER_HP);
				_vBallMonster.push_back(bm);
			}
		}
	}

	if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::BOSS_FLOOR)
	{
		boss* bs;
		bs = new boss;

		_shockAttack->x = bs->getX();
		_shockAttack->y = bs->getY();
		bs->init("boss", "boss", GAMESIZEX / 2 + 70, 1600, 0, 0, BOSS_HP);
		_vBoss.push_back(bs);
	}
}

//���ʹ̵��� AI
void enemyManager::enemyAI()
{
	//������
	for (int i = 0; i < _vBallMonster.size(); i++)
	{
		//���Ϳ� �÷��̾�� �Ÿ��� ������.
		_vBallMonster[i]->setTargetDistance(getDistance(_vBallMonster[i]->getX(), _vBallMonster[i]->getY(), _player->getPlayerX(), _player->getPlayerY()));
		RECT temp;
		if (IntersectRect(&temp, &_vBallMonster[i]->getRangeRect(), &_player->getPlayerRc()))
		{
			_vBallMonster[i]->setMoveType(FOLLOW_MOVE_TYPE); //�浹�ϸ� ���󰡴� Ÿ������
			_vBallMonster[i]->setState(BALLMONSTER_STATE_MOVE); //�׸��� ��ӿ�����.
			_vBallMonster[i]->setTargetAngle(getAngle(_vBallMonster[i]->getX(), _vBallMonster[i]->getY(), _player->getPlayerX(), _player->getPlayerY())); //���Ϳ� �÷��̾�� ���� ������.
			switch (_vBallMonster[i]->getState())
			{
			case BALLMONSTER_STATE_IDLE:
			case BALLMONSTER_STATE_MOVE:

				if (_vBallMonster[i]->getIsAttack() == false)
				{
					//�����ʰ��� �浹�����϶��� ���������� �Ѿƿ���
					if ((_vBallMonster[i]->getTargetAngle() < (PI / 180) * 25 && _vBallMonster[i]->getTargetAngle() > 0) ||
						(_vBallMonster[i]->getTargetAngle() > (PI / 180) * 335 && _vBallMonster[i]->getTargetAngle() < PI2))
					{
						_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_RIGHT);
					}
					//�������� �浹�����϶��� ���� �Ѿƿ���
					else if (_vBallMonster[i]->getTargetAngle() > (PI / 180) * 65 && _vBallMonster[i]->getTargetAngle() < (PI / 180) * 115)
					{
						_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_UP);
					}
					//���ʰ���
					else if (_vBallMonster[i]->getTargetAngle() > (PI / 180) * 155 && _vBallMonster[i]->getTargetAngle() < (PI / 180) * 205)
					{
						_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_LEFT);
					}
					//�Ʒ�����
					else if (_vBallMonster[i]->getTargetAngle() > (PI / 180) * 245 && _vBallMonster[i]->getTargetAngle() < (PI / 180) * 295)
					{
						_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_DOWN);
					}
				}
				break;
			}
		}
		//�Ÿ��� �־����� �ٽ� �⺻������Ÿ������
		else if (_vBallMonster[i]->getTargetDistance() > 480)
		{
			_vBallMonster[i]->setMoveType(BASIC_MOVE_TYPE);
			_vBallMonster[i]->setIsAttack(false);
		}

		//�Ÿ��� ��������� �����ϰ� �ð��Ҹ��� �����ϱ� ����.
		if (_vBallMonster[i]->getTargetDistance() < 300 && 1.5f + _vBallMonster[i]->getWorldTime() <= TIMEMANAGER->getWorldTime() && _vBallMonster[i]->getMoveType() == FOLLOW_MOVE_TYPE)
		{
			_vBallMonster[i]->setIsAttack(true);
		}

		//���ݻ��¶��
		if (_vBallMonster[i]->getIsAttack() == true && _vBallMonster[i]->getState() != BALLMONSTER_STATE_IDLE && _vBallMonster[i]->getState() == FOLLOW_MOVE_TYPE)
		{
			// 0.8������ ���ݰ��� �������
			if (0.8f + _vBallMonster[i]->getAttackWorldTime() >= TIMEMANAGER->getWorldTime())
			{
				_vBallMonster[i]->setAttackAngle(getAngle(_vBallMonster[i]->getX(), _vBallMonster[i]->getY(), _player->getPlayerX(), _player->getPlayerY()));
				//����� ó���� ���ǵ� 0�̶� �����Ӽӵ� ������ �ؼ� ������ �غ���.
				_vBallMonster[i]->getMotion()->setFPS(40);
				_vBallMonster[i]->setSpeed(0);
			}
			// 1�ʿ��� 1.5�� ���̿� ���ӵ��ָ鼭 ���ݰ����� ����
			else if (1.0f + _vBallMonster[i]->getAttackWorldTime() <= TIMEMANAGER->getWorldTime() && 1.5f + _vBallMonster[i]->getAttackWorldTime() > TIMEMANAGER->getWorldTime())
			{
				_vBallMonster[i]->setSpeed(_vBallMonster[i]->getSpeed() + 0.3f);
				_vBallMonster[i]->setX(_vBallMonster[i]->getX() + cosf(_vBallMonster[i]->getAttackAngle()) * _vBallMonster[i]->getSpeed());
				_vBallMonster[i]->setY(_vBallMonster[i]->getY() + -sinf(_vBallMonster[i]->getAttackAngle()) * _vBallMonster[i]->getSpeed());
			}

			//1.5�ʿ��� 2�ʻ��̿� ���ǵ�� ������ ������� ������.
			else if (1.5f + _vBallMonster[i]->getAttackWorldTime() <= TIMEMANAGER->getWorldTime() && 2.0f + _vBallMonster[i]->getAttackWorldTime() > TIMEMANAGER->getWorldTime())
			{
				_vBallMonster[i]->setSpeed(1.0f);
				_vBallMonster[i]->getMotion()->setFPS(5);
			}

			//2�ʰ� ������ ���ݲ�.
			else if (2.0f + _vBallMonster[i]->getAttackWorldTime() <= TIMEMANAGER->getWorldTime())
			{
				_vBallMonster[i]->setMoveType(BASIC_MOVE_TYPE);
				_vBallMonster[i]->setAttackWorldTime(TIMEMANAGER->getWorldTime());
				_vBallMonster[i]->setWorldTime(TIMEMANAGER->getWorldTime());
				_vBallMonster[i]->setIsAttack(false);
			}

			//�ε����� �ٲ���.
			if (IntersectRect(&temp, &_vBallMonster[i]->getRect(), &_player->getPlayerRc()))
			{
				_vBallMonster[i]->getMotion()->setFPS(5);
				_vBallMonster[i]->setWorldTime(TIMEMANAGER->getWorldTime());
				_vBallMonster[i]->setAttackWorldTime(TIMEMANAGER->getWorldTime());
				_vBallMonster[i]->setIsAttack(false);
				_vBallMonster[i]->setSpeed(1.0f);
			}
		}

	}

	//�Ҹ���
	for (int i = 0; i < _vFireMonster.size(); i++)
	{
		RECT temp;
		//�Ÿ� �����ְ�
		_vFireMonster[i]->setTargetDistance(getDistance(_vFireMonster[i]->getX(), _vFireMonster[i]->getY(), _player->getPlayerX(), _player->getPlayerY()));

		if (IntersectRect(&temp, &_vFireMonster[i]->getRangeRect(), &_player->getPlayerRc()))
		{
			//���� ������
			_vFireMonster[i]->setTargetAngle(getAngle(_vFireMonster[i]->getX(), _vFireMonster[i]->getY(), _player->getPlayerX(), _player->getPlayerY()));
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
				if (_vFireMonster[i]->getTargetDistance() < 400)
				{
					fireMonsterBulletFireCollision(i); //���Ѿ� �߻��ϰ� �浹�� �Լ�
				}
				break;
			}
		}
		else if (_vFireMonster[i]->getTargetDistance() > 500 && _vFireMonster[i]->getState() != FIREMONSTER_STATE_DEAD)
		{
			_vFireMonster[i]->setMoveType(BASIC_MOVE_TYPE);
		}

	}

	//��� ����
	for (int i = 0; i < _vKnightMonster.size(); i++)
	{
		RECT temp;
		//���� �÷��̾�� �Ÿ� ������.
		_vKnightMonster[i]->setTargetDistance(getDistance(_vKnightMonster[i]->getX(), _vKnightMonster[i]->getY(), _player->getPlayerX(), _player->getPlayerY()));

		//������Ʈ�� �浹�ϸ�
		if (IntersectRect(&temp, &_vKnightMonster[i]->getRangeRect(), &_player->getPlayerRc()))
		{
			//���� �÷��̾�� ���� ������.
			_vKnightMonster[i]->setTargetAngle(getAngle(_vKnightMonster[i]->getX(), _vKnightMonster[i]->getY(), _player->getPlayerX(), _player->getPlayerY()));
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
				break;
			}
		}
		//�����Ÿ� ������ �ٽ� �⺻������ Ÿ������
		else if (_vKnightMonster[i]->getTargetDistance() > 500 && _vKnightMonster[i]->getMoveType() == FOLLOW_MOVE_TYPE)
		{
			_vKnightMonster[i]->setMoveType(BASIC_MOVE_TYPE);
		}

		//�Ÿ��� ��������� ����
		if (_vKnightMonster[i]->getTargetDistance() <= 100 && _vKnightMonster[i]->getMoveType() == FOLLOW_MOVE_TYPE)
		{
			_vKnightMonster[i]->setAttackAngle(getAngle(_vKnightMonster[i]->getX(), _vKnightMonster[i]->getY(), _player->getPlayerX(), _player->getPlayerY()));
			//�����ʰ����� ������ �����ʰ�������
			if ((_vKnightMonster[i]->getAttackAngle() < (PI / 180) * 60 && _vKnightMonster[i]->getAttackAngle() > 0) ||
				(_vKnightMonster[i]->getAttackAngle() > (PI / 180) * 300 && _vKnightMonster[i]->getAttackAngle() < PI2))
			{
				_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_RIGHT);
				_vKnightMonster[i]->setState(KNIGHTMONSTER_STATE_ATTACK);
			}
			//���������� ������ ���ʰ�������
			else if (_vKnightMonster[i]->getAttackAngle() > (PI / 180) * 60 && _vKnightMonster[i]->getAttackAngle() < (PI / 180) * 120)
			{
				_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_UP);
				_vKnightMonster[i]->setState(KNIGHTMONSTER_STATE_ATTACK);
			}
			//���ʰ���
			else if (_vKnightMonster[i]->getAttackAngle() > (PI / 180) * 120 && _vKnightMonster[i]->getAttackAngle() < (PI / 180) * 210)
			{
				_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_LEFT);
				_vKnightMonster[i]->setState(KNIGHTMONSTER_STATE_ATTACK);
			}
			//�Ʒ�����
			else if (_vKnightMonster[i]->getAttackAngle() > (PI / 180) * 210 && _vKnightMonster[i]->getAttackAngle() < (PI / 180) * 300)
			{
				_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_DOWN);
				_vKnightMonster[i]->setState(KNIGHTMONSTER_STATE_ATTACK);
			}
			_vKnightMonster[i]->setAttackWorldTime(TIMEMANAGER->getWorldTime());
		}
		//�־����� �������� ������
		else if (_vKnightMonster[i]->getTargetDistance() > 100 && 1.0f + _vKnightMonster[i]->getAttackWorldTime() <= TIMEMANAGER->getWorldTime())
		{
			if (_vKnightMonster[i]->getRndState() == 0) _vKnightMonster[i]->setState(KNIGHTMONSTER_STATE_MOVE);
			else _vKnightMonster[i]->setState(KNIGHTMONSTER_STATE_IDLE);
		}
	}
}

//�÷��̾� ���ݿ� ���ʹ̵��� ���� �Լ�
void enemyManager::playerAttackEnemyCollision()
{
	//�÷��̾��� ���ݷ�Ʈ
	//������ ����, �뽬����, ���Ӱ���, ��������, �뽬 ��������
	if ((_player->getPlayerAni()->getFramePosArr().x >= 1 && _player->getPlayerAni()->getFramePosArr().x < 4 && _player->getPlayerAni()->getFramePosArr().y == 13)
		|| (_player->getPlayerAni()->getFramePosArr().x <= 2 && _player->getPlayerAni()->getFramePosArr().y == 28)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 180 && _player->getPlayerAni()->getFramePosArrOnce() <= 187)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 230 && _player->getPlayerAni()->getFramePosArrOnce() <= 235)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 279 && _player->getPlayerAni()->getFramePosArrOnce() <= 280))
	{
		_playerAttackRc = RectMakeCenter(_player->getPlayerX() + 30, _player->getPlayerY(), 100, 30);
	}
	//���� ����, �뽬����, ���Ӱ��� , ��������, �뽬 ��������
	else if ((_player->getPlayerAni()->getFramePosArr().x >= 5 && _player->getPlayerAni()->getFramePosArr().y == 13)
		|| (_player->getPlayerAni()->getFramePosArr().x >= 4 && _player->getPlayerAni()->getFramePosArr().y == 28)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 192 && _player->getPlayerAni()->getFramePosArrOnce() <= 199)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 242 && _player->getPlayerAni()->getFramePosArrOnce() <= 247)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 291 && _player->getPlayerAni()->getFramePosArrOnce() <= 292))
	{
		_playerAttackRc = RectMakeCenter(_player->getPlayerX() - 30, _player->getPlayerY(), 100, 30);
	}
	//������, �뽬����, ���Ӱ���, ��������, �뽬 ��������
	else if ((_player->getPlayerAni()->getFramePosArr().x >= 1 && _player->getPlayerAni()->getFramePosArr().x < 4 && _player->getPlayerAni()->getFramePosArr().y == 14)
		|| (_player->getPlayerAni()->getFramePosArr().x <= 2 && _player->getPlayerAni()->getFramePosArr().y == 29)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 204 && _player->getPlayerAni()->getFramePosArrOnce() <= 209)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 255 && _player->getPlayerAni()->getFramePosArrOnce() <= 259)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 303 && _player->getPlayerAni()->getFramePosArrOnce() <= 304))
	{
		_playerAttackRc = RectMakeCenter(_player->getPlayerX(), _player->getPlayerY() - 30, 30, 100);
	}
	//�Ʒ� ����, �뽬����, ���Ӱ���, ��������, �뽬 ��������
	else if ((_player->getPlayerAni()->getFramePosArr().x >= 5 && _player->getPlayerAni()->getFramePosArr().y == 14)
		|| (_player->getPlayerAni()->getFramePosArr().x >= 4 && _player->getPlayerAni()->getFramePosArr().y == 29)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 216 && _player->getPlayerAni()->getFramePosArrOnce() <= 222)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 267 && _player->getPlayerAni()->getFramePosArrOnce() <= 271)
		|| (_player->getPlayerAni()->getFramePosArrOnce() >= 315 && _player->getPlayerAni()->getFramePosArrOnce() <= 316))
	{
		_playerAttackRc = RectMakeCenter(_player->getPlayerX(), _player->getPlayerY() + 30, 30, 100);
	}
	//���ݳ����� ��Ʈ����.
	else _playerAttackRc = RectMakeCenter(NULL, NULL, NULL, NULL);

	switch (_player->getPlayerState())
	{
	case PLAYER_ATTACK:
	case PLAYER_JUMP_ATTACK:
	case PLAYER_DASH_JUMP_ATTACK:
	case PLAYER_DASH_ATTACK:
	case PLAYER_COMBINATION:

		RECT temp;
		if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::FIRST_FLOOR || _dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::SECOND_FLOOR)
		{
			//������
			for (int i = 0; i < _vBallMonster.size(); i++)
			{
				if (IntersectRect(&temp, &_playerAttackRc, &_vBallMonster[i]->getRect()) && _vBallMonster[i]->getIsHit() == false)
				{
					_vBallMonster[i]->setIsHit(true);
					_vBallMonster[i]->setCurrentHP(_vBallMonster[i]->getCurrentHP() - _player->getPlayerStr());
					if (_player->getPlayerDirection() == LEFT)
					{
						_vBallMonster[i]->setX(_vBallMonster[i]->getX() - 10);
					}
					else if (_player->getPlayerDirection() == RIGHT)
					{
						_vBallMonster[i]->setX(_vBallMonster[i]->getX() + 10);
					}
					else if (_player->getPlayerDirection() == UP)
					{
						_vBallMonster[i]->setY(_vBallMonster[i]->getY() - 10);
					}
					else if (_player->getPlayerDirection() == DOWN)
					{
						_vBallMonster[i]->setY(_vBallMonster[i]->getY() + 10);
					}
				}
			}

			//�Ҹ���
			for (int i = 0; i < _vFireMonster.size(); i++)
			{
				if (IntersectRect(&temp, &_playerAttackRc, &_vFireMonster[i]->getRect()) && _vFireMonster[i]->getIsHit() == false)
				{
					_vFireMonster[i]->setIsHit(true);
					_vFireMonster[i]->setCurrentHP(_vFireMonster[i]->getCurrentHP() - _player->getPlayerStr());
					if (_player->getPlayerDirection() == LEFT)
					{
						_vFireMonster[i]->setX(_vFireMonster[i]->getX() - 10);
					}
					else if (_player->getPlayerDirection() == RIGHT)
					{
						_vFireMonster[i]->setX(_vFireMonster[i]->getX() + 10);
					}
					else if (_player->getPlayerDirection() == UP)
					{
						_vFireMonster[i]->setY(_vFireMonster[i]->getY() - 10);
					}
					else if (_player->getPlayerDirection() == DOWN)
					{
						_vFireMonster[i]->setY(_vFireMonster[i]->getY() + 10);
					}
				}
			}

			//������
			for (int i = 0; i < _vKnightMonster.size(); i++)
			{
				if (IntersectRect(&temp, &_playerAttackRc, &_vKnightMonster[i]->getRect()) && _vKnightMonster[i]->getIsHit() == false)
				{
					_vKnightMonster[i]->setCurrentHP(_vKnightMonster[i]->getCurrentHP() - _player->getPlayerStr());
					_vKnightMonster[i]->setIsHit(true);
					if (_player->getPlayerDirection() == LEFT)
					{
						_vKnightMonster[i]->setX(_vKnightMonster[i]->getX() - 10);
					}
					else if (_player->getPlayerDirection() == RIGHT)
					{
						_vKnightMonster[i]->setX(_vKnightMonster[i]->getX() + 10);
					}
					else if (_player->getPlayerDirection() == UP)
					{
						_vKnightMonster[i]->setY(_vKnightMonster[i]->getY() - 10);
					}
					else if (_player->getPlayerDirection() == DOWN)
					{
						_vKnightMonster[i]->setY(_vKnightMonster[i]->getY() + 10);
					}
				}
			}
		}

		//�������϶���
		else if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::BOSS_FLOOR)
		{
			if (IntersectRect(&temp, &_playerAttackRc, &_vBoss[0]->getRect()) && _vBoss[0]->getIsHit() == false)
			{
				_vBoss[0]->setIsHit(true);
				_vBoss[0]->setCurrentHP(_vBoss[0]->getCurrentHP() - _player->getPlayerStr());
			}
		}


		break;
	}
}

//���Ѿ� �߻�, �浹�Լ�
void enemyManager::fireMonsterBulletFireCollision(int num)
{
	//���̾� ���Ͱ� �� �Ѿ��� ī��Ʈ �߻簡 true���
	if (_vFireMonster[num]->fireBulletCountFire() == true)
	{
		RECT fireRc; //�Ҹ��� ��Ʈ
		fireRc = _vFireMonster[num]->getRect();

		//�Ҹ��Ͱ� �Ѿ��� ���⺰�� �����.
		for (int i = 0; i < 4; i++)
		{
			_fireMonsterBullet->fire(fireRc.left + (fireRc.right - fireRc.left) / 2, fireRc.top + (fireRc.bottom - fireRc.top) / 2, PI + i * (PI / 2), _fireBulletSpeed);
		}
	}

	//���̾���� �Ѿ� �浹
	for (int i = 0; i < _fireMonsterBullet->getVFireBullet().size(); i++)
	{
		//Ÿ�� �浹��Ʈ�� ������Ʈ ����.
		(*_fireMonsterBullet->setVFireBullet())[i].tileCollisionRc = _fireMonsterBullet->getVFireBullet()[i].rc;

		//�ε��� �����ֱ�
		(*_fireMonsterBullet->setVFireBullet())[i].idX = _fireMonsterBullet->getVFireBullet()[i].x / TileSIZE;
		(*_fireMonsterBullet->setVFireBullet())[i].idY = _fireMonsterBullet->getVFireBullet()[i].y / TileSIZE;

		//Ÿ�� �ε����� �˻� 8���� �˻�
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[0].x = _fireMonsterBullet->getVFireBullet()[i].idX - 1;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[0].y = _fireMonsterBullet->getVFireBullet()[i].idY;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[1].x = _fireMonsterBullet->getVFireBullet()[i].idX - 1;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[1].y = _fireMonsterBullet->getVFireBullet()[i].idY - 1;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[2].x = _fireMonsterBullet->getVFireBullet()[i].idX - 1;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[2].y = _fireMonsterBullet->getVFireBullet()[i].idY + 1;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[3].x = _fireMonsterBullet->getVFireBullet()[i].idX;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[3].y = _fireMonsterBullet->getVFireBullet()[i].idY - 1;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[4].x = _fireMonsterBullet->getVFireBullet()[i].idX;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[4].y = _fireMonsterBullet->getVFireBullet()[i].idY + 1;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[5].x = _fireMonsterBullet->getVFireBullet()[i].idX + 1;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[5].y = _fireMonsterBullet->getVFireBullet()[i].idY;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[6].x = _fireMonsterBullet->getVFireBullet()[i].idX + 1;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[6].y = _fireMonsterBullet->getVFireBullet()[i].idY - 1;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[7].x = _fireMonsterBullet->getVFireBullet()[i].idX + 1;
		(*_fireMonsterBullet->setVFireBullet())[i].tileIndex[7].y = _fireMonsterBullet->getVFireBullet()[i].idY + 1;

		RECT temp;
		for (int j = 0; j < 8; j++)
		{
			//Ÿ�Ͽ� �ε�����
			if (_dungeonMap->getAttr(_fireMonsterBullet->getVFireBullet()[i].tileIndex[j].x, _fireMonsterBullet->getVFireBullet()[i].tileIndex[j].y) == ATTR_UNMOVE
				&& IntersectRect(&temp, &_dungeonMap->getTile(_fireMonsterBullet->getVFireBullet()[i].tileIndex[j].x, _fireMonsterBullet->getVFireBullet()[i].tileIndex[j].y)->rc, &_fireMonsterBullet->getVFireBullet()[i].tileCollisionRc))
			{
				//�Ѿ� ������
				(*_fireMonsterBullet->setVFireBullet())[i].isCollision = true;
				break;
			}

		}

		(*_fireMonsterBullet->setVFireBullet())[i].tileCollisionRc = RectMakeCenter(_fireMonsterBullet->getVFireBullet()[i].x, _fireMonsterBullet->getVFireBullet()[i].y, _fireMonsterBullet->getVFireBullet()[i].image->getFrameWidth(), _fireMonsterBullet->getVFireBullet()[i].image->getFrameHeight());

		if (IntersectRect(&temp, &_fireMonsterBullet->getVFireBullet()[i].rc, &_player->getPlayerRc()))
		{
			(*_fireMonsterBullet->setVFireBullet())[i].isCollision = true;
			_vFireMonster[num]->setAttackWorldTime(TIMEMANAGER->getWorldTime());
			_vFireMonster[num]->setIsAttack(true); //�Ұ��ݿ� �¾Ҵ�.
		}
		//0.8�ʵ� ���ݲ���
		if (0.8f + _vFireMonster[num]->getAttackWorldTime() <= TIMEMANAGER->getWorldTime())
		{
			_vFireMonster[num]->setIsAttack(false);
		}
	}
}

//���ʹ̵��� �����Լ�
void enemyManager::enemyDead()
{
	if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::FIRST_FLOOR || _dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::SECOND_FLOOR)
	{
		//������
		for (int i = 0; i < _vBallMonster.size();)
		{
			if (_vBallMonster[i]->getCurrentHP() <= 0.0f)
			{
				EFFECTMANAGER->play("deadEffect", _vBallMonster[i]->getViewX(), _vBallMonster[i]->getViewY());

				//���������� ������ġ �ް� Ǫ�ù�
				POINTFLOAT monster;
				monster.x = _vBallMonster[i]->getX() - TileSIZE;
				monster.y = _vBallMonster[i]->getY() - TileSIZE;
				_vEnemyDeadPoint.push_back(monster);
				_player->setPlayerExp(_player->getPlayerExp() + 4);
				_vBallMonster.erase(_vBallMonster.begin() + i);
			}
			else i++;
		}

		//�Ҹ���
		for (int i = 0; i < _vFireMonster.size();)
		{
			if (_vFireMonster[i]->getCurrentHP() <= 0.0f)
			{
				EFFECTMANAGER->play("deadEffect", _vFireMonster[i]->getViewX(), _vFireMonster[i]->getViewY());

				//���������� ������ġ �ް� Ǫ�ù�
				POINTFLOAT monster;
				monster.x = _vFireMonster[i]->getX() - _vFireMonster[i]->getImage()->getFrameWidth();
				monster.y = _vFireMonster[i]->getY() - _vFireMonster[i]->getImage()->getFrameHeight() / 2;
				_vEnemyDeadPoint.push_back(monster);
				_player->setPlayerExp(_player->getPlayerExp() + 5);
				_vFireMonster.erase(_vFireMonster.begin() + i);
			}
			else i++;
		}

		//������
		for (int i = 0; i < _vKnightMonster.size();)
		{
			if (_vKnightMonster[i]->getCurrentHP() <= 0.0f)
			{
				EFFECTMANAGER->play("deadEffect", _vKnightMonster[i]->getViewX(), _vKnightMonster[i]->getViewY());

				//���������� ������ġ �ް� Ǫ�ù�
				POINTFLOAT monster;
				monster.x = _vKnightMonster[i]->getX() - TileSIZE;
				monster.y = _vKnightMonster[i]->getY();
				_vEnemyDeadPoint.push_back(monster);
				_player->setPlayerExp(_player->getPlayerExp() + 6);
				_vKnightMonster.erase(_vKnightMonster.begin() + i);
			}
			else i++;
		}
	}

	//�������϶���
	if (_dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::BOSS_FLOOR)
	{
		if (_vBoss[0]->getCurrentHP() <= 0.0f)
		{
			//erase ������ �̷������� ó����.
			if (_isOnce[4] == false)
			{
				EFFECTMANAGER->play("bossDeadEffect", _vBoss[0]->getViewX(), _vBoss[0]->getViewY());
				_isOnce[4] = true;
			}
			//_vBoss.erase(_vBoss.begin());
		}
	}

}

//Ÿ�ϰ��� �浹 �Լ�
void enemyManager::tileCheckObjectCollision()
{
	//������
	for (int i = 0; i < _vBallMonster.size(); i++)
	{
		//�浹��Ʈ�� ������Ʈ ����
		_vBallMonster[i]->setTileCollisionRect(_vBallMonster[i]->getRect());

		//�ε��� �����ֱ�
		_vBallMonster[i]->setIdX(_vBallMonster[i]->getX() / TileSIZE);
		_vBallMonster[i]->setIdY(_vBallMonster[i]->getY() / TileSIZE);

		//���⿡���� Ÿ���ε����� Ÿ�ϰ����ϱ� ����
		switch (_vBallMonster[i]->getDirection())
		{
		case BALLMONSTER_DIRECTION_LEFT:
			_vBallMonster[i]->setTileIndex()[0].x = _vBallMonster[i]->getIdX() - 1;
			_vBallMonster[i]->setTileIndex()[0].y = _vBallMonster[i]->getIdY();
			_vBallMonster[i]->setTileIndex()[1].x = _vBallMonster[i]->getIdX() - 1;
			_vBallMonster[i]->setTileIndex()[1].y = _vBallMonster[i]->getIdY() + 1;
			_vBallMonster[i]->setTileIndex()[2].x = _vBallMonster[i]->getIdX() - 1;
			_vBallMonster[i]->setTileIndex()[2].y = _vBallMonster[i]->getIdY() - 1;
			break;
		case BALLMONSTER_DIRECTION_UP:
			_vBallMonster[i]->setTileIndex()[0].x = _vBallMonster[i]->getIdX();
			_vBallMonster[i]->setTileIndex()[0].y = _vBallMonster[i]->getIdY() - 1;
			_vBallMonster[i]->setTileIndex()[1].x = _vBallMonster[i]->getIdX() + 1;
			_vBallMonster[i]->setTileIndex()[1].y = _vBallMonster[i]->getIdY() - 1;
			_vBallMonster[i]->setTileIndex()[2].x = _vBallMonster[i]->getIdX() + 2;
			_vBallMonster[i]->setTileIndex()[2].y = _vBallMonster[i]->getIdY() - 1;
			break;
		case BALLMONSTER_DIRECTION_RIGHT:
			_vBallMonster[i]->setTileIndex()[0].x = _vBallMonster[i]->getIdX() + 1;
			_vBallMonster[i]->setTileIndex()[0].y = _vBallMonster[i]->getIdY();
			_vBallMonster[i]->setTileIndex()[1].x = _vBallMonster[i]->getIdX() + 1;
			_vBallMonster[i]->setTileIndex()[1].y = _vBallMonster[i]->getIdY() + 1;
			_vBallMonster[i]->setTileIndex()[2].x = _vBallMonster[i]->getIdX() + 1;
			_vBallMonster[i]->setTileIndex()[2].y = _vBallMonster[i]->getIdY() - 1;
			break;
		case BALLMONSTER_DIRECTION_DOWN:
			_vBallMonster[i]->setTileIndex()[0].x = _vBallMonster[i]->getIdX();
			_vBallMonster[i]->setTileIndex()[0].y = _vBallMonster[i]->getIdY() + 1;
			_vBallMonster[i]->setTileIndex()[1].x = _vBallMonster[i]->getIdX() + 1;
			_vBallMonster[i]->setTileIndex()[1].y = _vBallMonster[i]->getIdY() + 1;
			_vBallMonster[i]->setTileIndex()[2].x = _vBallMonster[i]->getIdX() + 2;
			_vBallMonster[i]->setTileIndex()[2].y = _vBallMonster[i]->getIdY() + 1;
			break;
		}

		RECT temp;
		//�ӽÿ�����Ʈ ��Ʈ �浹��
		if (IntersectRect(&temp, &_objectRc, &_vBallMonster[i]->getRect()) && _dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::SECOND_FLOOR)
		{
			switch (_vBallMonster[i]->getDirection())
			{
			case BALLMONSTER_DIRECTION_UP:
				_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().top - _objectRc.bottom);
				_vBallMonster[i]->setY(_vBallMonster[i]->getY() - _vBallMonster[i]->getTum());
				break;
			case BALLMONSTER_DIRECTION_DOWN:
				_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().bottom - _objectRc.top);
				_vBallMonster[i]->setY(_vBallMonster[i]->getY() - _vBallMonster[i]->getTum());
				break;
			}
			if (_vBallMonster[i]->getIsAttack() == true)
			{
				_vBallMonster[i]->setSpeed(0);
				_vBallMonster[i]->getMotion()->setFPS(5);
				_vBallMonster[i]->setIsAttack(false);
				_vBallMonster[i]->setAttackWorldTime(TIMEMANAGER->getWorldTime());
				_vBallMonster[i]->setWorldTime(TIMEMANAGER->getWorldTime());
			}
		}

		for (int j = 0; j < 3; i++)
		{
			if (_dungeonMap->getAttr(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y) == ATTR_UNMOVE
				&& IntersectRect(&temp, &_dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc, &_vBallMonster[i]->getTileCollisionRect()))
			{
				//�������� ������ ����.
				_rndMove[BALLMONSTER] = RND->getRandomInt(1, 3);

				if (_vBallMonster[i]->getState() != BALLMONSTER_STATE_IDLE && _vBallMonster[i]->getIsAttack() == false)
				{
					switch (_vBallMonster[i]->getDirection())
					{
					case BALLMONSTER_DIRECTION_LEFT:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().left - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.right);
						_vBallMonster[i]->setX(_vBallMonster[i]->getX() - _vBallMonster[i]->getTum());

						if (_rndMove[BALLMONSTER] == 1)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_DOWN);
						}
						else if (_rndMove[BALLMONSTER] == 2)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_RIGHT);
						}
						else if (_rndMove[BALLMONSTER] == 3)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_UP);
						}
						break;
					case BALLMONSTER_DIRECTION_RIGHT:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().right - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.left);
						_vBallMonster[i]->setX(_vBallMonster[i]->getX() - _vBallMonster[i]->getTum());

						if (_rndMove[BALLMONSTER] == 1)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_DOWN);
						}
						else if (_rndMove[BALLMONSTER] == 2)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_LEFT);
						}
						else if (_rndMove[BALLMONSTER] == 3)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_UP);
						}
						break;
					case BALLMONSTER_DIRECTION_UP:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().top - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.bottom);
						_vBallMonster[i]->setY(_vBallMonster[i]->getY() - _vBallMonster[i]->getTum());

						if (_rndMove[BALLMONSTER] == 1)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_DOWN);
						}
						else if (_rndMove[BALLMONSTER] == 2)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_RIGHT);
						}
						else if (_rndMove[BALLMONSTER] == 3)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_LEFT);
						}
						break;
					case BALLMONSTER_DIRECTION_DOWN:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().bottom - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.top);
						_vBallMonster[i]->setY(_vBallMonster[i]->getY() - _vBallMonster[i]->getTum());

						if (_rndMove[BALLMONSTER] == 1)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_LEFT);
						}
						else if (_rndMove[BALLMONSTER] == 2)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_RIGHT);
						}
						else if (_rndMove[BALLMONSTER] == 3)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_UP);
						}
						break;
					}

				}
				else if (_vBallMonster[i]->getIsAttack() == true && _vBallMonster[i]->getState() != BALLMONSTER_STATE_IDLE)
				{
					_vBallMonster[i]->setSpeed(0);
					_vBallMonster[i]->getMotion()->setFPS(5);
					_vBallMonster[i]->setIsAttack(false);
					_vBallMonster[i]->setAttackWorldTime(TIMEMANAGER->getWorldTime());
					_vBallMonster[i]->setWorldTime(TIMEMANAGER->getWorldTime());

					switch (_vBallMonster[i]->getDirection())
					{
					case BALLMONSTER_DIRECTION_LEFT:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().left - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.right);
						_vBallMonster[i]->setX(_vBallMonster[i]->getX() - _vBallMonster[i]->getTum());

						if (_rndMove[BALLMONSTER] == 1)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_DOWN);
						}
						else if (_rndMove[BALLMONSTER] == 2)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_RIGHT);
						}
						else if (_rndMove[BALLMONSTER] == 3)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_UP);
						}
						break;
					case BALLMONSTER_DIRECTION_RIGHT:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().right - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.left);
						_vBallMonster[i]->setX(_vBallMonster[i]->getX() - _vBallMonster[i]->getTum());

						if (_rndMove[BALLMONSTER] == 1)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_LEFT);
						}
						else if (_rndMove[BALLMONSTER] == 2)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_DOWN);
						}
						else if (_rndMove[BALLMONSTER] == 3)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_UP);
						}
						break;
					case BALLMONSTER_DIRECTION_UP:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().top - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.bottom);
						_vBallMonster[i]->setY(_vBallMonster[i]->getY() - _vBallMonster[i]->getTum());

						if (_rndMove[BALLMONSTER] == 1)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_LEFT);
						}
						else if (_rndMove[BALLMONSTER] == 2)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_RIGHT);
						}
						else if (_rndMove[BALLMONSTER] == 3)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_DOWN);
						}
						break;
					case BALLMONSTER_DIRECTION_DOWN:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().bottom - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.top);
						_vBallMonster[i]->setY(_vBallMonster[i]->getY() - _vBallMonster[i]->getTum());

						if (_rndMove[BALLMONSTER] == 1)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_LEFT);
						}
						else if (_rndMove[BALLMONSTER] == 2)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_RIGHT);
						}
						else if (_rndMove[BALLMONSTER] == 3)
						{
							_vBallMonster[i]->setDirection(BALLMONSTER_DIRECTION_UP);
						}
						break;
					}
				}
			}
			break;
		}
		_vBallMonster[i]->setTileCollisionRect(RectMakeCenter(_vBallMonster[i]->getX(), _vBallMonster[i]->getY(), _vBallMonster[i]->getImage()->getFrameWidth(), _vBallMonster[i]->getImage()->getFrameHeight()));
	}

	//�Ҹ���
	for (int i = 0; i < _vFireMonster.size(); i++)
	{
		_vFireMonster[i]->setTileCollisionRect(_vFireMonster[i]->getRect());

		//�ε��� �����ֱ�
		_vFireMonster[i]->setIdX(_vFireMonster[i]->getX() / TileSIZE);
		_vFireMonster[i]->setIdY(_vFireMonster[i]->getY() / TileSIZE);

		//�����˻�
		switch (_vFireMonster[i]->getDirection())
		{
		case FIREMONSTER_DIRECTION_LEFT:
			_vFireMonster[i]->setTileIndex()[0].x = _vFireMonster[i]->getIdX() - 1;
			_vFireMonster[i]->setTileIndex()[0].y = _vFireMonster[i]->getIdY();
			_vFireMonster[i]->setTileIndex()[1].x = _vFireMonster[i]->getIdX() - 1;
			_vFireMonster[i]->setTileIndex()[1].y = _vFireMonster[i]->getIdY() + 1;
			break;
		case FIREMONSTER_DIRECTION_UP:
			_vFireMonster[i]->setTileIndex()[0].x = _vFireMonster[i]->getIdX();
			_vFireMonster[i]->setTileIndex()[0].y = _vFireMonster[i]->getIdY() - 1;
			_vFireMonster[i]->setTileIndex()[1].x = _vFireMonster[i]->getIdX() + 1;
			_vFireMonster[i]->setTileIndex()[1].y = _vFireMonster[i]->getIdY() - 1;
			break;
		case FIREMONSTER_DIRECTION_RIGHT:
			_vFireMonster[i]->setTileIndex()[0].x = _vFireMonster[i]->getIdX() + 1;
			_vFireMonster[i]->setTileIndex()[0].y = _vFireMonster[i]->getIdY();
			_vFireMonster[i]->setTileIndex()[1].x = _vFireMonster[i]->getIdX() + 1;
			_vFireMonster[i]->setTileIndex()[1].y = _vFireMonster[i]->getIdY() + 1;
			break;
		case FIREMONSTER_DIRECTION_DOWN:
			_vFireMonster[i]->setTileIndex()[0].x = _vFireMonster[i]->getIdX();
			_vFireMonster[i]->setTileIndex()[0].y = _vFireMonster[i]->getIdY() + 1;
			_vFireMonster[i]->setTileIndex()[1].x = _vFireMonster[i]->getIdX() + 1;
			_vFireMonster[i]->setTileIndex()[1].y = _vFireMonster[i]->getIdY() + 1;
			break;
		}

		RECT temp;
		//�ӽÿ�����Ʈ ��Ʈ �浹�� 2���϶���
		if (IntersectRect(&temp, &_objectRc, &_vFireMonster[i]->getRect()) && _dungeonMap->getDungeonFloor() == DUNGEON_FLOOR::SECOND_FLOOR)
		{
			switch (_vFireMonster[i]->getDirection())
			{
			case FIREMONSTER_DIRECTION_UP:
				_vFireMonster[i]->setTum(_vFireMonster[i]->getRect().top - _objectRc.bottom);
				_vFireMonster[i]->setY(_vFireMonster[i]->getY() - _vFireMonster[i]->getTum());
				break;
			case FIREMONSTER_DIRECTION_DOWN:
				_vFireMonster[i]->setTum(_vFireMonster[i]->getRect().bottom - _objectRc.top);
				_vFireMonster[i]->setY(_vFireMonster[i]->getY() - _vFireMonster[i]->getTum());
				break;
			}
		}
		for (int j = 0; j < 2; i++)
		{
			if (_dungeonMap->getAttr(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y) == ATTR_UNMOVE
				&& IntersectRect(&temp, &_dungeonMap->getTile(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y)->rc, &_vFireMonster[i]->getTileCollisionRect()))
			{
				//�������� ������ ����.
				_rndMove[FIREMONSTER] = RND->getRandomInt(1, 3);

				switch (_vFireMonster[i]->getDirection())
				{
				case FIREMONSTER_DIRECTION_LEFT:
					_vFireMonster[i]->setTum(_vFireMonster[i]->getRect().left - _dungeonMap->getTile(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y)->rc.right);
					_vFireMonster[i]->setX(_vFireMonster[i]->getX() - _vFireMonster[i]->getTum());

					if (_rndMove[FIREMONSTER] == 1)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_DOWN);
					}
					else if (_rndMove[FIREMONSTER] == 2)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_RIGHT);
					}
					else if (_rndMove[FIREMONSTER] == 3)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_UP);
					}
					break;
				case FIREMONSTER_DIRECTION_RIGHT:
					_vFireMonster[i]->setTum(_vFireMonster[i]->getRect().right - _dungeonMap->getTile(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y)->rc.left);
					_vFireMonster[i]->setX(_vFireMonster[i]->getX() - _vFireMonster[i]->getTum());

					if (_rndMove[FIREMONSTER] == 1)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_DOWN);
					}
					else if (_rndMove[FIREMONSTER] == 2)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_LEFT);
					}
					else if (_rndMove[FIREMONSTER] == 3)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_UP);
					}
					break;
				case FIREMONSTER_DIRECTION_UP:
					_vFireMonster[i]->setTum(_vFireMonster[i]->getRect().top - _dungeonMap->getTile(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y)->rc.bottom);
					_vFireMonster[i]->setY(_vFireMonster[i]->getY() - _vFireMonster[i]->getTum());

					if (_rndMove[FIREMONSTER] == 1)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_DOWN);
					}
					else if (_rndMove[FIREMONSTER] == 2)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_LEFT);
					}
					else if (_rndMove[FIREMONSTER] == 3)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_RIGHT);
					}
					break;
				case FIREMONSTER_DIRECTION_DOWN:
					_vFireMonster[i]->setTum(_vFireMonster[i]->getRect().bottom - _dungeonMap->getTile(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y)->rc.top);
					_vFireMonster[i]->setY(_vFireMonster[i]->getY() - _vFireMonster[i]->getTum());

					if (_rndMove[FIREMONSTER] == 1)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_RIGHT);
					}
					else if (_rndMove[FIREMONSTER] == 2)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_LEFT);
					}
					else if (_rndMove[FIREMONSTER] == 3)
					{
						_vFireMonster[i]->setDirection(FIREMONSTER_DIRECTION_UP);
					}
					break;
				}

			}
			break;
		}
		_vFireMonster[i]->setTileCollisionRect(RectMakeCenter(_vFireMonster[i]->getX(), _vFireMonster[i]->getY(), _vFireMonster[i]->getImage()->getFrameWidth(), _vFireMonster[i]->getImage()->getFrameHeight()));

	}

	//������
	for (int i = 0; i < _vKnightMonster.size(); i++)
	{
		_vKnightMonster[i]->setTileCollisionRect(_vKnightMonster[i]->getRect());

		//�ε��� �����ֱ�
		_vKnightMonster[i]->setIdX(_vKnightMonster[i]->getX() / TileSIZE);
		_vKnightMonster[i]->setIdY(_vKnightMonster[i]->getY() / TileSIZE);

		//�����˻�
		switch (_vKnightMonster[i]->getDirection())
		{
		case KNIGHTMONSTER_DIRECTION_LEFT:
			_vKnightMonster[i]->setTileIndex()[0].x = _vKnightMonster[i]->getIdX() - 1;
			_vKnightMonster[i]->setTileIndex()[0].y = _vKnightMonster[i]->getIdY();
			_vKnightMonster[i]->setTileIndex()[1].x = _vKnightMonster[i]->getIdX() - 1;
			_vKnightMonster[i]->setTileIndex()[1].y = _vKnightMonster[i]->getIdY() + 1;
			break;
		case KNIGHTMONSTER_DIRECTION_UP:
			_vKnightMonster[i]->setTileIndex()[0].x = _vKnightMonster[i]->getIdX();
			_vKnightMonster[i]->setTileIndex()[0].y = _vKnightMonster[i]->getIdY() - 1;
			_vKnightMonster[i]->setTileIndex()[1].x = _vKnightMonster[i]->getIdX() + 1;
			_vKnightMonster[i]->setTileIndex()[1].y = _vKnightMonster[i]->getIdY() - 1;
			break;
		case KNIGHTMONSTER_DIRECTION_RIGHT:
			_vKnightMonster[i]->setTileIndex()[0].x = _vKnightMonster[i]->getIdX() + 1;
			_vKnightMonster[i]->setTileIndex()[0].y = _vKnightMonster[i]->getIdY();
			_vKnightMonster[i]->setTileIndex()[1].x = _vKnightMonster[i]->getIdX() + 1;
			_vKnightMonster[i]->setTileIndex()[1].y = _vKnightMonster[i]->getIdY() + 1;
			break;
		case KNIGHTMONSTER_DIRECTION_DOWN:
			_vKnightMonster[i]->setTileIndex()[0].x = _vKnightMonster[i]->getIdX();
			_vKnightMonster[i]->setTileIndex()[0].y = _vKnightMonster[i]->getIdY() + 1;
			_vKnightMonster[i]->setTileIndex()[1].x = _vKnightMonster[i]->getIdX() + 1;
			_vKnightMonster[i]->setTileIndex()[1].y = _vKnightMonster[i]->getIdY() + 1;
			break;
		}
		RECT temp;
		for (int j = 0; j < 2; j++)
		{
			if (_dungeonMap->getAttr(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y) == ATTR_UNMOVE
				&& IntersectRect(&temp, &_dungeonMap->getTile(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y)->rc, &_vKnightMonster[i]->getTileCollisionRect()))
			{
				//�������� ������.
				_rndMove[KNIGHTMONSTER] = RND->getRandomInt(1, 3);

				//�������� �ƴϰ� ���������� ��������
				if (_vKnightMonster[i]->getState() != KNIGHTMONSTER_STATE_ATTACK && _vKnightMonster[i]->getState() != KNIGHTMONSTER_STATE_IDLE)
				{
					switch (_vKnightMonster[i]->getDirection())
					{
					case KNIGHTMONSTER_DIRECTION_LEFT:
						_vKnightMonster[i]->setTum(_vKnightMonster[i]->getRect().left - _dungeonMap->getTile(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y)->rc.right);
						_vKnightMonster[i]->setX(_vKnightMonster[i]->getX() - _vKnightMonster[i]->getTum());

						if (_rndMove[KNIGHTMONSTER] == 1)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_RIGHT);
						}
						else if (_rndMove[KNIGHTMONSTER] == 2)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_DOWN);
						}
						else if (_rndMove[KNIGHTMONSTER] == 3)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_UP);
						}
						break;
					case KNIGHTMONSTER_DIRECTION_RIGHT:
						_vKnightMonster[i]->setTum(_vKnightMonster[i]->getRect().right - _dungeonMap->getTile(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y)->rc.left);
						_vKnightMonster[i]->setX(_vKnightMonster[i]->getX() - _vKnightMonster[i]->getTum());

						if (_rndMove[KNIGHTMONSTER] == 1)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_LEFT);
						}
						else if (_rndMove[KNIGHTMONSTER] == 2)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_DOWN);
						}
						else if (_rndMove[KNIGHTMONSTER] == 3)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_UP);
						}
						break;
					case KNIGHTMONSTER_DIRECTION_UP:
						_vKnightMonster[i]->setTum(_vKnightMonster[i]->getRect().top - _dungeonMap->getTile(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y)->rc.top);
						_vKnightMonster[i]->setY(_vKnightMonster[i]->getY() - _vKnightMonster[i]->getTum());

						if (_rndMove[KNIGHTMONSTER] == 1)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_RIGHT);
						}
						else if (_rndMove[KNIGHTMONSTER] == 2)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_DOWN);
						}
						else if (_rndMove[KNIGHTMONSTER] == 3)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_LEFT);
						}
						break;
					case KNIGHTMONSTER_DIRECTION_DOWN:
						_vKnightMonster[i]->setTum(_vKnightMonster[i]->getRect().bottom - _dungeonMap->getTile(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y)->rc.bottom);
						_vKnightMonster[i]->setY(_vKnightMonster[i]->getY() - _vKnightMonster[i]->getTum());

						if (_rndMove[KNIGHTMONSTER] == 1)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_RIGHT);
						}
						else if (_rndMove[KNIGHTMONSTER] == 2)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_LEFT);
						}
						else if (_rndMove[KNIGHTMONSTER] == 3)
						{
							_vKnightMonster[i]->setDirection(KNIGHTMONSTER_DIRECTION_UP);
						}
						break;
					}
				}
			}
			break;
		}
		_vKnightMonster[i]->setTileCollisionRect(RectMakeCenter(_vKnightMonster[i]->getX(), _vKnightMonster[i]->getY() + 50, _vKnightMonster[i]->getImage()->getFrameWidth() - 240, _vKnightMonster[i]->getImage()->getFrameHeight() - 180)); //Ÿ�ϱ���

	}
}

//���� ����
void enemyManager::bossAppear()
{
	if (_player->getPlayerY() <= 900 && _isOnce[BOSS_APPEAR] == false)
	{
		_isBossAppear = true;
		SOUNDMANAGER->play("bossSceneBGM");
		_isOnce[BOSS_APPEAR] = true;
	}

	if (_isBossAppear == true && _vBoss[0]->getState() == BOSS_STATE_IDLE) //��������
	{
		_appearGoal.x = GAMESIZEX / 2 + 70;
		_appearGoal.y = 650;

		_elapsedTime = TIMEMANAGER->getElapsedTime();

		//������� ������ �����ְ�
		_goalAngle = getAngle(_vBoss[0]->getX(), _vBoss[0]->getY(), _appearGoal.x, _appearGoal.y);
		//������� �Ÿ� ������.
		_goalDistance = getDistance(_vBoss[0]->getX(), _vBoss[0]->getY(), _appearGoal.x, _appearGoal.y);

		if (_isOnce[LINEAR_APPEAR] == false)
		{
			//�żӽ� ��������
			_appearSpeed = _goalDistance * (_elapsedTime / 6.0f);
			_worldTime = TIMEMANAGER->getWorldTime();
		}
		_isOnce[LINEAR_APPEAR] = true; //���������� �ѹ��� �ޱ�����.

		//8�������� ������.
		if (8.0f + _worldTime >= TIMEMANAGER->getWorldTime())
		{
			_vBoss[0]->setX(_vBoss[0]->getX() + cosf(_goalAngle) * _appearSpeed);
			_vBoss[0]->setY(_vBoss[0]->getY() + -sinf(_goalAngle) * _appearSpeed);
		}

		if (_goalDistance <= 10 && _isOnce[CAMERA_LINEAR] == false)
		{
			_camera->linearKeepMove(_appearGoal.x, _appearGoal.y + 50, 2, 100000);
			_isOnce[CAMERA_LINEAR] = true; //ī�޶� �ѹ��� �ޱ�����.
			_isBossAppear = false; //������������������ ���� �����ϴ°� ����.
			_vBoss[0]->setState(BOSS_STATE_MOVE);
		}
	}
}

//���� ���� ����
void enemyManager::bossMovePattern()
{
	//������ �� �����ϰ� ������ ���¶��
	if (_isBossAppear == false && _vBoss[0]->getState() == BOSS_STATE_MOVE)
	{
		if (_isOnce[WORLDTIME] == false)
		{
			_moveWorldTime = TIMEMANAGER->getWorldTime();
			_attackWorldTime[ATTACK_NONE] = TIMEMANAGER->getWorldTime();
			_isOnce[WORLDTIME] = true;
		}

		switch (_vBoss[0]->getState())
		{
		case BOSS_STATE_MOVE:

			//4���Ŀ� ��������
			if (4.0f + _moveWorldTime <= TIMEMANAGER->getWorldTime())
			{
				if (2.0f + _rndMoveWorldTime <= TIMEMANAGER->getWorldTime())
				{
					_rndMove[BOSS] = RND->getRandomInt(0, 3);

					_rndMoveWorldTime = TIMEMANAGER->getWorldTime();
				}

				if (_vBoss[0]->getTargetDistance() > 2)
				{
					_vBoss[0]->setX(_vBoss[0]->getX() + cosf(_vBoss[0]->getMoveAngle() * _moveSpeed));
					_vBoss[0]->setY(_vBoss[0]->getY() + -sinf(_vBoss[0]->getMoveAngle() * _moveSpeed));
				}
				else
				{
					_vBoss[0]->setX(_goalX);
					_vBoss[0]->setY(_goalY);
				}

				_goalX = _moveGoal[_rndMove[BOSS]].x;
				_goalY = _moveGoal[_rndMove[BOSS]].y;

				//������ �Ÿ��� ����
				_vBoss[0]->setMoveAngle(getAngle(_moveGoal[_rndMove[BOSS]].x, _moveGoal[_rndMove[BOSS]].y, _vBoss[0]->getX(), _vBoss[0]->getY()));
				_vBoss[0]->setTargetDistance(getDistance(_moveGoal[_rndMove[BOSS]].x, _moveGoal[_rndMove[BOSS]].y, _vBoss[0]->getX(), _vBoss[0]->getY()));

			}
			break;

		}
	}
}

//���� ���� ����
void enemyManager::bossAttackPattern()
{
	if (_isBossAppear == false && _vBoss[0]->getState() == BOSS_STATE_MOVE && 4.0f + _attackWorldTime[ATTACK_NONE] <= TIMEMANAGER->getWorldTime())
	{
		if (_bulletPattern != BULLET_PATTERN::RGB_BULLET)
		{
			//�Ѿ� �������� ���ϱ�
			for (int i = 0; i < _goldBullet->getVBossBullet().size(); i++)
			{
				if (_attackPattern == ATTACK_ONE)
				{
					_bulletPattern = BULLET_PATTERN::GOLD_BULLET;
					(*_goldBullet->setVBossBullet())[i].ani = KEYANIMANAGER->findAnimation("goldBullet", "gold");
				}
			}
			for (int i = 0; i < _blueBullet->getVBossBullet().size(); i++)
			{
				if (_attackPattern == ATTACK_TWO)
				{
					_bulletPattern = BULLET_PATTERN::BLUE_BULLET;
					(*_blueBullet->setVBossBullet())[i].ani = KEYANIMANAGER->findAnimation("blueBullet", "blue");
				}
			}
			for (int i = 0; i < _redBullet->getVBossBullet().size(); i++)
			{
				if (_attackPattern == ATTACK_THREE)
				{
					_bulletPattern = BULLET_PATTERN::RED_BULLET;
					(*_redBullet->setVBossBullet())[i].ani = KEYANIMANAGER->findAnimation("redBullet", "red");
				}
			}
		}
		else
		{
			for (int i = 0; i < _rgbBullet->getVBossBullet().size(); i++)
			{
				if (_attackPattern == ATTACK_ONE)
				{
					(*_rgbBullet->setVBossBullet())[i].ani = KEYANIMANAGER->findAnimation("goldBullet", "gold");
				}

				else if (_attackPattern == ATTACK_TWO)
				{
					(*_rgbBullet->setVBossBullet())[i].ani = KEYANIMANAGER->findAnimation("blueBullet", "blue");
				}

				else if (_attackPattern == ATTACK_THREE)
				{
					(*_rgbBullet->setVBossBullet())[i].ani = KEYANIMANAGER->findAnimation("redBullet", "red");
				}
			}
		}

		POINTFLOAT boss;
		boss.x = _vBoss[0]->getX();
		boss.y = _vBoss[0]->getY();

		//ü�� 100�����϶��� ���ϴٸ���
		if (_vBoss[0]->getCurrentHP() <= 100)
		{
			_bulletPattern = BULLET_PATTERN::RGB_BULLET;

			for (int i = 0; i < 4; i++)
			{
				_rndAttack[i] = RND->getRandomInt(1, 3);
				if (i <= 2) _rndAttackType[i] = RND->getRandomInt(1, 2);
			}

			if (_rndAttack[ATTACK_ONE] == 1) _attackPattern = ATTACK_TWO;
			else if (_rndAttack[ATTACK_ONE] == 2) _attackPattern = ATTACK_THREE;

			if (_rndAttack[ATTACK_TWO] == 1) _attackPattern = ATTACK_ONE;
			else if (_rndAttack[ATTACK_TWO] == 2) _attackPattern = ATTACK_THREE;

			if (_rndAttack[ATTACK_THREE] == 1) _attackPattern = ATTACK_ONE;
			else if (_rndAttack[ATTACK_THREE] == 2) _attackPattern = ATTACK_TWO;

			switch (_attackPattern)
			{
			case ATTACK_ONE:

				//�ð����� ����
				if (3.0f + _attackWorldTime[ATTACK_ONE] <= TIMEMANAGER->getWorldTime())
				{
					if (_rndAttackType[0] == 1)
					{
						_rgbBullet->bossFire(boss.x - 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 1);
						_rgbBullet->bossFire(boss.x + 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 1);
					}
					else if (_rndAttackType[0] == 2)
					{
						_rgbBullet->bossFire(boss.x - 100, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 3);
						_rgbBullet->bossFire(boss.x - 200, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 3);
						_rgbBullet->bossFire(boss.x + 100, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 3);
						_rgbBullet->bossFire(boss.x + 200, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 3);

					}

					_attackWorldTime[ATTACK_ONE] = TIMEMANAGER->getWorldTime();
				}
				break;
			case ATTACK_TWO:

				//�ð����� ����
				if (3.0f + _attackWorldTime[ATTACK_TWO] <= TIMEMANAGER->getWorldTime())
				{
					_rgbBullet->bossFire(boss.x - 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_TWO], 1);
					_rgbBullet->bossFire(boss.x + 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_TWO], 1);

					_attackWorldTime[ATTACK_TWO] = TIMEMANAGER->getWorldTime();
				}

				break;
			case ATTACK_THREE:

				//�ð����� ����
				if (3.0f + _attackWorldTime[ATTACK_THREE] <= TIMEMANAGER->getWorldTime())
				{
					if (_rndAttackType[1] == 1)
					{
						_rgbBullet->bossFire(boss.x - 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 1);
						_rgbBullet->bossFire(boss.x + 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 1);
					}
					else if (_rndAttackType[1] == 2)
					{
						_rgbBullet->bossFire(boss.x - 100, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 3);
						_rgbBullet->bossFire(boss.x - 200, boss.y - 100, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 3);
						_rgbBullet->bossFire(boss.x + 100, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 3);
						_rgbBullet->bossFire(boss.x + 200, boss.y - 100, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 3);
					}

					_attackWorldTime[ATTACK_THREE] = TIMEMANAGER->getWorldTime();
				}
				break;

			}

			if (0.1f + _attackWorldTime[ATTACK_SHOCK] >= TIMEMANAGER->getWorldTime())
			{
				if (_rndAttackType[2] == 1)
				{
					_shockAttack->ani = KEYANIMANAGER->findAnimation("shock", "leftShock");
				}
				else if (_rndAttackType[2] == 2)
				{
					_shockAttack->ani = KEYANIMANAGER->findAnimation("shock", "rightShock");
				}

				_shockAttack->x = _vBoss[0]->getX() - 70;
				_shockAttack->y = _vBoss[0]->getY() / 2 + 200;
				_shockAttack->rc = RectMakeCenter(_shockAttack->x, _shockAttack->y + 300, 50, 200);
				_shockAttack->rc2 = RectMakeCenter(_shockAttack->x + 140, _shockAttack->y + 300, 50, 200);

				_shockAttack->ani->start();

			}
			else
			{
				_shockAttack->rc = RectMakeCenter(NULL, NULL, NULL, NULL);
				_shockAttack->rc2 = RectMakeCenter(NULL, NULL, NULL, NULL);
				_attackWorldTime[ATTACK_SHOCK] = TIMEMANAGER->getWorldTime();
			}

		}

		//ü���� 100�̻��϶���
		else if (_vBoss[0]->getCurrentHP() > 100)
		{
			switch (_attackPattern)
			{
			case ATTACK_ONE:

				//�ð����� ����
				if (3.0f + _attackWorldTime[ATTACK_ONE] <= TIMEMANAGER->getWorldTime() && _bulletPattern == BULLET_PATTERN::GOLD_BULLET)
				{
					_rndAttack[ATTACK_ONE] = RND->getRandomInt(1, 3);
					_rndAttackType[0] = RND->getRandomInt(1, 2);

					if (_rndAttackType[0] == 1)
					{
						_goldBullet->bossFire(boss.x - 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 1);
						_goldBullet->bossFire(boss.x + 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 1);
					}
					else if (_rndAttackType[0] == 2)
					{
						_goldBullet->bossFire(boss.x - 100, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 3);
						_goldBullet->bossFire(boss.x - 200, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 3);
						_goldBullet->bossFire(boss.x + 100, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 3);
						_goldBullet->bossFire(boss.x + 200, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_ONE], 3);

					}

					//��������
					if (_rndAttack[ATTACK_ONE] == 1)
					{
						_bulletPattern = BULLET_PATTERN::BLUE_BULLET;
						_attackPattern = ATTACK_TWO;
					}
					else if (_rndAttack[ATTACK_ONE] == 2)
					{
						_bulletPattern = BULLET_PATTERN::RED_BULLET;
						_attackPattern = ATTACK_THREE;
					}
					else if (_rndAttack[ATTACK_ONE] == 3)
					{
						_attackPattern = ATTACK_SHOCK;
					}

					_attackWorldTime[ATTACK_ONE] = TIMEMANAGER->getWorldTime();
				}
				break;
			case ATTACK_TWO:

				//�ð����� ����
				if (3.0f + _attackWorldTime[ATTACK_TWO] <= TIMEMANAGER->getWorldTime() && _bulletPattern == BULLET_PATTERN::BLUE_BULLET)
				{
					_rndAttack[ATTACK_TWO] = RND->getRandomInt(1, 3);

					_blueBullet->bossFire(boss.x - 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_TWO], 1);
					_blueBullet->bossFire(boss.x + 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_TWO], 1);

					//��������
					if (_rndAttack[ATTACK_TWO] == 1)
					{
						_bulletPattern = BULLET_PATTERN::GOLD_BULLET;
						_attackPattern = ATTACK_ONE;
					}
					else if (_rndAttack[ATTACK_TWO] == 2)
					{
						_bulletPattern = BULLET_PATTERN::RED_BULLET;
						_attackPattern = ATTACK_THREE;
					}
					else if (_rndAttack[ATTACK_ONE] == 3)
					{
						_attackPattern = ATTACK_SHOCK;
					}

					_attackWorldTime[ATTACK_TWO] = TIMEMANAGER->getWorldTime();

				}

				break;
			case ATTACK_THREE:

				//�ð����� ����
				if (3.0f + _attackWorldTime[ATTACK_THREE] <= TIMEMANAGER->getWorldTime() && _bulletPattern == BULLET_PATTERN::RED_BULLET)
				{
					_rndAttack[ATTACK_THREE] = RND->getRandomInt(1, 3);
					_rndAttackType[1] = RND->getRandomInt(1, 2);

					if (_rndAttackType[1] == 1)
					{
						_redBullet->bossFire(boss.x - 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 1);
						_redBullet->bossFire(boss.x + 150, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 1);
					}
					else if (_rndAttackType[1] == 2)
					{
						_redBullet->bossFire(boss.x - 100, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 3);
						_redBullet->bossFire(boss.x - 200, boss.y - 100, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 3);
						_redBullet->bossFire(boss.x + 100, boss.y, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 3);
						_redBullet->bossFire(boss.x + 200, boss.y - 100, (PI / 180) * 270, _bulletSpeed[ATTACK_THREE], 3);
					}

					//��������
					if (_rndAttack[ATTACK_THREE] == 1)
					{
						_bulletPattern = BULLET_PATTERN::GOLD_BULLET;
						_attackPattern = ATTACK_ONE;
					}
					else if (_rndAttack[ATTACK_THREE] == 2)
					{
						_bulletPattern = BULLET_PATTERN::BLUE_BULLET;
						_attackPattern = ATTACK_TWO;
					}
					else if (_rndAttack[ATTACK_ONE] == 3)
					{
						_attackPattern = ATTACK_SHOCK;
					}

					_attackWorldTime[ATTACK_THREE] = TIMEMANAGER->getWorldTime();
				}
				break;
			case ATTACK_SHOCK:

				if (_vBoss[0]->countAttack() == true)
				{
					_rndAttack[ATTACK_SHOCK] = RND->getRandomInt(1, 3);
					_rndAttackType[2] = RND->getRandomInt(1, 2);

					if (_rndAttackType[2] == 1)
					{
						_shockAttack->ani = KEYANIMANAGER->findAnimation("shock", "leftShock");
					}
					else if (_rndAttackType[2] == 2)
					{
						_shockAttack->ani = KEYANIMANAGER->findAnimation("shock", "rightShock");
					}

					//��������
					if (_rndAttack[ATTACK_SHOCK] == 1)
					{
						_bulletPattern = BULLET_PATTERN::GOLD_BULLET;
						_attackPattern = ATTACK_ONE;
					}
					else if (_rndAttack[ATTACK_SHOCK] == 2)
					{
						_bulletPattern = BULLET_PATTERN::BLUE_BULLET;
						_attackPattern = ATTACK_TWO;
					}
					else if (_rndAttack[ATTACK_SHOCK] == 3)
					{
						_bulletPattern = BULLET_PATTERN::RED_BULLET;
						_attackPattern = ATTACK_THREE;
					}
					_shockAttack->x = _vBoss[0]->getX();
					_shockAttack->y = _vBoss[0]->getY() / 2 + 200;
					_shockAttack->rc = RectMakeCenter(_shockAttack->x, _shockAttack->y + 100, 50, 100);
					_shockAttack->ani->start();

				}
				else
				{
					_shockAttack->rc = RectMakeCenter(NULL, NULL, NULL, NULL);
				}
				break;

			}
		}

		//�ι�°�����̰� �Ķ����Ѿ��϶��� ������ �����Ѿ��϶���
		if ((_attackPattern == ATTACK_TWO && _bulletPattern == BULLET_PATTERN::BLUE_BULLET) || (_bulletPattern == BULLET_PATTERN::RGB_BULLET))
		{
			//����Ѿ��϶���
			if (_bulletPattern == BULLET_PATTERN::BLUE_BULLET)
			{
				for (int i = 0; i < _blueBullet->getVBossBullet().size(); i++)
				{
					//�ι�°�����϶��� ��������.
					if (_blueBullet->getVBossBullet()[i].y >= _player->getPlayerY())
					{
						(*_blueBullet->setVBossBullet())[0].angle = PI2;
						(*_blueBullet->setVBossBullet())[1].angle = PI;
					}
				}
			}
			//������ �� �����Ѿ��϶���
			else if (_bulletPattern == BULLET_PATTERN::RGB_BULLET)
			{
				for (int i = 0; i < _rgbBullet->getVBossBullet().size(); i++)
				{
					if (_rgbBullet->getVBossBullet()[i].y >= _player->getPlayerY())
					{
						(*_rgbBullet->setVBossBullet())[0].angle = PI2;
						(*_rgbBullet->setVBossBullet())[1].angle = PI;
					}
				}
			}
		}
		//����° �����̰� �����Ѿ��̰ų� ������ �����Ѿ��϶���
		if ((_attackPattern == ATTACK_THREE && _bulletPattern == BULLET_PATTERN::RED_BULLET) || (_bulletPattern == BULLET_PATTERN::RGB_BULLET))
		{
			//�����Ѿ�
			if (_bulletPattern == BULLET_PATTERN::RED_BULLET)
			{
				for (int i = 0; i < _redBullet->getVBossBullet().size(); i++)
				{
					if (_redBullet->getVBossBullet()[i].y >= _player->getPlayerY())
					{
						if (0.7f + _redBullet->getVBossBullet()[i].bulletWorlTime >= TIMEMANAGER->getWorldTime())
						{
							(*_redBullet->setVBossBullet())[i].speed = 0;
							(*_redBullet->setVBossBullet())[i].ani->setFPS(30);
						}
						else if (0.8f + _redBullet->getVBossBullet()[i].bulletWorlTime <= TIMEMANAGER->getWorldTime() && 1.8f + _redBullet->getVBossBullet()[i].bulletWorlTime > TIMEMANAGER->getWorldTime())
						{
							(*_redBullet->setVBossBullet())[i].speed = _redBullet->getVBossBullet()[i].speed + 0.3f;
							(*_redBullet->setVBossBullet())[0].angle = PI2;
							(*_redBullet->setVBossBullet())[1].angle = PI;
						}
						else if (1.8f + _redBullet->getVBossBullet()[i].bulletWorlTime <= TIMEMANAGER->getWorldTime())
						{
							(*_redBullet->setVBossBullet())[i].speed = 7;
							(*_redBullet->setVBossBullet())[i].ani->setFPS(7);
							(*_redBullet->setVBossBullet())[i].bulletWorlTime = TIMEMANAGER->getWorldTime();
						}
					}
				}
			}
			//�������� �����Ѿ�
			else if (_bulletPattern == BULLET_PATTERN::RGB_BULLET)
			{
				for (int i = 0; i < _rgbBullet->getVBossBullet().size(); i++)
				{
					if (_rgbBullet->getVBossBullet()[i].y >= _player->getPlayerY())
					{
						if (0.7f + _rgbBullet->getVBossBullet()[i].bulletWorlTime >= TIMEMANAGER->getWorldTime())
						{
							(*_rgbBullet->setVBossBullet())[i].speed = 0;
							(*_rgbBullet->setVBossBullet())[i].ani->setFPS(30);
						}
						else if (0.8f + _rgbBullet->getVBossBullet()[i].bulletWorlTime <= TIMEMANAGER->getWorldTime() && 1.8f + _rgbBullet->getVBossBullet()[i].bulletWorlTime > TIMEMANAGER->getWorldTime())
						{
							(*_rgbBullet->setVBossBullet())[i].speed = _rgbBullet->getVBossBullet()[i].speed + 0.5f;
							(*_rgbBullet->setVBossBullet())[0].angle = PI2;
							(*_rgbBullet->setVBossBullet())[1].angle = PI;
						}
						else if (1.8f + _rgbBullet->getVBossBullet()[i].bulletWorlTime <= TIMEMANAGER->getWorldTime())
						{
							(*_rgbBullet->setVBossBullet())[i].speed = 7;
							(*_rgbBullet->setVBossBullet())[i].ani->setFPS(7);
							(*_rgbBullet->setVBossBullet())[i].bulletWorlTime = TIMEMANAGER->getWorldTime();
						}
					}
				}
			}
		}
	}

}

void enemyManager::bossBulletDraw(float cameraX, float cameraY)
{
	if (_vBoss[0]->getCurrentHP() > 0)
	{
		if (_bulletPattern == BULLET_PATTERN::GOLD_BULLET || _bulletPattern == BULLET_PATTERN::BLUE_BULLET || _bulletPattern == BULLET_PATTERN::RED_BULLET)
		{
			_goldBullet->render(cameraX, cameraY);
			_blueBullet->render(cameraX, cameraY);
			_redBullet->render(cameraX, cameraY);
		}
		else if (_bulletPattern == BULLET_PATTERN::RGB_BULLET)
		{
			_rgbBullet->render(cameraX, cameraY);
		}

		if (_attackPattern == ATTACK_SHOCK)
		{
			_shockAttack->image->expandAniRenderCenter(getMemDC(), _shockAttack->x, _shockAttack->y, _shockAttack->ani, 3.f, 3.f);
		}

		if (_bulletPattern == BULLET_PATTERN::RGB_BULLET)
		{
			_shockAttack->image->expandAniRenderCenter(getMemDC(), _shockAttack->x, _shockAttack->y, _shockAttack->ani, 3.f, 3.f);
		}
	}
}

//���ʹ̵��� �������� �������� �Լ�
void enemyManager::enemyRespon()
{
}
