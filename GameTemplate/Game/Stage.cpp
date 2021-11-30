#include "stdafx.h"
#include "Stage.h"

bool Stage::Start()
{
	m_modelRender.Init("Assets/modelData/dangeon/dangeon_ground.tkm");
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	return true;
}
Stage::~Stage()
{

}

void Stage::Update()
{
	m_modelRender.Update();
}
void Stage::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}