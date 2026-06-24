#include "Sword.h"
#include "Paladin.h"
#include "Player.h"

namespace {
	const CVector3D SWORD_SCALE(50, 50, 50);
}

Sword::Sword()
	: ObjBase(eSword)
	, m_enableCap(false){
	m_obj = COPY_RESOURCE("Sword", CModelObj);
	m_rad = 0.04f;
}

void Sword::Update(){
	if (Paladin* p = dynamic_cast<Paladin*>(Base::FindObject(eEnemy))) {
		m_objMat = p->GetModel()->GetFrameMatrix(60)
			* CMatrix::MRotationZ(DtoR(90))
			* CMatrix::MRotationX(DtoR(90))
			* CMatrix::MScale(SWORD_SCALE);
	}
	m_capusle = CCapsule(m_objMat.GetPosition(),
		m_objMat.GetPosition() + m_objMat.GetUp() * 1.25f,
		m_rad);
}

void Sword::Render(){
	m_obj.Render(m_objMat);
	//if (m_enableCap) Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 0.5));
}

void Sword::Collision(Base* b){
	switch (b->GetType()){
	case ePlayer:
		float dist;
		CVector3D cross, dir;
		if (m_enableCap && CCollision::CollisionCapsule(m_capusle, b->m_capusle, &dist, &cross, &dir)) {
			if (Player* p = dynamic_cast<Player*>(b)) {
				p->TakeDamage(10);
				m_enableCap = false;
			}
		}
		break;
	}
}
