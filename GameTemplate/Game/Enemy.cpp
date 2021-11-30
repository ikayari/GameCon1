#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"

bool Enemy::Start()
{
	fontRender.SetPosition(0.0f, 0.0f, 0.0f);





	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Enemy/Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Enemy/Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/Enemy/slash.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_RotateAttack].Load("Assets/animData/Enemy/Rotating_slash.tka");
	m_animationClips[enAnimationClip_RotateAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/Enemy/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Backstep].Load("Assets/animData/Enemy/backstep.tka");
	m_animationClips[enAnimationClip_Backstep].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/Enemy/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);

	//モデルレンダーの読みこみ
	m_modelRender.Init("Assets/modelData/Enemy/shadownity.tkm", m_animationClips, enAnimationClip_Num);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	//アニメーションイベント用の関数
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_characterController.Init(25.0f, 75.0f, m_position);
	m_rotation.Apply(m_forward);
	m_player = FindGO<Player>("player");
	// ナビメッシュを構築。
	m_nvmMesh.Init("Assets/modelData/dangeon/dangeonnav.tkn");

	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);
	return true;
}
Enemy::~Enemy()
{

}
void Enemy::Update()
{

	//ステート管理
	ManageState();
	//追跡ステート管理
	ManageChaseState();
	//アニメーションの再生
	PlayAnimation();
	//当たり判定
	Collision();
	//回転処理
	Rotation();
	//モデルレンダーのアップデート
	m_modelRender.Update();
}
void Enemy::Chase()
{

	
}
void Enemy::Rotation()
{
	if (m_chaseState!=enChaseState_Chase) {
		//追跡中でなければ回転処理は必要ない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}
void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機ステート
	case enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
		//攻撃ステート
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
		//追跡ステート
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
		//被ダメージステート
	case enEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//ダウンステート
	case enEnemyState_Down:
		ProcessDownStateTransition();
		break;
		//回転攻撃ステート
	case enEnemyState_RotateAttack:
		ProcessRotateAttackStateTransition();
		break;
		//回避ステート
	case enEnemyState_Dodge:
		ProcessDodgeStateTransition();
		break;
	default:
		break;
	}
}
void Enemy::ManageChaseState()
{
	if (m_enemyState != enEnemyState_Down)
	{
		switch (m_chaseState)
		{
		case enChaseState_Idle:
			fontRender.SetText(L"・・・");
			m_chase = false;
			break;
		case enChaseState_Chase:
			//エネミーからプレイヤーに向かうベクトルを計算する。
			diff = m_player->GetPosition() - m_position;
			//ベクトルを正規化する。
			diff.Normalize();
			//移動速度を設定する。
			m_moveSpeed = diff * 250.0f;
			m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
			m_modelRender.SetPosition(m_position);
			if (m_characterController.IsOnGround()) {
				//地面についた。
				//重力を0にする。
				m_moveSpeed.y = 0.0f;
			}
			if (SearchPlayer() == false)
			{
				//見失ったらステートを遷移。
				m_chaseState = enChaseState_Pathmove;

			}
			fontRender.SetText(L"発見！");
			break;
		case enChaseState_Pathmove:
			isEnd = false;
			isEnd2 = false;
			fontRender.SetText(L"パアアアアアアアアアアアアアアアアアアアス探索");
			//帰還中じゃなければ
			if (m_return == false)
			{
				m_path.Clear();
				// パス検索 見えなくなった地点まで。
				m_pathFiding.Execute(
					m_path,							// 構築されたパスの格納先
					m_nvmMesh,						// ナビメッシュ
					m_position,						// 開始座標
					m_player->GetPosition(),		// 移動目標座標プレイヤーの位置。
					PhysicsWorld::GetInstance(),	// 物理エンジン	
					25.0f,							// AIエージェントの半径
					75.0f							// AIエージェントの高さ。
				);
				m_chaseState = enChaseState_Waitingtomove;
				return;
			}
			else
			{
				m_path.Clear();
				// パス検索 初期位置に帰る。
				m_pathFiding.Execute(
					m_path,							// 構築されたパスの格納先
					m_nvmMesh,						// ナビメッシュ
					m_position,						// 開始座標
					m_InitialPosition,				// 移動目標座標は初期位置。
					PhysicsWorld::GetInstance(),	// 物理エンジン	
					25.0f,							// AIエージェントの半径
					75.0f							// AIエージェントの高さ。
				);
				m_chaseState = enChaseState_Return;
				return;
			}



			break;
		case enChaseState_Waitingtomove:
			// パス上を移動する。
			m_position = m_path.Move(m_position, 7.0f, isEnd, m_forward);
			m_modelRender.SetPosition(m_position);
			//キャラクターの方向を変える。
			m_rotation.SetRotationYFromDirectionXZ(m_forward);
			//回転を設定。
			m_modelRender.SetRotation(m_rotation);
			//キャラクターコントローラーを動かす。
			m_characterController.SetPosition(m_position);
			m_characterController.Execute(move, 0.0f);

			if (isEnd == true)
			{
				m_waittimer = 0.0f;
				m_chaseState = enChaseState_SearchIdle;
				break;
			}
			if (SearchPlayer() == true)
			{
				//見つけたら追跡再開する。
				m_chaseState = enChaseState_Chase;
				break;
			}
			fontRender.SetText(L"パスを移動中");
			break;
		case enChaseState_SearchIdle:
			m_waittimer += g_gameTime->GetFrameDeltaTime();
			if (m_waittimer >= 5.0f)
			{
				m_return = true;
				m_chaseState = enChaseState_Pathmove;

				break;
			}
			if (SearchPlayer() == true)
			{
				//見つけたら追跡を再開する。
				m_chaseState = enChaseState_Chase;
				break;
			}
			fontRender.SetText(L"捜索失敗");
			break;
		case enChaseState_Return:
			// パス上を移動する。
			m_position = m_path.Move(m_position, 7.0f, isEnd2, m_forward);
			m_modelRender.SetPosition(m_position);
			m_characterController.SetPosition(m_position);
			m_characterController.Execute(move, 0.0f);
			//キャラクターの方向を変える。
			m_rotation.SetRotationYFromDirectionXZ(m_forward);
			//絵描きさんに回転を教える。
			m_modelRender.SetRotation(m_rotation);
			if (isEnd2 == true)
			{
				//帰還判定を初期化。
				m_return = false;
				//初期の回転を適用する。
				m_rotation = m_InitialRotation;
				//追跡ステートを待機にする。
				m_chaseState = enChaseState_Idle;
				//エネミーステートを待機にする。
				m_enemyState = enEnemyState_Idle;

				break;
			}
			if (SearchPlayer() == true)
			{
				//見つけたら追跡再開。
				m_chaseState = enChaseState_Chase;
				//帰還判定を初期化。
				m_return = false;
			}
			fontRender.SetText(L"帰還");
			break;

		}
	}
}
void Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemyState)
	{
	case enEnemyState_Idle:
		//待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
	case enEnemyState_Chase:
		//走りアニメーションを再生。
		if (m_chaseState==enChaseState_Return||
			m_chaseState==enChaseState_Chase||
			m_chaseState==enChaseState_Waitingtomove){
			m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		}
		else {
			//待機アニメーションを再生。
			m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		}
		break;
	case enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	case enEnemyState_ReceiveDamage:
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
	case enEnemyState_Dodge:
		m_modelRender.PlayAnimation(enAnimationClip_Backstep, 0.1f);
		break;
	case enEnemyState_Down:
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	case enEnemyState_RotateAttack:
		m_modelRender.PlayAnimation(enAnimationClip_RotateAttack, 0.1f);
		break;
	default:
		break;
	}
}
void Enemy::ProcessCommonStateTransition()
{

	//プレイヤーを見つけたら。
	if (SearchPlayer() == true)
	{		
		//追跡ステートに遷移する。
		m_enemyState = enEnemyState_Chase;

		return;
	}
	//プレイヤーを見つけられなければ。
	else
	{
		return;
	}
}
void Enemy::ProcessIdleStateTransition()
{
	//共通のステート遷移
	ProcessCommonStateTransition();
}
void Enemy::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}
void Enemy::ProcessChaseStateTransition()
{
	if (m_chase == false)
	{
		m_chaseState = enChaseState_Chase;
		m_chase = true;
	}
}
void Enemy::ProcessReceiveDamageStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃されたら距離関係無しに追跡させる。
		m_enemyState = enEnemyState_Chase;
		m_chaseState = enChaseState_Chase;
	}
}
void Enemy::ProcessDownStateTransition()
{
	m_characterController.RemoveRigidBoby();
}
void Enemy::ProcessRotateAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}
void Enemy::Collision()
{
	//被ダメージ、あるいはダウンステートの時は。
	//当たり判定処理はしない。
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		return;
	}

	//プレイヤーの攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_characterController))
		{
			//HPを1減らす。
			m_hp -= 1;

			//もしHPが0より上なら。
			if (m_hp > 0)
			{
				//被ダメージステートに遷移する。
				m_enemyState = enEnemyState_ReceiveDamage;
			}
			//HPが0なら。
			else if (m_hp == 0)
			{
				//ダウンステートに遷移する。
				m_enemyState = enEnemyState_Down;
			}
		}
	}
}
void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

	fontRender.Draw(rc);
}
void Enemy::ProcessDodgeStateTransition()
{
	
}
//衝突したときに呼ばれる関数オブジェクト(壁用)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//壁とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//衝突したのは壁ではない。
			return 0.0f;
		}

		//壁とぶつかったら。
		//フラグをtrueに。
		isHit = true;
		return 0.0f;
	}
};
const bool Enemy::SearchPlayer() const
{
	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;
	if (diff.Length() >= 700.0f)
	{
		//遠いので見えない。
		return false;
	}
	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));
	//プレイヤーが視界内に居なかったら。
	if (Math::PI * 0.35f <= fabsf(angle))
	{
		//プレイヤーは見つかっていない。
		return false;
	}

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(playerPosition.x, playerPosition.y + 70.0f, playerPosition.z));

	SweepResultWall callback;
	//コライダーを始点から終点まで動かして。
	//衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//壁と衝突した！
	if (callback.isHit == true)
	{
		//プレイヤーは見つかっていない。
		return false;
	}

	//壁と衝突してない！！
	//プレイヤー見つけたフラグをtrueに。
	return true;
}
void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	
}