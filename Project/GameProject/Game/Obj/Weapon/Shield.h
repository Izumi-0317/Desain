#pragma once
#include "Game/Obj/Weapon/WeaponBase.h"

class Shield :public WeaponBase {
private:
	Base* m_owner;		//所有者のポインタ
public:
	Shield(Base* owner);
	void Update() override;
	void Render() override;
	void Collision(Base* b) override;
};
