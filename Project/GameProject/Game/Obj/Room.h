#pragma once
#include "Base/Base.h"

class Room :public Base {
private:
	CModelObj m_room;
	CModelObj* m_roomPtr;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">部屋の中心座標</param>
	/// <param name="roty">部屋の向き(90度ずつ)</param>
	/// <param name="doorCnt">ドアの数(最大4)</param>
	/// <param name="IorL">true::I字 false::L字(ドアの数が2の時のみ)</param>
	Room(const CVector3D& pos, float roty, int doorCnt, bool IorL = true);
	void Update() override;
	void Render() override;
	CModel* GetModel() override { return m_roomPtr; }
};
