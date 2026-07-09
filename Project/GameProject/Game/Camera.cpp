#include "Camera.h"
#include "Game.h"
#include "Game/Obj/Chara/Player.h"

Camera::Camera()
	:Base(eCamera)
	, m_dist(2.0f)
	, m_speed(0.003f) 
	, m_at(0, 0, 0)
	, m_playerPos(0, 0, 0)
	, m_posOld(0, 0, 0){
	m_rot = CVector3D(DtoR(30), DtoR(-125), 0);
}

void Camera::UpdateCam(){
	m_posOld = m_pos;
	CVector2D mouse_vec = CInput::GetMouseVec();
	m_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * m_speed;
	m_rot.x = min(DtoR(35), max(DtoR(-35), m_rot.x));
	m_rot.y = Utility::NormalizeAngle(m_rot.y);
	if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
		m_playerPos = p->m_pos;
		if (Game::m_cameraMode) {
			m_camMat = CMatrix::MTranselate(p->GetModel()->GetFrameMatrix(7).GetPosition())
				* CMatrix::MTranselate(CVector3D(sin(m_rot.y), 0, cos(m_rot.y)) * 0.1f)
				* CMatrix::MRotation(m_rot);
		}
		else {
			m_camMat = CMatrix::MTranselate(p->m_pos)
				* CMatrix::MTranselate(CVector3D(0, 2.5, 0))
				* CMatrix::MRotation(m_rot)
				* CMatrix::MTranselate(CVector3D(-0.3f, 0.2f, -0.5f));
			/*m_dist = min(10.0f, max(2.0f, m_dist - CInput::GetMouseWheel()));
			m_at = p->m_pos + CVector3D(0, 1.5f, 0);
			m_pos = m_at + CMatrix::MRotation(m_rot).GetFront() * -m_dist;
			CCamera::GetCurrent()->LookAt(m_pos, m_at, CVector3D(0, 1, 0));*/
		}
		CCamera::GetCurrent()->SetTranseRot(m_camMat);
	}
}

void Camera::Collision(Base* b){
	/*switch (b->GetType()){
	case eField:
		CVector3D cross, normal;
		if (b->GetModel()->CollisionRay(&cross, &normal, m_playerPos, m_camMat.GetPosition())) {
			m_pos = m_posOld;
		}
		break;
	}*/
}
