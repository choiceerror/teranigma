#pragma once
#include "gameNode.h"

enum CAMERAMOD
{
	PLAYER_MODE,
	LINEAR_INTERPOLATION_MODE,
	LINEAR_INTERPOLATION_STOP_MODE
};

//	LINEAR == ����
//	INTERPOLATION == ����

class camera : public gameNode
{
private:
	//ī�޶� ��ǥ
	POINTFLOAT _camera;

	//enum ���� ��� ����
	CAMERAMOD _cameraMode;

	float _distance;
	float _angle;
	float _moveSpeed;

	float _moveTime;
	float _stopTime;
	float _worldTime;
	float _worldTime2;

	//ī�޶� �̵��� ����Ʈ
	POINTFLOAT _orizin;
	POINTFLOAT _new;

	//���������� ������ �ٸ���� ����ؼ� �����ξ�
	float _winSizeX;
	float _winSizeY;

	//��ü�� �������
	float _worldMapSizeX;
	float _worldMapSizeY;


public:
	camera();
	~camera();

	HRESULT init(float winSizeX,float winSizeY,float worldMapSizeX, float worldMapSizeY);
	void release();
	void update(float playerX, float playerY);
	void render();

	//�÷��̾ �߾ӿ����� ī�޶�
	void playerMode(float playerX, float playerY);

	//�Է��� �ð���ŭ ���������� ���� ī�޶�
	void linearMove(float newX, float newY, float cameraMoveTime);
	void linearMoving();

	//�Է��� ����ð���ŭ ���������� ���� ��ž�ð���ŭ �����ִ� ī�޶�
	void linearKeepMove(float newX, float newY, float cameraMoveTime, float stopTime);
	void linearKeepMoving();

	//����ī�޶� ��ġ ���� �Լ�
	POINTFLOAT getCameraPos(float x, float y);

	void exceptionPosition();

	//======================������ ������======================

	POINTFLOAT getCameraPos() { return _camera; }

	float getCameraX() { return _camera.x; }
	void setCameraX(float x) { _camera.x = x; }

	float getCameraY() { return _camera.y; }
	void setCameraY(float y) { _camera.y = y; }

	CAMERAMOD getCameraMode() { return _cameraMode; }
	void setCameraMode(CAMERAMOD v) { _cameraMode = v; }

	void setDistance(float v) { _distance = v; }

	void setAngle(float v) { _distance = v; }

};

