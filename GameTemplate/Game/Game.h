#pragma once

#include "Level3DRender/LevelRender.h"
#include "level2DRender/Level2DRender.h"

class Player;
class GameCamera;
class Stage;
class Wall;
class Enemy;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	Player* m_player = nullptr;
	SkyCube* m_skyCube = nullptr;							//‹óB
	Stage* m_stage = nullptr;
	GameCamera* m_gameCamera = nullptr;
	Enemy* m_enemy = nullptr;
	Wall* m_wall = nullptr;
	LevelRender m_levelRender;								//ƒŒƒxƒ‹
	
};


