#pragma once
#include "gameNode.h"

enum NPCDIRECTION
{
	NPC_LEFT = 0,
	NPC_RIGHT,
	NPC_UP,
	NPC_DOWN
	////����
	//BIRD_LEFT,
	//BIRD_RIGHT,
	//BIRD_UP,
	//BIRD_DOWN,
	//
	////�Һ���
	//ELDER_LEFT,
	//ELDER_RIGHT,
	//ELDER_UP,
	//ELDER_DOWN,
	//
	////��
	//ELLE_LEFT,
	//ELLE_RIGHT,
	//ELLE_UP,
	//ELLE_DOWN,
	//
	////�����ֹ�(���̵�)
	//TOWNHUMAN_MAID_LEFT,
	//TOWNHUMAN_MAID_RIGHT,
	//TOWNHUMAN_MAID_UP,
	//TOWNHUMAN_MAID_DOWN,
	//
	////�����ֹ�(�ҹ���)
	//TOWNHUMAN_GRANDMA_LEFT,
	//TOWNHUMAN_GRANDMA_RIGHT,
	//TOWNHUMAN_GRANDMA_UP,
	//TOWNHUMAN_GRANDMA_DOWN,
	//
	////�����ֹ�(�Һ���)
	//TOWNHUMAN_GRANDFA_LEFT,
	//TOWNHUMAN_GRANDFA_RIGHt,
	//TOWNHUMAN_GRANDFA_UP,
	//TOWNHUMAN_GRANDFA_DOWN,
	//
	////�����ֹ�(���ò�)
	//TOWNHUMAN_FISHMAN_LEFT,
	//TOWNHUMAN_FISHMAN_RIGHT,
	//TOWNHUMAN_FISHMAN_UP,
	//TOWNHUMAN_FISHMAN_DOWN,
	//
	////�����ֹ�(���)
	//TOWNHUMAN_NORMAL_LEFT,
	//TOWNHUMAN_NORMAL_RIGHT,
	//TOWNHUMAN_NORMAL_UP,
	//TOWNHUMAN_NORMAL_DOWN,
	

};

enum NPCSTATE
{
	//����
	BIRD_IDLE,
	BIRD_MOVE,
	BIRD_PECK,

	//�Һ���
	ELDER_IDLE,
	ELDER_SIT,
	ELDER_MOVE,

	//��
	ELLE_IDLE,
	ELLE_MOVE,

	//�����ֹ�(���̵�)
	TOWNHUMAN_MAID_IDLE,
	TOWNHUMAN_MAID_MOVE,
	TOWNHUMAN_MAID_IDLEACTION,

	//�����ֹ�(�ҹ���)
	TOWNHUMAN_GRANDMA_IDLE,

	//�����ֹ�(�Һ���)
	TOWNHUMAN_GRANDFA_IDLE,

	//�����ֹ�(���ò�)
	TOWNHUMAN_FISHMAN_IDLE,

	//�����ֹ�(���)
	TOWNHUMAN_TOWNMAN_IDLE,
	TOWNHUMAN_TOWNMAN_MOVE
};

enum TOWNHUMAN
{
	TOWN_MAID = 0,
	TOWN_GRANDMA,
	TOWN_GRANDFA,
	TOWN_FISHMAN,
	TOWN_TOWNMAN
};

struct TagNPC
{
	string name;
	image* image;
	animation* ani;
	NPCDIRECTION direction;
	NPCSTATE state;
	TOWNHUMAN townNumber;
	RECT rc;
	float x, y;
	float speed;
	int idX, idY;
};

class npc : public gameNode
{
protected:
	TagNPC _npc;

public:
	npc();
	~npc();

	HRESULT init(string npcName, const char* imageName, float x, float y, int idX, int idY);
	void release();
	void update();
	void render();

	void npcMove();


public:   //������ ������ ����

	void setNPCDirection(NPCDIRECTION direction) { _npc.direction = direction; }
	void setNPCState(NPCSTATE state) { _npc.state = state; }
	void setNPCPosX(float x) { _npc.x = x; }
	void setNPCPosY(float y) { _npc.y = y; }
	void setNPCSpeed(float speed) { _npc.speed = speed; }
	void setNPCIdX(int idX) { _npc.idX = idX; }
	void setNPCIdY(int idY) { _npc.idY = idY; }


	NPCDIRECTION getNPCDirection() { return _npc.direction; }
	NPCSTATE getNPCState() { return _npc.state; }
	RECT getRect() { return _npc.rc; }
	float getNPCPosX() { return _npc.x; };
	float getNPCPosY() { return _npc.y; };
	float getNPCSpeed() { return _npc.speed; }
	int getNPCIdX() { return _npc.idX; }
	int getNPCIdY() { return _npc.idY; }
};