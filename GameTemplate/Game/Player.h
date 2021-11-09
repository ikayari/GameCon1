#pragma once
class Player:public IGameObject
{
public:
	Player() {};
	~Player();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Move();
	void Rotation();
	Vector3 Getposition() { return m_position; };
private:
	ModelRender m_modelRender;
	CharacterController	m_characterController;
	Vector3 m_position;
	Vector3 m_moveSpeed;
	Quaternion m_rotation;
	SpriteRender mapRender;
	SpriteRender playerRender;
	FontRender m_positionrender[3];
	Quaternion rotation;
};

