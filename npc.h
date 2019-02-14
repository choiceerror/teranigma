#pragma once
#include "gameNode.h"

enum NPCDIRECTION
{
	//����
	BIRD_LEFT,
	BIRD_RIGHT,
	BIRD_UP,
	BIRD_DOWN,

	//�Һ���
	ELDER_LEFT,
	ELDER_RIGHT,
	ELDER_UP,
	ELDER_DOWN,

	//��
	ELLE_LEFT,
	ELLE_RIGHT,
	ELLE_UP,
	ELLE_DOWN,

	//�����ֹ�
	TOWNHUMAN_LEFT,
	TOWNHUMAN_RIGHT,
	TOWNHUMAN_UP,
	TOWNHUMAN_DOWN
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
	ELLE_MOVE
};

struct TagNPC
{
	string name;
	image* image;
	animation* ani;
	NPCDIRECTION direction;
	NPCSTATE state;
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
};