#include "stdafx.h"
#include "playGround.h"

//프로그램 인스턴스(메모리상에 할당되어 실행중인 프로그램의 번호)
HINSTANCE _hInstance;
//윈도우 핸들
HWND _hWnd;
LPCTSTR _lpszClass = TEXT("CWO");
POINT _ptMouse;

playGround _pg;

BOOL _leftButtonDown;

//함수의 프로토타입 선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//클라이언트 영역 재조정 함수
void setWindowsSize(int x, int y, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	_hInstance = hInstance;

	//운영체제에서 발생하는 메시지 정보를 저장하기 위한 구조체
	MSG message;

	//윈도우 정보를 저장하기 위한 구조체
	WNDCLASS wndClass;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = WINNAME;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndClass);

	_hWnd = CreateWindow(
		WINNAME,				//윈도우 구조체에 등록해준 클래스 이름
		WINNAME,				//윈도우 타이틀 바 이름
		WS_OVERLAPPEDWINDOW,	//윈도우 스타일
		WINSTARTX,				//윈도우 왼쪽상단 생성위치 X좌표
		WINSTARTY,				//윈도우 왼쪽상단 생성위치 Y좌표
		WINSIZEX,				//윈도우 가로크기
		WINSIZEY,				//윈도우 세로크기
		NULL,					//부모 윈도우
		(HMENU)NULL,			//메뉴 핸들
		hInstance,				//인스턴스 지정
		NULL);					//윈도우 및 자식 윈도우 생성시 등록부분

	setWindowsSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	//윈도우 창을 생성 한다음 눈에 보이게끔 해주는 함수
	ShowWindow(_hWnd, nCmdShow);

	//만약에 플레이그라운드 클래스의 init함수가 초기화 실패하면 0을 리턴하고 종료해라
	if (FAILED(_pg.init())) return 0;
	/*
	//윈도우 메시지 루프문(프로시져에서 넘어온 명령대로 처리하는 루프문)
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message); //윈도우에서 어떤 메세지가 발생했는지 받아들인다
		DispatchMessage(&message);	//실제로 처리를 얘가 한다
	}
	*/
	
	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TIMEMANAGER->update(60.0f);
			_pg.update();
			_pg.render();
		}
	}
	

	_pg.release();

	UnregisterClass(WINNAME, hInstance);

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _pg.MainProc(hWnd, iMessage, wParam, lParam);
	
}

void setWindowsSize(int x, int y, int width, int height)
{
	RECT winRect;

	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;
	winRect.bottom = height;

	AdjustWindowRect(&winRect, WINSTYLE, false);

	//실질적으로 클라이언트 영역 크기 셋팅을 한다
	SetWindowPos(_hWnd, NULL, x, y,
		(winRect.right - winRect.left),
		(winRect.bottom - winRect.top),
		SWP_NOZORDER | SWP_NOMOVE);

}