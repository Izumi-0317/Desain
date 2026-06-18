#include "Sword.h"
#include "Paladin.h"

namespace {
	const CVector3D SWORD_SCALE(50, 50, 50);
}

Sword::Sword()
	: ObjBase(eSword){
	m_obj = COPY_RESOURCE("Sword", CModelObj);
}

void Sword::Update(){
	if (Paladin* p = dynamic_cast<Paladin*>(Base::FindObject(eEnemy))) {
		m_objMat = p->GetModel()->GetFrameMatrix(60)
			* CMatrix::MRotationZ(DtoR(90))
			* CMatrix::MRotationX(DtoR(90))
			* CMatrix::MScale(SWORD_SCALE);
	}
}

void Sword::Render(){
	m_obj.Render(m_objMat);
}
