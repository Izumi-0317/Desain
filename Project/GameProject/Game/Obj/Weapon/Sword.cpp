#include "Sword.h"
#include "Game/Obj/Chara/Paladin.h"
#include "Game/Obj/Chara/Player.h""

Sword::Sword(Base* owner)
	: WeaponBase(eSword)
	, m_damage(0)
	, m_enableCap(false)
	, mp_owner(owner){
	m_weapon = COPY_RESOURCE("Sword", CModelObj);
}

void Sword::Update(){
	m_weaponMat = mp_owner->GetModel()->GetFrameMatrix(60)
		* CMatrix::MRotationZ(DtoR(90))
		* CMatrix::MRotationX(DtoR(90))
		* CMatrix::MScale(50, 50, 50);
	m_rad = mp_owner->GetModel()->GetScale().x * 4;
	m_capusle = CCapsule(m_weaponMat.GetPosition(),
		m_weaponMat.GetPosition() + m_weaponMat.GetUp() * 1.5f,
		m_rad);
}

void Sword::Render(){
	m_weapon.Render(m_weaponMat);
	//Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 0.5));
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
				//TODO::‰¹‚ð–Â‚ç‚·
				if (Paladin* pd = dynamic_cast<Paladin*>(mp_owner)) {
					if (!pd->GetSkillFlag()) pd->AddAttackCnt();
				}
			}
		}
		break;
	}
}
