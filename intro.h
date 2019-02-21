#pragma once
#include "gameNode.h"
#include "camera.h"
#include "button.h"

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

	MESSAGE_X = 100,
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
};
enum IMAGEINFO
{
	ALPHA = 1,
};

struct tagIntro
{
	image* image;
	int alphaValue; //���İ�
	int alphaValue2;
	POINTFLOAT cameraPos;
	float changeWorldTime; //ü���� �ð�
	float changeTime;
	bool isAlpahOn, isAlpahOut;
	bool isOnce[3];
	bool isSizeUpStop;
};

struct tagText
{
	image* image;
	float x, y;
	POINTFLOAT goal;
	float moveSpeed;
	float distance;
	float angle;
	bool isOnce;
	float moveWorldTime;
};

class intro : public gameNode
{
private:
	image* _earthImage;
	image* _buttonImage;
	float _buttonWorldTime;
	int _mapToolButtonFrameY;
	float _earthSizeX, _earthSizeY;
	tagIntro _intro;
	tagText _text;
	camera* _camera;
	HFONT _font;
	HFONT _oldFont;

	image* _menuImage;
	float _textWolrldTime;
	IMAGECHANGE _imageChange;
	int _textCount[2];
	int _textSizeMax[10];
	int _nextText;
	int _num;
	bool _isNextPage;
	float _certainWorldTime; //�����ð� �ڿ� �����Բ�.

	button* _mapToolButton; //������ư

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

	static void setWindowsSize(int x, int y, int width, int height);

	static void cbMapToolSceneChange();
	
	
};

