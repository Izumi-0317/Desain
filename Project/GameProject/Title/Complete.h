#pragma once
#include "Base/Base.h"

class Complete :public Base {
private:
	CImage m_completeImg;
	CImage m_returnImg;
	CImage m_backImg;
	int m_cnt;
	float m_alpha;
	float m_lerpTime;
public:
	Complete(const CTexture& texture);
	~Complete();
	void Update() override;
	void Draw() override;
};
