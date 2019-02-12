#pragma once
#include "gameNode.h"
#include "blackFadeOut.h"
#include "ItemManager.h"

enum UIROOM
{
	MAIN_ROOM,
	WEAPON_ROOM,
	ACCESSERY_ROOM,
	ARMOR_ROOM
};


class ui : public gameNode
{
private:
	blackFadeOut* _blackFade;

	ItemManager* _iMgr;

	RECT _goal;
	POINT _pt[36];

	//���ε���
	int _placeFrameX;

	//��� ������
	int _yomiFrameX;
	int _yomiFrameY;

	//����� ������Ʈ �ε���
	int _yomiIndex;

	//��� ����
	int _x, _y;
	int _goalX, _goalY;
	int _count;
	float _yomiAngle;
	float _yomiDistance;
	float _yomiSpeed;
	bool _move;
	bool _isRoomChanging;
	bool _onceTime;

	float _worldTime;

	//����׿�
	bool _isIndexMode;

public:
	ui();
	~ui();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setWindowsSize(int x, int y, int width, int height);
	void yomiFrame();
	void yomiMove();
	void movePoint();
	void placeChange();

	void dataLode();
	void itemSetting();
};

