#include "UIScope.h"
#include "Game/Obj/Chara/Player.h"

UIScope::UIScope() 
	: UIBase(ObjectType::eUIScope){
	m_scopeImg = COPY_RESOURCE("UIAim", CImage);
	m_scopeImg.SetSize(1920, 1080);
}

void UIScope::Update(){
	if (Player* p = ObjectManager::FindObject<Player>(ObjectType::ePlayer)) {
		if (p->GetState() == p->SAiming) {
			m_scopeImg = COPY_RESOURCE("UIScopeView", CImage);
			CCamera::GetCurrent()->Perspective(DtoR(30.0), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.03f, 1000.0f);
		}
		else {
			m_scopeImg = COPY_RESOURCE("UIAim", CImage);
			CCamera::GetCurrent()->Perspective(DtoR(60.0), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.03f, 1000.0f);
		}
	}
}

void UIScope::Draw(){
	if (Player* p = ObjectManager::FindObject<Player>(ObjectType::ePlayer)) {
		if (p->GetState() != p->SIdle && p->GetState() != p->SPunch)
			m_scopeImg.Draw();
	}
}
