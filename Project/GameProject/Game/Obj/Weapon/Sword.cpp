#include "Sword.h"
#include "Game/Obj/Chara/Paladin.h"
#include "Game/Obj/Chara/Player.h""

namespace {
	const CVector3D SWORD_SCALE(50, 50, 50);
}

Sword::Sword(Base* owner)
	: WeaponBase(eSword)
	, m_damage(0)
	, m_enableCap(false)
	, m_owner(owner){
	m_weapon = COPY_RESOURCE("Sword", CModelObj);
}

void Sword::Update(){
	m_weaponMat = m_owner->GetModel()->GetFrameMatrix(60)
		* CMatrix::MRotationZ(DtoR(90))
		* CMatrix::MRotationX(DtoR(90))
		* CMatrix::MScale(SWORD_SCALE);
	m_rad = m_owner->GetModel()->GetScale().x * 4;
	m_capusle = CCapsule(m_weaponMat.GetPosition(),
		m_weaponMat.GetPosition() + m_weaponMat.GetUp() * 1.5f,
		m_rad);
}

void Sword::Render(){
	m_weapon.Render(m_weaponMat);
	if (m_enableCap) Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 0.5));
}

void Sword::Collision(Base* b){
	switch (b->GetType()){
	case ePlayer:
		float dist;
		CVector3D cross, dir;
		if (m_enableCap && CCollision::CollisionCapsule(m_capusle, b->m_capusle, &dist, &cross, &dir)) {
			if (Player* p = dynamic_cast<Player*>(b)) {
				p->TakeDamage(m_damage);
				m_enableCap = false;
				if (Paladin* pd = dynamic_cast<Paladin*>(m_owner)) {
					if (!pd->GetSkillFlag()) pd->AddAttackCnt();
				}
			}
		}
		break;
	}
}
