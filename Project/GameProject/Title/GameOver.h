#pragma once
#include "Base/Base.h"

class GameOver :public Base {
private:
	CImage m_gameOverImg;
	CImage m_returnImg;
	CImage m_backImg;
	int m_cnt;
	float m_alpha;
public:
	GameOver();
	void Update() override;
	void Draw() override;
};
