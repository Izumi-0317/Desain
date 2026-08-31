#pragma once
#include "WeaponBase.h"

class Bullet :public WeaponBase {
private:
	int m_killCnt;
	bool m_isCol;			//着弾したか
	CVector3D m_posOld;		//1フレーム前の座標
	CVector3D m_effectPos;	//エフェクト発生位置
	CVector3D m_effectVec;	//エフェクト発生方向
public:
	Bullet(const CVector3D& pos, const CVector3D& dir);
	~Bullet();
	void Update() override;
	void Render() override;
	void Collision(ObjectBase* b) override;
};
