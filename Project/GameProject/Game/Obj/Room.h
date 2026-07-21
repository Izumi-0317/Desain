#pragma once
#include "Base/Base.h"

class Room :public Base {
private:
	CModelObj m_room;
	CModelObj* mp_room;
public:
	enum RoomType {
		R1,
		R2I,
		R2L,
		R3,
		R4,
		RBoss,
	};
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">部屋の中心座標</param>
	/// <param name="roty">部屋の向き(90度ずつ)</param>
	/// <param name="roomType">部屋の種類</param>
	Room(const CVector3D& pos, float roty, RoomType roomType);
	void Update() override;
	void Render() override;
	CModel* GetModel() override { return mp_room; }
};
