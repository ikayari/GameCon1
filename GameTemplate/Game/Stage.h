#pragma once
class Stage:public IGameObject
{
public:
	Stage() {};
	~Stage();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	ModelRender m_modelRender;
	PhysicsStaticObject m_physicsStaticObject;
	Vector3					m_position;					//座標。
	Vector3					m_scale{ 5.0f,5.0f,5.0f };					//大きさ。
	Quaternion				m_rotation;					//回転。
};

