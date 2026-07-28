#pragma once
#include "Base/Base.h"

class Title :public Base {
private:
	CImage m_titleImg;
	CImage m_startImg;
	CImage m_backImg;
	int m_cnt;
	float m_alpha;
public:
	Title();
	void Update() override;
	void Draw() override;
};
