#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Stage.h"
#include "GameCamera.h"

bool Game::Start()
{
	m_player = NewGO<Player>(0, "player");
	m_stage = NewGO<Stage>(0, "stage");
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{
	
}