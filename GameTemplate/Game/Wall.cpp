#include "stdafx.h"
#include "Wall.h"

bool Wall::Start()
{
	m_modelRender.Init("Assets/modelData/dangeon/dangeon_wall.tkm");
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	m_physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);
	return true;
}
Wall ::~Wall()
{

}

void Wall::Update()
{
	m_modelRender.Update();
}
void Wall ::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}