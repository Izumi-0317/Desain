#pragma once
#include "Base/ObjectBase.h"

class Complete :public ObjectBase {
private:
	CImage m_completeImg;
	CImage m_returnImg;
	CImage m_backImg;
	int m_cnt;
	float m_alpha;
	float m_lerpTime;
	bool m_isSound;
public:
	Complete();
	void Update() override;
	void Draw() override;
};
