#pragma once
#include "Base/Base.h"

class EffectMagic : public Base {
	CModelObj m_model;
	int m_time;
	float m_scale;
	float m_alpha;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">使用モデル</param>
	/// <param name="pos">出現座標</param>
	/// <param name="rot">初期回転値</param>
	/// <param name="scale">大きさ</param>
	/// <param name="time">生存時間</param>
	EffectMagic(const char* model, const CVector3D& pos, const CVector3D& rot, float scale, float time);
	void Update() override;
	void Render() override;
	void SetAlpha(float alpha) { m_alpha = alpha; }
};