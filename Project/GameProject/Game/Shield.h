#pragma once
#include "Base/ObjBase.h"

class Shield :public ObjBase {
private:
	
public:
	Shield();
	void Update() override;
	void Render() override;
};
