#include "stdafx.h"
#include "worldMap.h"


worldMap::worldMap()
{
}


worldMap::~worldMap()
{
}

HRESULT worldMap::init()
{

	load();

	return S_OK;
}

void worldMap::release()
{
}

void worldMap::update()
{
}

void worldMap::render()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			if (_vvMap[i][j]->a == 0)
			{
				IMAGEMANAGER->frameRender("Ÿ�ϸ�4", getMemDC(),
					_vvMap[i][j]->rc.left, _vvMap[i][j]->rc.top,
					_vvMap[i][j]->FrameX, _vvMap[i][j]->FrameY);
			}
			else if (_vvMap[i][j]->a == 1)
			{
				IMAGEMANAGER->frameRender("Ÿ�ϸ�", getMemDC(),
					_vvMap[i][j]->rc.left, _vvMap[i][j]->rc.top,
					_vvMap[i][j]->FrameX, _vvMap[i][j]->FrameY);
			}
			else if (_vvMap[i][j]->a == 2)
			{
				IMAGEMANAGER->frameRender("Ÿ�ϸ�2", getMemDC(),
					_vvMap[i][j]->rc.left, _vvMap[i][j]->rc.top,
					_vvMap[i][j]->FrameX, _vvMap[i][j]->FrameY);
			}
		}
	}

	////������Ʈ
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			if (_vvMap[i][j]->obj == OBJ_NONE) continue;

			if (_vvMap[i][j]->a == 0)
			{
				IMAGEMANAGER->frameRender("Ÿ�ϸ�4", getMemDC(),
					_vvMap[i][j]->rc.left, _vvMap[i][j]->rc.top,
					_vvMap[i][j]->objFrameX, _vvMap[i][j]->objFrameY);
			}
			else if (_vvMap[i][j]->a == 1)
			{
				IMAGEMANAGER->frameRender("Ÿ�ϸ�", getMemDC(),
					_vvMap[i][j]->rc.left, _vvMap[i][j]->rc.top,
					_vvMap[i][j]->objFrameX, _vvMap[i][j]->objFrameY);
			}
			else if (_vvMap[i][j]->a == 2)
			{
				IMAGEMANAGER->frameRender("Ÿ�ϸ�2", getMemDC(),
					_vvMap[i][j]->rc.left, _vvMap[i][j]->rc.top,
					_vvMap[i][j]->objFrameX, _vvMap[i][j]->objFrameY);
			}
		}
	}



}

void worldMap::load()
{

	HANDLE file2;
	DWORD read2;
	char mapSize[128];

	file2 = CreateFile("saveFile\\mapSize.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file2, mapSize, 128, &read2, NULL);
	CloseHandle(file2);

	string sizeX, sizeY;
	bool x = true;
	for (int i = 0; i < strlen(mapSize); i++)
	{
		if (mapSize[i] == ',')
		{
			x = false;
			continue;
		}
		if (mapSize[i] == NULL) break;
		if (x)
		{
			sizeX += mapSize[i];
		}
		else
		{
			sizeY += mapSize[i];
		}
	}


	TILEX = stoi(sizeX);
	TILEY = stoi(sizeY);
	_vvMap.resize(TILEY);


	for (int i = 0; i < TILEY; i++)
	{
		_vvMap[i].resize(TILEX);
	}

	tagTile* _tiles = new tagTile[TILEX * TILEY];
	HANDLE file;
	DWORD read;

	file = CreateFile("saveFile\\mapSave.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);


	CloseHandle(file);
	_attribute = new DWORD[TILEX * TILEY];

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			_attribute[j + i * TILEX] = NULL;
			_vvMap[i][j] = &_tiles[j + i * TILEX];

			if (_vvMap[i][j]->obj == OBJ_WALL)
			{
				_attribute[j + i * TILEX] |= ATTR_UNMOVE;
			}
		}
	}
}
