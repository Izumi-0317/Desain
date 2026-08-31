#pragma once
#include "WeaponBase.h"

class Gun :public WeaponBase {
private:
	CModelObj m_scope;
	CMatrix m_scopeMat;
	int m_loadedAmmo;	//‘•“UÏ‚İ‚Ì’e‚Ì”
public:
	Gun(std::string name);
	void UpdateGun();
	void Render() override;
	void Reloaded();
	CMatrix GetMatrix() const { return m_weaponMat; }
	void SetLoadedAmmo(int ammo) { m_loadedAmmo -= ammo; }
	int GetLoadedAmmo() const { return m_loadedAmmo; }
};
