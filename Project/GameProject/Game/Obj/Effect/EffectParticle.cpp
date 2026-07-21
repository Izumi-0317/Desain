#include "EffectParticle.h"

EffectParticle::EffectParticle(const char* model, const CVector3D& pos, const CVector3D& vec, const CVector3D& accel,
	float scale, float change_scale, float change_alpha, int blend, bool builbord, int time)
	: Base(eEffect),
	m_alpha(1.0f),
	m_vec_accel(accel),
	m_scale(scale),
	m_scale_speed(change_scale),
	m_alpha_speed(change_alpha),
	m_blend(blend),
	m_builbord(builbord),
	m_time(time) {
	m_model = COPY_RESOURCE(model, CModelObj);
	m_pos = pos;
	m_vec = vec;
}

void EffectParticle::Update(){
	m_vec += m_vec_accel;
	m_pos += m_vec;
	m_rot += m_rot_vec;
	m_scale += m_scale_speed;
	m_alpha += m_alpha_speed;
	if (m_time > 0) m_time--;
	if (m_alpha < 0 || m_scale < 0 || m_time == 0) SetKill();
}

void EffectParticle::Render(){
	CLight::SetLighting(false);
	glDepthMask(GL_FALSE);
	switch (m_blend) {
	case eBlendAlpha:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case eBlendAdd:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.GetMaterial(0)->m_alpha = m_alpha;
	m_model.SetScale(m_scale, m_scale, m_scale);
	if (m_builbord)
		m_model.RenderBuilbord(CCamera::GetCurrent()->GetBuilbordMatrix());
	else
		m_model.Render();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
	CLight::SetLighting(true);
}
