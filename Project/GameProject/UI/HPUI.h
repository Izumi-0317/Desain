#pragma once
#include "Base/Base.h"

class HPUI :public Base {
private:
	CImage m_hpImg;
	CImage m_lowHpImg;
	CImage m_backImg;
public:
	HPUI();
	void Update() override;
	void Draw() override;
};
