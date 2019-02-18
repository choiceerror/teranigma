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
	grandfaMessage();
	grandmaMessage();
	birdMessage();
	elderMessage();
	elleMessage();
	fishManMessage();
	maidMessage();
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
	vector<string> vStr;

	if (KEYMANAGER->isStayKeyDown('V'))
	{
		HFONT font;
		HFONT oldFont;
		font = CreateFont(30, 20, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, "Sam3KRFont");
		oldFont = (HFONT)SelectObject(getMemDC(), font);
		vStr = TXTDATA->txtLoad("messageFile/�Ҿƹ���.txt");
		TextOut(getMemDC(), 100, 100, vStr[0].c_str(), strlen(vStr[0].c_str()));
		SelectObject(getMemDC(), oldFont);
		DeleteObject(font);
	}
}

void messageSpear::grandfaMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "���";
	message[1] = "";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/�Ҿƹ���.txt", vStr);
}

void messageSpear::grandmaMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "���";
	message[1] = "";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/�ҸӴ�.txt", vStr);
}

void messageSpear::birdMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "���";
	message[1] = "";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/����.txt", vStr);
}

void messageSpear::elderMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "���";
	message[1] = "";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/����Ҿƹ�.txt", vStr);
}

void messageSpear::elleMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "���";
	message[1] = "";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/��.txt", vStr);
}

void messageSpear::fishManMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "���";
	message[1] = "";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/���ò�.txt", vStr);
}

void messageSpear::maidMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "���";
	message[1] = "";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/���̵�.txt", vStr);
}
