#pragma once
#include "Base/Base.h"

class Gun :public Base {
private:
	CModelObj m_gun;
	CModelObj m_scope;
	CMatrix m_gunMat;
	CMatrix m_scopeMat;
	int m_loadedAmmo;	//‘•“UÏ‚İ‚Ì’e‚Ì”
	int m_stockAmmo;	//Š’†‚Ì’e‚Ì”
public:
	Gun(std::string name);
	void UpdateGun();
	void Render() override;
	void Reloaded();
	CMatrix GetMatrix() { return m_gunMat; }
	void SetLoadedAmmo(int ammo) { m_loadedAmmo -= ammo; }
	int GetLoadedAmmo() { return m_loadedAmmo; }
	int GetStockAmmo() { return m_stockAmmo; }
};
