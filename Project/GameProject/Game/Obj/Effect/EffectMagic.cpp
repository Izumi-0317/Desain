#include "EffectMagic.h"

EffectMagic::EffectMagic(const char* model, const CVector3D& pos, const CVector3D& rot, float scale, float time)
	: Base(eEffect)
	, m_time(time)
	, m_scale(scale)
	, m_alpha(1.0f){
	m_model = COPY_RESOURCE(model, CModelObj);
	m_pos = pos;
	m_rot = rot;
}

void EffectMagic::Update(){
	if (m_time != -1) {
		m_time--;
		if (m_time < 30) m_alpha -= 1.0f / 30;
		if (m_time <= 0) SetKill();
	}
	m_rot.y += DtoR(1.0f);
}

void EffectMagic::Render(){
	CLight::SetLighting(false);
	glDepthMask(GL_FALSE);
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(m_scale, m_scale, m_scale);
	m_model.GetMaterial(0)->m_alpha = m_alpha;
	m_model.Render();
	glDepthMask(GL_TRUE);
	CLight::SetLighting(true);
}
