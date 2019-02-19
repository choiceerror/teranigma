#include "stdafx.h"
#include "fireMonsterBullet.h"


fireMonsterBullet::fireMonsterBullet()
{
}


fireMonsterBullet::~fireMonsterBullet()
{
}

HRESULT fireMonsterBullet::init(const char * imageName, float range, int fireBulletMax)
{
	_imageName = imageName;
	_range = range;
	_fireBulletMax = fireBulletMax;
	int fire[] = { 10, 11, 12, 13 };
	KEYANIMANAGER->addArrayFrameAnimation("fireBullet", "fire", _imageName, fire, 4, 5, true);

	return S_OK;
}

void fireMonsterBullet::release()
{
}

void fireMonsterBullet::update()
{
	move();
}

void fireMonsterBullet::render(float cameraX, float cameraY)
{
	for (_viFireBullet = _vFireBullet.begin(); _viFireBullet != _vFireBullet.end(); _viFireBullet++)
	{
		//ī�޶� ��ǥ������ ������ǥ ���
		_viFireBullet->viewX = _viFireBullet->x - cameraX;
		_viFireBullet->viewY = _viFireBullet->y - cameraY;
		_viFireBullet->image->expandAniRenderCenter(getMemDC(), _viFireBullet->viewX, _viFireBullet->viewY, _viFireBullet->ani, 1.5f, 1.5f);
		//Rectangle(getMemDC(), _viFireBullet->rc);
	}
}

void fireMonsterBullet::fire(float x, float y, float angle, float speed)
{
	if (_fireBulletMax < _vFireBullet.size()) return;

	tagFireBullet fireBullet;
	fireBullet.image = IMAGEMANAGER->findImage(_imageName);
	fireBullet.ani = KEYANIMANAGER->findAnimation("fireBullet", "fire");
	fireBullet.x = fireBullet.fireX = fireBullet.viewX = x;
	fireBullet.y = fireBullet.fireY = fireBullet.viewY = y;
	fireBullet.angle = angle;
	fireBullet.speed = speed;
	fireBullet.radius = fireBullet.image->getFrameWidth() / 2;
	fireBullet.rc = RectMakeCenter(fireBullet.x, fireBullet.y, fireBullet.image->getFrameWidth(), fireBullet.image->getFrameHeight());
	fireBullet.isCollision = false;

	_vFireBullet.push_back(fireBullet);
}

void fireMonsterBullet::move()
{
	for (_viFireBullet = _vFireBullet.begin(); _viFireBullet != _vFireBullet.end();)
	{
		//�Ѿ� �̵�
		_viFireBullet->x += cosf(_viFireBullet->angle) * _viFireBullet->speed;
		_viFireBullet->y += -sinf(_viFireBullet->angle) * _viFireBullet->speed;

		_viFireBullet->rc = RectMakeCenter(_viFireBullet->x, _viFireBullet->y, _viFireBullet->image->getFrameWidth() / 2, _viFireBullet->image->getFrameHeight() / 4);
		_viFireBullet->ani->start();

		if ((_range < getDistance(_viFireBullet->x, _viFireBullet->y, _viFireBullet->fireX, _viFireBullet->fireY)) || (_viFireBullet->isCollision == true))
		{
			_viFireBullet = _vFireBullet.erase(_viFireBullet);
		}
		else _viFireBullet++;
	}
}

HRESULT bossBullet::init(const char * imageName, float range, int bulletMax)
{
	_imageName = imageName;
	_range = range;
	_bulletMax = bulletMax;
	int goldBullet[] = { 0, 1, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("goldBullet", "gold", _imageName, goldBullet, 4, 7, true);
	int blueBullet[] = { 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("blueBullet", "blue", _imageName, blueBullet, 4, 7, true);
	int redBullet[] = { 8, 9, 10, 11 };
	KEYANIMANAGER->addArrayFrameAnimation("redBullet", "red", _imageName, redBullet, 4, 7, true);

	_tempAni = KEYANIMANAGER->findAnimation("goldBullet", "gold");
	return S_OK;
}

void bossBullet::release()
{
}

void bossBullet::update()
{
	move();
}

void bossBullet::render(float cameraX, float cameraY)
{
	for (_viBossBullet = _vBossBullet.begin(); _viBossBullet != _vBossBullet.end(); _viBossBullet++)
	{
		//ī�޶� ��ǥ������ ������ǥ ���
		_viBossBullet->viewX = _viBossBullet->x - cameraX;
		_viBossBullet->viewY = _viBossBullet->y - cameraY;
		_viBossBullet->image->expandAniRenderCenter(getMemDC(), _viBossBullet->viewX, _viBossBullet->viewY, _viBossBullet->ani, _viBossBullet->sizeX, _viBossBullet->sizeY);
		//Rectangle(getMemDC(), _viFireBullet->rc);
	}

}

void bossBullet::bossFire(float x, float y, float angle, float speed, int bulletMax)
{

	tagFireBullet bossBullet;
	bossBullet.max = bulletMax;
	if (bossBullet.max < _vBossBullet.size()) return;
	bossBullet.image = IMAGEMANAGER->findImage(_imageName);
	bossBullet.ani = _tempAni;
	bossBullet.x = bossBullet.fireX = bossBullet.viewX = x;
	bossBullet.y = bossBullet.fireY = bossBullet.viewY = y;
	bossBullet.angle = angle;
	bossBullet.speed = speed;
	bossBullet.radius = bossBullet.image->getFrameWidth() / 2;
	bossBullet.rc = RectMakeCenter(bossBullet.x, bossBullet.y, bossBullet.image->getFrameWidth(), bossBullet.image->getFrameHeight());
	bossBullet.isCollision = false;
	bossBullet.sizeX = bossBullet.sizeY = 2.5f;
	bossBullet.worldTime = TIMEMANAGER->getWorldTime();

	_vBossBullet.push_back(bossBullet);
}

void bossBullet::move()
{
	for (_viBossBullet = _vBossBullet.begin(); _viBossBullet != _vBossBullet.end();)
	{
		//�Ѿ� �̵�
		_viBossBullet->x += cosf(_viBossBullet->angle) * _viBossBullet->speed;
		_viBossBullet->y += -sinf(_viBossBullet->angle) * _viBossBullet->speed;

		_viBossBullet->rc = RectMakeCenter(_viBossBullet->x, _viBossBullet->y, _viBossBullet->image->getFrameWidth() / 2, _viBossBullet->image->getFrameHeight() / 4);
		_viBossBullet->ani->start();

		if ((_range < getDistance(_viBossBullet->x, _viBossBullet->y, _viBossBullet->fireX, _viBossBullet->fireY)) || (_viBossBullet->isCollision == true)
			|| (4.0f + _viBossBullet->worldTime <= TIMEMANAGER->getWorldTime()))
		{
			_viBossBullet->worldTime = TIMEMANAGER->getWorldTime();
			_viBossBullet = _vBossBullet.erase(_viBossBullet);
		}
		else _viBossBullet++;
	}

}
