#pragma once
#include "UIBase.h"

class UIScope :public UIBase {
private:
	CImage m_scopeImg;
public:
	UIScope();
	void Update() override;
	void Draw() override;
};
