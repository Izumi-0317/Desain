#include "Shield.h"
#include "Paladin.h"

namespace {
	const CVector3D SHIELD_SCALE(50, 50, 50);
}
//float i = 0.3, o = 0.0, p = 0.6;
Shield::Shield()
	: ObjBase(eShield){
	m_obj = COPY_RESOURCE("Shield", CModelObj);
}

void Shield::Update(){
	/*if (PUSH(CInput::eButton1)) i += 0.01f;
	if (PUSH(CInput::eButton3)) p += 0.01f;*/
	if (Paladin* p = dynamic_cast<Paladin*>(Base::FindObject(eEnemy))) {
		m_objMat = p->GetModel()->GetFrameMatrix(35)
			* CMatrix::MTranselate(0, -4, 0)
			* CMatrix::MRotationZ(DtoR(175))
			* CMatrix::MRotationX(DtoR(90))
			* CMatrix::MScale(SHIELD_SCALE);
		m_rot = p->m_rot;
	}
	m_obb = COBB(m_objMat.GetPosition(), m_rot, m_objMat.GetScale() * CMatrix::MScale(0.35f, 0.1f, 0.6f));
}

void Shield::Render(){
	m_obj.Render(m_objMat);
	//Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 0.5));
}

void Shield::Collision(Base* b){
	switch (b->GetType()){
	case eBullet:
		float len;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, b->m_capusle, &axis, &len)) {
			b->SetKill();
		}
		break;
	}
}
