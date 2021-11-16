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
	/// ���ʂ̃X�e�[�g�J��
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J��
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �����X�e�[�g�̑J��
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J��
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
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
	//�A�j���[�V�����N���b�v
	enum EnAnimationClip {
		enAnimationClip_Idle,				//�ҋ@�A�j���[�V�����B	
		enAnimationClip_Walk,				//�����A�j���[�V�����B
		enAnimationClip_Run,				//����A�j���[�V�����B
		/*enAnimationClip_Attack,				//�U���A�j���[�V�����B
		enAnimationClip_MagicAttack,		//���@�U���A�j���[�V�����B
		enAnimationClip_Damage,				//��_���[�W�A�j���[�V�����B
		enAnimationClip_Down,				//�_�E���A�j���[�V�����B
		enAnimationClip_Winner,				//�����A�j���[�V�����B*/
		enAnimationClip_Num,				//�A�j���[�V�����̐��B
	};
	AnimationClip m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	//�v���C���[�X�e�[�g
	enum EnPlayerState {
		enPlayerState_Idle,					//�ҋ@�B
		enPlayerState_Walk,					//�����B
		enPlayerState_Run,					//����B
		enPlayerState_Attack,				//�U���B
		enPlayerState_MagicAttack,			//���@�U���B
		enPlayerState_PushLever,			//���o�[�������B
		enPlayerState_ReceiveDamage,		//�_���\�W�󂯂��B
		enPlayerState_Down,					//HP��0�B
		enPlayerState_Clear					//�N���A�[�B
	};
	EnPlayerState m_playerState = enPlayerState_Idle;			//�v���C���[�X�e�[�g�B
	
};

