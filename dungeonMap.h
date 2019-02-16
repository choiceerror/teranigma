#pragma once
#include "gameNode.h"
#include "mapTool.h"
//���� ����
enum class DUNGEON_FLOOR
{
	FIRST_FLOOR, //1��
	SECOND_FLOOR, //2��
	BOSS_FLOOR, // ������
};

class dungeonMap : public  gameNode
{

private:
	mapTool* _mapT;

	vector<vector<tagTile*>> _vvMap;
	unsigned int TILEX;
	unsigned int TILEY;

	DWORD* _attribute;
	DUNGEON_FLOOR _dungeonFloor; //��������

	int tix;
	int tiy;
	int obx;
	int oby;

public:
	dungeonMap();
	~dungeonMap();

	HRESULT init(DUNGEON_FLOOR floor);
	void release();
	void update();
	void render(float cameraX, float cameraY);
	void load(DUNGEON_FLOOR floor);
	void tileDraw(float cameraX, float cameraY);

	unsigned int getTileX() { return TILEX; }
	unsigned int getTileY() { return TILEY; }

	tagTile* getTile(int x, int y) { return _vvMap[y][x]; }
	DWORD getAttr(int x, int y) { return _attribute[x + y * TILEX]; }

	DUNGEON_FLOOR getDungeonFloor() { return _dungeonFloor; }
	void setDungeonFloor(DUNGEON_FLOOR floor) { _dungeonFloor = floor; }
};

