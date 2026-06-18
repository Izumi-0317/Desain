#include "CharaBase.h"

CharaBase::CharaBase(int type)
	: Base(type)
	, m_state(-1)
	, m_hp(0)
{
	
}

void CharaBase::Update()
{
	//状態遷移
	switch (m_state) {
	case SIdle: StateIdle(); break;
	case SAttack: StateAttack(); break;
	case SDamage: StateDamage(); break;
	case SDeath: StateDeath(); break;
	}
	//重力
	m_vec.y = -GRAVITY * 10;
	m_pos += m_vec;
	//アニメーション更新
	//m_model.UpdateAnimation();
}

void CharaBase::Render()
{
}

void CharaBase::StateIdle()
{
}

void CharaBase::StateAttack()
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
		m_state = SDamage;
	}
	else {
		m_hp = 0;
		m_state = SDeath;
	}
}
