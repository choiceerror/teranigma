// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <string>

using namespace std;

#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "keyManager.h"
#include "imageManager.h"
#include "timeManager.h"
#include "effectManager.h"
#include "iniDataManager.h"
#include "soundManager.h"
#include "sceneManager.h"
#include "keyAniManager.h"
#include "utils.h"
#include "txtData.h"

using namespace BRAVO_UTIL;

//====================================
// ## 18.11.20 ## - 디파인문 -
//====================================

#define WINNAME (LPCTSTR)(TEXT("Beta Team"))
#define WINSTARTX 50		//윈도우 시작 X좌표 
#define WINSTARTY 50		//윈도우 시작 Y좌표
#define WINSIZEX 1800		//윈도우 가로크기
#define WINSIZEY 800		//윈도우 세로크기
#define WINSTYLE WS_CAPTION | WS_SYSMENU

#define GAMESIZEX 1024		//게임사이즈 가로크기
#define GAMESIZEY 768		//게임사이즈 세로크기

#define TOTALSIZEX 1920		//월드맵 사이즈 가로크기 (맵 찍어내면 입력해주세요)
#define TOTALSIZEY 1920		//월드맵 사이즈 세로크기 (맵 찍어내면 입력해주세요)

#define SOUNDSIZE 1			//사운드 크기

#define MAGENTA 0xff00ff
#define BLACK 0x000000
#define WHITE 0xffffff

#define RND randomFunction::getSingleton()
#define KEYMANAGER keyManager::getSingleton()
#define IMAGEMANAGER imageManager::getSingleton()
#define TIMEMANAGER timeManager::getSingleton()
#define EFFECTMANAGER effectManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define SCENEMANAGER sceneManager::getSingleton()
#define KEYANIMANAGER keyAniManager::getSingleton()
#define TXTDATA txtData::getSingleton()
#define INIDATA iniDataManager::getSingleton()

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#define SAFE_DELETE(p) {if(p) { delete(p); (p) = NULL; }}
#define SAFE_RELEASE(p) {if(p) {(p)->release(); }}
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[](p); (p) = NULL;}}

extern HINSTANCE _hInstance;
extern HWND _hWnd;
extern POINT _ptMouse;
extern BOOL _leftButtonDown;