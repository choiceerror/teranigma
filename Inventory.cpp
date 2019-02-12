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


}

void Inventory::render()
{
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
