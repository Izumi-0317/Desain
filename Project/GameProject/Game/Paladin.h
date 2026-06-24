#pragma once
#include "Base/CharaBase.h"

class Paladin :public CharaBase {
private:
	enum Anim {
		T,				//Tポーズ
		Idle,			//待機
		Walk,			//歩行
		BackWardWalk,	//後退
		Attack,			//攻撃
		SkillAttack,	//スキル攻撃
		Damage,			//ダメージ
		Death,			//死亡
	};
	int m_attackCT;		//攻撃クールタイム
	float m_moveTime;	//移動中
	float m_fovAng;		//視野角度
	float m_fovLen;		//視野範囲
	float m_attackLen;	//攻撃範囲
	Base* m_target;		//攻撃ターゲット
public:
	Paladin(const CVector3D& pos);
	void Update() override;
	void Render() override;
	void Collision(Base* b) override;
	void StateIdle() override;
	void StateAttack() override;
	void StateDamage() override;
	void StateDeath() override;
	void WanderMove();
	CModel* GetModel() override {
		return &m_model;
	}
	CCapsule* GetCapsule() override {
		return &m_capusle;
	}
};
