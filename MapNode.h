#pragma once

#define tileX 60
#define tileY 60 
#define TileMainSIZE 32

#define TileSIZE 32

#define basicTileX 30  //Ÿ�� x�� ���� ����
#define basicTileY 16  //Ÿ�� y�� ���� ����

#define ATTR_UNMOVE		0x00000001
#define ATTR_ENEMY		0x00000002
#define ATTR_START		0x00000004
#define ATTR_END		0x00000008


//����
enum TERRAIN
{
	TR_TILE_ONE, TR_TILE_TWO, TR_TILE_THREE, TR_TILE_FOUR, TR_TILE_FIVE, TR_END,
};

enum OBJECT
{

	OBJ_WALL, OBJ_ENEMY1, OBJ_ENEMY2, OBJ_ENEMY3,
	OBJ_PLAYER1, OBJ_PLAYER2,
	OBJ_NONE,
};

enum CTRL
{
	CTRL_SAVE,				//���̺� ��ư
	CTRL_LOAD,				//�ε�  ��ư
	CTRL_TERRAINDRAW,		//���� ��ư
	CTRL_OBJDRAW,			//������Ʈ ��ư
	CTRL_ERASER,			//���찳 ��ư
	CTRL_END
};

struct tagTile
{
	RECT rc;
	int FrameX;
	int FrameY;
	TERRAIN terrain; //����
	OBJECT obj; //������Ʈ
	int objFrameX; //������Ʈ ��ȣ
	int objFrameY;
	int a;
};


//Ÿ�� ����
struct TileSetting
{
	RECT tilerc;
	int tilex;
	int tiley;
};

// Ŭ���� Ÿ�� ���
struct tilebox
{
	//ùŸ�� ����
	int x;
	int y;
	//������ ����
	int lastX;
	int lastY;

	// ������ - ùŸ�� �� ����
	int q;
	int w;





};

