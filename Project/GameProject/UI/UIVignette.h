#pragma once
#include "UIBase.h"

class UIVignette :public UIBase {
private:
	CImage m_vignetteImg;
	float m_alpha;
	float m_lerpTime;
	int m_currentHp;
public:
	UIVignette();
	void Update() override;
	void Draw() override;
};
