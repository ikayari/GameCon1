#pragma once
class Wall:public IGameObject
{
public:
	Wall() {};
	~Wall();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// �X�P�[����ݒ�
	/// </summary>
	/// <param name="s">�X�P�[��</param>
	void SetScale(const Vector3& s)
	{
		m_scale = s;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="r">��]</param>
	void SetRotation(const Quaternion& r)
	{
		m_rotation = r;
	}
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="p">���W</param>
	void SetPosition(const Vector3& p)
	{
		m_position = p;
	}
private:
	ModelRender m_modelRender;
	PhysicsStaticObject m_physicsStaticObject;
	Vector3					m_position;					//���W�B
	Vector3					m_scale = Vector3::One;					//�傫���B
	Quaternion				m_rotation;					//��]�B
};

