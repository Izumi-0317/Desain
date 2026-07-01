#pragma once
#include "Base/Base.h"

class ObjBase :public Base {
public:
	CModelObj m_obj;
	CMatrix m_objMat;
public:
	ObjBase(int type);
	void Update() override;
	void Render() override;
};
