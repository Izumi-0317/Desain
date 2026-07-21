#pragma once
#include "CharaBase.h"

class Paladin :public CharaBase {
private:
	enum Anim {
		T,				//Tポーズ
		Idle,			//待機
		Walk,			//歩行
		WalkBack,		//後退
		Attack,			//攻撃
		SkillAttack,	//スキル攻撃
		Damage,			//ダメージ
		Death,			//死亡
	};
	int m_attackCT;		//攻撃クールタイム
	int m_attackCnt;	//攻撃回数カウント
	int m_chaseTime;	//追跡時間
	float m_moveTime;	//移動中
	float m_backRotX;	//後退する際のX軸の角度調整
	float m_backRotZ;	//後退する際のZ軸の角度調整
	bool m_skillFlag;	//次の攻撃がスキルか
	Base* mp_target;	//攻撃ターゲット
	Base* mp_shield;	//持っている盾のポインタ
	Base* mp_sword;		//持っている剣のポインタ
	Base* mp_effect;	//エフェクトのポインタ
public:
	enum AddState {
		SSkillAttack = 4,	//スキル攻撃
	};
	Paladin(const CVector3D& pos, bool servant = false);
	~Paladin();
	void Update() override;
	void Render() override;
	void Collision(Base* b) override;
	void StateIdle() override;
	void StateAttack() override;
	void StateSkillAttack();
	void StateDamage() override;
	void StateDeath() override;
	void WanderMove();
	void TakeDamage(int damage) override;
	void AddAttackCnt() { m_attackCnt++; }
	CModel* GetModel() override { return &m_model; }
	CCapsule* GetCapsule() override { return &m_capusle; }
	bool GetSkillFlag() const { return m_skillFlag; }
};
