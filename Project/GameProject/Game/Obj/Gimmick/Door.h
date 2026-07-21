#pragma once
#include "GimmickBase.h"

class Door :public GimmickBase {
private:
	int m_returnElapsedTime;	//Œ³‚ÌˆÊ’u‚É–ß‚é‚Ü‚Å‚ÌŒo‰ßŽžŠÔ
	float m_moveDist;			//ˆÚ“®‹——£
public:
	Door(const CVector3D& pos, float roty = 0);
	void Update() override;
	void Render() override;
	void Interact() override { m_isInteractable = true; }
	void Move(float& posXorZ);
};
