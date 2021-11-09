#include "stdafx.h"
#include "Player.h"

bool Player::Start()
{
	m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_characterController.Init(25.0f, 75.0f, m_position);
	m_modelRender.SetPosition(m_position);
	mapRender.Init("Assets/sprite/map.dds", 500.0f, 500.0f);
	playerRender.Init("Assets/sprite/player.dds", 50.0f, 50.0f);
	return true;
}
Player::~Player()
{

}
void Player::Update()
{
	Move();
	Rotation();
	m_modelRender.Update();
	playerRender.SetPosition({m_position.x,m_position.z,0.0f});
	playerRender.Update();

	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"x:%5f",m_position.x);

	//表示するテキストを設定。
	m_positionrender[0].SetText(wcsbuf);
	//フォントの位置を設定。
	m_positionrender[0].SetPosition(Vector3(200.0f, 200.0f, 0.0f));
	//フォントの大きさを設定。
	m_positionrender[0].SetScale(2.0f);

	wchar_t wcsbuf1[256];
	swprintf_s(wcsbuf1, 256, L"y:%5f", m_position.y);

	//表示するテキストを設定。
	m_positionrender[1].SetText(wcsbuf1);
	//フォントの位置を設定。
	m_positionrender[1].SetPosition(Vector3(200.0f, 250.0f, 0.0f));
	//フォントの大きさを設定。
	m_positionrender[1].SetScale(2.0f);

	wchar_t wcsbuf2[256];
	swprintf_s(wcsbuf2, 256, L"z:%5f", m_position.z);

	//表示するテキストを設定。
	m_positionrender[2].SetText(wcsbuf2);
	//フォントの位置を設定。
	m_positionrender[2].SetPosition(Vector3(200.0f, 300.0f, 0.0f));
	//フォントの大きさを設定。
	m_positionrender[2].SetScale(2.0f);
}
void Player::Move()
{
	//xzの移動速度を0.0fにする。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//左スティックの入力量を取得。
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();
	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;
	forward.Normalize();
	right.Normalize();
	//左スティックの入力量と120.0fを乗算。
	right *= stickL.x * 360.0f;
	forward *= stickL.y * 360.0f;
	//移動速度に上記で計算したベクトルを加算する。
	m_moveSpeed += right + forward;
	if (m_characterController.IsOnGround())
	{
		m_moveSpeed.y = 0.0f;
	}
	else
	{
		m_moveSpeed.y -= 5.0f;
	}
	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);
}
void Player::Rotation()
{
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		
		m_modelRender.SetRotation(m_rotation);
		Quaternion renderrot;
		renderrot.SetRotationZ(m_rotation.y);
		playerRender.SetRotation(renderrot);
	}
}
void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	mapRender.Draw(rc);
	playerRender.Draw(rc);
	m_positionrender[0].Draw(rc);
	m_positionrender[1].Draw(rc);
	m_positionrender[2].Draw(rc);
}