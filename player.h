#pragma once
#include "gameNode.h"
#include "jump.h"

#define PLAYERFPS 6			//ĳ���� ������ 
#define PLAYERJUMP 14		//���� ������
#define PLAYERATTACKFPS 12	//���� ������

//Ŭ���� ù��° ���� �빮�ڷ�
//�޼ҵ�� �Ǿտ� �ϴ���(���ϴ³��ΰ�) �� �ڷ� ����� �ϴ³༮�ΰ� 3. ���� 

class enemyManager;

enum PLAYERSTATE
{
	PLAYER_IDLE,
	PLAYER_IDLE_ACTION,
	PLAYER_WALK,
	PLAYER_RUN,
	PLAYER_TR_PUSH_UP,
	PLAYER_JUMP,
	PLAYER_LEVELUP,
	PLAYER_ATTACK,
	PLAYER_COMBINATION,
	PLAYER_JUMP_ATTACK,
	PLAYER_DASH_JUMP_ATTACK,
	PLAYER_DEATH,
	PLAYER_DASH_ATTACK,
	PLAYER_WORLDMAP,
	PLAYER_FALL,
	PLAYER_OBJ_PICKUP,
	PLAYER_OBJ_WALK,
	PLAYER_OBJ_THROW
};

enum PLAYERDIRECTION
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct tagPlayer
{
	RECT rc;
	image* image_obj;
	image* image;
	animation* ani;
	PLAYERSTATE state;
	PLAYERDIRECTION direction;
	float x, y;
	float speed;
	float jumpPower;
	float gravity;
	int idX, idY;
	int maxHP;
	int HP;
	int str;
	int def;
	int money;
};

class player : public gameNode
{
private:
	tagPlayer _player;
	image* _image;
	jump* _jump;

	int _doubleKey[4];					// Ű�� 2�� �������� �޸��� ���ֱ����� ��Ʈ�� ����
	int _curTime[4], _oldTime[4];

	int _attackComboKey;				// Ű�� 2�� �������� ���� �޺� (������ ���� ����)
	int _oldAttackTime;					// ������Ÿ��
	int _oldJumpTime;					// ������Ÿ��

	float _startX;						// ���� ���� ��ġ
	float _startY;						// ���� ���� ��ġ

	bool _isRun;
	bool _isAttack;
	bool _isJump;

	enemyManager* _enemyManager;
public:
	player();
	~player();

	HRESULT init();
	void release();
	void update();
	void render(float cameraX , float cameraY);

	//�� ���� ������ �Է� �ʱⰪ 
	void keyFrameInit();		
	//Ű �Է� 
	void keyInput();							
	//Ű�ٿ� �Է� �ߺ��� �ڵ� �Լ��� ���� ���
	void keyDownInput(PLAYERDIRECTION direction);
	//Ű�� �Է� �ߺ��� �ڵ� �Լ��� ���� ���
	void keyUpInput(PLAYERDIRECTION direction);	
	//�÷��̾� ���� �ִϸ��̼� ��� �Լ�
	void playerState();
	//����Ű(���� , �޸���)
	void DoubleKeyIntVoid();
	//����� �ߺ��Ǵ� �ڵ� �Լ�ȭ
	void playerAniName(string targetName, string aniName);

	void setEnemyManagerAddressLink(enemyManager* em) { _enemyManager = em; }

	//===================== �ݹ� �Լ� =======================

	//�ݹ� �����Լ�
	static void callBackAttack(void* obj);
	//�ݹ� �뽬����
	static void callBackDashAttack(void* obj);
	//�ݹ� ����
	static void callBackJump(void* obj);

	//===================== ������ ������ =======================

	PLAYERSTATE getPlayerState() { return _player.state; }
	PLAYERDIRECTION getPlayerDirection() { return _player.direction; }
	animation* getPlayerAni() { return _player.ani; }

	void setPlayerState(PLAYERSTATE state) { _player.state = state; }
	void setPlayerDirection(PLAYERDIRECTION direction) { _player.direction = direction; }
	void setPlayerAni(animation* animation) { _player.ani = animation; }
	void setPlayerPosX(float playerX) { _player.x = playerX; }
	void setPlayerPosY(float playerY) { _player.x = playerY; }
	void setPlayerSpeed(float speed) { _player.speed = speed; }

	RECT getPlayerRc() { return _player.rc; }

	float getPlayerX() { return _player.x; }
	float getPlayerY() { return _player.y; }
	float getPlayerSpeed() { return _player.speed; }
	float getPlayerJumpPower() { return _player.jumpPower; }
	float getPlayergravity() { return _player.gravity; }

	int getPlayerMoney() { return _player.money; }
	int getPlayerMaxHP() { return _player.maxHP; }
	int getPlayerHP() { return _player.HP; }
	int getPlayerStr() { return _player.str; }
	int getPlayerDef() { return _player.def; }

	bool getPlayerJump() { return _isJump; }


};

