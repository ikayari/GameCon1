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
	Vector3					m_position;					//���W�B
	Vector3					m_scale{ 5.0f,5.0f,5.0f };					//�傫���B
	Quaternion				m_rotation;					//��]�B
};

