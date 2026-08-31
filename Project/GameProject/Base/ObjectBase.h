#pragma once
#include "ObjectType.h"

class ObjectBase {
public:
	CVector3D m_pos;		//座標
	CVector3D m_rot;		//回転値
	CVector3D m_scale;		//スケール値
	CVector3D m_vec;		//移動ベクトル
	CVector3D m_dir;		//前方向ベクトル

	unsigned int m_type;	//オブジェクトの種類
	float m_rad;			//半径
	bool m_kill;			//削除フラグ

	CCapsule m_capusle;		//カプセル
	COBB m_obb;				//OBB
protected:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">オブジェクトの種類</param>
	ObjectBase(unsigned int type);
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ObjectBase() = default;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 3D描画処理
	/// </summary>
	virtual void Render();
	/// <summary>
	/// 2D描画処理
	/// </summary>
	virtual void Draw();
	/// <summary>
	/// 当たり判定処理
	/// </summary>
	virtual void Collision(ObjectBase* o);

	/// <summary>
	/// 削除フラグON
	/// </summary>
	void SetKill() { if (!m_kill) m_kill = true; }
	/// <summary>
	/// オブジェクトの種類取得
	/// </summary>
	/// <returns></returns>
	int GetType() { return m_type; }
	/// <summary>
	/// モデルの取得
	/// </summary>
	/// <returns></returns>
	virtual CModel* GetModel() { return nullptr; }
	/// <summary>
	/// カプセルの取得
	/// </summary>
	/// <returns></returns>
	virtual CCapsule* GetCapsule() { return nullptr; }

	/// <summary>
	/// キー入力
	/// </summary>
	/// <param name="kbM">キーマウの入力キー</param>
	/// <param name="pad">パッドの入力キー</param>
	/// <param name="state">キーの状態</param>
	/// <returns></returns>
	bool IsInput(CInput::E_BUTTON kbM, CInput::E_BUTTON pad, CInput::E_STATE state = CInput::ePush)
	{
		return CInput::GetPadData(0) ?
			CInput::GetState(0, (CInput::E_STATE)state, (CInput::E_BUTTON)pad) :
			CInput::GetState(0, (CInput::E_STATE)state, (CInput::E_BUTTON)kbM);
	}
};