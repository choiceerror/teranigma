#include "stdafx.h"
#include "messageSpear.h"


messageSpear::messageSpear()
{
}


messageSpear::~messageSpear()
{
}

HRESULT messageSpear::init()
{
	sprintf_s(_grandfa, "�Ҿƹ���");
	sprintf_s(_grandfaTitle, "Ÿ��Ʋ");
	sprintf_s(_grandfaMessage, "�޼���");

	INIDATA->addData(_grandfa, _grandfaTitle, _grandfaMessage);
	INIDATA->iniSave("messageFile/�Ҿƹ���.txt");

	sprintf_s(_grandma, "�ҸӴ�");
	sprintf_s(_grandmaTitle, "Ÿ��Ʋ");
	sprintf_s(_grandmaMessage, "�޼���");

	INIDATA->addData(_grandma, _grandmaTitle, _grandmaMessage);
	INIDATA->iniSave("messageFile/�ҸӴ�.txt");

	sprintf_s(_bird, "����");
	sprintf_s(_birdTitle, "Ÿ��Ʋ");
	sprintf_s(_birdMessage, "�޼���");

	INIDATA->addData(_bird, _birdTitle, _birdMessage);
	INIDATA->iniSave("messageFile/����.txt");

	sprintf_s(_elder, "����Ҿƹ�");
	sprintf_s(_elderTitle, "Ÿ��Ʋ");
	sprintf_s(_elderMessage, "�޼���");

	INIDATA->addData(_elder, _elderTitle, _elderMessage);
	INIDATA->iniSave("messageFile/����Ҿƹ�.txt");

	sprintf_s(_elle, "��");
	sprintf_s(_elleTitle, "Ÿ��Ʋ");
	sprintf_s(_elleMessage, "�޼���");

	INIDATA->addData(_elle, _elleTitle, _elleMessage);
	INIDATA->iniSave("messageFile/��.txt");

	sprintf_s(_maid, "���̵�");
	sprintf_s(_maidTitle, "Ÿ��Ʋ");
	sprintf_s(_maidMessage, "�޼���");

	INIDATA->addData(_maid, _maidTitle, _maidMessage);
	INIDATA->iniSave("messageFile/���̵�.txt");

	sprintf_s(_fishMan, "���ò�");
	sprintf_s(_fishManTitle, "Ÿ��Ʋ");
	sprintf_s(_fishManMessage, "�޼���");

	INIDATA->addData(_fishMan, _fishManTitle, _fishManMessage);
	INIDATA->iniSave("messageFile/�Ҿƹ���.txt");

	sprintf_s(_townMan, "�������");
	sprintf_s(_townManTitle, "Ÿ��Ʋ");
	sprintf_s(_townManMessage, "�޼���");

	INIDATA->addData(_townMan, _townManTitle, _townManMessage);
	INIDATA->iniSave("messageFile/�Ҿƹ���.txt");

	return S_OK;
}

void messageSpear::release()
{
}

void messageSpear::update()
{

}

void messageSpear::render()
{
	if (KEYMANAGER->isStayKeyDown('V'))
	{
		HFONT font;
		HFONT oldFont;
		font = CreateFont(30, 20, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, "Sam3KRFont");
		oldFont = (HFONT)SelectObject(getMemDC(), font);
		TextOut(getMemDC(), 100, 100, INIDATA->loadDataString("messageFile/�Ҿƹ���.txt", _grandfa, _grandfaTitle), strlen(INIDATA->loadDataString("messageFile/�Ҿƹ���.txt", _grandfa, _grandfaTitle)));
		SelectObject(getMemDC(), oldFont);
		DeleteObject(font);
	}
}

void messageSpear::grandfaMessage()
{
	
}

void messageSpear::grandmaMessage()
{
}

void messageSpear::birdMessage()
{
}

void messageSpear::elderMessage()
{
}

void messageSpear::elleMessage()
{
}

void messageSpear::fishManMessage()
{
}

void messageSpear::maidMessage()
{
}
