#pragma once
#include "Base/ObjectBase.h"

class GimmickBase :public ObjectBase {
public:
	CModelObj m_gimmick;
	bool m_isInteractable;		//インタラクト可能か
public:
	GimmickBase(int type);
	void Update() override;
	void Render() override;
	virtual void Interact() = 0;
};
