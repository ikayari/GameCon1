#pragma once
#include "level2DRender/Level2DRender.h"


class Player:public IGameObject
{
public:
	Player() {};
	~Player();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 重力を設定する。
	/// </summary>
	/// <param name="g"></param>
	/// <returns></returns>
	void SetGravity(const float& g)
	{
		m_moveSpeed.y -= g; 
	}
	/// <summary>
	/// 重力を取得
	/// </summary>
	/// <returns></returns>
	const float&  GetGravity()const
	{
		return m_moveSpeed.y;
	}
	/// <summary>
	/// 移動速度を取得する。
	/// </summary>
	const float& GetMoveSpeed()const
	{
		return m_speed;
	}
	/// <summary>
	/// 移動速度を設定する。
	/// </summary>
	/// <param name="s">移動速度</param>
	void  SetMoveSpeed(const float& s)
	{
		m_speed = s;
	}
	/// <summary>
	/// 回転を設定する。
	/// </summary>
	/// <param name="r">回転</param>
	void SetRotation(const Quaternion& r)
	{
		m_rotation = r;
	}
	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>座標。</returns>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}
private:
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Punch();
	/// <summary>
	/// 攻撃のコリジョン生成
	/// </summary>
	void MakePunchCollision();
	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void Collision();
	/// <summary>
	/// ガード処理
	/// </summary>
	void Guard();
	/// <summary>
	/// ガードのコリジョン生成
	/// </summary>
	void MakeGuardCollision();
	/// <summary>
	/// プレイヤー移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// プレイヤー回転処理
	/// </summary>
	void Rotation();
	/// <summary>
	/// デバッグ用。
	/// </summary>
	void Debug();
	/// <summary>
	/// UIRender関係のUpdate
	/// </summary>
	void UIRenderUpdates();
	/// <summary>
	/// UIの処理。
	/// </summary>
	void UI();
	/// <summary>
	/// ステート管理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// 共通のステート遷移
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機ステートの遷移
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 歩きステートの遷移
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// 走りステートの遷移
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	///	パンチステートの遷移
	/// </summary>
	void ProcessPunchStateTransition();
	/// <summary>
	/// ガードステートの遷移
	/// </summary>
	void ProcessGuardStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
	// アニメーションイベント用の関数。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	ModelRender				m_modelRender;								//モデルレンダー
	CharacterController		m_characterController;						//キャラクターコントローラー
	Vector3					m_position;									//ポジション
	Vector3					m_moveSpeed;								//動くスピード
	Vector3					m_scale1 = Vector3::One;					//スケール
	Vector3					m_scale2 = Vector3::One;
	Vector3					m_mapposition = { -751.0f,344.0f,0.0f };
	Vector3					m_forward = Vector3::AxisZ;					//プレイヤーの正面ベクトル。
	Quaternion m_rotation;

	SpriteRender m_mapRender;
	SpriteRender m_playerRender;
	SpriteRender m_GPRender[2];
	SpriteRender m_HPRender[2];

	FontRender m_positionrender[3];
	FontRender m_HPfontRender;
	FontRender m_GPfontRender;
	FontRender m_justGuardRender;

	Level2DRender m_level2DRender;

	int						m_HP = 8000;						//体力数値
	int						m_GP = 200;							//防御数値
	int						m_InitialHP = 8000;					//体力初期値
	int						m_InitialGP = 200;					//防御初期値
	bool					m_isUnderAttack = false;			//攻撃中？
	bool					m_isDefending = false;				//防御中？
	bool					m_SuccessDefence = false;			//防御成功？
	int                     m_PunchBoneId = -1;                 //「RightHand」ボーンのID。     
	float					m_speed=360.0f;						//移動速度
	float					m_guardtimer=0.0f;					//防御タイマー
	float					m_guardcooltimer=0.0f;				//連続ガード判定タイマー
	//アニメーションクリップ	
	enum EnAnimationClip {
		enAnimationClip_Idle,				//待機アニメーション。	
		enAnimationClip_Walk,				//歩きアニメーション。
		enAnimationClip_Run,				//走りアニメーション。
		enAnimationClip_Punch,				//攻撃アニメーション。
		enAnimationClip_Guard,				//防御アニメーション。
		enAnimationClip_Damage,				//被ダメージアニメーション。
		/*enAnimationClip_MagicAttack,		//魔法攻撃アニメーション。
		enAnimationClip_Down,				//ダウンアニメーション。
		enAnimationClip_Winner,				//勝利アニメーション。*/
		enAnimationClip_Num,				//アニメーションの数。
	};
	AnimationClip m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	//プレイヤーステート
	enum EnPlayerState {
		enPlayerState_Idle,					//待機。
		enPlayerState_Walk,					//歩き。
		enPlayerState_Run,					//走る。
		enPlayerState_Punch,				//攻撃。
		enPlayerState_Guard,				//防御。
		enPlayerState_MagicAttack,			//魔法攻撃。
		enPlayerState_PushLever,			//レバーを押す。
		enPlayerState_ReceiveDamage,		//ダメ―ジ受けた。
		enPlayerState_Down,					//HPが0。
		enPlayerState_Clear					//クリアー。
	};
	EnPlayerState m_playerState = enPlayerState_Idle;			//プレイヤーステート。
	
};

