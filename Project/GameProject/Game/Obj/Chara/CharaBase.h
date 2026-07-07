#pragma once
#include "Base/Base.h"
#include "Base/Interface.h"

class CharaBase :public Base, public Interface {
public:
	enum State {
		SIdle,
		SAttack,
		SDamage,
		SDeath,
	};
	CModelA3M m_model;
	int m_hp;
	int m_state;
	int m_undoState;
	int m_stateStep;
public:
	CharaBase(int type);
	void Update() override;
	void Render() override;
	/// <summary>
	/// ŽŸ‚Ìstate‚Ö‚Ì‘JˆÚ
	/// </summary>
	/// <param name="next">ŽŸ‚Ìstate</param>
	void NextState(int next);
	virtual void StateIdle();
	virtual void StateAttack();
	virtual void StateDamage();
	virtual void StateDeath();
	void TakeDamage(int damage) override;
	virtual CModel* GetModel() = 0;
	virtual CCapsule* GetCapsule() = 0;
};
