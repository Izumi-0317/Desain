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
	int m_attackCT[3];	//攻撃クールタイム　0：Punch　1：Haymaker　2：JumpAttack
	float m_attackRad;	//攻撃の半径
	bool m_isRoar;		//咆哮したか
	bool m_isAttack[3];	//攻撃ができるか
	CVector3D m_attackPos;	//攻撃の中心座標
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
	CModel* GetModel() override { return &m_model; }
	CCapsule* GetCapsule() override { return &m_capusle; }
};
