#pragma once
#include "Base/Base.h"

class Bullet :public Base {
private:
	//CModelObj m_bullet;
	int m_killCnt;
	CVector3D m_posOld;
public:
	Bullet(const CVector3D& pos, const CVector3D& dir);
	void Update() override;
	void Render() override;
	void Collision(Base* b) override;
};
