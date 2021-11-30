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

	//���f�������_�[�̓ǂ݂���
	m_modelRender.Init("Assets/modelData/Enemy/shadownity.tkm", m_animationClips, enAnimationClip_Num);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	//�A�j���[�V�����C�x���g�p�̊֐�
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_characterController.Init(25.0f, 75.0f, m_position);
	m_rotation.Apply(m_forward);
	m_player = FindGO<Player>("player");
	// �i�r���b�V�����\�z�B
	m_nvmMesh.Init("Assets/modelData/dangeon/dangeonnav.tkn");

	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);
	return true;
}
Enemy::~Enemy()
{

}
void Enemy::Update()
{

	//�X�e�[�g�Ǘ�
	ManageState();
	//�ǐՃX�e�[�g�Ǘ�
	ManageChaseState();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�����蔻��
	Collision();
	//��]����
	Rotation();
	//���f�������_�[�̃A�b�v�f�[�g
	m_modelRender.Update();
}
void Enemy::Chase()
{

	
}
void Enemy::Rotation()
{
	if (m_chaseState!=enChaseState_Chase) {
		//�ǐՒ��łȂ���Ή�]�����͕K�v�Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}
void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g
	case enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
		//�U���X�e�[�g
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
		//�ǐՃX�e�[�g
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
		//��_���[�W�X�e�[�g
	case enEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//�_�E���X�e�[�g
	case enEnemyState_Down:
		ProcessDownStateTransition();
		break;
		//��]�U���X�e�[�g
	case enEnemyState_RotateAttack:
		ProcessRotateAttackStateTransition();
		break;
		//����X�e�[�g
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
			fontRender.SetText(L"�E�E�E");
			m_chase = false;
			break;
		case enChaseState_Chase:
			//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
			diff = m_player->GetPosition() - m_position;
			//�x�N�g���𐳋K������B
			diff.Normalize();
			//�ړ����x��ݒ肷��B
			m_moveSpeed = diff * 250.0f;
			m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
			m_modelRender.SetPosition(m_position);
			if (m_characterController.IsOnGround()) {
				//�n�ʂɂ����B
				//�d�͂�0�ɂ���B
				m_moveSpeed.y = 0.0f;
			}
			if (SearchPlayer() == false)
			{
				//����������X�e�[�g��J�ځB
				m_chaseState = enChaseState_Pathmove;

			}
			fontRender.SetText(L"�����I");
			break;
		case enChaseState_Pathmove:
			isEnd = false;
			isEnd2 = false;
			fontRender.SetText(L"�p�A�A�A�A�A�A�A�A�A�A�A�A�A�A�A�A�A�A�A�X�T��");
			//�A�Ғ�����Ȃ����
			if (m_return == false)
			{
				m_path.Clear();
				// �p�X���� �����Ȃ��Ȃ����n�_�܂ŁB
				m_pathFiding.Execute(
					m_path,							// �\�z���ꂽ�p�X�̊i�[��
					m_nvmMesh,						// �i�r���b�V��
					m_position,						// �J�n���W
					m_player->GetPosition(),		// �ړ��ڕW���W�v���C���[�̈ʒu�B
					PhysicsWorld::GetInstance(),	// �����G���W��	
					25.0f,							// AI�G�[�W�F���g�̔��a
					75.0f							// AI�G�[�W�F���g�̍����B
				);
				m_chaseState = enChaseState_Waitingtomove;
				return;
			}
			else
			{
				m_path.Clear();
				// �p�X���� �����ʒu�ɋA��B
				m_pathFiding.Execute(
					m_path,							// �\�z���ꂽ�p�X�̊i�[��
					m_nvmMesh,						// �i�r���b�V��
					m_position,						// �J�n���W
					m_InitialPosition,				// �ړ��ڕW���W�͏����ʒu�B
					PhysicsWorld::GetInstance(),	// �����G���W��	
					25.0f,							// AI�G�[�W�F���g�̔��a
					75.0f							// AI�G�[�W�F���g�̍����B
				);
				m_chaseState = enChaseState_Return;
				return;
			}



			break;
		case enChaseState_Waitingtomove:
			// �p�X����ړ�����B
			m_position = m_path.Move(m_position, 7.0f, isEnd, m_forward);
			m_modelRender.SetPosition(m_position);
			//�L�����N�^�[�̕�����ς���B
			m_rotation.SetRotationYFromDirectionXZ(m_forward);
			//��]��ݒ�B
			m_modelRender.SetRotation(m_rotation);
			//�L�����N�^�[�R���g���[���[�𓮂����B
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
				//��������ǐՍĊJ����B
				m_chaseState = enChaseState_Chase;
				break;
			}
			fontRender.SetText(L"�p�X���ړ���");
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
				//��������ǐՂ��ĊJ����B
				m_chaseState = enChaseState_Chase;
				break;
			}
			fontRender.SetText(L"�{�����s");
			break;
		case enChaseState_Return:
			// �p�X����ړ�����B
			m_position = m_path.Move(m_position, 7.0f, isEnd2, m_forward);
			m_modelRender.SetPosition(m_position);
			m_characterController.SetPosition(m_position);
			m_characterController.Execute(move, 0.0f);
			//�L�����N�^�[�̕�����ς���B
			m_rotation.SetRotationYFromDirectionXZ(m_forward);
			//�G�`������ɉ�]��������B
			m_modelRender.SetRotation(m_rotation);
			if (isEnd2 == true)
			{
				//�A�Ҕ�����������B
				m_return = false;
				//�����̉�]��K�p����B
				m_rotation = m_InitialRotation;
				//�ǐՃX�e�[�g��ҋ@�ɂ���B
				m_chaseState = enChaseState_Idle;
				//�G�l�~�[�X�e�[�g��ҋ@�ɂ���B
				m_enemyState = enEnemyState_Idle;

				break;
			}
			if (SearchPlayer() == true)
			{
				//��������ǐՍĊJ�B
				m_chaseState = enChaseState_Chase;
				//�A�Ҕ�����������B
				m_return = false;
			}
			fontRender.SetText(L"�A��");
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
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
	case enEnemyState_Chase:
		//����A�j���[�V�������Đ��B
		if (m_chaseState==enChaseState_Return||
			m_chaseState==enChaseState_Chase||
			m_chaseState==enChaseState_Waitingtomove){
			m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		}
		else {
			//�ҋ@�A�j���[�V�������Đ��B
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

	//�v���C���[����������B
	if (SearchPlayer() == true)
	{		
		//�ǐՃX�e�[�g�ɑJ�ڂ���B
		m_enemyState = enEnemyState_Chase;

		return;
	}
	//�v���C���[���������Ȃ���΁B
	else
	{
		return;
	}
}
void Enemy::ProcessIdleStateTransition()
{
	//���ʂ̃X�e�[�g�J��
	ProcessCommonStateTransition();
}
void Enemy::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
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
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U�����ꂽ�狗���֌W�����ɒǐՂ�����B
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
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}
void Enemy::Collision()
{
	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
	//�����蔻�菈���͂��Ȃ��B
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		return;
	}

	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_characterController))
		{
			//HP��1���炷�B
			m_hp -= 1;

			//����HP��0����Ȃ�B
			if (m_hp > 0)
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_ReceiveDamage;
			}
			//HP��0�Ȃ�B
			else if (m_hp == 0)
			{
				//�_�E���X�e�[�g�ɑJ�ڂ���B
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
//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�ǂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//�Փ˂����͕̂ǂł͂Ȃ��B
			return 0.0f;
		}

		//�ǂƂԂ�������B
		//�t���O��true�ɁB
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
		//�����̂Ō����Ȃ��B
		return false;
	}
	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));
	//�v���C���[�����E���ɋ��Ȃ�������B
	if (Math::PI * 0.35f <= fabsf(angle))
	{
		//�v���C���[�͌������Ă��Ȃ��B
		return false;
	}

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(playerPosition.x, playerPosition.y + 70.0f, playerPosition.z));

	SweepResultWall callback;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�Փ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//�ǂƏՓ˂����I
	if (callback.isHit == true)
	{
		//�v���C���[�͌������Ă��Ȃ��B
		return false;
	}

	//�ǂƏՓ˂��ĂȂ��I�I
	//�v���C���[�������t���O��true�ɁB
	return true;
}
void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	
}