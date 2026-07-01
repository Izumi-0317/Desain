#pragma once
#include "Base/Base.h"

class Scope :public Base {
private:
	CImage m_scope;
public:
	Scope();
	void Update() override;
	void Draw() override;
};
