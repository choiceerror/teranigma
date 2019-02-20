#pragma once
#include "gameNode.h"
#include "camera.h"

struct tagIntro
{
	image* image;
	int alphaValue; //���İ�
	bool padeIn, padeOut; 
	bool isIntroImageShow; //��Ʈ���̹��� �����ٰ���
	POINTFLOAT cameraPos;
};

class intro : public gameNode
{
private:
	tagIntro _intro;
	camera* _camera;
	HFONT _font;
	HFONT _oldFont;
	vector<string*> _vMessage;

	float _textWolrldTime;

	int _talkCount;
public:
	intro();
	~intro();

	HRESULT init();
	void release();
	void update();
	void render();

	void opening(); //������

	void messageAll(); //�޽��� ����
	void messageDraw(const char* txtName); //�޽��� �׸���

	void setWindowsSize(int x, int y, int width, int height);
	
};

