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
	guardianMessage();
	
	_talkCount = _talkCount2 = 2;

	_texOldTime = GetTickCount();

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
		vector<string> vStr;
	//if (GetTickCount() - _texOldTime >= 300) 
	//{
	//	if (vStr[0].size() > _talkCount)
	//	{
	//		_talkCount += 2;
	//	}
	//	else if (vStr[1].size() > _talkCount2)
	//	{
	//		_talkCount2 += 2;
	//	}
	//}
		IMAGEMANAGER->findImage("messageSpear")->expandRender(getMemDC(), 0, GAMESIZEY / 2 + 80, 0, 0, 1.7, 1.7);
		_font = CreateFont(40, 25, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, "Sam3KRFont");
		_oldFont = (HFONT)SelectObject(getMemDC(), _font);
		vStr = TXTDATA->txtLoad("messageFile/�Ҿƹ���.txt");
		TextOut(getMemDC(), 80, 500, vStr[0].c_str(), _talkCount);
		TextOut(getMemDC(), 80, 550, vStr[1].c_str(), _talkCount2);
	}
	SelectObject(getMemDC(), _oldFont);
	DeleteObject(_font);
}

void messageSpear::grandfaMessage()
{
	string message[2];
	
	vector<string> vStr;

	message[0] = "����  ��ũ";
	message[1] = "�ٽ�  ����  ����  ����ϱ�!";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/�Ҿƹ���.txt", vStr);
}

void messageSpear::grandmaMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "��ũ��  ž����  ����  ����  ���Ŷ�";
	message[1] = "������  ���͵���  ���縮��  �ִܴ�.";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/�ҸӴ�.txt", vStr);
}

void messageSpear::birdMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "����..  ����..";
	message[1] = "������";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/����.txt", vStr);
}

void messageSpear::elderMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "��� :";
	message[1] = "��  ��  �׷���  �׻�  �����̳�?";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/����Ҿƹ�.txt", vStr);
}

void messageSpear::elleMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "��";
	message[1] = "������..  ������  ������  ���..";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/��.txt", vStr);
}

void messageSpear::fishManMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "....";
	message[1] = "���Ͷ�  �׾�ŷ!";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/���ò�.txt", vStr);
}

void messageSpear::maidMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "";
	message[1] = "";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/���̵�.txt", vStr);
}

void messageSpear::guardianMessage()
{
	string message[8];
	vector<string> vStr;

	message[0] = "Ÿ���� �밡��";
	message[1] = "����  ������̴�";
	message[2] = "��  Ÿ���ȿ���  ����";
	message[3] = "������ �Ǿ��ִ�.";
	message[4] = "�÷���  �غ��Ͽ�";
	message[5] = "���踦  �����ϴ�   ����  ����!";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	vStr.push_back(message[2]);
	vStr.push_back(message[3]);
	vStr.push_back(message[4]);
	vStr.push_back(message[5]);

	TXTDATA->txtSave("messageFile/�����.txt", vStr);
}
