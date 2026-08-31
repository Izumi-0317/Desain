#pragma once
#include "Base/ObjectBase.h"

class WeaponBase :public ObjectBase {
public:
	CModelObj m_weapon;
	CMatrix m_weaponMat;
public:
	WeaponBase(int type);
	void Update() override;
	void Render() override;
};
