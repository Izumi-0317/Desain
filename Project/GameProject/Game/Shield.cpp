#include "Shield.h"
#include "Paladin.h"

namespace {
	const CVector3D SHIELD_SCALE(50, 50, 50);
}

Shield::Shield()
	: ObjBase(eShield){
	m_obj = COPY_RESOURCE("Shield", CModelObj);
	m_rad = 0.1f;
}

void Shield::Update(){
	if (Paladin* p = dynamic_cast<Paladin*>(Base::FindObject(eEnemy))) {
		m_objMat = p->GetModel()->GetFrameMatrix(35)
			* CMatrix::MTranselate(0, -4, 0)
			* CMatrix::MRotationZ(DtoR(175))
			* CMatrix::MRotationX(DtoR(90))
			* CMatrix::MScale(SHIELD_SCALE);
	}
}

void Shield::Render(){
	m_obj.Render(m_objMat);
}
