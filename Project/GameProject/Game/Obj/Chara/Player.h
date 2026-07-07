#pragma once
#include "Game/Obj/Chara/CharaBase.h"

class GimmickBase;

class Player :public CharaBase {
private:
	enum Anim {
		T,				//Tポーズ
		Idle,			//待機
		Walk,			//歩行
		BackWardWalk,	//後退
		Run,			//走行
		SideStepL,		//左移動
		SideStepR,		//右移動
		Hit,			//殴打
		DownToAim,		//挙銃
		Aiming,			//構える
		Firing,			//発砲
		AimToDown,		//脱銃
		StrafeF,		//発砲中の前移動
		StrafeB,		//発砲中の後移動
		StrafeL,		//発砲中の左移動
		StrafeR,		//発砲中の右移動
		Reloading,		//リロード
		Damage,			//ダメージ
		Death,			//死亡
	};
	CMatrix m_playerMat;
	int m_fireTime;		//発砲時間
	bool m_animFlag;	//DownToAimの遷移用
	bool m_attackFlag;	//攻撃フラグ
	bool m_isMaxAmmo;	//現在の装填数が最大装填数か
	GimmickBase* m_intaractable;	//インタラクト可能なオブジェクトのポインタ
public:
	enum AddState {
		SHit = 4,		//殴打
		SHave,			//構える
		SAiming,		//狙撃準備
		SReloaded,		//リロード
	};
	Player(const CVector3D& pos);
	void Update() override;
	void Render() override;
	void StateIdle();
	void StateHit();
	void StateHave();
	void StateAiming();
	void StateReloaded();
	void StateDamage();
	void StateDeath();
	void Collision(Base* b) override;
	void Move(float speed);
	void Fire();
	void Interact();
	CModel* GetModel() override { return &m_model; }
	CCapsule* GetCapsule() override { return &m_capusle; }
	CMatrix GetMatrix() const { return m_playerMat; }
	int GetState() const { return m_state; }
};