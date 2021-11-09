#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Stage.h"
#include "GameCamera.h"

bool Game::Start()
{
	NewGO<Player>(0, "player");
	NewGO<Stage>(0, "stage");
	NewGO<GameCamera>(0, "gameCamera");

	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{
	
}