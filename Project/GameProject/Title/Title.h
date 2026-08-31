#pragma once
#include "Base/ObjectBase.h"

class Title :public ObjectBase {
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
