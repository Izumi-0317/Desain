#pragma once
#include "Base/Base.h"

class Field :public Base {
private:
	CModelObj* m_field;
public:
	Field();
	void Update() override;
	void Render() override;
	CModel* GetModel() override {
		return m_field;
	}
};
