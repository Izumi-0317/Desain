#include "UIBase.h"

UIBase::UIBase(int type)
	: Base(type)
{
	m_cntImg = COPY_RESOURCE("UICnt", CImage);
}

void UIBase::Update()
{
}

void UIBase::Draw()
{
}
