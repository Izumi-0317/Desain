#pragma once
#include "Base/ObjectBase.h"

class Game :public ObjectBase {
private:
	int m_delayTime;		//シーン遷移の待ち時間
	bool m_isComplete;		//ゲームのクリアフラグ
	bool m_isGameOver;		//ゲームオーバーフラグ
public:
	static bool m_cameraMode;	//true::FPS　false::TPS
	Game();
	void Update();
	void markAsComplete() { if (!m_isGameOver) m_isComplete = true; }
	void markAsGameOver() { m_isGameOver = true; }
};