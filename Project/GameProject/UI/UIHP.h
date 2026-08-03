#pragma once
#include "UIBase.h"

class UIHP :public UIBase {
private:
	CImage m_hpImg;
	CImage m_lowHpImg;
	CImage m_nameImg;
	int m_type;
	int m_maxHp;
	int m_currentHp;
	int m_lowStartHp;
	int m_delayTime;
	float m_lerpTime;
	float m_lowHpSize;
public:
	UIHP(int type);
	void Update() override;
	void Draw() override;
};
