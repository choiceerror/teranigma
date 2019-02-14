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
	_fireMonsterBullet->init("fireMonster", WINSIZEX / 2, 10);
	_fireBulletSpeed = 5;
	_moveWorldTime = TIMEMANAGER->getWorldTime();

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

	//������ ����, �뽬����
	if ((_player->getPlayerAni()->getFramePosArr().x >= 1 && _player->getPlayerAni()->getFramePosArr().x < 4 && _player->getPlayerAni()->getFramePosArr().y == 13)
		|| (_player->getPlayerAni()->getFramePosArr().x <= 2 && _player->getPlayerAni()->getFramePosArr().y == 28))
	{
		_attackRect = RectMakeCenter(_player->getPlayerX() + 30, _player->getPlayerY(), 100, 30);
	}
	//���� ����, �뽬����
	else if ((_player->getPlayerAni()->getFramePosArr().x >= 5 && _player->getPlayerAni()->getFramePosArr().y == 13)
		|| (_player->getPlayerAni()->getFramePosArr().x >= 4 && _player->getPlayerAni()->getFramePosArr().y == 28))
	{
		_attackRect = RectMakeCenter(_player->getPlayerX() - 30, _player->getPlayerY(), 100, 30);
	}
	//������, �뽬����
	else if ((_player->getPlayerAni()->getFramePosArr().x >= 1 && _player->getPlayerAni()->getFramePosArr().x < 4 && _player->getPlayerAni()->getFramePosArr().y == 14)
		|| (_player->getPlayerAni()->getFramePosArr().x <= 2 && _player->getPlayerAni()->getFramePosArr().y == 29))
	{
		_attackRect = RectMakeCenter(_player->getPlayerX(), _player->getPlayerY() - 30, 30, 100);
	}
	//�Ʒ� ����, �뽬����
	else if ((_player->getPlayerAni()->getFramePosArr().x >= 5 && _player->getPlayerAni()->getFramePosArr().y == 14)
		|| (_player->getPlayerAni()->getFramePosArr().x >= 4 && _player->getPlayerAni()->getFramePosArr().y == 29))
	{
		_attackRect = RectMakeCenter(_player->getPlayerX(), _player->getPlayerY() + 30, 30, 100);
	}
	else _attackRect = RectMakeCenter(0, 0, 0, 0);

	//���ʹ̵��� Ÿ�ϰ���
	updateCollection();

	//������Ʈ ���� �Լ�
	tileCheckObjectCollision();

	//���ʹ� AI �Լ�
	enemyAI();

	//�÷��̾� ���ݿ� ���ʹ̵��� ���� �Լ�
	playerAttackEnemyCollision();

	//���ʹ̵��� �����Լ�
	enemyDead();

}

void enemyManager::render(float cameraX, float cameraY)
{
	//Rectangle(getMemDC(), _attackRect);
	//������ ���� �Լ�
	enemyDraw(cameraX, cameraY);

	char str[128];

	for (int i = 0; i < _vKnightMonster.size(); i++)
	{

		//sprintf_s(str, "left : %d", _vKnightMonster[i]->getTileCollisionRect().left);
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

	for (int i = 0; i < _vBallMonster.size(); i++)
	{
		//sprintf_s(str, "targetDistance : %f", _vBallMonster[i]->getTargetDistance());
		//TextOut(getMemDC(), 600, 100 + i * 20, str, strlen(str));

		//sprintf_s(str, "ballviewX : %f", _vBallMonster[i]->getViewX());
		//TextOut(getMemDC(), 600, 200 + i * 20, str, strlen(str));

		//sprintf_s(str, "ballviewY : %f", _vBallMonster[i]->getViewY());
		//TextOut(getMemDC(), 600, 300 + i * 20, str, strlen(str));

		//sprintf_s(str, "ballx : %f", _vBallMonster[i]->getX());
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

//������ ����
void enemyManager::enemyDraw(float cameraX, float cameraY)
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

//���ʹ� ����
void enemyManager::setEnemy()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			ballMonster* bm;
			bm = new ballMonster;

			bm->init("ball", "ballMonster", 500 + j * 100, 100 + i * 100, j, i, 100);
			_vBallMonster.push_back(bm);

			fireMonster* fm;
			fm = new fireMonster;

			fm->init("fire", "fireMonster", 100 + j * 100, 500 + i * 100, j, i, 200);
			_vFireMonster.push_back(fm);

			knightMonster* km;
			km = new knightMonster;

			km->init("knight", "knightMonster", 300 + j * 100, 300 + i * 100, j, i, 300);

			_vKnightMonster.push_back(km);

		}
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
					fireMonsterBulletFire(i); //���Ѿ� �߻�
				}
				break;
			}
		}
		else if (_vFireMonster[i]->getTargetDistance() > 500)
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
			_vKnightMonster[i]->setState(KNIGHTMONSTER_STATE_ATTACK);
			_vKnightMonster[i]->setAttackWorldTime(TIMEMANAGER->getWorldTime());
		}
		//�־����� �������� ������
		else if (_vKnightMonster[i]->getTargetDistance() > 100 && 1.0f + _vKnightMonster[i]->getAttackWorldTime() <= TIMEMANAGER->getWorldTime() && _vKnightMonster[i]->getMoveType() == FOLLOW_MOVE_TYPE)
		{
			if (_vKnightMonster[i]->getRndState() == 0) _vKnightMonster[i]->setState(KNIGHTMONSTER_STATE_MOVE);
			else _vKnightMonster[i]->setState(KNIGHTMONSTER_STATE_IDLE);
		}
	}
}

//�÷��̾� ���ݿ� ���ʹ̵��� ���� �Լ�
void enemyManager::playerAttackEnemyCollision()
{
	switch (_player->getPlayerState())
	{
	case PLAYER_ATTACK:
	case PLAYER_JUMP_ATTACK:
	case PLAYER_DASH_JUMP_ATTACK:
	case PLAYER_DASH_ATTACK:

		RECT temp;
		//������
		for (int i = 0; i < _vKnightMonster.size(); i++)
		{
			if (IntersectRect(&temp, &_attackRect, &_vKnightMonster[i]->getRect()))
			{
				_vKnightMonster[i]->setCurrentHP(_vKnightMonster[i]->getCurrentHP() - 10);
				_vKnightMonster[i]->setIsHit(true);
				if (_player->getPlayerDirection() == LEFT)
				{
					_vKnightMonster[i]->setX(_vKnightMonster[i]->getX() - 2);
				}
				else if (_player->getPlayerDirection() == RIGHT)
				{
					_vKnightMonster[i]->setX(_vKnightMonster[i]->getX() + 2);
				}
				else if (_player->getPlayerDirection() == UP)
				{
					_vKnightMonster[i]->setY(_vKnightMonster[i]->getY() - 2);
				}
				else if (_player->getPlayerDirection() == DOWN)
				{
					_vKnightMonster[i]->setY(_vKnightMonster[i]->getY() + 2);
				}
			}
		}

		//������
		for (int i = 0; i < _vBallMonster.size(); i++)
		{
			if (IntersectRect(&temp, &_attackRect, &_vBallMonster[i]->getRect()))
			{
				_vBallMonster[i]->setIsHit(true);
				_vBallMonster[i]->setCurrentHP(_vBallMonster[i]->getCurrentHP() - 10);
				if (_player->getPlayerDirection() == LEFT)
				{
					_vBallMonster[i]->setX(_vBallMonster[i]->getX() - 2);
				}
				else if (_player->getPlayerDirection() == RIGHT)
				{
					_vBallMonster[i]->setX(_vBallMonster[i]->getX() + 2);
				}
				else if (_player->getPlayerDirection() == UP)
				{
					_vBallMonster[i]->setY(_vBallMonster[i]->getY() - 2);
				}
				else if (_player->getPlayerDirection() == DOWN)
				{
					_vBallMonster[i]->setY(_vBallMonster[i]->getY() + 2);
				}
			}
		}

		//�Ҹ���
		for (int i = 0; i < _vFireMonster.size(); i++)
		{
			if (IntersectRect(&temp, &_attackRect, &_vFireMonster[i]->getRect()))
			{
				_vFireMonster[i]->setIsHit(true);
				_vFireMonster[i]->setCurrentHP(_vFireMonster[i]->getCurrentHP() - 10);
				if (_player->getPlayerDirection() == LEFT)
				{
					_vFireMonster[i]->setX(_vFireMonster[i]->getX() - 2);
				}
				else if (_player->getPlayerDirection() == RIGHT)
				{
					_vFireMonster[i]->setX(_vFireMonster[i]->getX() + 2);
				}
				else if (_player->getPlayerDirection() == UP)
				{
					_vFireMonster[i]->setY(_vFireMonster[i]->getY() - 2);
				}
				else if (_player->getPlayerDirection() == DOWN)
				{
					_vFireMonster[i]->setY(_vFireMonster[i]->getY() + 2);
				}
			}
		}
		break;
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
	for (int j = 0; j < _fireMonsterBullet->getVFireBullet().size(); j++)
	{
		RECT temp;
		if (IntersectRect(&temp, &_fireMonsterBullet->getVFireBullet()[j].rc, &_player->getPlayerRc()))
		{
			(*_fireMonsterBullet->setVFireBullet())[j].isCollision = true;
			_vFireMonster[i]->setAttackWorldTime(TIMEMANAGER->getWorldTime());
			_vFireMonster[i]->setIsAttack(true); //�Ұ��ݿ� �¾Ҵ�.
		}
		//0.8�ʵ� ���ݲ���
		if (0.8f + _vFireMonster[i]->getAttackWorldTime() <= TIMEMANAGER->getWorldTime())
		{
			_vFireMonster[i]->setIsAttack(false);
		}
	}
}

//���ʹ̵��� �����Լ�
void enemyManager::enemyDead()
{
	//������
	for (int i = 0; i < _vKnightMonster.size();)
	{
		if (_vKnightMonster[i]->getCurrentHP() <= 0.0f)
		{
			EFFECTMANAGER->play("deadEffect", _vKnightMonster[i]->getViewX(), _vKnightMonster[i]->getViewY());
			_vKnightMonster.erase(_vKnightMonster.begin() + i);
		}
		else i++;
	}

	//������
	for (int i = 0; i < _vBallMonster.size();)
	{
		if (_vBallMonster[i]->getCurrentHP() <= 0.0f)
		{
			EFFECTMANAGER->play("deadEffect", _vBallMonster[i]->getViewX(), _vBallMonster[i]->getViewY());
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
			_vFireMonster.erase(_vFireMonster.begin() + i);
		}
		else i++;
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

		//Ÿ�� �ε��� �����ֱ�
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
			break;
		case BALLMONSTER_DIRECTION_UP:
			_vBallMonster[i]->setTileIndex()[0].x = _vBallMonster[i]->getIdX();
			_vBallMonster[i]->setTileIndex()[0].y = _vBallMonster[i]->getIdY() - 1;
			_vBallMonster[i]->setTileIndex()[1].x = _vBallMonster[i]->getIdX() + 1;
			_vBallMonster[i]->setTileIndex()[1].y = _vBallMonster[i]->getIdY() - 1;
			break;
		case BALLMONSTER_DIRECTION_RIGHT:
			_vBallMonster[i]->setTileIndex()[0].x = _vBallMonster[i]->getIdX() + 1;
			_vBallMonster[i]->setTileIndex()[0].y = _vBallMonster[i]->getIdY();
			_vBallMonster[i]->setTileIndex()[1].x = _vBallMonster[i]->getIdX() + 1;
			_vBallMonster[i]->setTileIndex()[1].y = _vBallMonster[i]->getIdY() + 1;
			break;
		case BALLMONSTER_DIRECTION_DOWN:
			_vBallMonster[i]->setTileIndex()[0].x = _vBallMonster[i]->getIdX();
			_vBallMonster[i]->setTileIndex()[0].y = _vBallMonster[i]->getIdY() + 1;
			_vBallMonster[i]->setTileIndex()[1].x = _vBallMonster[i]->getIdX() + 1;
			_vBallMonster[i]->setTileIndex()[1].y = _vBallMonster[i]->getIdY() + 1;
			break;
		}

		//if (_vBallMonster[i]->getIsAttack() == true && _vBallMonster[i]->getAttackAngle() > 0 && _vBallMonster[i]->getAttackAngle() < PI / 2)
		//{
		//	_vBallMonster[i]->setTileIndex()[0].x = _vBallMonster[i]->getIdX();
		//	_vBallMonster[i]->setTileIndex()[0].y = _vBallMonster[i]->getIdY() - 1;
		//	_vBallMonster[i]->setTileIndex()[1].x = _vBallMonster[i]->getIdX() + 1;
		//	_vBallMonster[i]->setTileIndex()[1].y = _vBallMonster[i]->getIdY() - 1;
		//	_vBallMonster[i]->setTileIndex()[2].x = _vBallMonster[i]->getIdX() + 1;
		//	_vBallMonster[i]->setTileIndex()[2].y = _vBallMonster[i]->getIdY();
		//	_vBallMonster[i]->setTileIndex()[3].x = _vBallMonster[i]->getIdX() + 1;
		//	_vBallMonster[i]->setTileIndex()[3].y = _vBallMonster[i]->getIdY() + 1;
		//}
		//if (_vBallMonster[i]->getIsAttack() == true && _vBallMonster[i]->getAttackAngle() > PI / 2 && _vBallMonster[i]->getAttackAngle() < PI)
		//{
		//	_vBallMonster[i]->setTileIndex()[0].x = _vBallMonster[i]->getIdX() - 1;
		//	_vBallMonster[i]->setTileIndex()[0].y = _vBallMonster[i]->getIdY();
		//	_vBallMonster[i]->setTileIndex()[1].x = _vBallMonster[i]->getIdX() - 1;
		//	_vBallMonster[i]->setTileIndex()[1].y = _vBallMonster[i]->getIdY() + 1;
		//	_vBallMonster[i]->setTileIndex()[2].x = _vBallMonster[i]->getIdX();
		//	_vBallMonster[i]->setTileIndex()[2].y = _vBallMonster[i]->getIdY() - 1;
		//	_vBallMonster[i]->setTileIndex()[3].x = _vBallMonster[i]->getIdX() + 1;
		//	_vBallMonster[i]->setTileIndex()[3].y = _vBallMonster[i]->getIdY() - 1;
		//}
		//if (_vBallMonster[i]->getIsAttack() == true && _vBallMonster[i]->getAttackAngle() > PI && _vBallMonster[i]->getAttackAngle() < (PI / 180) * 270)
		//{
		//	_vBallMonster[i]->setTileIndex()[0].x = _vBallMonster[i]->getIdX() - 1;
		//	_vBallMonster[i]->setTileIndex()[0].y = _vBallMonster[i]->getIdY();
		//	_vBallMonster[i]->setTileIndex()[1].x = _vBallMonster[i]->getIdX() - 1;
		//	_vBallMonster[i]->setTileIndex()[1].y = _vBallMonster[i]->getIdY() + 1;
		//	_vBallMonster[i]->setTileIndex()[2].x = _vBallMonster[i]->getIdX();
		//	_vBallMonster[i]->setTileIndex()[2].y = _vBallMonster[i]->getIdY() + 1;
		//	_vBallMonster[i]->setTileIndex()[3].x = _vBallMonster[i]->getIdX() + 1;
		//	_vBallMonster[i]->setTileIndex()[3].y = _vBallMonster[i]->getIdY() + 1;
		//}
		//if (_vBallMonster[i]->getIsAttack() == true && _vBallMonster[i]->getAttackAngle() > (PI / 180) * 270 && _vBallMonster[i]->getAttackAngle() < PI2)
		//{
		//	_vBallMonster[i]->setTileIndex()[0].x = _vBallMonster[i]->getIdX() + 1;
		//	_vBallMonster[i]->setTileIndex()[0].y = _vBallMonster[i]->getIdY();
		//	_vBallMonster[i]->setTileIndex()[1].x = _vBallMonster[i]->getIdX() + 1;
		//	_vBallMonster[i]->setTileIndex()[1].y = _vBallMonster[i]->getIdY() + 1;
		//	_vBallMonster[i]->setTileIndex()[2].x = _vBallMonster[i]->getIdX();
		//	_vBallMonster[i]->setTileIndex()[2].y = _vBallMonster[i]->getIdY() + 1;
		//	_vBallMonster[i]->setTileIndex()[3].x = _vBallMonster[i]->getIdX() + 1;
		//	_vBallMonster[i]->setTileIndex()[3].y = _vBallMonster[i]->getIdY() + 1;
		//}
		RECT temp;
		for (int j = 0; j < 2; i++)
		{
			if (_dungeonMap->getAttr(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y) == ATTR_UNMOVE
				&& IntersectRect(&temp, &_dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc, &_vBallMonster[i]->getTileCollisionRect()))
			{
				if (_vBallMonster[i]->getState() != BALLMONSTER_STATE_IDLE && _vBallMonster[i]->getIsAttack() == false)
				{
					switch (_vBallMonster[i]->getDirection())
					{
					case BALLMONSTER_DIRECTION_LEFT:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().left - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.right);
						_vBallMonster[i]->setX(_vBallMonster[i]->getX() - _vBallMonster[i]->getTum());
						break;
					case BALLMONSTER_DIRECTION_RIGHT:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().right - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.left);
						_vBallMonster[i]->setX(_vBallMonster[i]->getX() - _vBallMonster[i]->getTum());
						break;
					case BALLMONSTER_DIRECTION_UP:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().top - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.bottom);
						_vBallMonster[i]->setY(_vBallMonster[i]->getY() - _vBallMonster[i]->getTum());
						break;
					case BALLMONSTER_DIRECTION_DOWN:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().bottom - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.top);
						_vBallMonster[i]->setY(_vBallMonster[i]->getY() - _vBallMonster[i]->getTum());
						break;
					}

				}
				else if (_vBallMonster[i]->getIsAttack() == true)
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
						break;
					case BALLMONSTER_DIRECTION_RIGHT:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().right - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.left);
						_vBallMonster[i]->setX(_vBallMonster[i]->getX() - _vBallMonster[i]->getTum());
						break;
					case BALLMONSTER_DIRECTION_UP:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().top - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.bottom);
						_vBallMonster[i]->setY(_vBallMonster[i]->getY() - _vBallMonster[i]->getTum());
						break;
					case BALLMONSTER_DIRECTION_DOWN:
						_vBallMonster[i]->setTum(_vBallMonster[i]->getRect().bottom - _dungeonMap->getTile(_vBallMonster[i]->getTileIndex(j).x, _vBallMonster[i]->getTileIndex(j).y)->rc.top);
						_vBallMonster[i]->setY(_vBallMonster[i]->getY() - _vBallMonster[i]->getTum());
						break;
					}
					_vBallMonster[i]->setState(BALLMONSTER_STATE_IDLE);
					_vBallMonster[i]->setMoveType(BASIC_MOVE_TYPE);
				}
			}
			break;
		}
		_vBallMonster[i]->setTileCollisionRect(RectMakeCenter(_vBallMonster[i]->getX(), _vBallMonster[i]->getY(), _vBallMonster[i]->getImage()->getFrameWidth(), _vBallMonster[i]->getImage()->getFrameHeight()));
		//_vBallMonster[i]->setRect(_vBallMonster[i]->getTileCollisionRect());
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
		for (int j = 0; j < 2; i++)
		{
			if (_dungeonMap->getAttr(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y) == ATTR_UNMOVE
				&& IntersectRect(&temp, &_dungeonMap->getTile(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y)->rc, &_vFireMonster[i]->getTileCollisionRect()))
			{
				switch (_vFireMonster[i]->getDirection())
				{
				case FIREMONSTER_DIRECTION_LEFT:
					_vFireMonster[i]->setTum(_vFireMonster[i]->getRect().left - _dungeonMap->getTile(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y)->rc.right);
					_vFireMonster[i]->setX(_vFireMonster[i]->getX() - _vFireMonster[i]->getTum());
					break;
				case FIREMONSTER_DIRECTION_RIGHT:
					_vFireMonster[i]->setTum(_vFireMonster[i]->getRect().right - _dungeonMap->getTile(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y)->rc.left);
					_vFireMonster[i]->setX(_vFireMonster[i]->getX() - _vFireMonster[i]->getTum());
					break;
				case FIREMONSTER_DIRECTION_UP:
					_vFireMonster[i]->setTum(_vFireMonster[i]->getRect().top - _dungeonMap->getTile(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y)->rc.bottom);
					_vFireMonster[i]->setY(_vFireMonster[i]->getY() - _vFireMonster[i]->getTum());
					break;
				case FIREMONSTER_DIRECTION_DOWN:
					_vFireMonster[i]->setTum(_vFireMonster[i]->getRect().bottom - _dungeonMap->getTile(_vFireMonster[i]->getTileIndex(j).x, _vFireMonster[i]->getTileIndex(j).y)->rc.top);
					_vFireMonster[i]->setY(_vFireMonster[i]->getY() - _vFireMonster[i]->getTum());
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
			//_vKnightMonster[i]->setTileIndex()[2].x = _vKnightMonster[i]->getIdX() - 1;
			//_vKnightMonster[i]->setTileIndex()[2].y = _vKnightMonster[i]->getIdY() - 1;
			break;
		case KNIGHTMONSTER_DIRECTION_UP:
			_vKnightMonster[i]->setTileIndex()[0].x = _vKnightMonster[i]->getIdX();
			_vKnightMonster[i]->setTileIndex()[0].y = _vKnightMonster[i]->getIdY() - 1;
			_vKnightMonster[i]->setTileIndex()[1].x = _vKnightMonster[i]->getIdX() + 1;
			_vKnightMonster[i]->setTileIndex()[1].y = _vKnightMonster[i]->getIdY() - 1;
			//_vKnightMonster[i]->setTileIndex()[2].x = _vKnightMonster[i]->getIdX() - 1;
			//_vKnightMonster[i]->setTileIndex()[2].y = _vKnightMonster[i]->getIdY() - 1;
			break;
		case KNIGHTMONSTER_DIRECTION_RIGHT:
			_vKnightMonster[i]->setTileIndex()[0].x = _vKnightMonster[i]->getIdX() + 1;
			_vKnightMonster[i]->setTileIndex()[0].y = _vKnightMonster[i]->getIdY();
			_vKnightMonster[i]->setTileIndex()[1].x = _vKnightMonster[i]->getIdX() + 1;
			_vKnightMonster[i]->setTileIndex()[1].y = _vKnightMonster[i]->getIdY() + 1;
			//_vKnightMonster[i]->setTileIndex()[2].x = _vKnightMonster[i]->getIdX() + 1;
			//_vKnightMonster[i]->setTileIndex()[2].y = _vKnightMonster[i]->getIdY() - 1;
			break;
		case KNIGHTMONSTER_DIRECTION_DOWN:
			_vKnightMonster[i]->setTileIndex()[0].x = _vKnightMonster[i]->getIdX();
			_vKnightMonster[i]->setTileIndex()[0].y = _vKnightMonster[i]->getIdY() + 1;
			_vKnightMonster[i]->setTileIndex()[1].x = _vKnightMonster[i]->getIdX() + 1;
			_vKnightMonster[i]->setTileIndex()[1].y = _vKnightMonster[i]->getIdY() + 1;
			//_vKnightMonster[i]->setTileIndex()[2].x = _vKnightMonster[i]->getIdX() - 1;
			//_vKnightMonster[i]->setTileIndex()[2].y = _vKnightMonster[i]->getIdY() + 2;
			break;
		}
		RECT temp;
		for (int j = 0; j < 2; j++)
		{
			if (_dungeonMap->getAttr(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y) == ATTR_UNMOVE
				&& IntersectRect(&temp, &_dungeonMap->getTile(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y)->rc, &_vKnightMonster[i]->getTileCollisionRect()))
			{
				//�������� �ƴϰ� ���������� ��������
				if (_vKnightMonster[i]->getState() != KNIGHTMONSTER_STATE_ATTACK && _vKnightMonster[i]->getState() != KNIGHTMONSTER_STATE_IDLE)
				{
					switch (_vKnightMonster[i]->getDirection())
					{
					case KNIGHTMONSTER_DIRECTION_LEFT:
						_vKnightMonster[i]->setTum(_vKnightMonster[i]->getRect().left - _dungeonMap->getTile(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y)->rc.right);
						_vKnightMonster[i]->setX(_vKnightMonster[i]->getX() - _vKnightMonster[i]->getTum());
						break;
					case KNIGHTMONSTER_DIRECTION_RIGHT:
						_vKnightMonster[i]->setTum(_vKnightMonster[i]->getRect().right - _dungeonMap->getTile(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y)->rc.left);
						_vKnightMonster[i]->setX(_vKnightMonster[i]->getX() - _vKnightMonster[i]->getTum());
						break;
					case KNIGHTMONSTER_DIRECTION_UP:
						_vKnightMonster[i]->setTum(_vKnightMonster[i]->getRect().top - _dungeonMap->getTile(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y)->rc.top);
						_vKnightMonster[i]->setY(_vKnightMonster[i]->getY() - _vKnightMonster[i]->getTum());
						break;
					case KNIGHTMONSTER_DIRECTION_DOWN:
						_vKnightMonster[i]->setTum(_vKnightMonster[i]->getRect().bottom - _dungeonMap->getTile(_vKnightMonster[i]->getTileIndex(j).x, _vKnightMonster[i]->getTileIndex(j).y)->rc.bottom);
						_vKnightMonster[i]->setY(_vKnightMonster[i]->getY() - _vKnightMonster[i]->getTum());
						break;
					}
				}
			}
			break;
		}
		_vKnightMonster[i]->setTileCollisionRect(RectMakeCenter(_vKnightMonster[i]->getX(), _vKnightMonster[i]->getY() + 50, _vKnightMonster[i]->getImage()->getFrameWidth() - 236, _vKnightMonster[i]->getImage()->getFrameHeight() - 176)); //Ÿ�ϱ���

	}
}

//���ʹ̵��� �������� �������� �Լ�
void enemyManager::enemyRespon()
{
}

