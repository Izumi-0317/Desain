#include "Room.h"

Room::Room(const CVector3D& pos, float roty, int doorCnt, bool IorL)
	: Base(eRoom){
	m_pos = pos;
	m_rot.y = DtoR(roty);
	std::string str;
	switch (doorCnt){
	case 1: str = "Room1"; break;
	case 2: str = (IorL) ? "Room2I" : "Room2L"; break;
	case 3: str = "Room3"; break;
	case 4: str = "Room4"; break;
	default: SetKill();
	}
	m_room = COPY_RESOURCE(str, CModelObj);
	m_roomPtr = &m_room;
}

void Room::Update()
{
}

void Room::Render(){
	m_room.SetPos(m_pos);
	m_room.SetRot(m_rot);
	m_room.Render();
}
