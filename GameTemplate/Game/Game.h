#pragma once

#include "Level3DRender/LevelRender.h"
#include "level2DRender/Level2DRender.h"

class Player;
class GameCamera;
class Stage;
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
	Stage* m_stage = nullptr;
	GameCamera* m_gameCamera = nullptr;
};


