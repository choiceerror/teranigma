#pragma once
#include "image.h"

//����� ���� ����
static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", WINSIZEX, WINSIZEY);

static image* _backBuffer1 = IMAGEMANAGER->addImage("background", 800, WINSIZEY);

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

	void fileDelete();
	void deleteFile(const char* fileAddress);

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	
	image* getBackBuffer() { return _backBuffer; }
	image* getBackBuffer1() { return _backBuffer1; }

	//DC ������
	HDC getMemDC() { return _backBuffer->getMemDC(); }
	HDC getHDC() { return _hdc; }

};

