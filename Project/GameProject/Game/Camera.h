#pragma once
#include "Base/ObjectBase.h"

class Camera :public ObjectBase {
private:
	CMatrix m_camMat;
	float m_dist;		//プレイヤーまでの距離
	float m_speed;		//回転速度
	float m_shakeTime;	//振動時間
	float m_shakeIntensity;	//振動の強さ
	CVector3D m_at;		//注視点
public:
	Camera();
	/// <summary>
	/// カメラの更新はプレイヤーで行う
	/// </summary>
	void UpdateCam();
	void Collision(ObjectBase* b) override;
	void SetShake(float time, float intensity) {
		m_shakeTime = time;
		m_shakeIntensity = intensity;
	}
	CMatrix GetMatrix() { return m_camMat; }
};