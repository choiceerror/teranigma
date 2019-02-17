#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"
#include "dungeonMap.h"
#include "townMap.h"


void player::enemyDeathExpGet()
{
	for (int i = 0; i < _enemyManager->getVBallMonster().size(); i++)
	{
		if (_enemyManager->getVBallMonster()[i]->getCurrentHP() == 0)
		{
			_player.exp += 3;
			if (_player.exp >= _player.maxExp)
			{
				_player.exp = _player.maxExp;
			}
		}
	}
	for (int i = 0; i < _enemyManager->getVFireMonster().size(); i++)
	{
		if (_enemyManager->getVFireMonster()[i]->getCurrentHP() == 0)
		{
			_player.exp += 3;
			if (_player.exp >= _player.maxExp)
			{
				_player.exp = _player.maxExp;
			}
		}
	}
	for (int i = 0; i < _enemyManager->getVKnightMonster().size(); i++)
	{
		if (_enemyManager->getVKnightMonster()[i]->getCurrentHP() == 0)
		{
			_player.exp += 4;
			if (_player.exp >= _player.maxExp)
			{
				_player.exp = _player.maxExp;
			}
		}
	}

}

void player::playerUIRender()
{
	IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), 100, 100, 3, 1, 1.3, 1.3);	//����
	IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), 250, 100, 2, 1, 1.3, 1.3); //������
	IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), 100, 150, 1, 0, 1.3, 1.3); //���� ����
	IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), GAMESIZEX - 150, 100, 1, 1, 1.3, 1.3);	//HP�۾�
	if (!((_player.HP / 10) % 10 == 0))
	{
		IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), GAMESIZEX - 210, 150, (_player.HP / 10) % 10, 0, 1.3, 1.3); //�����ڸ�
	}
	IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), GAMESIZEX - 180, 150, _player.HP % 10, 0, 1.3, 1.3); //�����ڸ�
	IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), GAMESIZEX - 150, 150, 4, 1, 1.3, 1.3); //¦���
	IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), GAMESIZEX - 120, 150, (_player.maxHP / 10) % 10, 0, 1.3, 1.3); //max �����ڸ�
	IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), GAMESIZEX - 90, 150, _player.maxHP % 10, 0, 1.3, 1.3); // "   �����ڸ�
	IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), GAMESIZEX - 200, GAMESIZEY - 100, 0, 1, 1.3, 1.3); //���̹���
	if (!((_player.money / 100) % 10 == 0))
	{
		IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), GAMESIZEX - 150, GAMESIZEY - 100, (_player.money / 100) % 10, 0, 1.3, 1.3); //�� ���� �ڸ�
	}
	if (!((_player.money / 10) % 10 == 0))
	{
		IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), GAMESIZEX - 120, GAMESIZEY - 100, (_player.money / 10) % 10, 0, 1.3, 1.3); //�� ���� �ڸ�
	}
	IMAGEMANAGER->findImage("player_ui")->expandRenderCenter(getMemDC(), GAMESIZEX - 90, GAMESIZEY - 100, _player.money % 10, 0, 1.3, 1.3); //�� ���� �ڸ�
}
