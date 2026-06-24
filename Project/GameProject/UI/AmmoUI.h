#pragma once
#include "Base/Base.h"

class AmmoUI :public Base {
private:
	CImage m_ammoImg;
	CImage m_rifleImg;
	CImage m_backImg;
	int m_ammo;			//’e‚ÌŠ”
public:
	AmmoUI();
	void Update() override;
	void Draw() override;
};
