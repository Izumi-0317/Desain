#pragma once
#include "Base/ObjectBase.h"
#include "Base/ObjectManager.h"

class UIBase :public ObjectBase {
public:
	CImage m_backImg;
	CImage m_cntImg;
public:
	UIBase(int type = ObjectType::eUI);
	void Update() override;
	void Draw() override;
};
