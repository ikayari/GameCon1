#include "stdafx.h"
#include "Player.h"

bool Player::Start()
{


	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	/*m_animationClips[enAnimationClip_Attack].Load("Assets/animData/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/receivedamage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Winner].Load("Assets/animData/winner.tka");
	m_animationClips[enAnimationClip_Winner].SetLoopFlag(false);*/

	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimationClip_Num);
	m_characterController.Init(25.0f, 75.0f, m_position);
	m_modelRender.SetPosition(m_position);

	m_mapRender.Init("Assets/sprite/map.dds", 315.0f, 315.0f);
	m_playerRender.Init("Assets/sprite/player.dds", 43.0f, 43.0f);
	//レベルを構築する。
	m_level2DRender.Init("Assets/sprite/UI/HUD.casl", [&](Level2DObjectData& objData) {
		//名前画一致していたら。
		if (objData.EqualObjectName("HUD") == true) {


			//return falseにすると、Level2D側で画像が読み込まれます。
			return false;
		}
		else if (objData.EqualObjectName("unitychan") == true) {


			//return falseにすると、Level2D側で画像が読み込まれます。
			return false;
		}
		else if (objData.EqualObjectName("HP") == true) {

			//レベルのデータを使用して画像を読み込む。
			m_HPRender[0].Init(objData.ddsFilePath, objData.width, objData.height);
			m_HPRender[0].SetScale(objData.scale);
			m_HPRender[0].SetPosition(objData.position);
			m_HPRender[0].SetPivot(objData.pivot);
			m_HPRender[1].Init(objData.ddsFilePath, objData.width, objData.height);
			m_HPRender[1].SetScale(objData.scale);
			m_HPRender[1].SetPosition(objData.position);
			m_HPRender[1].SetPivot(objData.pivot);
			//自身で画像を読み込みたい時は必ずtrueにしてください。
			return true;
		}
		else if (objData.EqualObjectName("MP") == true) {

			//レベルのデータを使用して画像を読み込む。
			m_MPRender[0].Init(objData.ddsFilePath, objData.width, objData.height);
			m_MPRender[0].SetScale(objData.scale);
			m_MPRender[0].SetPosition(objData.position);
			m_MPRender[0].SetPivot(objData.pivot);
			m_MPRender[1].Init(objData.ddsFilePath, objData.width, objData.height);
			m_MPRender[1].SetScale(objData.scale);
			m_MPRender[1].SetPosition(objData.position);
			m_MPRender[1].SetPivot(objData.pivot);
			//自身で画像を読み込みたい時は必ずtrueにしてください。
			return true;
		}
		return false;
		});
	m_HPfontRender.SetPosition({ -540.0f,-365.0f,0.0f });
	m_MPfontRender.SetPosition({ -540.0f,-440.0f,0.0f });
	m_HPRender[1].SetMulColor({ 0.0f,0.0f,0.0f,1.0f });
	m_MPRender[1].SetMulColor({ 0.0f,0.0f,0.0f,1.0f });
	return true;
}
Player::~Player()
{

}
void Player::Update()
{
	Move();
	Rotation();
	Debug();
	UI();
	UIRenderUpdates();
	ManageState();
	PlayAnimation();
	m_MP -= 1;
	m_HP -= 50;
}
void Player::UI()
{
	m_playerRender.SetPosition({ m_position.x * 0.136f + m_mapposition.x ,m_position.z * 0.136f + m_mapposition.y,0.0f });
	m_mapRender.SetPosition(m_mapposition);

	wchar_t wcsbuf5[256];
	swprintf_s(wcsbuf5, 256, L"%4d", m_HP);
	m_HPfontRender.SetText(wcsbuf5);
	m_HPfontRender.SetScale(1.0f);
	m_HPfontRender.SetColor({ 0.0f,0.0f,0.0f,1.0f });

	wchar_t wcsbuf6[256];
	swprintf_s(wcsbuf6, 256, L"%4d", m_MP);
	m_MPfontRender.SetText(wcsbuf6);
	m_MPfontRender.SetScale(1.0f);
	m_MPfontRender.SetColor({ 0.0f,0.0f,0.0f,1.0f });

	float hp = m_InitialHP - m_HP;//減った体力
	m_scale.x = 1.0f - (1.0f / float(m_InitialHP)) * hp;
	m_HPRender[0].SetScale(m_scale);

	float mp = m_InitialMP - m_MP;//減った体力
	m_scale2.x = 1.0f - (1.0f / float(m_InitialMP)) * mp;
	m_MPRender[0].SetScale(m_scale2);
	
}
void Player::ManageState() 
{
	switch (m_playerState)
	{
	case enPlayerState_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//歩きステートの時。
	case enPlayerState_Walk:
		//歩きステートのステート遷移処理。
		ProcessWalkStateTransition();
		break;
		//走りステートの時。
	case enPlayerState_Run:
		//走りステートのステート遷移処理。
		ProcessRunStateTransition();
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		m_playerState = enPlayerState_Walk;
		if (g_pad[0]->IsPress(enButtonA))
		{
			m_playerState = enPlayerState_Run;
			return;
		}
	}
	else
	{
		m_playerState = enPlayerState_Idle;
		return;
	}
	
}

void Player::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_playerState)
	{
		//待機ステートの時。
	case enPlayerState_Idle:
		//待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//歩きステートの時。
	case enPlayerState_Walk:
		m_modelRender.SetAnimationSpeed(1.2f);
		//歩きアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//走りステートの時。
	case enPlayerState_Run:
		m_modelRender.SetAnimationSpeed(1.2f);
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
	}
}


void Player::UIRenderUpdates()
{
	m_modelRender.Update();
	m_mapRender.Update();
	m_playerRender.Update();
	m_HPRender[1].Update();
	m_MPRender[1].Update();
	m_HPRender[0].Update();
	m_MPRender[0].Update();
	m_level2DRender.Update();
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
	}
}
void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_mapRender.Draw(rc);
	m_playerRender.Draw(rc);
	m_positionrender[0].Draw(rc);
	m_positionrender[1].Draw(rc);
	m_positionrender[2].Draw(rc);
	m_level2DRender.Draw(rc);
	m_HPRender[1].Draw(rc);
	m_MPRender[1].Draw(rc);
	m_HPRender[0].Draw(rc);
	m_MPRender[0].Draw(rc);
	m_HPfontRender.Draw(rc);
	m_MPfontRender.Draw(rc);
}
void Player::Debug()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"x:%5f", m_position.x);

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