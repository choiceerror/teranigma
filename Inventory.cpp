#include "stdafx.h"
#include "Inventory.h"


Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

HRESULT Inventory::init()
{
	inventoryReset();

	_weaponCount = 0;
	_accessoryCount = 0;
	_armorCount = 0;

	//����׿�
	//for (int i = 0; i < WEAPON_MAX_ARR; ++i)
	//{
	//	_weaponInv[i] = WEAPONTYPE::IRON_SPEAR;
	//}
	//for (int i = 0; i < ARMOR_MAX_ARR; ++i)
	//{
	//	_armorInv[i] = ARMORTYPE::IRON_ARMOR;
	//}
	//for (int i = 0; i < POTION_MAX_ARR; ++i)
	//{
	//	_potionInv[i] = POTIONTYPE::SMALL;
	//}
	//for (int i = 0; i < ACCESSORY_MAX_ARR; ++i)
	//{
	//	//_accessoryInv[i] = ACCESSORYTYPE::LEEF;
	//}


	//����
	//��
	//�Ǽ�����
	//����

	return S_OK;
}

void Inventory::release()
{
}

void Inventory::update()
{
	//������ �Ծ������ �̵�
	itemMove();

	//�κ��丮�� ��� ��
	itemCount();

	//����� �ʱ�ȭ
	inventoryReBooting();
}

void Inventory::render()
{
	//char str[120];
	//sprintf_s(str, "%d  %d  %d  %d", _weaponCount, _armorCount, _accessoryCount, _potionCount);
	//TextOut(getMemDC(), 300, 600, str, strlen(str));

}

void Inventory::inventoryReset()
{
	//�ʱ�ȭ �Լ�

	//����
	for (int i = 0; i < 16; ++i)
	{
		_weaponInv[i] = WEAPONTYPE::NONE;
	}

	//��
	for (int i = 0; i < 16; ++i)
	{
		_armorInv[i] = ARMORTYPE::NONE;
	}

	//�Ǽ�����
	for (int i = 0; i < 32; ++i)
	{
		_accessoryInv[i] = ACCESSORYTYPE::NONE;
	}

	//����
	for (int i = 0; i < 32; ++i)
	{
		_potionInv[i] = POTIONTYPE::NONE;
	}

	_currentWeapon = WEAPONTYPE::NONE;
	_currentArmor = ARMORTYPE::NONE;
	_currentAccessory = ACCESSORYTYPE::NONE;

	_tempWeapon = WEAPONTYPE::NONE;
	_tempArmor = ARMORTYPE::NONE;
	_tempAccessory = ACCESSORYTYPE::NONE;
	_tempPorion = POTIONTYPE::NONE;

	



}

void Inventory::itemMove()
{
	//�������� �Ծ��ٸ� (temp�� NONE�� �ƴ� �ٸ��� �ִٸ�) ���� -> �κ��丮�� �̵� �� �ٽ� NONE����

	//����
	if (_tempWeapon != WEAPONTYPE::NONE)
	{
		for (int i = 0; i < WEAPON_MAX_ARR; ++i)
		{
			//�ƹ��͵� �ȴ���ִٸ�
			if (_weaponInv[i] == WEAPONTYPE::NONE)
			{
				_weaponInv[i] = _tempWeapon;
				break;
			}
			//�ٸ��������� ����ִٸ�
			else
			{
				continue;
			}
		}

		_tempWeapon = WEAPONTYPE::NONE;
	}

	//��
	if (_tempArmor != ARMORTYPE::NONE)
	{
		for (int i = 0; i < ARMOR_MAX_ARR; ++i)
		{
			//�ƹ��͵� �ȴ���ִٸ�
			if (_armorInv[i] == ARMORTYPE::NONE)
			{
				_armorInv[i] = _tempArmor;
				break;
			}
			//�ٸ��������� ����ִٸ�
			else
			{
				continue;
			}
		}

		_tempArmor = ARMORTYPE::NONE;
	}

	//�Ǽ�����
	if (_tempAccessory != ACCESSORYTYPE::NONE)
	{
		for (int i = 0; i < ACCESSORY_MAX_ARR; ++i)
		{
			//�ƹ��͵� �ȴ���ִٸ�
			if (_accessoryInv[i] == ACCESSORYTYPE::NONE && _potionInv[i] == POTIONTYPE::NONE)
			{
				_accessoryInv[i] = _tempAccessory;
				break;
			}
			//�ٸ��������� ����ִٸ�
			else
			{
				continue;
			}
		}

		_tempAccessory = ACCESSORYTYPE::NONE;
	}

	//����
	if (_tempPorion != POTIONTYPE::NONE)
	{
		for (int i = 0; i < POTION_MAX_ARR; ++i)
		{
			//�ƹ��͵� �ȴ���ִٸ�
			if (_potionInv[i] == POTIONTYPE::NONE && _accessoryInv[i] == ACCESSORYTYPE::NONE)
			{
				_potionInv[i] = _tempPorion;
				break;
			}
			//�ٸ��������� ����ִٸ�
			else
			{
				continue;
			}
		}

		_tempPorion = POTIONTYPE::NONE;
	}
}

void Inventory::itemCount()
{
	int weaponNum = 0;
	int armorNum = 0;
	int accessoryNum = 0;
	int potionNum = 0;

	//����
	for (int i = 0; i < WEAPON_MAX_ARR; ++i)
	{
		//�ٸ��� ����ִٸ�
		if (_weaponInv[i] != WEAPONTYPE::NONE)
		{
			weaponNum++;
		}
	}

	//��
	for (int i = 0; i < ARMOR_MAX_ARR; ++i)
	{
		//�ٸ��� ����ִٸ�
		if (_armorInv[i] != ARMORTYPE::NONE)
		{
			armorNum++;
		}
	}

	//�Ǽ�����
	for (int i = 0; i < ACCESSORY_MAX_ARR; ++i)
	{
		//�ٸ��� ����ִٸ�
		if (_accessoryInv[i] != ACCESSORYTYPE::NONE)
		{
			accessoryNum++;
		}
	}

	//����
	for (int i = 0; i < POTION_MAX_ARR; ++i)
	{
		//�ٸ��� ����ִٸ�
		if (_potionInv[i] != POTIONTYPE::NONE)
		{
			potionNum++;
		}
	}

	_weaponCount = weaponNum;
	_armorCount = armorNum;
	_accessoryCount = accessoryNum;
	_potionCount = potionNum;

}

void Inventory::inventorySave()
{
	HANDLE file;
	DWORD save;

	file = CreateFile("saveFile/inventory.inv", GENERIC_WRITE, NULL, NULL,
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, _weaponInv, sizeof(WEAPONTYPE) * WEAPON_MAX_ARR, &save, NULL);
	WriteFile(file, _armorInv, sizeof(ARMORTYPE) * ARMOR_MAX_ARR, &save, NULL);
	WriteFile(file, _accessoryInv, sizeof(ACCESSORYTYPE) * ACCESSORY_MAX_ARR, &save, NULL);
	WriteFile(file, _potionInv, sizeof(POTIONTYPE) * POTION_MAX_ARR, &save, NULL);

	CloseHandle(file);
}

void Inventory::inventoryLoad()
{
	HANDLE file;
	DWORD lode;

	file = CreateFile("saveFile/inventory.inv", GENERIC_READ, NULL, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _weaponInv, sizeof(WEAPONTYPE) * WEAPON_MAX_ARR, &lode, NULL);
	ReadFile(file, _armorInv, sizeof(ARMORTYPE) * ARMOR_MAX_ARR, &lode, NULL);
	ReadFile(file, _accessoryInv, sizeof(ACCESSORYTYPE) * ACCESSORY_MAX_ARR, &lode, NULL);
	ReadFile(file, _potionInv, sizeof(POTIONTYPE) * POTION_MAX_ARR, &lode, NULL);

	CloseHandle(file);
}

void Inventory::inventoryReBooting()
{
	if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE))
	{
		inventoryReset();
		inventorySave();
	}
}
