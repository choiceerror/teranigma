#pragma once
#include "gameNode.h"
#include "camera.h"

enum MESSAGE
{
	MESSAGE_ONE,
	MESSAGE_TWO,
	MESSAGE_THREE,
	MESSAGE_FOUR,
	MESSAGE_FIVE,
	MESSAGE_SIX,
	MESSAGE_SEVEN,
	MESSAGE_EIGHT,
	MESSAGE_NINE,
	MESSAGE_TEN,

	MESSAGE_X = 150,
	MESSAGE_Y = GAMESIZEY / 2,
};

enum class IMAGECHANGE
{
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
};
enum IMAGEINFO
{
	ALPHA = 5,
};

struct tagIntro
{
	image* image;
	int alphaValue; //���İ�
	POINTFLOAT cameraPos;
	float changeWorldTime; //ü���� �ð�
	float changeTime;
	bool isAlpahOn, isAlpahOut;
	bool isOnce[4];
	bool isSizeUpStop;
};

class intro : public gameNode
{
private:
	image* _earthImage;
	float _earthSizeX, _earthSizeY;
	tagIntro _intro;
	camera* _camera;
	HFONT _font;
	HFONT _oldFont;

	float _textWolrldTime;
	IMAGECHANGE _imageChange;
	int _textCount[2];
	int _textSizeMax[10];
	int _nextText;
	int _num;
	bool _isNextPage;

public:
	intro();
	~intro();

	HRESULT init();
	void release();
	void update();
	void render();

	void opening(); //������

	void messageAll(); //�޽��� ����
	void messageDraw(); //�޽��� �׸���

	void setWindowsSize(int x, int y, int width, int height);
	
};

