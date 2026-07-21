#pragma once
#include "GimmickBase.h"

class Chest :public GimmickBase {
private:
	std::vector<Base*> m_enemyList;		//¶¬‚µ‚½“G‚ÌƒŠƒXƒg
	float m_enemyMaxCnt;					//¶¬‚µ‚½“G‚Ì”
	bool m_isOpen;						//ŠJ‚¯‚é‚©
	Base* mp_effect;
public:
	Chest(const CVector3D& pos, float roty = 0, int enemyCnt = 0);
	void Update() override;
	void Render() override;
	void Remove();
	void Interact() override { m_isInteractable = true; }
};
