#pragma once
#include "WeaponBase.h"

class Shield :public WeaponBase {
private:
	ObjectBase* mp_owner;		//所有者のポインタ
public:
	Shield(ObjectBase* owner);
	void Update() override;
	void Render() override;
};
