#include "Room.h"

Room::Room(const CVector3D& pos, float roty, RoomType roomType)
	: Base(eRoom)
	, m_isCol(false){
	m_pos = pos;
	m_rot.y = DtoR(roty);
	std::string str;
	switch (roomType){
	case R1: str = "Room1"; break;
	case R2I: str = "Room2I"; break;
	case R2L: str = "Room2L"; break;
	case R3: str = "Room3"; break;
	case R4: str = "Room4"; break;
	case RBoss: str = "RoomBoss"; break;
	default: SetKill();
	}
	m_room = COPY_RESOURCE(str, CModelObj);
	mp_room = &m_room;
}

void Room::Update()
{
}

void Room::Render(){
	m_room.SetPos(m_pos);
	m_room.SetRot(m_rot);
	m_room.Render();
	//Utility::DrawAABB(CVector3D(-0.5f, -0.5f, -80), CVector3D(35, 5, -41.5f), CVector4D(1, 0, 0, 0.5f));
}

void Room::Collision(Base* b) {
	switch (b->GetType()) {
	case ePlayer:
		if (CCollision::CollisionAABBPoint(
			CVector3D(-0.5f, -0.5f, -80),
			CVector3D(35, 5, -41.5f), b->m_pos)) {
			m_isCol = true;
		}
		else m_isCol = false;
		break;
	}
}
