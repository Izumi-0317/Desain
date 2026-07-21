#pragma once
#include "GimmickBase.h"

class Potion :public GimmickBase {
private:
	
public:
	Potion(const CVector3D& pos);
	void Update() override;
	void Render() override;
	void Interact() override;
};
