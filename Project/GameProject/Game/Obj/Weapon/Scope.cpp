#include "Scope.h"
#include "Game/Obj/Chara/Player.h"

Scope::Scope()
	: Base(eScope){
	m_scope = COPY_RESOURCE("ScopeView", CImage);
	m_scope.SetSize(1920, 1080);
}

void Scope::Update(){
	CCamera::GetCurrent()->Perspective(DtoR(20.0), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.03f, 1000.0f);
	if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
		if (p->GetState() != p->SAiming) {
			CCamera::GetCurrent()->Perspective(DtoR(60.0), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.03f, 1000.0f);
			SetKill();
		}
	}
}

void Scope::Draw(){
	m_scope.Draw();
}
