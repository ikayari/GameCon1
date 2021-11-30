#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Stage.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "Wall.h"

bool Game::Start()
{
	//レベルを構築する。
	m_levelRender.Init("Assets/level3D/stage2.tkl",
		[&](LevelObjectData& objData)
		{
			//名前がunityChanだったら。
			if (objData.EqualObjectName(L"unityChan") == true) {
				//プレイヤーのオブジェクトを作成する。
				m_player = NewGO<Player>(0, "player");
				//座標を設定する。
				m_player->SetPosition(objData.position);
				//回転を設定する。
				m_player->SetRotation(objData.rotation);
				//trueにすると、レベルの方でモデルが読み込まれない。
				return true;
			}
			//名前がgroundだったら。
			else if (objData.EqualObjectName(L"ground") == true) {
				//ステージを作成する。
				m_stage = NewGO<Stage>(0, "stage");
				//座標を設定する。
				m_stage->SetPosition(objData.position);
				//大きさを設定する。
				m_stage->SetScale(objData.scale);
				//回転を設定する。
				m_stage->SetRotation(objData.rotation);
				return true;
			}
			//名前がwallだったら
			else if (objData.EqualObjectName(L"wall") == true) {
				//ステージを作成する。
				m_wall = NewGO<Wall>(0, "wall");
				//座標を設定する。
				m_wall->SetPosition(objData.position);
				//大きさを設定する。
				m_wall->SetScale(objData.scale);
				//回転を設定する。
				m_wall->SetRotation(objData.rotation);
				return true;
			}
			else if (objData.EqualObjectName(L"shadownity") == true) {
				//エネミーを作成する。
				m_enemy = NewGO<Enemy>(0, "enemy");
				//座標を設定する。				
				m_enemy->SetPosition(objData.position);
				//初期座標を記憶させる。
				m_enemy->SetInitialPosition(objData.position);
				//大きさを設定する。
				m_enemy->SetScale(objData.scale);
				//回転を設定する。
				m_enemy->SetRotation(objData.rotation);
				//初期回転を記憶させる。
				m_enemy->SetInitialRotation(objData.rotation);
				return true;
			}
			return true;
		});
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	//ワイヤーフレームを表示　デバッグ用。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{
	
}