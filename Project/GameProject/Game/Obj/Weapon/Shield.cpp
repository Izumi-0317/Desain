#include "Shield.h"

int i = 0;
Shield::Shield(Base* owner)
	: WeaponBase(eShield)
	, m_owner(owner){
	m_weapon = COPY_RESOURCE("Shield", CModelObj);
	m_rot.z = DtoR(175);
}

void Shield::Update(){
	//if (i++ >= 360)i = 0;
	m_weaponMat = m_owner->GetModel()->GetFrameMatrix(35)
		* CMatrix::MTranselate(0, -4, 0)
		* CMatrix::MRotationZ(DtoR(175))
		* CMatrix::MRotationX(DtoR(180))
		* CMatrix::MScale(50, 50, 50);
	/*m_weaponMat = CMatrix::MTranselate(0,0.5,0)
		*CMatrix::MRotationX(DtoR(i));
	CVector3D offsetOBB(0, 0.055, 0.01);*/
	
	m_obb = COBB(m_weaponMat.GetPosition(), m_weaponMat.GetEuler(CMatrix::eZYX), m_weaponMat.GetScale() * CMatrix::MScale(0.35f, 0.1f, 0.54f));
}

void Shield::Render(){
	m_weapon.Render(m_weaponMat);
	//Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 0.5));
}

void Shield::Collision(Base* b){
	/*switch (b->GetType()){
	case eBullet:
		float len;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, b->m_capusle, &axis, &len)) {
			b->SetKill();
		}
		break;
	}*/
}
