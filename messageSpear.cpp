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
	//INIDATA->iniSave("")

	sprintf_s(_grandma, "�ҸӴ�");
	sprintf_s(_grandmaTitle, "Ÿ��Ʋ");
	sprintf_s(_grandmaMessage, "�޼���");

	sprintf_s(_bird, "����");
	sprintf_s(_birdTitle, "Ÿ��Ʋ");
	sprintf_s(_birdMessage, "�޼���");

	sprintf_s(_elder, "����Ҿƹ�");
	sprintf_s(_elderTitle, "Ÿ��Ʋ");
	sprintf_s(_elderMessage, "�޼���");

	sprintf_s(_elle, "��");
	sprintf_s(_elleTitle, "Ÿ��Ʋ");
	sprintf_s(_elleMessage, "�޼���");

	sprintf_s(_maid, "���̵�");
	sprintf_s(_maidTitle, "Ÿ��Ʋ");
	sprintf_s(_maidMessage, "�޼���");

	sprintf_s(_fishMan, "���ò�");
	sprintf_s(_fishManTitle, "Ÿ��Ʋ");
	sprintf_s(_fishManMessage, "�޼���");

	sprintf_s(_townMan, "�������");
	sprintf_s(_townManTitle, "Ÿ��Ʋ");
	sprintf_s(_townManMessage, "�޼���");
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
