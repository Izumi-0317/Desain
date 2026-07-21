#pragma once
#include "UIBase.h"

class UIHP :public UIBase {
private:
	CImage m_hpImg;
	CImage m_lowHpImg;
	int m_currentHP;
	int m_lowStartHP;
	int m_delayTime;
	float m_lerpTime;
	float m_lowHPSize;
public:
	UIHP();
	void Update() override;
	void Draw() override;
};
