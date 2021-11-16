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
	void Move();
	void Rotation();
	void Debug();
	void UIRenderUpdates();
	void UI();
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
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
	Vector3 Getposition() { return m_position; };
private:
	ModelRender m_modelRender;
	CharacterController	m_characterController;
	Vector3 m_position;
	Vector3 m_moveSpeed;
	Vector3 m_scale = { 1.0f,1.0f,1.0f };
	Vector3 m_scale2 = { 1.0f,1.0f,1.0f };
	Vector3 m_mapposition = { -751.0f,344.0f,0.0f };
	Quaternion m_rotation;

	SpriteRender m_mapRender;
	SpriteRender m_playerRender;
	SpriteRender m_MPRender[2];
	SpriteRender m_HPRender[2];

	FontRender m_positionrender[3];
	FontRender m_HPfontRender;
	FontRender m_MPfontRender;

	Level2DRender m_level2DRender;

	int m_HP = 8000;
	int m_MP = 200;
	int m_InitialHP = 8000;
	int m_InitialMP = 200;
	//アニメーションクリップ
	enum EnAnimationClip {
		enAnimationClip_Idle,				//待機アニメーション。	
		enAnimationClip_Walk,				//歩きアニメーション。
		enAnimationClip_Run,				//走りアニメーション。
		/*enAnimationClip_Attack,				//攻撃アニメーション。
		enAnimationClip_MagicAttack,		//魔法攻撃アニメーション。
		enAnimationClip_Damage,				//被ダメージアニメーション。
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
		enPlayerState_Attack,				//攻撃。
		enPlayerState_MagicAttack,			//魔法攻撃。
		enPlayerState_PushLever,			//レバーを押す。
		enPlayerState_ReceiveDamage,		//ダメ―ジ受けた。
		enPlayerState_Down,					//HPが0。
		enPlayerState_Clear					//クリアー。
	};
	EnPlayerState m_playerState = enPlayerState_Idle;			//プレイヤーステート。
	
};

