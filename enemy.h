#pragma once
#include "gameNode.h"
#define ATTACKFPS 5
#define MOVEFPS 5
//���͵��� ����
enum ENEMYDIRECTION
{
	//������ ����
	BALLMONSTER_DIRECTION_LEFT,
	BALLMONSTER_DIRECTION_RIGHT,
	BALLMONSTER_DIRECTION_UP,
	BALLMONSTER_DIRECTION_DOWN,

	//�Ҹ��� ����
	FIREMONSTER_DIRECTION_LEFT,
	FIREMONSTER_DIRECTION_RIGHT,
	FIREMONSTER_DIRECTION_UP,
	FIREMONSTER_DIRECTION_DOWN,


	//����Ʈ���� ����
	KNIGHTMONSTER_DIRECTION_LEFT,
	KNIGHTMONSTER_DIRECTION_RIGHT,
	KNIGHTMONSTER_DIRECTION_UP,
	KNIGHTMONSTER_DIRECTION_DOWN,

	//��������
	BOSS_DIRECTION_LEFT,
	BOSS_DIRECTION_RIGHT,
	BOSS_DIRECTION_UP,
	BOSS_DIRECTION_MOVE,
};
//���͵��� ����
enum ENEMYSTATE
{
	//������ ����
	BALLMONSTER_STATE_IDLE,
	BALLMONSTER_STATE_MOVE,
	BALLMONSTER_STATE_ATTACK,
	BALLMONSTER_STATE_DEAD,

	//�Ҹ��� ����
	FIREMONSTER_STATE_IDLE,
	FIREMONSTER_STATE_MOVE,
	FIREMONSTER_STATE_ATTACK,
	FIREMONSTER_STATE_DEAD,

	//����Ʈ���� ����
	KNIGHTMONSTER_STATE_IDLE,
	KNIGHTMONSTER_STATE_MOVE,
	KNIGHTMONSTER_STATE_ATTACK,
	KNIGHTMONSTER_STATE_DEAD,

	//���� ����
	BOSS_STATE_IDLE,
	BOSS_STATE_MOVE,
	BOSS_STATE_ATTACK,
	BOSS_STATE_DEAD,

};

enum ENEMYMOVETYPE
{
	BASIC_MOVE_TYPE, //�⺻ �����ӻ���
	FOLLOW_MOVE_TYPE, //���󰡴� ������ ����
};

struct tagEnemy
{
	RECT rc;
	RECT attackRc; //���ݷ�Ʈ
	RECT rangeRc; //�÷��̾ �����ȿ� ���ý� �������� �Ǻ����� ��Ʈ
	float x, y;
	float viewX, viewY;
	int idX, idY;
	float speed;
	image* image;
	ENEMYDIRECTION direction;
	ENEMYSTATE state;
	ENEMYMOVETYPE moveType;
	string name;
	animation* motion;
	int damage; //������
	int currentHP, maxHP; //����, �ִ� ü��
	float moveAngle; //������ ����
	float targetAngle;
	float targetDistance;
};

class enemy : public gameNode
{
protected:
	tagEnemy _enemy;

public:
	enemy();
	~enemy();

	virtual HRESULT init(string enemyName, const char* imageName, float x, float y, int idX, int idY, int hp);
	virtual void release();
	virtual void update(float cameraX, float cameraY);
	virtual void render(float viewX, float viewY);

	virtual void enemyMove(float cameraX, float cameraY);

public:
	//������ ������ ����
	float getX() { return _enemy.x; }
	float getY() { return _enemy.y; }
	float getViewX() { return _enemy.viewX; }
	float getViewY() { return _enemy.viewY; }

	void setX(float x) { _enemy.x = x; }
	void setY(float y) { _enemy.y = y; }
	void setViewX(float viewX) { _enemy.viewX = viewX; }
	void setViewY(float viewY) { _enemy.viewY = viewY; }

	RECT getRect() { return _enemy.rc; }
	RECT getRangeRect() { return _enemy.rangeRc; }

	ENEMYDIRECTION getDirection() { return _enemy.direction; }
	void setDirection(ENEMYDIRECTION direction) { _enemy.direction = direction; }

	ENEMYSTATE getState() {return _enemy.state;}
	void setState(ENEMYSTATE state) {_enemy.state = state;}

	void setMoveType(ENEMYMOVETYPE type) {_enemy.moveType = type;}
	float getMoveAngle() {return _enemy.moveAngle;}

	int getCurrentHP() {return _enemy.currentHP;}
	int getMaxHP() {return _enemy.maxHP;}

	void setCurrentHP(int hp) {_enemy.currentHP = hp;}
	void setMaxHP(int maxHp) {_enemy.maxHP = maxHp;}

	int getIdX() {return _enemy.idX;}
	int getIdY() {return _enemy.idY;}

	float getTargetAngle() {return _enemy.targetAngle;}
	void setTargetAngle(float angle) {_enemy.targetAngle = angle;}

	float getTargetDistance() {return _enemy.targetDistance;}
	void setTargetDistance(float distance) {_enemy.targetDistance = distance;}

	float getSpeed() {return _enemy.speed;}
	
};

