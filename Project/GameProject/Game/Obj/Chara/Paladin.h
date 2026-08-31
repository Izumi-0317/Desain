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
	ObjectBase* mp_target;	//攻撃ターゲット
	ObjectBase* mp_shield;	//持っている盾のポインタ
	ObjectBase* mp_sword;	//持っている剣のポインタ
	ObjectBase* mp_effect;	//エフェクトのポインタ
public:
	enum AddState {
		SAttack = 3,	//攻撃
		SSkillAttack,	//スキル攻撃
	};
	Paladin(const CVector3D& pos, bool servant = false);
	~Paladin();
	void Update() override;
	void Render() override;
	void Collision(ObjectBase* b) override;
	void StateIdle() override;
	void StateAttack();
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
