#pragma once
#include "WeaponBase.h"

class Sword :public WeaponBase {
private:
	int m_damage;		//与えるダメージ量
	bool m_enableCap;	//カプセル有効化
	Base* mp_owner;		//所有者のポインタ
public:
	Sword(Base* owner);
	void Update() override;
	void Render() override;
	void Collision(Base* b) override;
	void SetCapsule(bool cap) { m_enableCap = cap; }
	void SetDamage(int damage) { m_damage = damage; }
};
