#include "stdafx.h"
#include "intro.h"


intro::intro()
{
}


intro::~intro()
{
}

HRESULT intro::init()
{
	setWindowsSize(WINSTARTX, WINSTARTY, GAMESIZEX, GAMESIZEY);

	_camera = new camera;

	_camera->init(GAMESIZEX, GAMESIZEY, WINSIZEX, GAMESIZEY);

	IMAGEMANAGER->findImage("black")->setAlpahBlend(true);

	_intro.image = IMAGEMANAGER->findImage("��Ʈ��1");

	_intro.alphaValue = 255;
	_intro.padeIn = _intro.padeOut = _intro.isIntroImageShow = false;

	_intro.cameraPos.x = WINSIZEX / 2;
	_intro.cameraPos.y = 0;

	return S_OK;
}

void intro::release()
{
	SAFE_DELETE(_camera);
}

void intro::update()
{

	opening();


	//�̹����� ��������
	if (_intro.isIntroImageShow == true)
	{
		_intro.cameraPos.x -= 2;
		if (_intro.cameraPos.x <= 0)
		{
			_intro.cameraPos.x = 0;
			_intro.isIntroImageShow = false; //ī�޶� ���� ���޾����� ���̻� ��������
		}
	}
	
	_camera->update(_intro.cameraPos.x, _intro.cameraPos.y);
	
}

void intro::render()
{
	_intro.image->render(getMemDC(), 0, 0, _intro.cameraPos.x - _camera->getCameraX(), _intro.cameraPos.y - _camera->getCameraY(), GAMESIZEX, GAMESIZEY);

	IMAGEMANAGER->findImage("black")->alphaRender(getMemDC(), _intro.alphaValue);

	char str[128];

	sprintf_s(str, "alpha : %d", _intro.alphaValue);
	TextOut(getMemDC(), 100, 100, str, strlen(str));

	sprintf_s(str, "show : %d", _intro.isIntroImageShow);
	TextOut(getMemDC(), 100, 120, str, strlen(str));

	sprintf_s(str, "cameraX : %f", _intro.cameraPos.x);
	TextOut(getMemDC(), 100, 140, str, strlen(str));
}

void intro::opening()
{
	if (_intro.alphaValue > 0)
	{
		_intro.alphaValue -= 1;
	}
	else if (_intro.alphaValue <= 0)
	{
		_intro.alphaValue = 0;
		_intro.isIntroImageShow = true;
	}



}

void intro::messageAll()
{
	string* message;
	message = new string;

	message[0] = "�� ���� ������ �־��� �ΰ��� ������";
	message[1] = "������� ��ο���� �ΰ��� ���̾���.....";
	message[2] = "�̺��� �¾ �� 46����� ������..";
	message[3] = "Ŀ�ٶ� �ΰ��� ������ ���� ��ȭ�� ���� �ݺ��ϰ� �ִ�.";
	message[4] = "���� ���� ������ ���ο� ������ �¾��,";
	message[5] = "��ο� ���� ������ ���ϱⰡ �´�.";
	message[6] = "���� ������ ���ο� ����� ���������,";
	message[7] = "������ ��������...";
	message[8] = "��ο� ���� ������ ���� �� ����ڰ� ���´�..";
	message[9] = "�η��� �� �ΰ��� ������ �Ű� �Ǹ� ��� �ҷ���";

	_vMessage.push_back(message);
	//TXTDATA->txtSave("messageFile/��Ʈ��.txt", &_vMessage);
	
}

void intro::messageDraw(const char* txtName)
{
}

void intro::setWindowsSize(int x, int y, int width, int height)
{
	RECT winRect;

	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;
	winRect.bottom = height;

	AdjustWindowRect(&winRect, WINSTYLE, false);

	//���������� Ŭ���̾�Ʈ ���� ũ�� ������ �Ѵ�
	SetWindowPos(_hWnd, NULL, x, y,
		(winRect.right - winRect.left),
		(winRect.bottom - winRect.top),
		SWP_NOZORDER | SWP_NOMOVE);
}
