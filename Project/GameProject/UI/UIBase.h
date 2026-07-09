#pragma once
#include "Base/Base.h"

class UIBase :public Base {
private:
	CImage m_img;
	CImage m_backImg;
	CImage m_cntImg;
public:
	UIBase(int type);
	void Update() override;
	void Draw() override;
};
