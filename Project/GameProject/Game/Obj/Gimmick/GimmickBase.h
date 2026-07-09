#pragma once
#include "Base/Base.h"

class GimmickBase :public Base {
public:
	CModelObj m_gimmick;
	bool m_isInteractable;		//インタラクト可能か
public:
	GimmickBase(int type);
	void Update() override;
	void Render() override;
	virtual void Interact() = 0;
};
