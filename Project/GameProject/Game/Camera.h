#pragma once
#include "Base/Base.h"

class Camera :public Base {
private:
	CMatrix m_camMat;
	float m_dist;		//プレイヤーまでの距離
	float m_speed;		//回転速度
	CVector3D m_at;		//注視点
	CVector3D m_playerPos;
	CVector3D m_posOld;
public:
	Camera();
	/// <summary>
	/// カメラの更新はプレイヤーで行う
	/// </summary>
	void UpdateCam();
	void Collision(Base* b) override;
	CMatrix GetMatrix() {
		return m_camMat;
	}
};