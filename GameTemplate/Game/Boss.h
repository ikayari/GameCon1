#pragma once
/*#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"
class Player;//プレイヤークラス

class Boss :public IGameObject
{
public:
	Boss() {};
	~Boss();
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
	/// <summary>
	/// 初期座標を設定。
	/// </summary>
	/// <param name="p">座標</param>
	void SetInitialPosition(const Vector3& p)
	{
		m_InitialPosition = p;
	}
	/// <summary>
	/// 初期回転を設定。
	/// </summary>
	/// <param name="r">回転</param>
	void SetInitialRotation(const Quaternion& r)
	{
		m_InitialRotation = r;
	}
private:
	/// <summary>
	/// ステート管理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// 追跡ステートの管理。
	/// </summary>
	void ManageBattleState();
	/// <summary>
	/// 当たり判定の処理。
	/// </summary>
	void Collision();
	/// <summary>
	/// 攻撃の判定。
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();
	/// <summary>
	/// アニメーションの再生。
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// プレイヤーに向かって移動する。
	/// </summary>
	/// <param name="t">スピード</param>
	void MoveChara(float s);
	/// <summary>
	/// 指定ベクトルに向かって移動する。
	/// </summary>
	/// <param name="s">スピード</param>
	/// <param name="v">前方向ベクトル</param>
	void MoveChara(float s, Vector3 v);
	/// <summary>
	/// 共通のステート遷移
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機中のステート遷移処理。
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 追跡中のステート遷移処理。
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移処理。
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理。
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// 回転攻撃時のステート遷移処理
	/// </summary>
	void ProcessRotateAttackStateTransition();
	/// <summary>
	/// 攻撃中のステート遷移処理。
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// 連続攻撃中のステート遷移処理。
	/// </summary>
	void ProcessAttackComboStateTransition();
	/// <summary>
	/// プレイヤーを探索する。
	/// </summary>
	/// <returns>プレイヤーが見つかったらtrue。</returns>
	const bool SearchPlayer() const;
	/// <summary>
	/// 攻撃できる距離かどうか調べる。
	/// </summary>
	/// <returns>攻撃できるならtrue。</returns>
	const bool IsCanAttack(const int n) const;
	// アニメーションイベント用の関数。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//エネミーステート。
	enum EnEnemyState {
		enEnemyState_Idle,					//待機。
		enEnemyState_Chase,					//追跡。
		enEnemyState_Attack,				//攻撃。
		enEnemyState_RotateAttack,			//回転攻撃。
		enEnemyState_AttackCombo,			//連続攻撃。
		enEnemyState_Dodge,					//回避。
		enEnemyState_ReceiveDamage,			//被ダメージ。
		enEnemyState_Down,					//ダウン。
	};
	EnEnemyState m_enemyState = enEnemyState_Idle;						//ステート
	enum EnAnimationClip {						//アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Walk,					//歩きアニメーション
		enAnimationClip_Run,					//走りアニメーション。
		enAnimationClip_Attack,					//攻撃アニメーション。
		enAnimationClip_RotateAttack,			//回転攻撃アニメーション。
		enAnimationClip_AttackCombo,			//連続攻撃アニメーション。
		enAnimationClip_Down,					//ダウンアニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Backstep,				//バックステップ
		//enAnimationClip_MagicAttack,			//魔法攻撃アニメーション。
		enAnimationClip_Num,					//アニメーションの数。
	};
	enum EnBattleState {
		enBattleState_Idle,						//待機中。
		enBattleState_Chase,					//追跡中。
		enBattleState_Waitingtomove,			//捜索移動中。
		enBattleState_SearchIdle,				//捜索待機。
		enBattleState_Return,					//帰還。
		enBattleState_Attack,					//攻撃。
		enBattleState_Strafe,					//横移動。
		enBattleState_BackStep,					//後ろ回避。
	};
	EnBattleState m_battleState = enBattleState_Idle;					//ステート
	AnimationClip				m_animationClips[enAnimationClip_Num];	//アニメーションクリップ。
	ModelRender				m_modelRender;								//モデルレンダー
	CharacterController		m_characterController;						//キャラクターコントローラー
	Vector3					m_position = { 0.0f,0.0f,50.0f };			//ポジション
	Vector3					m_forward = Vector3::AxisZ;					//エネミーの正面ベクトル。
	Quaternion				m_rotation;									//回転。
	Vector3					m_scale = Vector3::One;						//大きさ。
	Vector3					m_moveSpeed;								//移動速度。
	Player* m_player = nullptr;							//プレイヤー。
	SphereCollider			m_sphereCollider;
	Vector3					m_InitialPosition = Vector3::Zero;			//初期位置。
	Quaternion				m_InitialRotation = Quaternion::Identity;	//初期回転。

	FontRender fontRender;
	FontRender fontRender2;

	TknFile m_tknFile;													//ナビメッシュ
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
	float m_waittimer = 0.0f;											//待機タイマー
	float m_backtimer = 0.0f;											//バックステップタイマー
	int m_hp = 20;														//体力
	bool m_chase = false;												//追いかける。
	bool m_return = false;												//帰還
	bool isEnd;															//パス探索の終了判定
	float Angle;
	Vector3 diff = Vector3::Zero;										//プレイヤーまでの距離。
	Vector3 move = Vector3::Zero;
	int m_BoneId = -1;													//攻撃判定のボーンID 「Character1_RightHand」
	bool m_Attack = false;
	bool m_AttackMove = false;

	float m_backsteptimer = 0.0f;										//バックステップタイマー
};

*/