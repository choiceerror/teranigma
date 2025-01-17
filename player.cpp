#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"
#include "dungeonMap.h"
#include "townMap.h"
#include "npcManager.h"
#include "messageSpear.h"
#pragma warning(disable:4996)

player::player()
{
}


player::~player()
{
}

HRESULT player::init(bool unAttack)
{
	IMAGEMANAGER->addFrameImage("player_obj", "image/player_obj.bmp", 0, 0, 1540, 800, 10, 10, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_ui", "image/playerUIFont.bmp", 0, 0, 450, 90, 10, 2, true, RGB(255, 0, 255));
	_jump = new jump;
	_dashAttack = new DashAttack;
	_dashJumpAttack = new DashJumpAttack;
	_inventory = new Inventory;
	_txtData = new txtData;
	_jump->init();
	_dashAttack->init();
	_inventory->init();
	_inventory->inventoryLoad();
	check = false;
	keyFrameInit();

	_player.image = IMAGEMANAGER->findImage("player");
	_player.image_obj = IMAGEMANAGER->findImage("player_obj");
	_player.image->setAlpahBlend(true);

	_oldJumpTime = -100;
	_levelOldTime = 0;

	_player.HP = 28;
	_player.maxHP = 28;
	_player.damage = 10;
	_player.def = 1;
	_player.money = 10;
	_player.exp = 0;
	_player.maxExp = 5;
	_player.level = 1;

	_player.x = GAMESIZEX / 2;
	_player.y = GAMESIZEY / 2;
	_player.speed = 3;
	_player.rc = RectMakeCenter(_player.x, _player.y + 10, 40, 60);
	_player.state = PLAYER_IDLE;
	_player.direction = DOWN;
	_player.jumpPower = 8.0f;
	_player.gravity = 0.4f;
	_player.alphaRender = 255;

	for (int i = 0; i < 4; i++)
	{
		_curTime[i] = GetTickCount();
		_oldTime[i] = _curTime[i];
		_doubleKey[i] = 0;
	}

	_attackComboKey = 0;

	_isRun = _isAttack = _isJump = _isWalk = false;

	_playerProtect = _alphaChange = false;

	_attackMoveStop = false;

	_unMove = false;

	_death = _levelUP = false;
	_tileCheck = true;

	_unAttack = unAttack;

	playerLoad();
	playerUiLoad();

	return S_OK;
}

void player::release()
{
	SAFE_DELETE(_jump);
	SAFE_DELETE(_dashAttack);
	SAFE_DELETE(_dashJumpAttack);
	SAFE_DELETE(_inventory);
	SAFE_DELETE(_txtData);
}

void player::update(bool enemyCheck, int a)
{
	if (_player.state == PLAYER_RUN)
	{
		_isRun = true;
	}
	else _isRun = false;

	if (_player.state == PLAYER_DASH_ATTACK || _player.state == PLAYER_ATTACK || _player.state == PLAYER_JUMP_ATTACK || _player.state == PLAYER_DASH_JUMP_ATTACK || _player.state == PLAYER_COMBINATION)
	{
		_isAttack = true;
	}
	else _isAttack = false;

	if (_player.state == PLAYER_JUMP || _player.state == PLAYER_JUMP_ATTACK || _player.state == PLAYER_DASH_JUMP_ATTACK)
	{
		_isJump = true;
	}
	else  _isJump = false;
	if (_player.state == PLAYER_WALK) _isWalk = true;
	else _isWalk = false;

	if (!_attackMoveStop) _attackMoveStopTime = GetTickCount();
	if (GetTickCount() - _attackMoveStopTime >= 1 * 200)
	{
		_attackMoveStop = false;
	}

	levelUP();
	if (!_death)
	{
		if (!_levelUP && !(_player.state == PLAYER_FALL))
		{
			enemyCollision(enemyCheck);
			if ((!(_player.state == PLAYER_ENEMY_ATTACK) || _player.state == PLAYER_IDLE) && !_unMove &&
				!(_player.state == PLAYER_DASH_ATTACK))
			{
				keyInput();
			}
		}
	}
	

	playerState();
	_jump->update();
	_dashAttack->update(&_player.x, &_player.y, check, _player.state);
	_dashJumpAttack->update(&_player.x, &_player.y);
	_inventory->update();
	if (_tileCheck)
	{
		if (a == 1)
		{
			tileCheck();
		}
		else if (a == 2)
		{
			townCheck();
			npcCheck();
		}
	}

	_player.rc = RectMakeCenter(_player.x, _player.y + 10, 40, 50);

	if (_player.currentScene == PLAYERSCENE::DUNGEON_1F || _player.currentScene == PLAYERSCENE::DUNGEON_2F || _player.currentScene == PLAYERSCENE::BOSS)
	{
		if (_player.HP > 0)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SHIFT))
			{
				playerSave();
				playerUiSave();
				playerSceneSave();
				_inventory->inventorySave();
				SOUNDMANAGER->stop("theTower");
				SCENEMANAGER->changeScene("ui");
			}
		}
	}

	if (KEYMANAGER->isOnceKeyDown('Y'))
	{
		playerSave();
	}
	if (KEYMANAGER->isOnceKeyDown('U'))
	{
		playerLoad();
	}

	if (KEYMANAGER->isOnceKeyDown('0'))
	{
		_player.damage = 100;
	}
	if (KEYMANAGER->isOnceKeyDown('9'))
	{
		_player.damage = 10;
	}
	

	playerDeath(enemyCheck);
}

void player::render(float cameraX, float cameraY, bool uiRender)
{
	_player.image->alphaAniRenderCenter(getMemDC(), _player.x - cameraX, _player.y - cameraY, _player.ani, _player.alphaRender);
	//Rectangle(getMemDC(), _player.rc);
	char str[128];
	//for (int i = 0; i < 4; i++)
	//{
	//	sprintf_s(str, "%d", _doubleKey[i]);
	//	//SetTextColor(getMemDC(), RGB(0, 0, 0));
	//	TextOut(getMemDC(), 100 + i * 30, 100, str, strlen(str));
	//}
	//
	//sprintf_s(str, "%d", check);
	//SetTextColor(getMemDC(), RGB(0, 0, 0));
	//TextOut(getMemDC(), 300, 120, str, strlen(str));
	
	//sprintf_s(str, "%d", (int)_player.currentScene);
	//TextOut(getMemDC(), 140, 320, str, strlen(str));

	if (_player.HP > 0)
	{
		playerUIRender(uiRender);
	}

	//sprintf_s(str, "%d", _unAttack);
	//TextOut(getMemDC(), 120, 120, str, strlen(str));
	//sprintf_s(str, "%d", _player.maxExp);
	//TextOut(getMemDC(), 140, 120, str, strlen(str));
	//sprintf_s(str, "%d", _levelUP);
	//TextOut(getMemDC(), 160, 120, str, strlen(str));
}
// 캐릭터 프레임 초기값
void player::keyFrameInit()
{

	KEYANIMANAGER->deleteAll();
	int idleLeft[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "idleLeft", "player", idleLeft, 1, PLAYERFPS, true);
	int idleRight[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "idleRight", "player", idleRight, 1, PLAYERFPS, true);
	int idleUp[] = { 2 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "idleUp", "player", idleUp, 1, PLAYERFPS, true);
	int idleDown[] = { 3 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "idleDown", "player", idleDown, 1, PLAYERFPS, true);
	int idleAction[] = { 4, 5, 6, 7, 6, 7, 5, 4 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "idleAction", "player", idleAction, 8, PLAYERFPS, true);
	int moveLeft[] = { 24, 25, 26, 27, 28, 29 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "moveLeft", "player", moveLeft, 6, PLAYERFPS, true);
	int moveRight[] = { 12, 13, 14, 15, 16, 17 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "moveRight", "player", moveRight, 6, PLAYERFPS, true);
	int moveUp[] = { 36, 37, 38, 40, 41, 40, 37 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "moveUp", "player", moveUp, 7, PLAYERFPS, true);
	int moveDown[] = { 48, 49, 50, 51, 52, 53 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "moveDown", "player", moveDown, 6, PLAYERFPS, true);
	int enemyAttackLeft[] = { 30, 31 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "enemyAttackLeft", "player", enemyAttackLeft, 2, 10, false);
	int enemyAttackRight[] = { 18, 19 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "enemyAttackRight", "player", enemyAttackRight, 2, 10, false);
	int enemyAttackUp[] = { 42, 43 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "enemyAttackUp", "player", enemyAttackUp, 2, 10, false);
	int enemyAttackDown[] = { 54 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "enemyAttackDown", "player", enemyAttackDown, 1, 10, false);
	int runLeft[] = { 65, 66, 67, 68, 69 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "runLeft", "player", runLeft, 5, PLAYERFPS, true);
	int runRight[] = { 60, 61, 62, 63, 64 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "runRight", "player", runRight, 5, PLAYERFPS, true);
	int runUp[] = { 72, 73, 74, 75, 76 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "runUp", "player", runUp, 5, PLAYERFPS, true);
	int runDown[] = { 77, 78, 79, 80, 81 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "runDown", "player", runDown, 5, PLAYERFPS, true);
	int push[] = { 84, 85 , 86, 87 , 88 , 89 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "push", "player", push, 6, PLAYERFPS, true);
	int jumpLeft[] = { 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "jumpLeft", "player", jumpLeft, 11, PLAYERJUMP, false, callBackJump, this);
	int jumpRight[] = { 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "jumpRight", "player", jumpRight, 11, PLAYERJUMP, false, callBackJump, this);
	int jumpUp[] = { 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "jumpUp", "player", jumpUp, 11, PLAYERJUMP, false, callBackJump, this);
	int jumpDown[] = { 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "jumpDown", "player", jumpDown, 11, PLAYERJUMP, false, callBackJump, this);
	int levelUp[] = { 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "levelUp", "player", levelUp, 11, 6, false);
	int attackLeft[] = { 160, 161, 162, 163 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "attackLeft", "player", attackLeft, 4, PLAYERATTACKFPS, false, callBackAttack, this);
	int attackRight[] = { 156, 157, 158, 159 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "attackRight", "player", attackRight, 4, PLAYERATTACKFPS, false, callBackAttack, this);
	int attackUp[] = { 168, 169, 170, 171 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "attackUp", "player", attackUp, 4, PLAYERATTACKFPS, false, callBackAttack, this);
	int attackDown[] = { 172, 173, 174 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "attackDown", "player", attackDown, 3, PLAYERATTACKFPS, false, callBackAttack, this);
	int combinationLeft[] = { 192, 193, 194, 195, 196, 197, 198, 199 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "combinationLeft", "player", combinationLeft, 8, PLAYERATTACKFPS, false, callBackAttack, this);
	int combinationRight[] = { 180, 181, 182, 183, 184, 185, 186, 187 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "combinationRight", "player", combinationRight, 8, PLAYERATTACKFPS, false, callBackAttack, this);
	int combinationUp[] = { 204, 205, 206, 207, 208, 209 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "combinationUp", "player", combinationUp, 6, PLAYERATTACKFPS, false, callBackAttack, this);
	int combinationDown[] = { 216, 217, 218, 219, 220, 221, 222 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "combinationDown", "player", combinationDown, 7, PLAYERATTACKFPS, false, callBackAttack, this);
	int jumpAttackLeft[] = { 240, 241, 242, 243, 244, 245,246, 247 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "jumpAttackLeft", "player", jumpAttackLeft, 8, PLAYERJUMP, false, callBackJump, this);
	int jumpAttackRight[] = { 228, 229, 230, 231, 232, 233, 234, 235 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "jumpAttackRight", "player", jumpAttackRight, 8, PLAYERJUMP, false, callBackJump, this);
	int jumpAttackUp[] = { 252, 253, 254, 255, 256, 257, 258, 259 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "jumpAttackUp", "player", jumpAttackUp, 8, PLAYERJUMP, false, callBackJump, this);
	int jumpAttackDown[] = { 264, 265, 266, 267, 268, 269, 270, 271 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "jumpAttackDown", "player", jumpAttackDown, 8, PLAYERJUMP, false, callBackJump, this);
	int dashJumpAttackLeft[] = { 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "dashJumpAttackLeft", "player", dashJumpAttackLeft, 12, 12, false, callBackDashJumpAttack, this);
	int dashJumpAttackRight[] = { 276,277,278,279,280,281,282,283,284,285,286,287 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "dashJumpAttackRight", "player", dashJumpAttackRight, 12, 12, false, callBackDashJumpAttack, this);
	int dashJumpAttackUp[] = { 300, 301, 302, 303, 304, 305, 306, 307 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "dashJumpAttackUp", "player", dashJumpAttackUp, 8, 12, false, callBackDashJumpAttack, this);
	int dashJumpAttackDown[] = { 312, 313, 314, 315, 316, 317, 318, 319 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "dashJumpAttackDown", "player", dashJumpAttackDown, 8, 12, false, callBackDashJumpAttack, this);
	int death[] = { 324,325,326,327,328,329 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "death", "player", death, 6, PLAYERFPS, false);
	int fall[] = { 330, 331,331, 332 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "fall", "player", fall, 4, 4, false, callBackFall, this);
	int dashAttackLeft[] = { 340, 341, 342, 343 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "dashAttackLeft", "player", dashAttackLeft, 4, PLAYERFPS, false, callBackJump, this);
	int dashAttackRight[] = { 336, 337, 338, 339 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "dashAttackRight", "player", dashAttackRight, 4, PLAYERFPS, false, callBackJump, this);
	int dashAttackUp[] = { 348, 349, 350, 351 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "dashAttackUp", "player", dashAttackUp, 4, PLAYERFPS, false, callBackJump, this);
	int dashAttackDown[] = { 352, 353, 354, 355 };
	KEYANIMANAGER->addArrayFrameAnimation("ark", "dashAttackDown", "player", dashAttackDown, 4, PLAYERFPS, false, callBackJump, this);
	int pickupLeft[] = { 0, 1, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "pickupLeft", "player_obj", pickupLeft, 4, PLAYERFPS, false);
	int pickupRight[] = { 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "pickupRight", "player_obj", pickupRight, 4, PLAYERFPS, false);
	int pickupUp[] = { 14, 15, 16, 17 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "pickupUp", "player_obj", pickupUp, 4, PLAYERFPS, false);
	int pickupDown[] = { 10, 11, 12, 13 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "pickupDown", "player_obj", pickupDown, 4, PLAYERFPS, false);
	int objMoveLeft[] = { 30, 31, 32, 33, 34, 35 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "objMoveLeft", "player_obj", objMoveLeft, 6, PLAYERFPS, true);
	int objMoveRight[] = { 20, 21, 22, 23, 24, 25 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "objMoveRight", "player_obj", objMoveRight, 6, PLAYERFPS, true);
	int objMoveUp[] = { 40, 41, 42, 43, 44, 45 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "objMoveUp", "player_obj", objMoveUp, 6, PLAYERFPS, true);
	int objMoveDown[] = { 50, 51, 52, 53, 54, 55 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "objMoveDown", "player_obj", objMoveDown, 6, PLAYERFPS, true);
	int objThrowLeft[] = { 71, 72, 73, 74, 75, 76, 77, 78 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "objThrowLeft", "player_obj", objThrowLeft, 8, PLAYERFPS, false);
	int objThrowRight[] = { 60, 61, 62, 63, 64, 65, 66, 67, 68 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "objThrowRight", "player_obj", objThrowRight, 9, PLAYERFPS, false);
	int objThrowUp[] = { 80, 81, 82, 83 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "objThrowUp", "player_obj", objThrowUp, 8, PLAYERFPS, false);
	int objThrowDown[] = { 71, 72, 73, 74, 75, 76, 77, 78 };
	KEYANIMANAGER->addArrayFrameAnimation("ark_obj", "objThrowDown", "player_obj", objThrowDown, 8, PLAYERFPS, false);

	_player.ani = KEYANIMANAGER->findAnimation("ark", "idleLeft");


}
// 키 입력
void player::keyInput()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		if (!_attackMoveStop && !(_player.state == PLAYER_DASH_ATTACK))						// 공격 후 다른방향으로 전환 방지
		{
			keyDownInput(LEFT);
			_player.x -= _player.speed;
		}
		else if (!(_player.state == PLAYER_COMBINATION) && !(_player.state == PLAYER_DASH_ATTACK))  _player.x -= 0.5f;
		else if (_player.state == PLAYER_DASH_ATTACK) _player.x -= _player.speed;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
	{
		keyUpInput(LEFT);
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		if (!_attackMoveStop && !(_player.state == PLAYER_DASH_ATTACK))
		{
			keyDownInput(RIGHT);
			_player.x += _player.speed;
		}
		else if (!(_player.state == PLAYER_COMBINATION) && !(_player.state == PLAYER_DASH_ATTACK)) _player.x += 0.5f;
		else if (_player.state == PLAYER_DASH_ATTACK) _player.x += _player.speed;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
	{
		keyUpInput(RIGHT);
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{

		if ((!_attackMoveStop) && !(_player.state == PLAYER_DASH_ATTACK))
		{
			keyDownInput(UP);
			_player.y -= _player.speed;
			if (_isJump)
			{
				_startY -= _player.speed;
			}
		}
		else if (!(_player.state == PLAYER_COMBINATION) && !(_player.state == PLAYER_DASH_ATTACK))  _player.y -= 0.5f;
		else if (_player.state == PLAYER_DASH_ATTACK) _player.y -= _player.speed;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_UP))
	{
		keyUpInput(UP);
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{

		if (!_attackMoveStop && !(_player.state == PLAYER_DASH_ATTACK))
		{
			keyDownInput(DOWN);
			_player.y += _player.speed;
			if (_isJump)
			{
				_startY += _player.speed;
			}
		}
		else if (!(_player.state == PLAYER_COMBINATION) && !(_player.state == PLAYER_DASH_ATTACK))  _player.y += 0.5f;
		else if (_player.state == PLAYER_DASH_ATTACK) _player.y += _player.speed;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
	{
		keyUpInput(DOWN);
	}

	if (!_unAttack)
	{
		if (KEYMANAGER->isOnceKeyDown('C'))
		{
			if (!_isRun && !_isJump)
			{
				_attackMoveStop = true;
				if (_attackComboKey >= 1)
				{
					SOUNDMANAGER->play("comboAttack", 1);
					_player.state = PLAYER_COMBINATION;
				}
				if (!(_player.state == PLAYER_COMBINATION))
				{
					SOUNDMANAGER->play("playerAttack", 1);
					_player.state = PLAYER_ATTACK;
				}
			}
			else if (_isRun && !_isJump && _player.speed == 5)
			{
				SOUNDMANAGER->play("dashAttack", 1);
				_player.state = PLAYER_DASH_ATTACK;
				_startX = _player.x;
				_startY = _player.y;
				_dashAttack->dashAttacking(&_player.x, &_player.y, &_startX, &_startY, _player.direction, _player.speed);
			}
			if (_isJump && !(_player.speed == 5))
			{
				SOUNDMANAGER->play("dashJumpAttack", 1);
				_player.state = PLAYER_JUMP_ATTACK;
				_jump->jumping(&_player.x, &_player.y, &_startX, &_startY, _player.jumpPower, _player.gravity);
			}
			else if (_isJump && _player.speed == 5)
			{
				SOUNDMANAGER->play("dashJumpAttack", 1);
				_player.state = PLAYER_DASH_JUMP_ATTACK;
				_jump->jumping(&_player.x, &_player.y, &_startX, &_startY, _player.jumpPower, _player.gravity);
			}
		}
		if (KEYMANAGER->isOnceKeyUp('C'))
		{

			if (_attackComboKey < 2) _attackComboKey++;
		}
	}

	if (KEYMANAGER->isOnceKeyDown('X'))
	{
		SOUNDMANAGER->play("playerJump", 1);
		if (GetTickCount() - _oldJumpTime >= 1 * 900)
		{
			if (!_isAttack)
			{
				_startX = _player.x;
				_startY = _player.y;
				_jump->jumping(&_player.x, &_player.y, &_startX, &_startY, _player.jumpPower, _player.gravity);
				_player.state = PLAYER_JUMP;
				_oldJumpTime = GetTickCount();
			}
		}
	}
	DoubleKeyIntVoid();
}
// 키 눌렀을때
void player::keyDownInput(PLAYERDIRECTION direction)
{

	
	if (!(_player.state == PLAYER_JUMP || _player.state == PLAYER_DASH_ATTACK || _player.state == PLAYER_JUMP_ATTACK || _player.state == PLAYER_DASH_JUMP_ATTACK))
	{
		if (!_isAttack)
		{
			if (!(_player.state == PLAYER_JUMP_ATTACK) && !(_player.state == PLAYER_COMBINATION))
			{
				_player.direction = direction;
			}
		}
		if (!(_player.state == PLAYER_DASH_ATTACK))
		{
			if (!_isRun)
			{
				_player.speed = 3;
				_player.state = PLAYER_WALK;
			}
			if (_doubleKey[direction] == 1)
			{
				_player.state = PLAYER_RUN;
				_player.speed = 5;
			}
		}
	}

}
// 키 뗄때
void player::keyUpInput(PLAYERDIRECTION direction)
{
	if (!(_player.state == PLAYER_JUMP))
	{
		if ((!(_player.state == PLAYER_RUN) || _player.direction == direction))
		{
			if (!(_player.state == PLAYER_JUMP_ATTACK || _player.state == PLAYER_DASH_JUMP_ATTACK))
			{
				_player.state = PLAYER_IDLE;
				_player.speed = 3;
			}
		}

		if (_doubleKey[direction] == 0) _doubleKey[direction]++;
		_curTime[direction] = GetTickCount();
		_oldTime[direction] = _curTime[direction];
	}
	else
	{
		_isJump = false;
		_player.speed = 0;
	}
}

// 키 연속으로 눌렀을때 예외처리 모음
void player::DoubleKeyIntVoid()
{
	//예외처리 key - LEFT->UP->LEFT 뛰는거 방지
	if (_doubleKey[LEFT] == 1 && (_player.state == PLAYER_WALK && !(_player.direction == LEFT)))
	{
		_doubleKey[LEFT]--;
	}
	else if (_doubleKey[RIGHT] == 1 && (_player.state == PLAYER_WALK && !(_player.direction == RIGHT)))
	{
		_doubleKey[RIGHT]--;
	}
	else if (_doubleKey[UP] == 1 && (_player.state == PLAYER_WALK && !(_player.direction == UP)))
	{
		_doubleKey[UP]--;
	}
	else if (_doubleKey[DOWN] == 1 && (_player.state == PLAYER_WALK && !(_player.direction == DOWN)))
	{
		_doubleKey[DOWN]--;
	}

	for (int i = 0; i < 4; i++) _curTime[i] = GetTickCount();

	//doubleKey를 0.2초가 지나면 0으로 초기화하게 
	if ((_curTime[LEFT] - _oldTime[LEFT] >= 1 * 150 && _doubleKey[LEFT] == 1) && !(_player.state == PLAYER_RUN && _player.direction == LEFT))
	{
		_doubleKey[LEFT]--;
		_oldTime[LEFT] = GetTickCount();
	}
	else if ((_curTime[RIGHT] - _oldTime[RIGHT] >= 1 * 150 && _doubleKey[RIGHT] == 1) && !(_player.state == PLAYER_RUN))
	{
		_doubleKey[RIGHT]--;
		_oldTime[RIGHT] = GetTickCount();
	}
	else if ((_curTime[UP] - _oldTime[UP] >= 1 * 150 && _doubleKey[UP] == 1) && !(_player.state == PLAYER_RUN))
	{
		_doubleKey[UP]--;
		_oldTime[UP] = GetTickCount();
	}
	else if ((_curTime[DOWN] - _oldTime[DOWN] >= 1 * 150 && _doubleKey[DOWN] == 1) && !(_player.state == PLAYER_RUN))
	{
		_doubleKey[DOWN]--;
		_oldTime[DOWN] = GetTickCount();
	}

	else if (GetTickCount() - _oldAttackTime >= 1 * 400 && _attackComboKey > 0) //더블키 공격 
	{
		_attackComboKey--;
		_oldAttackTime = GetTickCount();
	}
}
//중복되는 코드 줄일라고 만든 함수

void player::callBackAttack(void * obj)
{
	player* playerAttack = (player*)obj;

	playerAttack->setPlayerState(PLAYER_IDLE);
}

void player::callBackDashAttack(void * obj)
{
	player* playerAttack = (player*)obj;

	if (playerAttack->getPlayerState() == PLAYER_IDLE)  playerAttack->setPlayerState(PLAYER_IDLE);
	else if (playerAttack->getPlayerSpeed() == 5)  playerAttack->setPlayerState(PLAYER_RUN);
}

void player::callBackJump(void * obj)
{
	player* playerAttack = (player*)obj;

	if (playerAttack->getPlayerJump())
	{
		playerAttack->setPlayerState(PLAYER_IDLE);
	}

	if (playerAttack->getPlayerSpeed() == 0)
	{
		playerAttack->setPlayerState(PLAYER_IDLE);
	}
	else if (playerAttack->getPlayerSpeed() == 5)
	{
		playerAttack->setPlayerState(PLAYER_RUN);
	}

}

void player::callBackFall(void * obj)
{
	player* playerAttack = (player*)obj;

	playerAttack->setPlayerState(PLAYER_IDLE);
	playerAttack->setPlayerDirection(DOWN);

}

void player::callBackDashJumpAttack(void * obj)
{
	player* playerAttack = (player*)obj;

	if (playerAttack->getPlayerSpeed() == 5)
	{
		playerAttack->setPlayerState(PLAYER_RUN);
	}
	else playerAttack->setPlayerState(PLAYER_IDLE);
}

void player::tileCheck()
{
	RECT rcCollision;
	RECT rc;

	rcCollision = _player.rc;

	//rcCollision.left += 4;
	//rcCollision.top += 4;
	//rcCollision.right -= 4;
	//rcCollision.bottom -= 4;

	TileX = _player.x / TileSIZE;
	TileY = _player.y / TileSIZE;


	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _player.direction != LEFT || _player.direction != RIGHT)
	{
		tileIndex[0].x = TileX - 1;
		tileIndex[0].y = TileY;


		if (IntersectRect(&rc, &_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))
		{
			if (_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WALL || _dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_FIRE)
			{
				_player.x += _player.speed;

				check = true;
			}
		}
		if (!(IntersectRect(&rc, &_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision)))
		{
			check = false;
		}
	}


	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _player.direction != LEFT || _player.direction != RIGHT)
	{
		tileIndex[0].x = TileX + 1;
		tileIndex[0].y = TileY;

		if (IntersectRect(&rc, &_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))
		{
			if (_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WALL || _dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_FIRE)
			{
				_player.x -= _player.speed;

				check = true;
			}

		}
		else if (!(IntersectRect(&rc, &_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision)))
		{
			check = false;
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP) && _player.direction != UP || _player.direction != DOWN)
	{
		tileIndex[0].x = TileX;
		tileIndex[0].y = TileY - 1;

		tileIndex[1].x = TileX + 1;
		tileIndex[1].y = TileY;

		tileIndex[2].x = TileX - 1;
		tileIndex[2].y = TileY;

		if (IntersectRect(&rc, &_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))
		{
			if (_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WALL || _dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_FIRE)
			{
				_player.y += _player.speed;

				check = true;
			}

		}
		else if (!(IntersectRect(&rc, &_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision)))
		{
			check = false;
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_DOWN) && _player.direction != UP || _player.direction != DOWN)
	{
		tileIndex[0].x = TileX;
		tileIndex[0].y = TileY + 1;

		tileIndex[1].x = TileX + 1;
		tileIndex[1].y = TileY;

		tileIndex[2].x = TileX - 1;
		tileIndex[2].y = TileY;


		if (IntersectRect(&rc, &_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))
		{
			if (_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WALL || _dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_FIRE)
			{
				_player.y -= _player.speed;

				check = true;
			}

		}
		else if (!(IntersectRect(&rc, &_dungeon->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision)))
		{
			check = false;
		}
	}
}

void player::townCheck()
{
	RECT rcCollision;
	RECT rc;

	rcCollision = _player.rc;

	//rcCollision.left += 4;
	//rcCollision.top += 4;
	//rcCollision.right -= 4;
	//rcCollision.bottom -= 4;

	TileX = _player.x / TileSIZE;
	TileY = _player.y / TileSIZE;


	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _player.direction != LEFT || _player.direction != RIGHT)
	{
		tileIndex[0].x = TileX - 1;
		tileIndex[0].y = TileY;

		if (IntersectRect(&rc, &_town->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))
		{
			if (_town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WALL || _town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WATER
				|| _town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_SHIRT)
			{
				_player.x += _player.speed;
				check = true;
			}
		}
		else if (check == true || (!(IntersectRect(&rc, &_town->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))))
		{
			check = false;
		}


	}

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _player.direction != LEFT || _player.direction != RIGHT)
	{
		tileIndex[0].x = TileX + 1;
		tileIndex[0].y = TileY;

		if (IntersectRect(&rc, &_town->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))
		{
			if (_town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WALL || _town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WATER
				|| _town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_SHIRT)
			{
				_player.x -= _player.speed;
				check = true;
			}
		}
		else if (check == true || (!(IntersectRect(&rc, &_town->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))))
		{
			check = false;
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP) && _player.direction != UP || _player.direction != DOWN)
	{
		tileIndex[0].x = TileX;
		tileIndex[0].y = TileY - 1;

		if (IntersectRect(&rc, &_town->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))
		{
			if (_town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WALL || _town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WATER
				|| _town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_SHIRT)
			{
				_player.y += _player.speed;
				check = true;
			}
		}
		else if (check == true || (!(IntersectRect(&rc, &_town->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))))
		{
			check = false;
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_DOWN) && _player.direction != UP || _player.direction != DOWN)
	{
		tileIndex[0].x = TileX;
		tileIndex[0].y = TileY + 1;

		if (IntersectRect(&rc, &_town->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))
		{
			if (_town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WALL || _town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_WATER
				|| _town->getTile(tileIndex[0].x, tileIndex[0].y)->obj == OBJ_SHIRT)
			{
				_player.y -= _player.speed;
				check = true;
			}
		}
		else if (check == true || (!(IntersectRect(&rc, &_town->getTile(tileIndex[0].x, tileIndex[0].y)->rc, &rcCollision))))
		{
			check = false;
		}
	}
}

void player::npcCheck()
{
	RECT rc;
	for (int i = 0; i < _npcMgr->getVBird().size(); ++i)
	{
		if (IntersectRect(&rc, &_player.rc, &_npcMgr->getVBird()[i]->getRect()))
		{
			// 대화 //
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setBird(true);
			}
			if (_player.direction == UP)
			{
				_player.y += _player.speed;
			}
			else if (_player.direction == DOWN)
			{
				_player.y -= _player.speed;
			}
			else if (_player.direction == LEFT)
			{
				_player.x += _player.speed;
			}
			else if (_player.direction == RIGHT)
			{
				_player.x -= _player.speed;
			}
		}
		// 대화가 진행중일때 //
		else if (_message->getBird() == true)
		{

			//한번더 스페이스를 누르면 다 꺼진닷//
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setBird(false);
				_message->setOn(true);
				_message->setPage(false);
			}
		}
	}

	for (int i = 0; i < _npcMgr->getVElder().size(); ++i)
	{
		if (IntersectRect(&rc, &_player.rc, &_npcMgr->getVElder()[i]->getRect()))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setElder(true);
			}

			if (_player.direction == UP)
			{
				_player.y += _player.speed;
			}
			else if (_player.direction == DOWN)
			{
				_player.y -= _player.speed;
			}
			else if (_player.direction == LEFT)
			{
				_player.x += _player.speed;
			}
			else if (_player.direction == RIGHT)
			{
				_player.x -= _player.speed;
			}
		}
		else if (_message->getElder() == true)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setElder(false);
				_message->setOn(true);
				_message->setPage(false);
			}
		}
	}

	for (int i = 0; i < _npcMgr->getVElle().size(); ++i)
	{
		if (IntersectRect(&rc, &_player.rc, &_npcMgr->getVElle()[i]->getRect()))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setElle(true);

			}
			if (_player.direction == UP)
			{
				_player.y += _player.speed;
			}
			else if (_player.direction == DOWN)
			{
				_player.y -= _player.speed;
			}
			else if (_player.direction == LEFT)
			{
				_player.x += _player.speed;
			}
			else if (_player.direction == RIGHT)
			{
				_player.x -= _player.speed;
			}
		}
		else if (_message->getElle() == true)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setElle(false);
				_message->setOn(true);
				_message->setPage(false);
			}
		}
	}



	for (int i = 0; i < _npcMgr->getVTownHuman().size(); ++i)
	{
		if (IntersectRect(&rc, &_player.rc, &_npcMgr->getVTownHuman()[i]->getRect()))
		{
			if (_player.direction == UP)
			{
				_player.y += _player.speed;
			}
			else if (_player.direction == DOWN)
			{
				_player.y -= _player.speed;
			}
			else if (_player.direction == LEFT)
			{
				_player.x += _player.speed;
			}
			else if (_player.direction == RIGHT)
			{
				_player.x -= _player.speed;
			}
		}

		if (IntersectRect(&rc, &_player.rc, &_npcMgr->getVTownHuman()[0]->getRect()))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setFishMan(true);
			}
		}

		else if (_message->getFishMan() == true)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setFishMan(false);
				_message->setOn(true);
				_message->setPage(false);
			}
		}
		if (IntersectRect(&rc, &_player.rc, &_npcMgr->getVTownHuman()[1]->getRect()))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setGrandFa(true);
			}
		}
		else if (_message->getGrandFa() == true)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setGrandFa(false);
				_message->setOn(true);
				_message->setPage(false);
			}
		}

		if (IntersectRect(&rc, &_player.rc, &_npcMgr->getVTownHuman()[2]->getRect()))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setGrandMa(true);
			}
		}
		else if (_message->getGrandMa() == true)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setGrandMa(false);
				_message->setOn(true);
				_message->setPage(false);
			}
		}
		if (IntersectRect(&rc, &_player.rc, &_npcMgr->getVTownHuman()[3]->getRect()))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setMaid(true);
			}
		}
		else if (_message->getMaid() == true)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setMaid(false);
				_message->setOn(true);
				_message->setPage(false);
			}
		}

		if (IntersectRect(&rc, &_player.rc, &_npcMgr->getVTownHuman()[4]->getRect()))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setHuman(true);
			}
		}
		else if (_message->getHuman() == true)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_message->setHuman(false);
				_message->setOn(true);
				_message->setPage(false);
			}
		}
	}
}

void player::enemyCollision(bool enemyCheck)
{
	if (enemyCheck == true)
	{
		if (_playerProtect)
		{
			if (GetTickCount() - _alphaChangeTime >= 200) _player.alphaRender = 200;
			{
				if (!_alphaChange) _alphaChange = true;
				else _alphaChange = false;
				_alphaChangeTime = GetTickCount();
			}
			if (!_alphaChange) 	_player.alphaRender = 150;
			else 	_player.alphaRender = 200;

			if (GetTickCount() - _playerProtectTime >= 1 * 1300)
			{
				_player.alphaRender = 255;
				_playerProtect = false;
			}
			else if (GetTickCount() - _playerProtectTime >= 1 * 600)
			{
				_player.state = PLAYER_IDLE;
			}

			if (_player.state == PLAYER_ENEMY_ATTACK)
			{
				switch (_player.direction)
				{
				case LEFT:
					_player.x += 0.4;
					break;
				case RIGHT:
					_player.x -= 0.4;
					break;
				case UP:
					_player.y += 0.4;
					break;
				case DOWN:
					_player.y -= 0.4;
					break;
				}
			}

		}
		else
		{
			RECT rc;
			if (!(_player.state == PLAYER_JUMP_ATTACK || _player.state == PLAYER_DASH_ATTACK || _player.state == PLAYER_DASH_JUMP_ATTACK))
			{
				for (int i = 0; i < _enemyManager->getVBallMonster().size(); i++)
				{
					if (IntersectRect(&rc, &_player.rc, &_enemyManager->getVBallMonster()[i]->getRect()))
					{
						if (_playerProtect == false)
						{
							_player.HP -= 4 - _player.def;
							if (_player.HP <= 0) _player.HP = 0;
						}
						_playerProtect = true;
						_alphaChangeTime = GetTickCount();
						_playerProtectTime = GetTickCount();
						_player.state = PLAYER_ENEMY_ATTACK;

					}
				}
				for (int i = 0; i < _enemyManager->getVFireMonster().size(); i++)
				{
					if (IntersectRect(&rc, &_player.rc, &_enemyManager->getVFireMonster()[i]->getRect()))
					{
						if (_playerProtect == false)
						{
							_player.HP -= 3 - _player.def;
							if (_player.HP <= 0) _player.HP = 0;
						}
						_playerProtect = true;
						_alphaChangeTime = GetTickCount();
						_playerProtectTime = GetTickCount();
						_player.state = PLAYER_ENEMY_ATTACK;
					}
				}
				for (int i = 0; i < _enemyManager->getFireMonsterBullet()->getVFireBullet().size(); i++)
				{
					if (IntersectRect(&rc, &_player.rc, &_enemyManager->getFireMonsterBullet()->getVFireBullet()[i].rc))
					{
						if (_playerProtect == false)
						{
							_player.HP -= 4 - _player.def;
							if (_player.HP <= 0) _player.HP = 0;
						}
						_playerProtect = true;
						_alphaChangeTime = GetTickCount();
						_playerProtectTime = GetTickCount();
						_player.state = PLAYER_ENEMY_ATTACK;
					}
				}
				for (int i = 0; i < _enemyManager->getVKnightMonster().size(); i++)
				{
					if (IntersectRect(&rc, &_player.rc, &_enemyManager->getVKnightMonster()[i]->getRect()) ||
						IntersectRect(&rc, &_player.rc, &_enemyManager->getVKnightMonster()[i]->getAttackRect()))
					{
						if (_playerProtect == false)
						{
							_player.HP -= 6 - _player.def;
							if (_player.HP <= 0) _player.HP = 0;
						}
						_playerProtect = true;
						_alphaChangeTime = GetTickCount();
						_playerProtectTime = GetTickCount();
						_player.state = PLAYER_ENEMY_ATTACK;
					}
				}


				//보스 층이라면
				if (_dungeon->getDungeonFloor() == DUNGEON_FLOOR::BOSS_FLOOR)
				{
					for (int i = 0; i < _enemyManager->getGoldBullet()->getVBossBullet().size(); i++)
					{
						if (IntersectRect(&rc, &_player.rc, &_enemyManager->getGoldBullet()->getVBossBullet()[i].rc))
						{
							if (_playerProtect == false)
							{
								_player.HP -= 5 - _player.def;
								if(_player.HP <= 0) _player.HP = 0;
							}

							_playerProtect = true;
							_alphaChangeTime = GetTickCount();
							_playerProtectTime = GetTickCount();
							_player.state = PLAYER_ENEMY_ATTACK;
						}
					}
					for (int i = 0; i < _enemyManager->getBlueBullet()->getVBossBullet().size(); i++)
					{
						if (IntersectRect(&rc, &_player.rc, &_enemyManager->getBlueBullet()->getVBossBullet()[i].rc))
						{
							if (_playerProtect == false)
							{
								_player.HP -= 5 - _player.def;
								if (_player.HP <= 0) _player.HP = 0;
							}

							_playerProtect = true;
							_alphaChangeTime = GetTickCount();
							_playerProtectTime = GetTickCount();
							_player.state = PLAYER_ENEMY_ATTACK;
						}
					}
					for (int i = 0; i < _enemyManager->getRedBullet()->getVBossBullet().size(); i++)
					{
						if (IntersectRect(&rc, &_player.rc, &_enemyManager->getRedBullet()->getVBossBullet()[i].rc))
						{
							if (_playerProtect == false)
							{
								_player.HP -= 5 - _player.def;
								if (_player.HP <= 0) _player.HP = 0;
							}

							_playerProtect = true;
							_alphaChangeTime = GetTickCount();
							_playerProtectTime = GetTickCount();
							_player.state = PLAYER_ENEMY_ATTACK;
						}
					}
					for (int i = 0; i < _enemyManager->getRgbBullet()->getVBossBullet().size(); i++)
					{
						if (IntersectRect(&rc, &_player.rc, &_enemyManager->getRgbBullet()->getVBossBullet()[i].rc))
						{
							if (_playerProtect == false)
							{
								_player.HP -= 5 - _player.def;
								if (_player.HP <= 0) _player.HP = 0;
							}

							_playerProtect = true;
							_alphaChangeTime = GetTickCount();
							_playerProtectTime = GetTickCount();
							_player.state = PLAYER_ENEMY_ATTACK;
						}
					}

					if (IntersectRect(&rc, &_player.rc, &_enemyManager->getShockAttack()->rc)
						|| IntersectRect(&rc, &_player.rc, &_enemyManager->getShockAttack()->rc2))
					{
						if (_playerProtect == false)
						{
							_player.HP -= 5 - _player.def;
							if(_player.HP <= 0) _player.HP = 0;
						}

						_playerProtect = true;
						_alphaChangeTime = GetTickCount();
						_playerProtectTime = GetTickCount();
						_player.state = PLAYER_ENEMY_ATTACK;
					}
					
				}

			}
		}
	}
}

//플레이어가 피가 0이 되서 죽을때
void player::playerDeath(bool enemyCheck)
{
	if (_player.HP == 0)
	{
		_death = true;
		_player.state = PLAYER_DEATH;
	}
}

void player::levelUP()
{
	if (_player.exp >= _player.maxExp)
	{
		SOUNDMANAGER->play("levelUP", 1);
		_player.level += 1;
		_player.damage += 5;
		_player.def += 1;
		_player.HP += 5;
		_player.maxHP += 5;
		_player.maxExp += 10;
		_player.exp = 0;
		_player.money = _player.money;
		_levelUP = true;
		_levelOldTime = GetTickCount();
		playerSave();

	}

	if (_levelUP)
	{
		_player.state = PLAYER_LEVELUP;
	}

	if (GetTickCount() - _levelOldTime >= 3 * 1000)
	{
		_levelUP = false;
	}
}

void player::playerSave()
{
	char temp[128];

	vector<string> vStr;

	vStr.push_back(itoa(_player.HP, temp, 10));
	vStr.push_back(itoa(_player.maxHP, temp, 10));
	vStr.push_back(itoa(_player.damage, temp, 10));
	vStr.push_back(itoa(_player.def, temp, 10));
	vStr.push_back(itoa(_player.exp, temp, 10));
	vStr.push_back(itoa(_player.maxExp, temp, 10));
	vStr.push_back(itoa(_player.money, temp, 10));
	vStr.push_back(itoa(_player.level, temp, 10));

	_txtData->txtSave("saveFile/플레이어.txt", vStr);

}

void player::playerLoad()
{
	vector<string> vStr;
	vStr = _txtData->txtLoad("saveFile/플레이어.txt");

	if (vStr.size() > 5)
	{
		_player.HP = (atoi(vStr[0].c_str()));
		_player.maxHP = (atoi(vStr[1].c_str()));
		_player.damage = (atoi(vStr[2].c_str()));
		_player.def = (atoi(vStr[3].c_str()));
		_player.exp = (atoi(vStr[4].c_str()));
		_player.maxExp = (atoi(vStr[5].c_str()));
		_player.money = (atoi(vStr[6].c_str()));
		_player.level = (atoi(vStr[7].c_str()));
	}
}

void player::playerDungeonFall(RECT fallRc, RECT unMoveRc)
{
	RECT rc;
	if (IntersectRect(&rc, &_player.rc, &fallRc) && _player.rc.bottom >= fallRc.top )
	{
		_player.state = PLAYER_FALL;

	}
	if (_player.state == PLAYER_FALL && !(KEYANIMANAGER->findAnimation("ark", "fall")->getFramePosArrOnce() == 332))
	{
		_player.y += 2.8f;
	}

	if (IntersectRect(&rc, &_player.rc, &unMoveRc) && !(_player.state == PLAYER_FALL))
	{
		_player.y += _player.speed;
	}
}

void player::playerUiSave()
{
	HANDLE file;
	DWORD save;

	file = CreateFile("saveFile/UiSave.txt", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	float x, y;
	int scene;

	x = _player.x;
	y = _player.y;
	scene = (int)_player.currentScene;

	WriteFile(file, &x, sizeof(float), &save, NULL);
	WriteFile(file, &y, sizeof(float), &save, NULL);
	WriteFile(file, &scene, sizeof(int), &save, NULL);

	CloseHandle(file);
}

void player::playerSceneSave()
{
	HANDLE file;
	DWORD save;

	file = CreateFile("saveFile/playerScene.txt", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	int scene = (int)_player.currentScene;

	WriteFile(file, &scene, sizeof(int), &save, NULL);

	CloseHandle(file);
}

void player::playerUiLoad()
{
	HANDLE file;
	DWORD load;

	file = CreateFile("saveFile/UiSave.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	float x, y;
	int scene;

	ReadFile(file, &x, sizeof(float), &load, NULL);
	ReadFile(file, &y, sizeof(float), &load, NULL);
	ReadFile(file, &scene, sizeof(int), &load, NULL);

	_player.x = x;
	_player.y = y;
	_player.currentScene = (PLAYERSCENE)scene;

	CloseHandle(file);
}



