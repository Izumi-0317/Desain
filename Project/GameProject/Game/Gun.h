#pragma once
#include "Base/Base.h"

class Gun :public Base {
private:
	CModelObj m_gun;
	CModelObj m_scope;
	CMatrix m_gunMat;
	CMatrix m_scopeMat;
public:
	Gun(std::string name);
	void UpdateGun();
	void Render() override;
	CMatrix GetMatrix() {
		return m_gunMat;
	}
};
