#pragma once
#include "Base/ObjectBase.h"

class GameOver :public ObjectBase {
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
