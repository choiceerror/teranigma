#include "stdafx.h"
#include "knightMonster.h"


knightMonster::knightMonster()
{
}


knightMonster::~knightMonster()
{
}

HRESULT knightMonster::init(string enemyName, const char * imageName, float x, float y, int idX, int idY, int hp)
{
	enemy::init(enemyName, imageName, x, y, idX, idY, hp);
	_enemy.name = enemyName;
	_enemy.currentHP = _enemy.maxHP = hp;
	_enemy.state = KNIGHTMONSTER_STATE_IDLE;
	_enemy.direction = KNIGHTMONSTER_DIRECTION_DOWN;

	int downIdle[] = { 3 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "downIdle", imageName, downIdle, 1, 3, true);

	int upIdle[] = { 9 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "upIdle", imageName, upIdle, 1, 3, true);

	int leftIdle[] = { 15 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "leftIdle", imageName, leftIdle, 1, 3, true);

	int rightIdle[] = { 22 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "rightIdle", imageName, rightIdle, 1, 3, true);

	int downMove[] = { 0, 1, 2, 3, 4, 3, 2 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "downMove", imageName, downMove, 7, MOVEFPS, true);

	int upMove[] = { 7, 8, 9, 10, 11, 10, 9, 8 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "upMove", imageName, upMove, 8, MOVEFPS, true);

	int leftMove[] = { 14, 15, 16, 17 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "leftMove", imageName, leftMove, 4, MOVEFPS, true);

	int rightMove[] = { 21, 22, 23, 24 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "rightMove", imageName, rightMove, 4, MOVEFPS, true);

	int downAttack[] = { 28, 29, 30, 31, 32, 33, 34 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "downAttack", imageName, downAttack, 7, ATTACKFPS, false, cbAttack, this);

	int upAttack[] = { 35, 36, 37, 38, 39 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "upAttack", imageName, upAttack, 5, ATTACKFPS, false, cbAttack, this);

	int leftAttack[] = { 42, 43, 44, 45, 46, 47, 48 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "leftAttack", imageName, leftAttack, 7, ATTACKFPS, false, cbAttack, this);

	int rightAttack[] = { 49, 50, 51, 52, 53, 54, 55 };
	KEYANIMANAGER->addArrayFrameAnimation(_enemy.name, "rightAttack", imageName, rightAttack, 7, ATTACKFPS, false, cbAttack, this);

	_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "leftMove");

	return S_OK;
}

void knightMonster::release()
{
}

void knightMonster::update()
{
	enemy::update();
	move();
	attack();

	//���İ�����
	if (_enemy.isHit == true)
	{
		_enemy.alphaValue -= 20;

		if (_enemy.alphaValue <= 80)
		{
			_enemy.isHit = false;
			_enemy.alphaValue = 255;
		}
	}
	//����Ʈ���ʹ� ��Ʈ�� �ʹ�ũ�⶧���� ���⼭ ������
	_enemy.rc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth() / 5 + 10, _enemy.image->getFrameHeight() / 2 + 20);
	_enemy.rangeRc = RectMakeCenter(_enemy.x, _enemy.y, _enemy.image->getFrameWidth() * 2, _enemy.image->getFrameHeight() * 2);
}

void knightMonster::render(float cameraX, float cameraY)
{
	_enemy.viewX = _enemy.x - cameraX;
	_enemy.viewY = _enemy.y - cameraY;

	//Rectangle(getMemDC(), _enemy.rangeRc);
	//Rectangle(getMemDC(), _enemy.rc);
	_enemy.image->alphaAniRenderCenter(getMemDC(), _enemy.viewX, _enemy.viewY, _enemy.motion, _enemy.alphaValue);
	//Rectangle(getMemDC(), _enemy.tileCollisionRc);
	//Rectangle(getMemDC(), _enemy.attackRc);
	//_enemy.image->expandAniRenderCenter(getMemDC(), _enemy.viewX, _enemy.viewY, _enemy.motion);

}

void knightMonster::move()
{
	//���¿� ���� ������ ����
	if (_enemy.moveType == BASIC_MOVE_TYPE)
	{
		switch (_enemy.state)
		{
		case KNIGHTMONSTER_STATE_IDLE:
			if (_enemy.direction == KNIGHTMONSTER_DIRECTION_DOWN)	    _enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "downIdle");
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_UP)	    _enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "upIdle");
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_LEFT)   _enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "leftIdle");
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_RIGHT)  _enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "rightIdle");
			_enemy.motion->start();
			break;
		case KNIGHTMONSTER_STATE_MOVE:
			if (_enemy.direction == KNIGHTMONSTER_DIRECTION_DOWN)
			{
				_enemy.moveAngle = (PI / 180 * 270);
				_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "downMove");
				_enemy.tileCollisionRc = RectMakeCenter(_enemy.x, _enemy.y + 50, _enemy.image->getFrameWidth() -240, _enemy.image->getFrameHeight() -180);
			}
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_UP)
			{
				_enemy.moveAngle = PI / 2;
				_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "upMove");
				_enemy.tileCollisionRc = RectMakeCenter(_enemy.x, _enemy.y + 50, _enemy.image->getFrameWidth() -240, _enemy.image->getFrameHeight() -180);
			}
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_LEFT)
			{
				_enemy.moveAngle = PI;
				_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "leftMove");
				_enemy.tileCollisionRc = RectMakeCenter(_enemy.x, _enemy.y + 50, _enemy.image->getFrameWidth() -240, _enemy.image->getFrameHeight() -180);
			}
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_RIGHT)
			{
				_enemy.moveAngle = PI2;
				_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "rightMove");
				_enemy.tileCollisionRc = RectMakeCenter(_enemy.x, _enemy.y + 50, _enemy.image->getFrameWidth() -240, _enemy.image->getFrameHeight() -180);
			}
			_enemy.motion->start();
			_enemy.x += cosf(_enemy.moveAngle) * _enemy.speed;
			_enemy.y += -sinf(_enemy.moveAngle) * _enemy.speed;
			break;
		}
		//�������� ������.
		if (_directionTime + _enemy.directionWorldTime <= TIMEMANAGER->getWorldTime())
		{
			_rndDirection[KNIGHTMONSTER] = RND->getInt(4);
			_rndState[KNIGHTMONSTER] = RND->getInt(2);

			if (_rndDirection[KNIGHTMONSTER] == 0)
			{
				_enemy.direction = KNIGHTMONSTER_DIRECTION_DOWN;
			}
			else if (_rndDirection[KNIGHTMONSTER] == 1)
			{
				_enemy.direction = KNIGHTMONSTER_DIRECTION_UP;
			}
			else if (_rndDirection[KNIGHTMONSTER] == 2)
			{
				_enemy.direction = KNIGHTMONSTER_DIRECTION_RIGHT;
			}
			else if (_rndDirection[KNIGHTMONSTER] == 3)
			{
				_enemy.direction = KNIGHTMONSTER_DIRECTION_LEFT;
			}

			if (_rndState[KNIGHTMONSTER] == 0)	_enemy.state = KNIGHTMONSTER_STATE_IDLE;
			else _enemy.state = KNIGHTMONSTER_STATE_MOVE;
			_enemy.directionWorldTime = TIMEMANAGER->getWorldTime();
		}
	}
	//�÷��̾� ���󰡴�Ÿ��
	else if (_enemy.moveType == FOLLOW_MOVE_TYPE)
	{
		switch (_enemy.state)
		{
		case KNIGHTMONSTER_STATE_MOVE:
			if (_enemy.direction == KNIGHTMONSTER_DIRECTION_DOWN)
			{
				_enemy.moveAngle = (PI / 180 * 270);
				_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "downMove");
				_enemy.tileCollisionRc = RectMakeCenter(_enemy.x, _enemy.y + 50, _enemy.image->getFrameWidth() - 240, _enemy.image->getFrameHeight() - 180);
			}
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_UP)
			{
				_enemy.moveAngle = PI / 2;
				_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "upMove");
				_enemy.tileCollisionRc = RectMakeCenter(_enemy.x, _enemy.y + 50, _enemy.image->getFrameWidth() - 240, _enemy.image->getFrameHeight() - 180);
			}
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_LEFT)
			{
				_enemy.moveAngle = PI;
				_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "leftMove");
				_enemy.tileCollisionRc = RectMakeCenter(_enemy.x, _enemy.y + 50, _enemy.image->getFrameWidth() - 240, _enemy.image->getFrameHeight() - 180);
			}
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_RIGHT)
			{
				_enemy.moveAngle = PI2;
				_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "rightMove");
				_enemy.tileCollisionRc = RectMakeCenter(_enemy.x, _enemy.y + 50, _enemy.image->getFrameWidth() - 240, _enemy.image->getFrameHeight() - 180);
			}
			_enemy.motion->start();
			_enemy.x += cosf(_enemy.moveAngle) * _enemy.speed;
			_enemy.y += -sinf(_enemy.moveAngle) * _enemy.speed;
			break;
		case KNIGHTMONSTER_STATE_ATTACK:
			if (_enemy.direction == KNIGHTMONSTER_DIRECTION_DOWN)		_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "downAttack");
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_UP)	_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "upAttack");
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_LEFT) 	_enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "leftAttack");
			else if (_enemy.direction == KNIGHTMONSTER_DIRECTION_RIGHT) _enemy.motion = KEYANIMANAGER->findAnimation(_enemy.name, "rightAttack");
			_enemy.motion->start();
			break;
		}
	}
}

void knightMonster::attack()
{
	//����Ʈ ������
	if (_enemy.motion->getFramePosArr().x >= 5 && _enemy.motion->getFramePosArr().y == 4) _enemy.attackRc = RectMakeCenter(_enemy.x, _enemy.y + 70, 30, 100); //�Ʒ�����
	else if (_enemy.motion->getFramePosArr().x >= 3 && _enemy.motion->getFramePosArr().y == 5) _enemy.attackRc = RectMakeCenter(_enemy.x, _enemy.y - 70, 30, 100); //������
	else if (_enemy.motion->getFramePosArr().x >= 5 && _enemy.motion->getFramePosArr().y == 6) _enemy.attackRc = RectMakeCenter(_enemy.x - 70, _enemy.y, 100, 30); //���ʰ���
	else if (_enemy.motion->getFramePosArr().x >= 5 && _enemy.motion->getFramePosArr().y == 7) _enemy.attackRc = RectMakeCenter(_enemy.x + 70, _enemy.y, 100, 30); //�����ʰ���
	else _enemy.attackRc = RectMakeCenter(NULL, NULL, NULL, NULL); //���ݳ����� ��Ʈ ����
}

//���ݳ����� �ٸ���� ���ϱ����� �ݹ��Լ�
void knightMonster::cbAttack(void * obj)
{
	knightMonster* k = (knightMonster*)obj;

	k->setState(KNIGHTMONSTER_STATE_MOVE);
	switch (k->getDirection())
	{
	case KNIGHTMONSTER_DIRECTION_DOWN:
		k->setDirection(KNIGHTMONSTER_DIRECTION_DOWN);
		k->setMotion(KEYANIMANAGER->findAnimation(k->getName(), "downMove"));
		k->getMotion()->start();
		break;
	case KNIGHTMONSTER_DIRECTION_UP:
		k->setDirection(KNIGHTMONSTER_DIRECTION_UP);
		k->setMotion(KEYANIMANAGER->findAnimation(k->getName(), "upMove"));
		k->getMotion()->start();
		break;
	case KNIGHTMONSTER_DIRECTION_LEFT:
		k->setDirection(KNIGHTMONSTER_DIRECTION_LEFT);
		k->setMotion(KEYANIMANAGER->findAnimation(k->getName(), "leftMove"));
		k->getMotion()->start();
		break;
	case KNIGHTMONSTER_DIRECTION_RIGHT:
		k->setDirection(KNIGHTMONSTER_DIRECTION_RIGHT);
		k->setMotion(KEYANIMANAGER->findAnimation(k->getName(), "rightMove"));
		k->getMotion()->start();
		break;
	}

}
