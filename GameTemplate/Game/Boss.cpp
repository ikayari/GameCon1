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

	//�uCharacter1_RightHand�v�{�[����ID(�ԍ�)���擾����B
	m_BoneId = m_modelRender.FindBoneID(L"Character1_RightHand");
	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);
	//�������������B
	srand((unsigned)time(NULL));

	return true;
}
Boss::~Boss()
{

}
void Boss::Update()
{


	//�ǐՃX�e�[�g�Ǘ�
	ManageBattleState();
	//�X�e�[�g�Ǘ�
	ManageState();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�U��
	Attack();
	//�����蔻��
	Collision();
	//��]����
	Rotation();
	//���f�������_�[�̃A�b�v�f�[�g
	m_modelRender.Update();
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d", m_hp);

	//�\������e�L�X�g��ݒ�B
	fontRender2.SetText(wcsbuf);
}
void Boss::Rotation()
{
	if (m_battleState != enBattleState_Chase &&
		m_battleState != enBattleState_Attack) {

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
void Boss::ManageState()
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
	case enEnemyState_AttackCombo:
		ProcessAttackComboStateTransition();
		break;
	default:
		break;
	}
}
void Boss::Attack()
{
	//�U���X�e�[�g�łȂ��Ȃ珈�������Ȃ��B
	if (m_enemyState != enEnemyState_Attack &&
		m_enemyState != enEnemyState_RotateAttack &&
		m_enemyState != enEnemyState_AttackCombo)
	{
		return;
	}
	//�U�����ł���΁B
	if (m_Attack == true)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
	}
	//�U���ړ���true�ł����
	if (m_AttackMove == true)
	{
		if (m_enemyState == enEnemyState_AttackCombo)
		{
			MoveChara(75.0f);
		}
		else
		{
			//�ړ�����B
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
			fontRender.SetText(L"�E�E�E");
			m_chase = false;
			m_return = false;
			break;
		case enBattleState_Chase:
			MoveChara(250.0f);
			if (m_characterController.IsOnGround()) {
				//�n�ʂɂ����B
				//�d�͂�0�ɂ���B
				m_moveSpeed.y = 0.0f;
			}
			if (SearchPlayer() == false)
			{
				//����������
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
				isEnd = false;
				m_battleState = enBattleState_Waitingtomove;

			}
			/////////////�U������///////////////////////////////////////////////
			if (IsCanAttack(1) == true)
			{
				//�U���X�e�[�g�ɑJ�ڂ���B
				m_battleState = enBattleState_Attack;
				/*int ram = rand() % 100;
				//������70�ȏ�Ȃ�B
				if (ram > 0)
				{
					m_battleState = enBattleState_Strafe;
				}
				//������70�����Ȃ�B
				else {
					//�U���X�e�[�g�ɑJ�ڂ���B
					m_battleState = enBattleState_Attack;
				}*/
	/*		}
			fontRender.SetText(L"�����I");
			break;
		case enBattleState_Waitingtomove:
			if (SearchPlayer() == true)
			{*
				//��������ǐՍĊJ����B
				m_battleState = enBattleState_Chase;
				break;
			}
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
				m_battleState = enBattleState_SearchIdle;
				break;
			}
			fontRender.SetText(L"�p�X���ړ���");
			break;
		case enBattleState_SearchIdle:
			if (SearchPlayer() == true)
			{
				//��������ǐՂ��ĊJ����B
				m_battleState = enBattleState_Chase;
				break;
			}
			m_waittimer += g_gameTime->GetFrameDeltaTime();
			if (m_waittimer >= 5.0f)
			{
				//���b�o������
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
				isEnd = false;
				m_battleState = enBattleState_Return;
				break;
			}

			fontRender.SetText(L"�{�����s");
			break;
		case enBattleState_Return:
			fontRender.SetText(L"�A��");
			if (m_enemyState != enEnemyState_ReceiveDamage)
			{
				// �p�X����ړ�����B
				m_position = m_path.Move(m_position, 7.0f, isEnd, m_forward);
				m_modelRender.SetPosition(m_position);
				m_characterController.SetPosition(m_position);
				m_characterController.Execute(move, 0.0f);
				//�L�����N�^�[�̕�����ς���B
				m_rotation.SetRotationYFromDirectionXZ(m_forward);
				//�G�`������ɉ�]��������B
				m_modelRender.SetRotation(m_rotation);
			}
			if (isEnd == true)
			{
				//�A�Ҕ�����������B
				m_return = false;
				//�����̉�]��K�p����B
				m_rotation = m_InitialRotation;
				//�ǐՃX�e�[�g��ҋ@�ɂ���B
				m_battleState = enBattleState_Idle;
				//�G�l�~�[�X�e�[�g��ҋ@�ɂ���B
				m_enemyState = enEnemyState_Idle;

				break;
			}
			if (SearchPlayer() == true)
			{
				//��������ǐՍĊJ�B
				m_battleState = enBattleState_Chase;
				//�A�Ҕ�����������B
				m_return = false;
			}
			fontRender.SetText(L"�A��");
			break;
		case enBattleState_Attack:
			fontRender.SetText(L"��풆");
			if (m_enemyState != enEnemyState_Attack &&
				m_enemyState != enEnemyState_RotateAttack &&
				m_enemyState != enEnemyState_AttackCombo)
			{
				MoveChara(75.0f);
			}
			if (IsCanAttack(0) == true)
			{
				//0�`99�̐��l�B
				int ram = rand() % 100;
				//������50�ȏ�Ȃ�B
				if (ram > 66)
				{
					//�U���X�e�[�g�ɑJ�ڂ���
					m_enemyState = enEnemyState_RotateAttack;
					return;
				}
				//������50�����Ȃ�B
				else if (ram > 33)
				{
					//��]�U���X�e�[�g�ɑJ�ڂ���B
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
			/*fontRender.SetText(L"�B�l�̊ԍ���");
			//�L�����N�^�[�̕�����ς���B
			m_rotation.SetRotationYFromDirectionXZ(m_player->GetPosition() - m_position);
			//��]��ݒ�B
			m_modelRender.SetRotation(m_rotation);
			m_rotation.Apply(m_forward);
			Vector3 side;

			side.Cross(m_forward, Vector3::AxisY);
			side.Normalize();
			MoveChara(55.0f,side);*/
	/*		break;
		case enBattleState_BackStep:
			fontRender.SetText(L"����`�I");
			MoveChara(-500.0f);
			m_backsteptimer += g_gameTime->GetFrameDeltaTime();
			//�L�����N�^�[�̕�����ς���B
			m_rotation.SetRotationYFromDirectionXZ(m_player->GetPosition() - m_position);
			//��]��ݒ�B
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
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
	case enEnemyState_Chase:
		//����A�j���[�V�������Đ��B
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
			//�ҋ@�A�j���[�V�������Đ��B
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
	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	diff = m_player->GetPosition() - m_position;
	//�x�N�g���𐳋K������B
	diff.Normalize();
	//�ړ����x��ݒ肷��B
	m_moveSpeed = diff * s;
	m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);

}
void Boss::MoveChara(float s, Vector3 v)
{
	v.Normalize();
	//�ړ����x��ݒ肷��B
	m_moveSpeed = v * s;
	m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);
}
void Boss::ProcessCommonStateTransition()
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
void Boss::ProcessIdleStateTransition()
{
	//���ʂ̃X�e�[�g�J��
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
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U�����ꂽ�狗���֌W�����ɒǐՂ�����B
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
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_enemyState = enEnemyState_Chase;
		m_battleState = enBattleState_BackStep;
		m_backsteptimer = 0.0f;
	}
}
void Boss::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_enemyState = enEnemyState_Chase;
		m_battleState = enBattleState_BackStep;
		m_backsteptimer = 0.0f;
	}
}
void Boss::ProcessAttackComboStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_enemyState = enEnemyState_Chase;
		m_battleState = enBattleState_BackStep;
		m_backsteptimer = 0.0f;
	}
}
void Boss::Collision()
{
	//��_���[�W����
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
			else if (m_hp <= 0)
			{
				//�_�E���X�e�[�g�ɑJ�ڂ���B
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
const bool Boss::SearchPlayer() const
{
	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;
	if (diff.Length() >= 2000.0f)
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
void Boss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O��[attack_start]�̎��B
	if (wcscmp(eventName, L"attack_start") == 0) {
		//�U���������true�ɂ���B
		m_Attack = true;
	}
	//�L�[�̖��O��[attack_end]�̎��B
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//�U���������false�ɂ���B
		m_Attack = false;
	}
	//�L�[�̖��O��[move_start]�̎��B
	if (wcscmp(eventName, L"move_start") == 0) {
		//�ړ������true�ɂ���B
		m_AttackMove = true;
	}
	//�L�[�̖��O��[move_end]�̎��B
	else if (wcscmp(eventName, L"move_end") == 0) {
		//�ړ������false�ɂ���B
		m_AttackMove = false;
	}
}
void Boss::MakeAttackCollision()
{
	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//�r�̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_BoneId)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(115.0f, 10.0f, 10.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
}

const bool Boss::IsCanAttack(const int n) const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//�{�X�ƃv���C���[�̋������߂�������B
	if (n == 0)
	{
		if (diff.LengthSq() <= 150.0f * 100.0f)
		{
			//�U���ł���I
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
	//�U���ł��Ȃ��B
	return false;
}

*/