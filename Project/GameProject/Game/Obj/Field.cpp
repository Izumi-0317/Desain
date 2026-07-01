#include "Field.h"

Field::Field()
	:Base(eField) {
	m_field = GET_RESOURCE("Field", CModelObj);
}

void Field::Update()
{
}

void Field::Render(){
	m_field->Render();
}
