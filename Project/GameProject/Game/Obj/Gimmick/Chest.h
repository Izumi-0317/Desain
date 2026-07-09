#pragma once
#include "Game/Obj/Gimmick/GimmickBase.h"

class Chest :public GimmickBase {
private:
	std::vector<Base*> m_enemyList;		//¶¬‚µ‚½“G‚ÌƒŠƒXƒg
	bool m_isOpen;						//ŠJ‚¯‚é‚©
public:
	Chest(const CVector3D& pos, float roty = 0, int enemyCnt = 0);
	void Update() override;
	void Render() override;
	void Interact() override { m_isInteractable = true; }
};
