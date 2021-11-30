#include "stdafx.h"
#include "Player.h"

bool Player::Start()
{


	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Player/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/Player/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Player/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Punch].Load("Assets/animData/Player/punch.tka");
	m_animationClips[enAnimationClip_Punch].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Guard].Load("Assets/animData/Player/barrier.tka");
	m_animationClips[enAnimationClip_Guard].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/Player/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	/*m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Winner].Load("Assets/animData/winner.tka");
	m_animationClips[enAnimationClip_Winner].SetLoopFlag(false);*/

	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimationClip_Num);
	m_characterController.Init(25.0f, 75.0f, m_position);
	m_modelRender.SetPosition(m_position);
	m_mapRender.Init("Assets/sprite/map.dds", 315.0f, 315.0f);
	m_playerRender.Init("Assets/sprite/player.dds", 15.0f, 15.0f);
	//���x�����\�z����B
	m_level2DRender.Init("Assets/sprite/UI/HUD.casl", [&](Level2DObjectData& objData) {
		//���O���v���Ă�����B
		if (objData.EqualObjectName("HUD") == true) {


			//Level2D���ŉ摜�ǂݍ��݁B			
			return false;
		}
		else if (objData.EqualObjectName("unitychan") == true) {


			//Level2D���ŉ摜�ǂݍ��݁B
			return false;
		}
		else if (objData.EqualObjectName("HP") == true) {

			//���x���̃f�[�^���g�p���ĉ摜��ǂݍ��ށB
			m_HPRender[0].Init(objData.ddsFilePath, objData.width, objData.height);
			m_HPRender[0].SetScale(objData.scale);
			m_HPRender[0].SetPosition(objData.position);
			m_HPRender[0].SetPivot(objData.pivot);
			m_HPRender[1].Init(objData.ddsFilePath, objData.width, objData.height);
			m_HPRender[1].SetScale(objData.scale);
			m_HPRender[1].SetPosition(objData.position);
			m_HPRender[1].SetPivot(objData.pivot);
			return true;
		}
		else if (objData.EqualObjectName("MP") == true) {

			//���x���̃f�[�^���g�p���ĉ摜��ǂݍ��ށB
			m_MPRender[0].Init(objData.ddsFilePath, objData.width, objData.height);
			m_MPRender[0].SetScale(objData.scale);
			m_MPRender[0].SetPosition(objData.position);
			m_MPRender[0].SetPivot(objData.pivot);
			m_MPRender[1].Init(objData.ddsFilePath, objData.width, objData.height);
			m_MPRender[1].SetScale(objData.scale);
			m_MPRender[1].SetPosition(objData.position);
			m_MPRender[1].SetPivot(objData.pivot);
			return true;
		}
		return false;
		});
	m_HPfontRender.SetPosition({ -540.0f,-365.0f,0.0f });
	m_MPfontRender.SetPosition({ -540.0f,-440.0f,0.0f });
	m_HPRender[1].SetMulColor({ 0.0f,0.0f,0.0f,1.0f });
	m_MPRender[1].SetMulColor({ 0.0f,0.0f,0.0f,1.0f });
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//�uCharacter1_RightHand�v�{�[����ID(�ԍ�)���擾����B
	m_PunchBoneId = m_modelRender.FindBoneID(L"Character1_RightHand");
	return true;
}
Player::~Player()
{

}
void Player::Update()
{
	Move();//�ړ�����
	Rotation();//��]����
	Debug();//�f�o�b�O�p
	UI();//UI
	UIRenderUpdates();//UIRender�A�b�v�f�[�g
	Punch();//���菈��
	Guard();//�K�[�h����
	PlayAnimation();//�A�j���[�V�����Đ�
	ManageState();//�X�e�[�g�Ǘ�
}
void Player::UI()
{
	m_playerRender.SetPosition({ m_position.x * 0.059f + m_mapposition.x ,m_position.z * 0.059f + m_mapposition.y,0.0f });
	m_mapRender.SetPosition(m_mapposition);

	wchar_t wcsbuf5[256];
	swprintf_s(wcsbuf5, 256, L"%4d", m_HP);
	m_HPfontRender.SetText(wcsbuf5);
	m_HPfontRender.SetScale(1.0f);
	m_HPfontRender.SetColor({ 0.0f,0.0f,0.0f,1.0f });

	wchar_t wcsbuf6[256];
	swprintf_s(wcsbuf6, 256, L"%4d", m_MP);
	m_MPfontRender.SetText(wcsbuf6);
	m_MPfontRender.SetScale(1.0f);
	m_MPfontRender.SetColor({ 0.0f,0.0f,0.0f,1.0f });

	float hp = m_InitialHP - m_HP;//�������̗�
	m_scale1.x = 1.0f - (1.0f / float(m_InitialHP)) * hp;
	m_HPRender[0].SetScale(m_scale1);

	float mp = m_InitialMP - m_MP;//����������
	m_scale2.x = 1.0f - (1.0f / float(m_InitialMP)) * mp;
	m_MPRender[0].SetScale(m_scale2);
	
}
void Player::ManageState() 
{
	switch (m_playerState)
	{
	case enPlayerState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//�����X�e�[�g�̎��B
	case enPlayerState_Walk:
		//�����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessWalkStateTransition();
		//�ړ����x��ݒ�B
		SetMoveSpeed(620.0f);
		break;
		//����X�e�[�g�̎��B
	case enPlayerState_Run:
		//����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessRunStateTransition();
		//�ړ����x��ݒ�B
		SetMoveSpeed(1200.0f);
		break;
	case enPlayerState_Punch:
		//�p���`�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessPunchStateTransition();
		//�ړ����x��ݒ�
		SetMoveSpeed(180.0f);
		break;
	case enPlayerState_Guard:
		//�K�[�h�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessGuardStateTransition();
		//�ړ����x��ݒ�
		SetMoveSpeed(45.0f);
		break;
	case enPlayerState_ReceiveDamage:
		//��_���[�W���̃X�e�[�g�J�ڏ���

		
		break;
	default:
		break;
	}
}
void Player::Collision()
{
	//��_���[�W�A�_�E�����A�N���A���̓_���[�W��������Ȃ��B
	if (m_playerState == enPlayerState_ReceiveDamage ||
		m_playerState == enPlayerState_Down ||
		m_playerState == enPlayerState_Clear)
	{
		return;
	}
	//�G�̍U���p�̃R���W�����̔z����擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	//�z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_characterController))
		{
			//HP�����炷�B
			m_HP -= 50;
			///HP���O�ɂȂ�����B
			if (m_HP <= 0)
			{
				//�_�E��������B
				m_playerState = enPlayerState_Down;
				m_HP = 0;
			}
			//HP���O�ł͂Ȃ�������B
			else
			{
				//��_���[�W���A�N�V�������w����B
				m_playerState = enPlayerState_ReceiveDamage;
			}
			//���ʉ����Đ�����B
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(7);
			se->Play(false);
			se->SetVolume(0.7f);
			return;
		}
	}
	//�G�̌������t�@�C���[�{�[���̃R���W�����̔z����擾����B
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("enemy_fireball");
	//�z���for���ŉ񂷁B
	for (auto collision : collisions2)
	{
		if (collision->IsHit(m_characterController))
		{
			//HP�����炷�B
			m_HP -= 20;
			///HP���O�ɂȂ�����B
			if (m_HP == 0)
			{
				//�_�E��������B
				m_playerState = enPlayerState_Down;
			}
			//HP���O�ł͂Ȃ�������B
			else
			{
				//��_���[�W���A�N�V�������w����B
				m_playerState = enPlayerState_ReceiveDamage;
			}
			//���ʉ����Đ�����B
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(7);
			se->Play(false);
			se->SetVolume(0.7f);
			return;
		}
	}

}
void Player::ProcessCommonStateTransition()
{
	//LB1�{�^���������ƃK�[�h
	if (g_pad[0]->IsPress(enButtonLB1))
	{
		m_playerState = enPlayerState_Guard;
		m_isDefending = true;
		return;
	}
	//B�{�^�����������牣��B
	if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		m_playerState = enPlayerState_Punch;
		return;
	}
	//�ړ����Ă���Ƃ�
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//A�{�^���������Ƒ���B
		if (g_pad[0]->IsPress(enButtonA))
		{
			m_playerState = enPlayerState_Run;
			return;
		}
		//�����Ă��Ȃ��Ƃ��͕����B
		else
		{
			m_playerState = enPlayerState_Walk;
			return;
		}

	}
	//�ړ����Ă��Ȃ��Ƃ��͑ҋ@���[�V����
	else
	{
		m_playerState = enPlayerState_Idle;
		return;
	}
	
	
}

void Player::ProcessIdleStateTransition()
{
	//�X�e�[�g�J�ڏ���
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	//�X�e�[�g�J�ڏ���
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::ProcessPunchStateTransition() 
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
	else
	{
		//LB1�{�^���������ƃK�[�h
		if (g_pad[0]->IsPress(enButtonLB1))
		{
			//�X�e�[�g��J��
			ProcessCommonStateTransition();
			m_isDefending = true;
			return;
		}
	}
}

void Player::ProcessGuardStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enPlayerState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�����X�e�[�g�̎��B
	case enPlayerState_Walk:
		m_modelRender.SetAnimationSpeed(1.2f);
		//�����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//����X�e�[�g�̎��B
	case enPlayerState_Run:
		m_modelRender.SetAnimationSpeed(1.2f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;

	case enPlayerState_Punch:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Punch, 0.1f);
		break;
	case enPlayerState_Guard:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Guard);
	default:
		break;
	}
}


void Player::UIRenderUpdates()
{
	m_modelRender.Update();
	m_mapRender.Update();
	m_playerRender.Update();
	m_HPRender[1].Update();
	m_MPRender[1].Update();
	m_HPRender[0].Update();
	m_MPRender[0].Update();
	m_level2DRender.Update();
}
void Player::Move()
{
	//xz�̈ړ����x��0.0f�ɂ���B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��擾�B
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();
	//�J�����̑O�����ƉE�����̃x�N�g���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;
	//���K��
	forward.Normalize();
	right.Normalize();
	//���X�e�B�b�N�̓��͗ʂƈړ����x����Z
	right *= stickL.x * GetMoveSpeed();
	forward *= stickL.y * GetMoveSpeed();
	//�ړ����x���Z
	m_moveSpeed += right + forward;
	//�n�ʂɂ���Ƃ���
	if (m_characterController.IsOnGround())
	{
		m_position.y = 0.0f;
	}
	else
	{
		m_position.y -= 5.0f;
	}
	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);
}
void Player::Rotation()
{
	//�ړ����x������Ƃ�
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{	
		//��]���쐬
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//��]��K�p
		m_modelRender.SetRotation(m_rotation);
	}
}
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O���uattack_start�v�̎��B
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//�U�����ɂ���B
		m_isUnderAttack = true;
	}
	//�L�[�̖��O���uattack_end�v�̂Ƃ�
	if (wcscmp(eventName, L"attack_end") == 0)
	{
		//�U�����I���B
		m_isUnderAttack = false;
	}
}
void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_mapRender.Draw(rc);
	m_playerRender.Draw(rc);


	//�f�o�b�O�p
	m_positionrender[0].Draw(rc);
	m_positionrender[1].Draw(rc);
	m_positionrender[2].Draw(rc);
	m_justGuardRender.Draw(rc);

	m_level2DRender.Draw(rc);
	m_HPRender[1].Draw(rc);
	m_MPRender[1].Draw(rc);
	m_HPRender[0].Draw(rc);
	m_MPRender[0].Draw(rc);
	m_HPfontRender.Draw(rc);
	m_MPfontRender.Draw(rc);
}
void Player::Punch()
{
	//�U�����łȂ��Ȃ�A���������Ȃ��B
	if (m_playerState != enPlayerState_Punch)
	{
		return;
	}

	//�U�����蒆�ł���΁B
	if (m_isUnderAttack == true)
	{
		//�U���p�̃R���W�������쐬����B
		MakePunchCollision();
	}
}
void Player::MakePunchCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition += m_forward * 50.0f;
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(collisionPosition,               //���W�B
		Quaternion::Identity,                                       //��]�B
		Vector3(110.0f, 15.0f, 15.0f)                               //�傫���B
	);
	collisionObject->SetName("player_attack");
	//�uSword�v�{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_PunchBoneId)->GetWorldMatrix();
	//�uSword�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject->SetWorldMatrix(matrix);
}
void Player::Guard()
{
	//�K�[�h���łȂ��Ȃ�A���������Ȃ��B
	if (m_playerState != enPlayerState_Guard)
	{
		m_guardcooltimer+=g_gameTime->GetFrameDeltaTime();//�N�[���^�C�}�[���N���B
		m_guardtimer = 0;//�K�[�h���Ԃ����Z�b�g�B
		return;
	}
	//�K�[�h���ł���΁B
	if (m_isDefending == true)
	{
		//�K�[�h�p�̃R���W�������쐬����B
		MakeGuardCollision();
		//�U���p�̃R���W�����̐������~�߂�B
		m_isUnderAttack = false;
		m_guardtimer += g_gameTime->GetFrameDeltaTime();//�K�[�h���Ԃ��v���B
		m_guardcooltimer = 0;//�N�[���^�C�}�[�����Z�b�g�B
	}
}
void Player::MakeGuardCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�A�����㑤�ɐݒ�
	collisionPosition += m_forward * 25.0f;
	collisionPosition.y += 90.0f;
	//�R���W��������v���C���[�܂ł̃x�N�g�����v�Z
	Vector3 topos = collisionPosition - m_position;
	//�O�q�̃x�N�g���Ƀv���C���[�̉�]��K�p
	m_rotation.Apply(topos);
	//�X�t�B�A�R���W�����𐶐�
	collisionObject->CreateBox(m_position + topos,               //���W�B
		m_rotation,											    //��]�B
		Vector3(50.0f,180.0f,25.0f)					           //�傫���B
	);
	collisionObject->SetName("player_guard");
}
void Player::Debug()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"x:%5f", m_position.x);

	//�\������e�L�X�g��ݒ�B
	m_positionrender[0].SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_positionrender[0].SetPosition(Vector3(200.0f, 200.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_positionrender[0].SetScale(2.0f);

	wchar_t wcsbuf1[256];
	swprintf_s(wcsbuf1, 256, L"y:%5f", m_position.y);

	//�\������e�L�X�g��ݒ�B
	m_positionrender[1].SetText(wcsbuf1);
	//�t�H���g�̈ʒu��ݒ�B
	m_positionrender[1].SetPosition(Vector3(200.0f, 250.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_positionrender[1].SetScale(2.0f);

	wchar_t wcsbuf2[256];
	swprintf_s(wcsbuf2, 256, L"z:%5f", m_position.z);

	//�\������e�L�X�g��ݒ�B
	m_positionrender[2].SetText(wcsbuf2);
	//�t�H���g�̈ʒu��ݒ�B
	m_positionrender[2].SetPosition(Vector3(200.0f, 300.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_positionrender[2].SetScale(2.0f);
}