#include "UIBase.h"

UIBase::UIBase(int type)
	: ObjectBase(type)
{
	m_cntImg = COPY_RESOURCE("UICnt", CImage);
}

void UIBase::Update()
{
}

void UIBase::Draw()
{
}
