#pragma once
#include "Game/Obj/Chara/CharaBase.h"

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
	int m_attackCnt;	//攻撃回数カウント
	int m_chaseTime;	//追跡時間
	float m_moveTime;	//移動中
	float m_fovAng;		//視野角度
	float m_fovLen;		//視野範囲
	float m_attackLen;	//攻撃範囲
	bool m_skillFlag;	//次の攻撃がスキルか
	Base* m_target;		//攻撃ターゲット
	Base* m_shield;		//持っている盾のポインタ
	Base* m_sword;		//持っている剣のポインタ
public:
	enum AddState {
		SSkillAttack = 4,	//スキル攻撃
	};
	Paladin(const CVector3D& pos);
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
