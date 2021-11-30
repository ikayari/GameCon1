#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"
class Player;//�v���C���[�N���X

class Enemy:public IGameObject
{
public:
	Enemy() {};
	~Enemy() ;
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// �X�P�[����ݒ�
	/// </summary>
	/// <param name="s">�X�P�[��</param>
	void SetScale(const Vector3& s)
	{
		m_scale = s;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="r">��]</param>
	void SetRotation(const Quaternion& r)
	{
		m_rotation = r;
	}
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="p">���W</param>
	void SetPosition(const Vector3& p)
	{
		m_position = p;
	}
	/// <summary>
	/// �������W��ݒ�B
	/// </summary>
	/// <param name="p">���W</param>
	void SetInitialPosition(const Vector3& p)
	{
		m_InitialPosition = p;
	}
	/// <summary>
	/// ������]��ݒ�B
	/// </summary>
	/// <param name="r">��]</param>
	void SetInitialRotation(const Quaternion& r)
	{
		m_InitialRotation = r;
	}
private:
	/// <summary>
	/// �X�e�[�g�Ǘ��B
	/// </summary>
	void ManageState();
	/// <summary>
	/// �ǐՃX�e�[�g�̊Ǘ��B
	/// </summary>
	void ManageChaseState();
	/// <summary>
	/// �����蔻��̏����B
	/// </summary>
	void Collision();
	/// <summary>
	/// �ǐՏ����B
	/// </summary>
	void Chase();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �A�j���[�V�����̍Đ��B
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// ���ʂ̃X�e�[�g�J��
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@���̃X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �U�����̃X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// �ǐՒ��̃X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// ��]�U�����̃X�e�[�g�J�ڏ���
	/// </summary>
	void ProcessRotateAttackStateTransition();
	/// <summary>
	/// ����̃X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessDodgeStateTransition();
	/// <summary>
	/// �v���C���[��T������B
	/// </summary>
	/// <returns>�v���C���[������������true�B</returns>
	const bool SearchPlayer() const;
	// �A�j���[�V�����C�x���g�p�̊֐��B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//�G�l�~�[�X�e�[�g�B
	enum EnEnemyState {
		enEnemyState_Idle,					//�ҋ@�B
		enEnemyState_Chase,					//�ǐՁB
		enEnemyState_Attack,				//�U���B
		enEnemyState_RotateAttack,			//��]�U���B
		enEnemyState_Dodge,					//����B
		enEnemyState_ReceiveDamage,			//��_���[�W�B
		enEnemyState_Down,					//�_�E���B
	};
	EnEnemyState m_enemyState = enEnemyState_Idle;						//�X�e�[�g
	enum EnAnimationClip {						//�A�j���[�V�����B
		enAnimationClip_Idle,					//�ҋ@�A�j���[�V�����B
		enAnimationClip_Run,					//����A�j���[�V�����B
		enAnimationClip_Attack,					//�U���A�j���[�V�����B
		enAnimationClip_RotateAttack,			//��]�U���A�j���[�V����
		enAnimationClip_Down,					//�_�E���A�j���[�V�����B
		enAnimationClip_Damage,					//��_���[�W�A�j���[�V�����B
		enAnimationClip_Backstep,				//�o�b�N�X�e�b�v
		/*enAnimationClip_MagicAttack,			//���@�U���A�j���[�V�����B*/
		enAnimationClip_Num,					//�A�j���[�V�����̐��B
	};
	enum EnChaseState {
		enChaseState_Idle,						//�ҋ@���B
		enChaseState_Chase,						//�ǐՒ��B
		enChaseState_Pathmove,					//�o�H�T���ړ��B
		enChaseState_Waitingtomove,				//�{���ړ����B
		enChaseState_SearchIdle,				//�{���ҋ@�B
		enChaseState_Return,					//�A�ҁB
	};
	EnChaseState m_chaseState = enChaseState_Idle;						//�X�e�[�g
	AnimationClip				m_animationClips[enAnimationClip_Num];	//�A�j���[�V�����N���b�v�B
	ModelRender				m_modelRender;								//���f�������_�[
	CharacterController		m_characterController;						//�L�����N�^�[�R���g���[���[
	Vector3					m_position = { 0.0f,0.0f,50.0f };									//�|�W�V����
	Vector3					m_forward = Vector3::AxisZ;					//�G�l�~�[�̐��ʃx�N�g���B
	Quaternion				m_rotation;									//��]�B
	Vector3					m_scale = Vector3::One;						//�傫���B
	Vector3					m_moveSpeed;								//�ړ����x�B
	Player*					m_player = nullptr;							//�v���C���[�B
	SphereCollider			m_sphereCollider;
	Vector3					m_InitialPosition = Vector3::Zero;			//�����ʒu�B
	Quaternion				m_InitialRotation = Quaternion::Identity;	//������]�B

	FontRender fontRender;

	TknFile m_tknFile;													//�i�r���b�V��
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::Path m_path2;
	nsAI::PathFinding m_pathFiding;
	nsAI::PathFinding m_pathFiding2;
	float m_waittimer = 0.0f;												//�ҋ@�^�C�}�[
	int m_hp = 5;														//�̗�
	bool m_chase = false;												//�ǂ�������B
	bool m_return = false;												//�A��
	bool isEnd;															//�p�X�T���̏I������
	bool isEnd2;
	float Angle;	
	Vector3 diff = Vector3::Zero;										//�v���C���[�܂ł̋����B
	Vector3 move = Vector3::Zero;
};

