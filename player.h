#pragma once
#include "gameNode.h"
#include "jump.h"
#include "DashAttack.h"
#include "Inventory.h"

#define PLAYERFPS 6			//ĳ���� ������ 
#define PLAYERJUMP 14		//���� ������
#define PLAYERATTACKFPS 12	//���� ������

//Ŭ���� ù��° ���� �빮�ڷ�
//�޼ҵ�� �Ǿտ� �ϴ���(���ϴ³��ΰ�) �� �ڷ� ����� �ϴ³༮�ΰ� 3. ���� 

class enemyManager;
class dungeonMap;
class townMap;
enum PLAYERSTATE
{
	PLAYER_IDLE = 0,			//ĳ���� ���̵� ����
	PLAYER_IDLE_ACTION,		//ĳ���� ���̵� �׼�
	PLAYER_WALK,			//ĳ���� �ȴ� ����
	PLAYER_RUN,				//�� ��
	PLAYER_TR_PUSH_UP,		//�� �ж�
	PLAYER_JUMP,			//����
	PLAYER_LEVELUP,			//������
	PLAYER_ATTACK,			//����
	PLAYER_COMBINATION,		//���Ӱ���
	PLAYER_JUMP_ATTACK,		//��������
	PLAYER_DASH_JUMP_ATTACK,//�뽬��������
	PLAYER_DEATH,			//�ֱ�
	PLAYER_DASH_ATTACK,		//�뽬����
	PLAYER_WORLDMAP,		//����ʻ���
	PLAYER_FALL,			//��������
	PLAYER_OBJ_PICKUP,		//������Ʈ �鶧
	PLAYER_OBJ_WALK,		//������Ʈ��� ������
	PLAYER_OBJ_THROW,		//������Ʈ ������
	PLAYER_ENEMY_ATTACK
};

enum PLAYERDIRECTION
{
	LEFT = 0,
	RIGHT,
	UP,
	DOWN
};

struct tagPlayer
{
	RECT rc;						//ĳ���� ��Ʈ
	RECT attackRc;
	image* image_obj;				//ĳ���� ������Ʈ �鶧 �̹���
	image* image;					//ĳ���� �̹���
	animation* ani;					//ĳ���� �ִϸ��̼�
	PLAYERSTATE state;				//ĳ���� ����
	PLAYERDIRECTION direction;		//ĳ���� �����¿� ����
	float x, y;						//ĳ���� ��ǥ
	float speed;					//���ǵ�
	float jumpPower;				//���� �Ŀ�
	float gravity;					//�߷°�
	int idX, idY;					//Ÿ�� �ε�����
	int maxHP;						//�ִ�ü��
	int HP;							//����ü��
	int str;						//���ݷ�
	int def;						//����
	int money;						//��
	int alphaRender;				//����
};

class player : public gameNode
{
private:
	tagPlayer _player;
	image* _image;
	jump* _jump;
	DashAttack* _dashAttack;

	Inventory* _inventory;

	int _doubleKey[4];					// Ű�� 2�� �������� �޸��� ���ֱ����� ��Ʈ�� ����
	int _curTime[4], _oldTime[4];

	int _attackComboKey;				// Ű�� 2�� �������� ���� �޺� (������ ���� ����)
	int _oldAttackTime;					// ������Ÿ��
	int _oldJumpTime;					// ������Ÿ��

	float _startX;						// ���� ���� ��ġ
	float _startY;						// ���� ���� ��ġ

	bool _attackMoveStop;				// ���� �߿� ����������
	int _attackMoveStopTime;			// �� �� ����

	int _playerProtectTime;
	int _alphaChangeTime;
	bool _playerProtect;				//���Ϳ��� ���ݴ����� ��
	bool _alphaChange;					//�÷��̾� ���ĺ��� ������


	bool _isRun;						// �ٴ���
	bool _isAttack;						// ������
	bool _isJump;						// ������
	bool _isWalk;						// �ȴ���
	bool _unMove;						// �÷��̾� �������� ���ϰ�


	int TileX, TileY;

	int rightX, rightY;
	POINT tileIndex[2];

	enemyManager* _enemyManager;
	dungeonMap* _dungeon;
	townMap* _town;
public:
	player();
	~player();

	HRESULT init();
	void release();
	void update(bool enemyCheck, int a);
	void render(float cameraX, float cameraY);

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

	void enemyCollision(bool enemyCheck);

	void playerUI();

	void setEnemyManagerAddressLink(enemyManager* em) { _enemyManager = em; }
	void setMapManagerAddressLink(dungeonMap* dun) { _dungeon = dun; }
	void setTownManagerAddressLink(townMap* to) { _town = to; }
	//===================== �ݹ� �Լ� =======================

	//�ݹ� �����Լ�
	static void callBackAttack(void* obj);
	//�ݹ� �뽬����
	static void callBackDashAttack(void* obj);
	//�ݹ� ����
	static void callBackJump(void* obj);

	//===================== Ÿ�� ���� =======================
	void tileCheck();
	void townCheck();
	//===================== ������ ������ =======================

	PLAYERSTATE getPlayerState() { return _player.state; }
	PLAYERDIRECTION getPlayerDirection() { return _player.direction; }
	animation* getPlayerAni() { return _player.ani; }
	Inventory* getInventory() {return _inventory;}

	void setPlayerState(PLAYERSTATE state) { _player.state = state; }
	void setPlayerDirection(PLAYERDIRECTION direction) { _player.direction = direction; }
	void setPlayerAni(animation* animation) { _player.ani = animation; }
	void setPlayerPosX(float playerX) { _player.x = playerX; }
	void setPlayerPosY(float playerY) { _player.y = playerY; }
	void setPlayerSpeed(float speed) { _player.speed = speed; }
	void setPlayerUnMove(bool unMove) { _unMove = unMove; }

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
	bool getPlayerRun() { return _isRun; }
};

