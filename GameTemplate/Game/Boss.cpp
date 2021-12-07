#include "stdafx.h"
//#include "Boss.h"
//#include "Player.h"

//#include <time.h>

/*bool Boss::Start()
{
	fontRender.SetPosition(0.0f, 0.0f, 0.0f);
	fontRender2.SetPosition(-40.0f, 0.0f, 0.0f);

	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Enemy/Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/Enemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Enemy/Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/Enemy/slash.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_RotateAttack].Load("Assets/animData/Enemy/Rotating_slash.tka");
	m_animationClips[enAnimationClip_RotateAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_AttackCombo].Load("Assets/animData/Enemy/slash_combo.tka");
	m_animationClips[enAnimationClip_AttackCombo].SetLoopFlag(false);
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

	//「Character1_RightHand」ボーンのID(番号)を取得する。
	m_BoneId = m_modelRender.FindBoneID(L"Character1_RightHand");
	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);
	//乱数を初期化。
	srand((unsigned)time(NULL));

	return true;
}
Boss::~Boss()
{

}
void Boss::Update()
{


	//追跡ステート管理
	ManageBattleState();
	//ステート管理
	ManageState();
	//アニメーションの再生
	PlayAnimation();
	//攻撃
	Attack();
	//当たり判定
	Collision();
	//回転処理
	Rotation();
	//モデルレンダーのアップデート
	m_modelRender.Update();
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d", m_hp);

	//表示するテキストを設定。
	fontRender2.SetText(wcsbuf);
}
void Boss::Rotation()
{
	if (m_battleState != enBattleState_Chase &&
		m_battleState != enBattleState_Attack) {

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
void Boss::ManageState()
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
	case enEnemyState_AttackCombo:
		ProcessAttackComboStateTransition();
		break;
	default:
		break;
	}
}
void Boss::Attack()
{
	//攻撃ステートでないなら処理をしない。
	if (m_enemyState != enEnemyState_Attack &&
		m_enemyState != enEnemyState_RotateAttack &&
		m_enemyState != enEnemyState_AttackCombo)
	{
		return;
	}
	//攻撃中であれば。
	if (m_Attack == true)
	{
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision();
	}
	//攻撃移動がtrueであれば
	if (m_AttackMove == true)
	{
		if (m_enemyState == enEnemyState_AttackCombo)
		{
			MoveChara(75.0f);
		}
		else
		{
			//移動する。
			MoveChara(55.0f);
		}
	}
}
void Boss::ManageBattleState()
{
	if (m_enemyState != enEnemyState_Down &&
		m_enemyState != enEnemyState_ReceiveDamage &&
		m_enemyState != enEnemyState_Attack &&
		m_enemyState != enEnemyState_RotateAttack &&
		m_enemyState != enEnemyState_AttackCombo)
	{
		switch (m_battleState)
		{
		case enBattleState_Idle:
			fontRender.SetText(L"・・・");
			m_chase = false;
			m_return = false;
			break;
		case enBattleState_Chase:
			MoveChara(250.0f);
			if (m_characterController.IsOnGround()) {
				//地面についた。
				//重力を0にする。
				m_moveSpeed.y = 0.0f;
			}
			if (SearchPlayer() == false)
			{
				//見失ったら
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
				isEnd = false;
				m_battleState = enBattleState_Waitingtomove;

			}
			/////////////攻撃処理///////////////////////////////////////////////
			if (IsCanAttack(1) == true)
			{
				//攻撃ステートに遷移する。
				m_battleState = enBattleState_Attack;
				/*int ram = rand() % 100;
				//乱数が70以上なら。
				if (ram > 0)
				{
					m_battleState = enBattleState_Strafe;
				}
				//乱数が70未満なら。
				else {
					//攻撃ステートに遷移する。
					m_battleState = enBattleState_Attack;
				}*/
	/*		}
			fontRender.SetText(L"発見！");
			break;
		case enBattleState_Waitingtomove:
			if (SearchPlayer() == true)
			{*
				//見つけたら追跡再開する。
				m_battleState = enBattleState_Chase;
				break;
			}
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
				m_battleState = enBattleState_SearchIdle;
				break;
			}
			fontRender.SetText(L"パスを移動中");
			break;
		case enBattleState_SearchIdle:
			if (SearchPlayer() == true)
			{
				//見つけたら追跡を再開する。
				m_battleState = enBattleState_Chase;
				break;
			}
			m_waittimer += g_gameTime->GetFrameDeltaTime();
			if (m_waittimer >= 5.0f)
			{
				//数秒経ったら
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
				isEnd = false;
				m_battleState = enBattleState_Return;
				break;
			}

			fontRender.SetText(L"捜索失敗");
			break;
		case enBattleState_Return:
			fontRender.SetText(L"帰還");
			if (m_enemyState != enEnemyState_ReceiveDamage)
			{
				// パス上を移動する。
				m_position = m_path.Move(m_position, 7.0f, isEnd, m_forward);
				m_modelRender.SetPosition(m_position);
				m_characterController.SetPosition(m_position);
				m_characterController.Execute(move, 0.0f);
				//キャラクターの方向を変える。
				m_rotation.SetRotationYFromDirectionXZ(m_forward);
				//絵描きさんに回転を教える。
				m_modelRender.SetRotation(m_rotation);
			}
			if (isEnd == true)
			{
				//帰還判定を初期化。
				m_return = false;
				//初期の回転を適用する。
				m_rotation = m_InitialRotation;
				//追跡ステートを待機にする。
				m_battleState = enBattleState_Idle;
				//エネミーステートを待機にする。
				m_enemyState = enEnemyState_Idle;

				break;
			}
			if (SearchPlayer() == true)
			{
				//見つけたら追跡再開。
				m_battleState = enBattleState_Chase;
				//帰還判定を初期化。
				m_return = false;
			}
			fontRender.SetText(L"帰還");
			break;
		case enBattleState_Attack:
			fontRender.SetText(L"交戦中");
			if (m_enemyState != enEnemyState_Attack &&
				m_enemyState != enEnemyState_RotateAttack &&
				m_enemyState != enEnemyState_AttackCombo)
			{
				MoveChara(75.0f);
			}
			if (IsCanAttack(0) == true)
			{
				//0～99の数値。
				int ram = rand() % 100;
				//乱数が50以上なら。
				if (ram > 66)
				{
					//攻撃ステートに遷移する
					m_enemyState = enEnemyState_RotateAttack;
					return;
				}
				//乱数が50未満なら。
				else if (ram > 33)
				{
					//回転攻撃ステートに遷移する。
					m_enemyState = enEnemyState_Attack;
					return;
				}
				else
				{
					m_enemyState = enEnemyState_AttackCombo;
					return;
				}

			}
			else if (IsCanAttack(1) == false)
			{
				m_battleState = enBattleState_Chase;
				return;
			}
			break;
		case enBattleState_Strafe:
			/*fontRender.SetText(L"達人の間合い");
			//キャラクターの方向を変える。
			m_rotation.SetRotationYFromDirectionXZ(m_player->GetPosition() - m_position);
			//回転を設定。
			m_modelRender.SetRotation(m_rotation);
			m_rotation.Apply(m_forward);
			Vector3 side;

			side.Cross(m_forward, Vector3::AxisY);
			side.Normalize();
			MoveChara(55.0f,side);*/
	/*		break;
		case enBattleState_BackStep:
			fontRender.SetText(L"回避～！");
			MoveChara(-500.0f);
			m_backsteptimer += g_gameTime->GetFrameDeltaTime();
			//キャラクターの方向を変える。
			m_rotation.SetRotationYFromDirectionXZ(m_player->GetPosition() - m_position);
			//回転を設定。
			m_modelRender.SetRotation(m_rotation);
			m_rotation.Apply(m_forward);
			if (m_backsteptimer >= 0.5f)
			{
				m_battleState = enBattleState_Chase;
			}
			break;

		}


	}
}
void Boss::PlayAnimation()
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
		if (m_battleState == enBattleState_Return ||
			m_battleState == enBattleState_Chase ||
			m_battleState == enBattleState_Waitingtomove) {
			m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		}
		else if (m_battleState == enBattleState_Attack)
		{
			m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		}
		else if (m_battleState == enBattleState_BackStep)
		{
			m_modelRender.PlayAnimation(enAnimationClip_Backstep, 0.1f);
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
		m_modelRender.SetAnimationSpeed(2.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
	case enEnemyState_Down:
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	case enEnemyState_RotateAttack:
		m_modelRender.SetAnimationSpeed(1.5f);
		m_modelRender.PlayAnimation(enAnimationClip_RotateAttack, 0.1f);
		break;
	case enEnemyState_AttackCombo:
		m_modelRender.SetAnimationSpeed(0.7f);
		m_modelRender.PlayAnimation(enAnimationClip_AttackCombo, 0.1f);
		break;
	default:
		break;
	}
}
void Boss::MoveChara(float s)
{
	//エネミーからプレイヤーに向かうベクトルを計算する。
	diff = m_player->GetPosition() - m_position;
	//ベクトルを正規化する。
	diff.Normalize();
	//移動速度を設定する。
	m_moveSpeed = diff * s;
	m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);

}
void Boss::MoveChara(float s, Vector3 v)
{
	v.Normalize();
	//移動速度を設定する。
	m_moveSpeed = v * s;
	m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);
}
void Boss::ProcessCommonStateTransition()
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
void Boss::ProcessIdleStateTransition()
{
	//共通のステート遷移
	ProcessCommonStateTransition();
}

void Boss::ProcessChaseStateTransition()
{
	if (m_chase == false)
	{
		m_battleState = enBattleState_Chase;
		m_chase = true;
	}
}
void Boss::ProcessReceiveDamageStateTransition()
{
	m_Attack = false;
	m_AttackMove = false;
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃されたら距離関係無しに追跡させる。
		m_enemyState = enEnemyState_Chase;
		m_battleState = enBattleState_BackStep;
		m_backsteptimer = 0.0f;
	}
}
void Boss::ProcessDownStateTransition()
{
	m_Attack = false;
	m_AttackMove = false;
	m_characterController.RemoveRigidBoby();
}
void Boss::ProcessRotateAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_enemyState = enEnemyState_Chase;
		m_battleState = enBattleState_BackStep;
		m_backsteptimer = 0.0f;
	}
}
void Boss::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_enemyState = enEnemyState_Chase;
		m_battleState = enBattleState_BackStep;
		m_backsteptimer = 0.0f;
	}
}
void Boss::ProcessAttackComboStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_enemyState = enEnemyState_Chase;
		m_battleState = enBattleState_BackStep;
		m_backsteptimer = 0.0f;
	}
}
void Boss::Collision()
{
	//被ダメージ時は
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
			else if (m_hp <= 0)
			{
				//ダウンステートに遷移する。
				m_enemyState = enEnemyState_Down;
			}
		}
	}
}
void Boss::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

	fontRender.Draw(rc);
	fontRender2.Draw(rc);
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
const bool Boss::SearchPlayer() const
{
	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;
	if (diff.Length() >= 2000.0f)
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
void Boss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//キーの名前が[attack_start]の時。
	if (wcscmp(eventName, L"attack_start") == 0) {
		//攻撃中判定をtrueにする。
		m_Attack = true;
	}
	//キーの名前が[attack_end]の時。
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//攻撃中判定をfalseにする。
		m_Attack = false;
	}
	//キーの名前が[move_start]の時。
	if (wcscmp(eventName, L"move_start") == 0) {
		//移動判定をtrueにする。
		m_AttackMove = true;
	}
	//キーの名前が[move_end]の時。
	else if (wcscmp(eventName, L"move_end") == 0) {
		//移動判定をfalseにする。
		m_AttackMove = false;
	}
}
void Boss::MakeAttackCollision()
{
	//攻撃当たり判定用のコリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	//腕のボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_BoneId)->GetWorldMatrix();
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(115.0f, 10.0f, 10.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
}

const bool Boss::IsCanAttack(const int n) const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//ボスとプレイヤーの距離が近かったら。
	if (n == 0)
	{
		if (diff.LengthSq() <= 150.0f * 100.0f)
		{
			//攻撃できる！
			return true;
		}
	}
	else if (n == 1)
	{
		if (diff.LengthSq() <= 600.0f * 100.0f)
		{
			return true;
		}
	}
	//攻撃できない。
	return false;
}

*/