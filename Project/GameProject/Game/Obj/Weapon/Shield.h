#pragma once
#include "WeaponBase.h"

class Shield :public WeaponBase {
private:
	Base* mp_owner;		//所有者のポインタ
public:
	Shield(Base* owner);
	void Update() override;
	void Render() override;
};
