#pragma once
#include "Base/ObjBase.h"

class Sword :public ObjBase {
private:
	
public:
	Sword();
	void Update() override;
	void Render() override;
};
