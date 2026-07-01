#pragma once
#include "Base/Base.h"

class WeaponBase :public Base {
public:
	CModelObj m_weapon;
	CMatrix m_weaponMat;
public:
	WeaponBase(int type);
	void Update() override;
	void Render() override;
};
