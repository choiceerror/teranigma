#pragma once
#include "gameNode.h"
#include "blackFadeOut.h"
#include "ItemManager.h"
#include "Inventory.h"
#include "ClockFadeOut.h"
#include "player.h"

class ui : public gameNode
{
private:
	enum class ROOMTYPE
	{
		MAIN_ROOM,
		WEAPON_ROOM,
		ACCESSERY_ROOM,
		ARMOR_ROOM
	};

	blackFadeOut* _blackFade;
	ItemManager* _iMgr;
	Inventory* _inventory;
	player* _player;

	//=====�׽�Ʈ��====
	ClockFadeOut* _clockFade;


	//================

	RECT _goal;
	POINT _uiPoint[36];

	POINT _weaponPoint[16];
	POINT _accessoryPoint[32];
	POINT _armorPoint[16];

	//���ε���
	ROOMTYPE _room;

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

	//�ð�����
	float _worldTime;
	float _roomVisitTime;


	//����׿�
	bool _isIndexMode;

	//ȯ�漳��
	bool _preferences;

	float _backgroundSoundSize;
	float _effectSoundSize;

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
	void roomPointSetting();
	void placeChange();

	void dataLoad();
	void itemSetting();
	void itemDraw();

	void uiIndexNameMessageBox();
	void uiReferencesRender();

	void yomiMoveSound();

public:

	float getBackgroundSoundSize() { return _backgroundSoundSize; }
	float getEffectSoundSize() { return _effectSoundSize; }
};

