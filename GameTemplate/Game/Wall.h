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
	/// スケールを設定
	/// </summary>
	/// <param name="s">スケール</param>
	void SetScale(const Vector3& s)
	{
		m_scale = s;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="r">回転</param>
	void SetRotation(const Quaternion& r)
	{
		m_rotation = r;
	}
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="p">座標</param>
	void SetPosition(const Vector3& p)
	{
		m_position = p;
	}
private:
	ModelRender m_modelRender;
	PhysicsStaticObject m_physicsStaticObject;
	Vector3					m_position;					//座標。
	Vector3					m_scale = Vector3::One;					//大きさ。
	Quaternion				m_rotation;					//回転。
};

