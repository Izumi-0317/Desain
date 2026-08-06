#include "UIVignette.h"
#include "Game/Obj/Chara/Player.h"

UIVignette::UIVignette()
	: m_alpha(0.0f)
    , m_lerpTime(1.0f){
	m_vignetteImg = COPY_RESOURCE("UIVignette", CImage);
    if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
        m_currentHp = p->m_maxHp;
    }
}

void UIVignette::Update(){
    if (m_lerpTime < 1.0f) m_lerpTime += 0.01f;
    if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
        if (m_currentHp != p->m_hp) {
            if (m_currentHp > p->m_hp) {
                m_alpha = 1.0f;
                m_lerpTime = 0.0f;
            }
            m_currentHp = p->m_hp;
        }
    }
}

void UIVignette::Draw(){
    if (m_alpha > 0.0f) 
        m_alpha = CVector3D::Leap(CVector3D(m_alpha, 0, 0), CVector3D::zero, m_lerpTime).x;
    m_vignetteImg.SetColor(1, 0, 0, m_alpha);
    m_vignetteImg.Draw();
}
