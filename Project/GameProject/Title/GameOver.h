#pragma once
#include "Base/Base.h"

class GameOver :public Base {
private:
	CImage m_backImg;
	int m_cnt;
public:
	GameOver();
	void Update() override;
	void Draw() override;
};
