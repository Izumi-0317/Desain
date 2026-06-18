#pragma once
#include "Base/Base.h"

class Game :public Base {
private:
public:
	static bool m_cameraMode;		//true::FPSÅ@false::TPS
	Game();
	void Update();
};