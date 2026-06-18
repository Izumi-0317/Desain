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
	float m_moveTime;	//移動中
public:
	Paladin(const CVector3D& pos);
	void Update() override;
	void Render() override;
	void Collision(Base* b) override;
	void StateIdle() override;
	void StateAttack() override;
	void StateDamage() override;
	void StateDeath() override;
	CModel* GetModel() override {
		return &m_model;
	}
	CCapsule* GetCapsule() override {
		return &m_capusle;
	}
};
