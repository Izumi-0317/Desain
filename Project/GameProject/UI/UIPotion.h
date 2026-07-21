#pragma once
#include "UIBase.h"

class UIPotion :public UIBase {
private:
	CImage m_potionImg;
	int m_potionCnt;
public:
	UIPotion();
	void Update() override;
	void Draw() override;
	static TexAnimData _anim[];
};
