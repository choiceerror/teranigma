#pragma once
#include "gameNode.h"
#include "ballMonster.h"
#include "fireMonster.h"
#include "knightMonster.h"
#include "fireMonsterBullet.h"
#include "dungeonMap.h"
#include "boss.h"
#include "camera.h"

enum ENEMYINFO
{
	BALLMONSTER_HP = 50,
	FIREMONSTER_HP = 30,
	KNIGHTMONSTER_HP = 100,
	BOSS_HP = 10,
};

enum ONCE //�ѹ��� �ޱ�����.
{
	LINEAR_APPEAR,
	CAMERA_LINEAR,
	BOSS_APPEAR,
	WORLDTIME,
};

enum class BULLET_PATTERN
{
	GOLD_BULLET,
	BLUE_BULLET,
	RED_BULLET,
	RGB_BULLET,
};

enum ATTACK_PATTERN
{
	ATTACK_ONE,
	ATTACK_TWO,
	ATTACK_THREE,
	ATTACK_SHOCK, //��ũ����
	ATTACK_NONE,
};

struct tagShockAttack
{
	RECT rc;
	RECT rc2;
	float x, y;
	image* image;
	animation* ani;
	bool isAttack;
};

class player;

class enemyManager : public gameNode
{
private:
	//������ ����
	vector<ballMonster*> _vBallMonster;
	vector<ballMonster*>::iterator _viBallMonster;

	//���̾� ���� ����
	vector<fireMonster*> _vFireMonster;
	vector<fireMonster*>::iterator _viFireMonster;

	//����Ʈ ���� ����
	vector<knightMonster*> _vKnightMonster;
	vector<knightMonster*>::iterator _viKnightMonster;

	//���� ����
	vector<boss*> _vBoss;
	vector<boss*>::iterator _viBoss;

	//���̾���� �Ѿ�
	fireMonsterBullet* _fireMonsterBullet;

	vector<POINTFLOAT> _vEnemyDeadPoint;
	camera* _camera;

	//=================���� ����==================
	bossBullet* _goldBullet; //����Ѿ�
	bossBullet* _blueBullet; //����Ѿ�
	bossBullet* _redBullet; //�����Ѿ�
	bossBullet* _rgbBullet; //������ ��ģ�Ѿ�
	BULLET_PATTERN _bulletPattern; //�Ѿ� ������ ����
	ATTACK_PATTERN _attackPattern; //��������
	tagShockAttack* _shockAttack;

	float _worldTime;
	bool _isBossAppear; //���� ������� �Ǻ����� �Ұ�
	bool _isOnce[5]; //������Ʈ�κп� �ѹ��� �ޱ�����.

	POINTFLOAT _appearGoal;
	float _goalAngle;
	float _elapsedTime;
	float _goalDistance;
	float _appearSpeed; //���� ���ǵ�

	POINTFLOAT _moveGoal[4];

	float _goalX;
	float _goalY;
	float _moveWorldTime;
	float _rndMoveWorldTime;
	float _attackWorldTime[5];

	int _rndAttack[4]; //3���߿� �Ѱ�����Ÿ��
	int _rndAttackType[3]; //1�� �����߿� �ٸ��� ����Ÿ��.

	float _moveSpeed;
	float _bulletSpeed[3];
	//=================���� ����==================

	player* _player;

	dungeonMap* _dungeonMap; //������

	int _fireBulletSpeed;

	RECT _playerAttackRc; //������ ���ݷ�Ʈ

	int _rndMove[4];


	RECT _objectRc; //���͵� ������ �������� �ӽ÷�Ʈ

public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render(float cameraX, float cameraY);

	void updateCollection(); //������Ʈ ���� �Լ�
	void drawAll(float cameraX, float cameraY); //������ �����Լ�
	void setEnemy(); //���ʹ� ����
	void enemyAI(); //���ʹ̵��� AI
	void playerAttackEnemyCollision(); //�÷��̾� ���ݿ� ���ʹ̵��� �����Լ�
	void fireMonsterBulletFireCollision(int num); //���̾���� �Ѿ� �߻��ϰ� �浹�� �Լ�
	void enemyDead(); //���ʹ̵��� ���� �Լ�
	void tileCheckObjectCollision(); //Ÿ�ϰ��� �浹 �Լ�
	void bossAppear(); //���� ����
	void bossMovePattern(); //���� ��������
	void bossAttackPattern(); //���� ��������
	void bossBulletDraw(float cameraX, float cameraY);
	void enemyRespon(); //���ʹ̵��� �������� �������� �Լ�


public:
	//������ ������ ����
	vector<ballMonster*> getVBallMonster() { return _vBallMonster; }
	vector<ballMonster*>* setVBallMonster() { return &_vBallMonster; }

	vector<fireMonster*> getVFireMonster() { return _vFireMonster; }
	vector<fireMonster*>* setVFireMonster() { return &_vFireMonster; }

	vector<knightMonster*> getVKnightMonster() { return _vKnightMonster; }
	vector<knightMonster*>* setVKnightMonster() { return &_vKnightMonster; }

	vector<boss*> getVBoss() { return _vBoss; }
	vector<boss*>* setVBOss() { return &_vBoss; }

	vector<POINTFLOAT> getVEnemyDeadPoint() { return _vEnemyDeadPoint; }
	vector<POINTFLOAT>* setVEnemyDeadPoint() { return &_vEnemyDeadPoint; }

	fireMonsterBullet* getFireMonsterBullet() { return _fireMonsterBullet; }
	bossBullet* getGoldBullet() { return _goldBullet; }
	bossBullet* getBlueBullet() { return _blueBullet; }
	bossBullet* getRedBullet() { return _redBullet; }
	bossBullet* getRgbBullet() { return _rgbBullet; }
	tagShockAttack* getShockAttack() { return _shockAttack; }

	void setPlayerMemoryAddressLink(player* player) { _player = player; }
	void setDungeonMapAddressLink(dungeonMap* map) { _dungeonMap = map; }
	void setCameraAddressLink(camera* camera) { _camera = camera; }
};

