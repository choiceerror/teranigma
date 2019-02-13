#pragma once
#include "gameNode.h"
#include "player.h"
#include "camera.h"

enum GUARDIANDIRECTION
{
	GUARDIAN_CLOCKING = 0,
	GUARDIAN_COMEON,
	GUARDIAN_OUT,
	GUARDIAN_IDLE,
	GUARDIAN_TALK
};

class IntroDungeon : public gameNode
{
private:
	player* _player;
	camera* _camera;
	POINT _dungeonPos;

	image* _door;
	animation* _doorAni;
	POINT _doorPos;
	image* _guardian;
	animation* _guardianAni;

	GUARDIANDIRECTION _guardianDirection;
	int _doorDirection;

	int _count;

	bool _guardianOn;
	bool _guardianTalk;
	bool _playerAccept;
	bool _doorOpen;
public:
	IntroDungeon();
	~IntroDungeon();
	
	HRESULT init();
	void release();
	void update();
	void render();

	void setWindowsSize(int x, int y, int width, int height);
	// �÷��̾ ���Ǹ� ������ ������� ������� ���� �����ϴ�.
	bool getPlayerAccept() { return _playerAccept; }
	// �̰� ����ϸ� ����� ���ϴ°� �����Ҽ�����
	bool getGuardianTalk() { return _guardianTalk; }		
};

