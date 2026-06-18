#include "Gun.h"
#include "Camera.h"
#include "Player.h"
//float b=0;
namespace {
	const CVector3D GUN_ROT(DtoR(145), DtoR(110), DtoR(55));
	const CVector3D GUN_SCALE(60.0f, 60.0f, 60.0f);
}

Gun::Gun(std::string name)
	:Base(eGun){
	m_gun = COPY_RESOURCE(name, CModelObj);
	m_scope = COPY_RESOURCE("Scope", CModelObj);
}

void Gun::UpdateGun() {
	//if (PUSH(CInput::eButton2)) b += 0.01f;
	if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
		m_pos = CVector3D(5.92f, 11.66f, 0.0f);
		//銃行列
		switch (p->GetState()) {
		case p->SAiming:
			if (Camera* c = dynamic_cast<Camera*>(Base::FindObject(eCamera))) {
				m_gunMat = c->GetMatrix()
					* CMatrix::MTranselate(0.0f, -0.075f, 0.03f)
					* CMatrix::MRotation(CVector3D(DtoR(180.2f), DtoR(0), DtoR(180)))
					* CMatrix::MScale(1, 1, 1);
				/* p->GetModel()->GetFrameMatrix(7)
					* CMatrix::MTranselate(p->GetModel()->GetFrameMatrix(7).GetPosition().GetNormalize().x,
						3.0f,
						p->GetModel()->GetFrameMatrix(7).GetPosition().GetNormalize().z)
					* CMatrix::MTranselate(p->GetModel()->GetFrameMatrix(7).GetPosition() + CVector3D(0,3,0))
					* CMatrix::MTranselate(m_pos + CVector3D(-5.92f,-3.0f,20.0f))*/
			}
			break;
		default:
			m_gunMat = p->GetModel()->GetFrameMatrix(36)
				* CMatrix::MTranselate(m_pos)
				* CMatrix::MRotation(GUN_ROT)
				* CMatrix::MScale(GUN_SCALE);
			break;
		}
	}
	//スコープ行列
	m_scopeMat = m_gunMat
		* CMatrix::MTranselate(0, 0.075f, 0)
		* CMatrix::MScale(1, 1, 1);
}

void Gun::Render(){
	m_gun.Render(m_gunMat);
	m_scope.Render(m_scopeMat);
}
