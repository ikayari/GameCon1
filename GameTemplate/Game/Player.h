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
	/// ���W���擾����B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// ���W��ݒ肷��B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// �d�͂�ݒ肷��B
	/// </summary>
	/// <param name="g"></param>
	/// <returns></returns>
	void SetGravity(const float& g)
	{
		m_moveSpeed.y -= g; 
	}
	/// <summary>
	/// �d�͂��擾
	/// </summary>
	/// <returns></returns>
	const float&  GetGravity()const
	{
		return m_moveSpeed.y;
	}
	/// <summary>
	/// �ړ����x���擾����B
	/// </summary>
	const float& GetMoveSpeed()const
	{
		return m_speed;
	}
	/// <summary>
	/// �ړ����x��ݒ肷��B
	/// </summary>
	/// <param name="s">�ړ����x</param>
	void  SetMoveSpeed(const float& s)
	{
		m_speed = s;
	}
	/// <summary>
	/// ��]��ݒ肷��B
	/// </summary>
	/// <param name="r">��]</param>
	void SetRotation(const Quaternion& r)
	{
		m_rotation = r;
	}
	/// <summary>
	/// ���W���擾����B
	/// </summary>
	/// <returns>���W�B</returns>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}
private:
	/// <summary>
	/// �U������
	/// </summary>
	void Punch();
	/// <summary>
	/// �U���̃R���W��������
	/// </summary>
	void MakePunchCollision();
	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	void Collision();
	/// <summary>
	/// �K�[�h����
	/// </summary>
	void Guard();
	/// <summary>
	/// �K�[�h�̃R���W��������
	/// </summary>
	void MakeGuardCollision();
	/// <summary>
	/// �v���C���[�ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// �v���C���[��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �f�o�b�O�p�B
	/// </summary>
	void Debug();
	/// <summary>
	/// UIRender�֌W��Update
	/// </summary>
	void UIRenderUpdates();
	/// <summary>
	/// UI�̏����B
	/// </summary>
	void UI();
	/// <summary>
	/// �X�e�[�g�Ǘ��B
	/// </summary>
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
	///	�p���`�X�e�[�g�̑J��
	/// </summary>
	void ProcessPunchStateTransition();
	/// <summary>
	/// �K�[�h�X�e�[�g�̑J��
	/// </summary>
	void ProcessGuardStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J��
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
	// �A�j���[�V�����C�x���g�p�̊֐��B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	ModelRender				m_modelRender;								//���f�������_�[
	CharacterController		m_characterController;						//�L�����N�^�[�R���g���[���[
	Vector3					m_position;									//�|�W�V����
	Vector3					m_moveSpeed;								//�����X�s�[�h
	Vector3					m_scale1 = Vector3::One;					//�X�P�[��
	Vector3					m_scale2 = Vector3::One;
	Vector3					m_mapposition = { -751.0f,344.0f,0.0f };
	Vector3					m_forward = Vector3::AxisZ;					//�v���C���[�̐��ʃx�N�g���B
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

	int						m_HP = 8000;						//�̗͐��l
	int						m_GP = 200;							//�h�䐔�l
	int						m_InitialHP = 8000;					//�̗͏����l
	int						m_InitialGP = 200;					//�h�䏉���l
	bool					m_isUnderAttack = false;			//�U�����H
	bool					m_isDefending = false;				//�h�䒆�H
	bool					m_SuccessDefence = false;			//�h�䐬���H
	int                     m_PunchBoneId = -1;                 //�uRightHand�v�{�[����ID�B     
	float					m_speed=360.0f;						//�ړ����x
	float					m_guardtimer=0.0f;					//�h��^�C�}�[
	float					m_guardcooltimer=0.0f;				//�A���K�[�h����^�C�}�[
	//�A�j���[�V�����N���b�v	
	enum EnAnimationClip {
		enAnimationClip_Idle,				//�ҋ@�A�j���[�V�����B	
		enAnimationClip_Walk,				//�����A�j���[�V�����B
		enAnimationClip_Run,				//����A�j���[�V�����B
		enAnimationClip_Punch,				//�U���A�j���[�V�����B
		enAnimationClip_Guard,				//�h��A�j���[�V�����B
		enAnimationClip_Damage,				//��_���[�W�A�j���[�V�����B
		/*enAnimationClip_MagicAttack,		//���@�U���A�j���[�V�����B
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
		enPlayerState_Punch,				//�U���B
		enPlayerState_Guard,				//�h��B
		enPlayerState_MagicAttack,			//���@�U���B
		enPlayerState_PushLever,			//���o�[�������B
		enPlayerState_ReceiveDamage,		//�_���\�W�󂯂��B
		enPlayerState_Down,					//HP��0�B
		enPlayerState_Clear					//�N���A�[�B
	};
	EnPlayerState m_playerState = enPlayerState_Idle;			//�v���C���[�X�e�[�g�B
	
};

