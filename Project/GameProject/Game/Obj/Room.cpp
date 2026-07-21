#include "Room.h"

Room::Room(const CVector3D& pos, float roty, RoomType roomType)
	: Base(eRoom){
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
}
