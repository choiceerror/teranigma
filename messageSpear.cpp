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
	//grandfaMessage();
	//grandmaMessage();
	//birdMessage();
	//elderMessage();
	//elleMessage();
	//fishManMessage();
	//maidMessage();
	//guardianMessage();
	
	_once = false;

	_talkCount = _talkCount2 = 0;

	_pageFrameX = 0;

	_birdTalk = _elderTalk = _elleTalk = _maidTalk = false;
	_grandfaTalk = _grandmaTalk = _fishManTalk = _townManTalk = _guardianTalk = false;

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
	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		if(!_grandfaTalk) _grandfaTalk = true;
		else
		{
			_once = true;
			_grandfaTalk = false;
		}
			
	}
	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		if (!_grandmaTalk) _grandmaTalk = true;
		else
		{
			_once = true;
			_grandmaTalk = false;
		}
	}
	if (KEYMANAGER->isOnceKeyDown('3'))
	{
		if (!_elleTalk) _elleTalk = true;
		else
		{
			_once = true;
			_elleTalk = false;
		}
	}
	if (KEYMANAGER->isOnceKeyDown('4'))
	{
		if (!_birdTalk) _birdTalk = true;
		
		else 
		{
			_once = true;
			_birdTalk = false;
		}
	}
	if (KEYMANAGER->isOnceKeyDown('5'))
	{
		if (!_fishManTalk) _fishManTalk = true;
		else
		{
			_once = true;
			_fishManTalk = false;
		}
	}
	messageRender("messageFile/�Ҿƹ���.txt", _grandfaTalk, 8 , 26);
	messageRender("messageFile/�ҸӴ�.txt", _grandmaTalk, 33, 34);
	messageRender("messageFile/��.txt", _elleTalk, 14, 31);
	messageRender("messageFile/����.txt", _birdTalk, 14, 6);
	messageRender("messageFile/���ò�.txt", _fishManTalk, 33, 34);


	if (_once)
	{
		_talkCount = _talkCount2 = 0;
		_once = false;
	}

}

void messageSpear::grandfaMessage()
{
	string message[2];
	
	vector<string> vStr;


	message[0].assign("����  ��ũ");
	message[1].assign("�ٽ�  ����  ����  ����ϱ�!");

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

	message[0] = "....";
	message[1] = "��  ��  �׷���  �׻�  �����̳�?";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	TXTDATA->txtSave("messageFile/����Ҿƹ�.txt", vStr);
}

void messageSpear::elleMessage()
{
	string message[2];
	vector<string> vStr;

	message[0] = "��ũ��  ž��..";
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

	
	message[0] = "����  ������̴�";
	message[1] = "��  Ÿ���ȿ���  ����";
	message[2] = "������ �Ǿ��ִ�.";
	message[3] = "�÷���  �غ��Ͽ�";
	message[4] = "���踦  �����ϴ�   ����  ����!";
	message[5] = "�̰��� ���� ����̴�!";

	vStr.push_back(message[0]);
	vStr.push_back(message[1]);
	vStr.push_back(message[2]);
	vStr.push_back(message[3]);
	vStr.push_back(message[4]);
	vStr.push_back(message[5]);

	TXTDATA->txtSave("messageFile/�����.txt", vStr);
}

void messageSpear::messageRender(const char* txtName, bool messageBool, int timeNum, int timeNum2)
{
	

	if (messageBool)
	{
		vector<string> vStr;
		_font = CreateFont(35, 25, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, "Sam3KRFont");
		_oldFont = (HFONT)SelectObject(getMemDC(), _font);
		vStr = TXTDATA->txtLoad(txtName);
		if (GetTickCount() - _texOldTime >= 1 * 100)
		{
			if (timeNum >= _talkCount)
			{
				_talkCount += 2;
			}
			else if (timeNum2 >= _talkCount2)
			{
				_talkCount2 += 2;
			}
			else
			{
				IMAGEMANAGER->findImage("talkPage")->expandRender(getMemDC(), GAMESIZEX - 100, GAMESIZEY - 100, _pageFrameX, 0, 1.5, 1.5);
			}
			_texOldTime = GetTickCount();
		}
		IMAGEMANAGER->findImage("messageSpear")->expandRender(getMemDC(), 0, GAMESIZEY / 2 + 80, 0, 0, 1.7, 1.7);
		TextOut(getMemDC(), 70, 550, vStr[0].c_str(), _talkCount);
		TextOut(getMemDC(), 70, 600, vStr[1].c_str(), _talkCount2);
		SelectObject(getMemDC(), _oldFont);
		DeleteObject(_font);
	}
}
