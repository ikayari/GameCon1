#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Stage.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "Wall.h"

bool Game::Start()
{
	//���x�����\�z����B
	m_levelRender.Init("Assets/level3D/stage2.tkl",
		[&](LevelObjectData& objData)
		{
			//���O��unityChan��������B
			if (objData.EqualObjectName(L"unityChan") == true) {
				//�v���C���[�̃I�u�W�F�N�g���쐬����B
				m_player = NewGO<Player>(0, "player");
				//���W��ݒ肷��B
				m_player->SetPosition(objData.position);
				//��]��ݒ肷��B
				m_player->SetRotation(objData.rotation);
				//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
				return true;
			}
			//���O��ground��������B
			else if (objData.EqualObjectName(L"ground") == true) {
				//�X�e�[�W���쐬����B
				m_stage = NewGO<Stage>(0, "stage");
				//���W��ݒ肷��B
				m_stage->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_stage->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_stage->SetRotation(objData.rotation);
				return true;
			}
			//���O��wall��������
			else if (objData.EqualObjectName(L"wall") == true) {
				//�X�e�[�W���쐬����B
				m_wall = NewGO<Wall>(0, "wall");
				//���W��ݒ肷��B
				m_wall->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_wall->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_wall->SetRotation(objData.rotation);
				return true;
			}
			else if (objData.EqualObjectName(L"shadownity") == true) {
				//�G�l�~�[���쐬����B
				m_enemy = NewGO<Enemy>(0, "enemy");
				//���W��ݒ肷��B				
				m_enemy->SetPosition(objData.position);
				//�������W���L��������B
				m_enemy->SetInitialPosition(objData.position);
				//�傫����ݒ肷��B
				m_enemy->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_enemy->SetRotation(objData.rotation);
				//������]���L��������B
				m_enemy->SetInitialRotation(objData.rotation);
				return true;
			}
			return true;
		});
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	//���C���[�t���[����\���@�f�o�b�O�p�B
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{
	
}