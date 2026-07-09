#pragma once
#include "Base/Base.h"

class PotionUI :public Base {
private:
	CImage m_cntImg;
	CImage m_potionImg;
	CImage m_backImg;
public:
	PotionUI();
	void Update() override;
	void Draw() override;
};
