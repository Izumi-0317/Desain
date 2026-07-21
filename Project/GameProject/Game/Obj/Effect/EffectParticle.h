#pragma once
#include "Base/Base.h"

class EffectParticle :public Base {
private:
	CModelObj	m_model;
	CVector3D	m_vec_accel;
	CVector3D	m_rot_vec;
	int			m_blend;
	int			m_time;
	float		m_scale;
	float		m_scale_speed;
	float		m_alpha;
	float		m_alpha_speed;
	bool		m_builbord;
public:
	enum {
		eBlendAlpha,		//通常ブレンド
		eBlendAdd,			//加算ブレンド
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">使用モデル</param>
	/// <param name="pos">出現位置</param>
	/// <param name="vec">初期移動ベクトル</param>
	/// <param name="accel">加速ベクトル</param>
	/// <param name="scale">初期大きさ</param>
	/// <param name="change_scale">大きさ変化量</param>
	/// <param name="change_alpha">不透明度変化量</param>
	/// <param name="blend">ブレンド方法</param>
	/// <param name="builbord">ビルボード描画</param>
	/// <param name="time">生存時間</param>
	EffectParticle(const char* model, const CVector3D& pos, const CVector3D& vec, const CVector3D& accel,
		float scale, float change_scale, float change_alpha, int blend, bool builbord, int time = -1);
	void Update() override;
	void Render() override;
};