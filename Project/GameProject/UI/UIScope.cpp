#include "UIScope.h"
#include "Game/Obj/Chara/Player.h"

UIScope::UIScope() 
	: UIBase(eUIScope){
	m_scopeImg = COPY_RESOURCE("UIScopeView", CImage);
	m_scopeImg.SetSize(1920, 1080);
}

void UIScope::Update(){
	CCamera::GetCurrent()->Perspective(DtoR(20.0), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.03f, 1000.0f);
	if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
		if (p->GetState() != p->SAiming) {
			CCamera::GetCurrent()->Perspective(DtoR(60.0), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.03f, 1000.0f);
			SetKill();
		}
	}
}

void UIScope::Draw(){
	m_scopeImg.Draw();
}
