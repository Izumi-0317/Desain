#include "ObjectBase.h"
#include "ObjectManager.h"

ObjectBase::ObjectBase(unsigned int type)
	: m_pos(CVector3D::zero)
	, m_vec(CVector3D::zero)
	, m_type(type)
	, m_rad(0.0f)
	, m_kill(false)
{
	ObjectManager::GetInstance()->Add(this);
}

void ObjectBase::Update()
{
}

void ObjectBase::Render()
{
}

void ObjectBase::Draw()
{
}

void ObjectBase::Collision(ObjectBase* o)
{
}
