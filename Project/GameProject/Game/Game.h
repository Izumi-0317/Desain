#pragma once
#include "Base/Base.h"

class Game :public Base {
private:
	CTextureFrame* m_completeTF;
	int m_delayTime;		//シーン遷移の待ち時間
	bool m_isComplete;		//ゲームのクリアフラグ
	bool m_isGameOver;		//ゲームオーバーフラグ
	bool m_isCaptured;
public:
	static bool m_cameraMode;	//true::FPS　false::TPS
	Game();
	void Update();
	void markAsComplete() { if (!m_isGameOver) m_isComplete = true; }
	void markAsGameOver() { m_isGameOver = true; }
};