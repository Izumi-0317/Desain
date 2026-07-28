#pragma once
#include "Base/Base.h"

class Complete :public Base {
private:
	CImage m_backImg;
	int m_cnt;
public:
	Complete();
	void Update() override;
	void Draw() override;
};
