#pragma once
#include "Base/Base.h"

class Gun :public Base {
private:
	CModelObj m_gun;
	CModelObj m_scope;
	CMatrix m_gunMat;
	CMatrix m_scopeMat;
	int m_loadedAmmo;	//‘•“UÏ‚İ‚Ì’e‚Ì”
public:
	Gun(std::string name);
	void UpdateGun();
	void Render() override;
	void Reloaded();
	CMatrix GetMatrix() const { return m_gunMat; }
	void SetLoadedAmmo(int ammo) { m_loadedAmmo -= ammo; }
	int GetLoadedAmmo() const { return m_loadedAmmo; }
};
