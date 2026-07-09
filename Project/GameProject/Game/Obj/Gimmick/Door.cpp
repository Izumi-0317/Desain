#include "Door.h"
#include "Game/Obj/Chara/Player.h"
float l = 0, p = 0;
namespace {
	constexpr float MOVE_DISTANCE = 6.1f;
}

Door::Door(const CVector3D& pos, float roty)
	: GimmickBase(eDoor)
	, m_moveDist(0)
	, m_returnElapsedTime(0){
	m_gimmick = COPY_RESOURCE("Door", CModelObj);
	m_pos = pos;
	m_rot.y = DtoR(roty);
}

void Door::Update(){
	//if (PUSH(CInput::eButton1)) l-=0.1;
	//if (PUSH(CInput::eButton2)) p+=0.1;
	//m_pos = CVector3D(-8.2+l, 0, -15.45f+p);
	
	switch ((int)RtoD(m_rot.y)) {
	case 0:
		Move(m_pos.z);
		m_obb = COBB(m_pos + CVector3D(0, 2.1f, -1.15f), m_rot, CVector3D(0.3f, 2.1f, 1.2f));
		break;
	case 90:
		Move(m_pos.x);
		m_obb = COBB(m_pos + CVector3D(-1.15f, 2.1f, 0), m_rot, CVector3D(0.3f, 2.1f, 1.2f));
		break;
	}
}

void Door::Render(){
	m_gimmick.SetPos(m_pos);
	m_gimmick.SetRot(m_rot);
	m_gimmick.Render();
	//Utility::DrawOBB(m_obb, CVector4D(0, 1, 0, 0.5));
}

void Door::Move(float& posXorZ){
	//ˆÚ“®‚Å‚«‚é‚È‚ç
	if (m_isInteractable) {
		//ˆÚ“®‹——£‚ªŽw’è‚Ì‹——£‚É‚È‚é‚Ü‚ÅˆÚ“®‚·‚é
		if (m_moveDist < MOVE_DISTANCE) {
			posXorZ += 0.1f;
			m_moveDist += 0.1f;
		}
		else m_isInteractable = false;
	}
	//ˆÚ“®‚Å‚«‚È‚¢‚È‚ç
	else {
		//2•bŒo‚Á‚½‚ç
		if (m_returnElapsedTime++ >= 120) {
			//ˆÚ“®‹——£‚ª0.1‚É‚È‚é‚Ü‚ÅˆÚ“®‚·‚é
			if (m_moveDist > 0.1f) {
				posXorZ -= 0.1f;
				m_moveDist -= 0.1f;
			}
			else m_returnElapsedTime = 0;
		}
	}
}
