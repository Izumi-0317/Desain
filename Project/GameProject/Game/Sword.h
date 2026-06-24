#pragma once
#include "Base/ObjBase.h"

class Sword :public ObjBase {
private:
	bool m_enableCap;	//カプセル有効化
public:
	Sword();
	void Update() override;
	void Render() override;
	void Collision(Base* b) override;
	void SetCapsule(bool cap) { m_enableCap = cap; }
};
