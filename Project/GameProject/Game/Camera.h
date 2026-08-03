#pragma once
#include "Base/Base.h"

class Camera :public Base {
private:
	CMatrix m_camMat;
	float m_dist;		//プレイヤーまでの距離
	float m_speed;		//回転速度
	float m_shakeTime;	//揺れる時間
	float m_shakeIntensity;	//揺れる強さ
	CVector3D m_at;		//注視点
public:
	Camera();
	/// <summary>
	/// カメラの更新はプレイヤーで行う
	/// </summary>
	void UpdateCam();
	void Collision(Base* b) override;
	void SetShake(float time, float intensity) {
		m_shakeTime = time;
		m_shakeIntensity = intensity;
	}
	CMatrix GetMatrix() { return m_camMat; }
};