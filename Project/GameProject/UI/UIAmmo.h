#pragma once
#include "UIBase.h"

class UIAmmo :public UIBase {
private:
	CImage m_rifleImg;
	CImage m_backImg;
	int m_ammo;			//’e‚ÌŠ”
public:
	UIAmmo();
	void Update() override;
	void Draw() override;
};
