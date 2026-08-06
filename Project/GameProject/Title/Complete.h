#pragma once
#include "Base/Base.h"

class Complete :public Base {
private:
	CImage m_completeImg;
	CImage m_returnImg;
	CImage m_backImg;
	float m_alpha;
	float m_lerpTime;
	bool m_isSound;
public:
	Complete();
	void Update() override;
	void Draw() override;
};
