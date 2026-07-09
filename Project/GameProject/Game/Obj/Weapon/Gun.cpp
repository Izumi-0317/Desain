#include "Gun.h"
#include "Game/Camera.h"
#include "Game/Obj/Chara/Player.h"

namespace {
	constexpr int MAX_AMMO = 30;	//最大装填数
}

Gun::Gun(std::string name)
	:Base(eGun)
	, m_loadedAmmo(MAX_AMMO){
	m_gun = COPY_RESOURCE(name, CModelObj);
	m_scope = COPY_RESOURCE("Scope", CModelObj);
}

void Gun::UpdateGun() {
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
				* CMatrix::MRotation(DtoR(145), DtoR(110), DtoR(55))
				* CMatrix::MScale(60.0f, 60.0f, 60.0f);
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

void Gun::Reloaded(){
	//MAX_AMMOと同量になるように装填する
	m_loadedAmmo += MAX_AMMO - m_loadedAmmo;
	//リロード音
	SOUND("Reloaded")->Play3D(m_gunMat.GetPosition(), CVector3D(1, 1, 1));
}
