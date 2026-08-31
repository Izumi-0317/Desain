#pragma once
#include "Base/ObjectBase.h"
#include "Base/Interface.h"

class CharaBase :public ObjectBase, public Interface {
public:
	enum State {
		SIdle,
		SDamage,
		SDeath,
	};
	CModelA3M m_model;
	int m_hp;
	int m_maxHp;
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
	/// <param name="next">‘JˆÚ‚·‚éstate</param>
	void ChangeState(int next);
	virtual void StateIdle();
	virtual void StateDamage();
	virtual void StateDeath();
	void TakeDamage(int damage) override;
	virtual CModel* GetModel() = 0;
	virtual CCapsule* GetCapsule() = 0;
};
