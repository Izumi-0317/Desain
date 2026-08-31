#include "CharaBase.h"
#include "Base/ObjectManager.h"

CharaBase::CharaBase(int type)
	: ObjectBase(type)
	, m_hp(0)
	, m_maxHp(0)
	, m_state(-1)
	, m_stateStep(0)
	, m_undoState(0)
{
	
}

void CharaBase::Update()
{
	//d—Í
	m_vec.y = -GRAVITY * 10;
	m_pos += m_vec;
}

void CharaBase::Render()
{
}

void CharaBase::ChangeState(int next)
{
	if (m_state == next) return;
	m_undoState = m_state;
	m_state = next;
	m_stateStep = 0;
}

void CharaBase::StateIdle()
{
}

void CharaBase::StateDamage()
{
}

void CharaBase::StateDeath()
{
}

void CharaBase::TakeDamage(int damage)
{
	if (m_hp == 0) return;
	if (m_hp - damage > 0) {
		m_hp -= damage;
		ChangeState(SDamage);
	}
	else {
		m_hp = 0;
		ChangeState(SDeath);
	}
}
