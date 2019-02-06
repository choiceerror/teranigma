#pragma once
#include "image.h"

//����� ���� ����
static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", WINSIZEX, WINSIZEY);

class gameNode
{
private:
	HDC _hdc;
	//�Ŵ����� �̴��Ҳ��� ����
	bool _managerInit;

public:
	gameNode();
	virtual ~gameNode();

	virtual HRESULT init();
	virtual HRESULT init(bool managerInit);
	virtual void release();
	virtual void update();
	virtual void render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	
	//DC ������
	HDC getMemDC() { return _backBuffer->getMemDC(); }
	HDC getHDC() { return _hdc; }

};

