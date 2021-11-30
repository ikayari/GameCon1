#include "stdafx.h"
#include "Player.h"

bool Player::Start()
{


	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Player/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/Player/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Player/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Punch].Load("Assets/animData/Player/punch.tka");
	m_animationClips[enAnimationClip_Punch].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Guard].Load("Assets/animData/Player/barrier.tka");
	m_animationClips[enAnimationClip_Guard].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/Player/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	/*m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Winner].Load("Assets/animData/winner.tka");
	m_animationClips[enAnimationClip_Winner].SetLoopFlag(false);*/

	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimationClip_Num);
	m_characterController.Init(25.0f, 75.0f, m_position);
	m_modelRender.SetPosition(m_position);
	m_mapRender.Init("Assets/sprite/map.dds", 315.0f, 315.0f);
	m_playerRender.Init("Assets/sprite/player.dds", 15.0f, 15.0f);
	//レベルを構築する。
	m_level2DRender.Init("Assets/sprite/UI/HUD.casl", [&](Level2DObjectData& objData) {
		//名前画一致していたら。
		if (objData.EqualObjectName("HUD") == true) {


			//Level2D側で画像読み込み。			
			return false;
		}
		else if (objData.EqualObjectName("unitychan") == true) {


			//Level2D側で画像読み込み。
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
			return true;
		}
		return false;
		});
	m_HPfontRender.SetPosition({ -540.0f,-365.0f,0.0f });
	m_MPfontRender.SetPosition({ -540.0f,-440.0f,0.0f });
	m_HPRender[1].SetMulColor({ 0.0f,0.0f,0.0f,1.0f });
	m_MPRender[1].SetMulColor({ 0.0f,0.0f,0.0f,1.0f });
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//「Character1_RightHand」ボーンのID(番号)を取得する。
	m_PunchBoneId = m_modelRender.FindBoneID(L"Character1_RightHand");
	return true;
}
Player::~Player()
{

}
void Player::Update()
{
	Move();//移動処理
	Rotation();//回転処理
	Debug();//デバッグ用
	UI();//UI
	UIRenderUpdates();//UIRenderアップデート
	Punch();//殴り処理
	Guard();//ガード処理
	PlayAnimation();//アニメーション再生
	ManageState();//ステート管理
}
void Player::UI()
{
	m_playerRender.SetPosition({ m_position.x * 0.059f + m_mapposition.x ,m_position.z * 0.059f + m_mapposition.y,0.0f });
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
	m_scale1.x = 1.0f - (1.0f / float(m_InitialHP)) * hp;
	m_HPRender[0].SetScale(m_scale1);

	float mp = m_InitialMP - m_MP;//減った魔力
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
		//移動速度を設定。
		SetMoveSpeed(620.0f);
		break;
		//走りステートの時。
	case enPlayerState_Run:
		//走りステートのステート遷移処理。
		ProcessRunStateTransition();
		//移動速度を設定。
		SetMoveSpeed(1200.0f);
		break;
	case enPlayerState_Punch:
		//パンチステートのステート遷移処理。
		ProcessPunchStateTransition();
		//移動速度を設定
		SetMoveSpeed(180.0f);
		break;
	case enPlayerState_Guard:
		//ガードステートのステート遷移処理
		ProcessGuardStateTransition();
		//移動速度を設定
		SetMoveSpeed(45.0f);
		break;
	case enPlayerState_ReceiveDamage:
		//被ダメージ時のステート遷移処理

		
		break;
	default:
		break;
	}
}
void Player::Collision()
{
	//被ダメージ、ダウン中、クリア時はダメージ判定をしない。
	if (m_playerState == enPlayerState_ReceiveDamage ||
		m_playerState == enPlayerState_Down ||
		m_playerState == enPlayerState_Clear)
	{
		return;
	}
	//敵の攻撃用のコリジョンの配列を取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	//配列をfor分で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_characterController))
		{
			//HPを減らす。
			m_HP -= 50;
			///HPが０になったら。
			if (m_HP <= 0)
			{
				//ダウンさせる。
				m_playerState = enPlayerState_Down;
				m_HP = 0;
			}
			//HPが０ではなかったら。
			else
			{
				//被ダメージリアクションを指せる。
				m_playerState = enPlayerState_ReceiveDamage;
			}
			//効果音を再生する。
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(7);
			se->Play(false);
			se->SetVolume(0.7f);
			return;
		}
	}
	//敵の撃ったファイヤーボールのコリジョンの配列を取得する。
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("enemy_fireball");
	//配列をfor文で回す。
	for (auto collision : collisions2)
	{
		if (collision->IsHit(m_characterController))
		{
			//HPを減らす。
			m_HP -= 20;
			///HPが０になったら。
			if (m_HP == 0)
			{
				//ダウンさせる。
				m_playerState = enPlayerState_Down;
			}
			//HPが０ではなかったら。
			else
			{
				//被ダメージリアクションを指せる。
				m_playerState = enPlayerState_ReceiveDamage;
			}
			//効果音を再生する。
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(7);
			se->Play(false);
			se->SetVolume(0.7f);
			return;
		}
	}

}
void Player::ProcessCommonStateTransition()
{
	//LB1ボタンを押すとガード
	if (g_pad[0]->IsPress(enButtonLB1))
	{
		m_playerState = enPlayerState_Guard;
		m_isDefending = true;
		return;
	}
	//Bボタンを押したら殴り。
	if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		m_playerState = enPlayerState_Punch;
		return;
	}
	//移動しているとき
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//Aボタンを押すと走る。
		if (g_pad[0]->IsPress(enButtonA))
		{
			m_playerState = enPlayerState_Run;
			return;
		}
		//押していないときは歩く。
		else
		{
			m_playerState = enPlayerState_Walk;
			return;
		}

	}
	//移動していないときは待機モーション
	else
	{
		m_playerState = enPlayerState_Idle;
		return;
	}
	
	
}

void Player::ProcessIdleStateTransition()
{
	//ステート遷移処理
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	//ステート遷移処理
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::ProcessPunchStateTransition() 
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
	else
	{
		//LB1ボタンを押すとガード
		if (g_pad[0]->IsPress(enButtonLB1))
		{
			//ステートを遷移
			ProcessCommonStateTransition();
			m_isDefending = true;
			return;
		}
	}
}

void Player::ProcessGuardStateTransition()
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

	case enPlayerState_Punch:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Punch, 0.1f);
		break;
	case enPlayerState_Guard:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Guard);
	default:
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
	//カメラの前方向と右方向のベクトル。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;
	//正規化
	forward.Normalize();
	right.Normalize();
	//左スティックの入力量と移動速度を乗算
	right *= stickL.x * GetMoveSpeed();
	forward *= stickL.y * GetMoveSpeed();
	//移動速度加算
	m_moveSpeed += right + forward;
	//地面にいるときは
	if (m_characterController.IsOnGround())
	{
		m_position.y = 0.0f;
	}
	else
	{
		m_position.y -= 5.0f;
	}
	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);
}
void Player::Rotation()
{
	//移動速度があるとき
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{	
		//回転を作成
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//回転を適用
		m_modelRender.SetRotation(m_rotation);
	}
}
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//キーの名前が「attack_start」の時。
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//攻撃中にする。
		m_isUnderAttack = true;
	}
	//キーの名前が「attack_end」のとき
	if (wcscmp(eventName, L"attack_end") == 0)
	{
		//攻撃を終わる。
		m_isUnderAttack = false;
	}
}
void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_mapRender.Draw(rc);
	m_playerRender.Draw(rc);


	//デバッグ用
	m_positionrender[0].Draw(rc);
	m_positionrender[1].Draw(rc);
	m_positionrender[2].Draw(rc);
	m_justGuardRender.Draw(rc);

	m_level2DRender.Draw(rc);
	m_HPRender[1].Draw(rc);
	m_MPRender[1].Draw(rc);
	m_HPRender[0].Draw(rc);
	m_MPRender[0].Draw(rc);
	m_HPfontRender.Draw(rc);
	m_MPfontRender.Draw(rc);
}
void Player::Punch()
{
	//攻撃中でないなら、処理をしない。
	if (m_playerState != enPlayerState_Punch)
	{
		return;
	}

	//攻撃判定中であれば。
	if (m_isUnderAttack == true)
	{
		//攻撃用のコリジョンを作成する。
		MakePunchCollision();
	}
}
void Player::MakePunchCollision()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前に設定する。
	collisionPosition += m_forward * 50.0f;
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(collisionPosition,               //座標。
		Quaternion::Identity,                                       //回転。
		Vector3(110.0f, 15.0f, 15.0f)                               //大きさ。
	);
	collisionObject->SetName("player_attack");
	//「Sword」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_PunchBoneId)->GetWorldMatrix();
	//「Sword」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}
void Player::Guard()
{
	//ガード中でないなら、処理をしない。
	if (m_playerState != enPlayerState_Guard)
	{
		m_guardcooltimer+=g_gameTime->GetFrameDeltaTime();//クールタイマーを起動。
		m_guardtimer = 0;//ガード時間をリセット。
		return;
	}
	//ガード中であれば。
	if (m_isDefending == true)
	{
		//ガード用のコリジョンを作成する。
		MakeGuardCollision();
		//攻撃用のコリジョンの生成を止める。
		m_isUnderAttack = false;
		m_guardtimer += g_gameTime->GetFrameDeltaTime();//ガード時間を計測。
		m_guardcooltimer = 0;//クールタイマーをリセット。
	}
}
void Player::MakeGuardCollision()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前、少し上側に設定
	collisionPosition += m_forward * 25.0f;
	collisionPosition.y += 90.0f;
	//コリジョンからプレイヤーまでのベクトルを計算
	Vector3 topos = collisionPosition - m_position;
	//前述のベクトルにプレイヤーの回転を適用
	m_rotation.Apply(topos);
	//スフィアコリジョンを生成
	collisionObject->CreateBox(m_position + topos,               //座標。
		m_rotation,											    //回転。
		Vector3(50.0f,180.0f,25.0f)					           //大きさ。
	);
	collisionObject->SetName("player_guard");
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