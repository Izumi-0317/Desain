#pragma once
#include "Base/Base.h"

class UIBase :public Base {
public:
	CImage m_backImg;
	CImage m_cntImg;
public:
	UIBase(int type = eUI);
	void Update() override;
	void Draw() override;
};
