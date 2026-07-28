#pragma once
#include "CharaBase.h"

class Boss :public CharaBase {
private:
	enum Anim {
		T,				//Tポーズ
		Idle,			//待機
		Roaring,		//咆哮
		Run,			//走る
		Punch,			//殴打
		Haymaker,		//猛撃
		JumpAttack,		//跳躍攻撃
		ShowOff,		//誇示
		Death,			//死亡
	};
	int m_atkCT[3];		//攻撃クールタイム　0：Punch　1：Haymaker　2：JumpAttack
	int m_atkDmg;		//与ダメージ
	float m_atkRad;		//攻撃の半径
	bool m_isRoar;		//咆哮したか
	bool m_isAtk[3];	//攻撃ができるか
	bool m_isHit;		//攻撃が当たったか
	CVector3D m_spawnPos;	//初期位置
	CVector3D m_atkPos;		//攻撃の中心座標
	CVector3D m_jumpPos;	//跳躍点
	CVector3D m_landingPos;	//着地点
	Base* mp_target;	//ターゲットのポインタ
public:
	enum AddState {
		SHaymaker = 4,
		SJumpAttack,
		SShowOff,
	};
	Boss(const CVector3D& pos);
	void Update() override;
	void Render() override;
	void Collision(Base* b) override;
	void StateIdle() override;
	void StateAttack() override;
	void StateHaymaker();
	void StateJumpAttack();
	void StateShowOff();
	void StateDeath() override;
	void TakeDamage(int damage) override;
	/// <summary>
	/// 攻撃時の球生成
	/// </summary>
	/// <param name="pos">球の中心座標</param>
	/// <param name="dmg">与ダメージ</param>
	/// <param name="rad">球の半径</param>
	void SetAtkShpere(const CVector3D& pos, int dmg, float rad);
	/// <summary>
	/// 攻撃時の球リセット
	/// </summary>
	void ClearAtkShpere();
	void ResetStatus();
	CModel* GetModel() override { return &m_model; }
	CCapsule* GetCapsule() override { return &m_capusle; }
};
