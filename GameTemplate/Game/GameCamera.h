#pragma once
class Game;
class Player;
//ゲーム中のカメラを制御する。
class GameCamera : public IGameObject
{
public:
	GameCamera() {};
	~GameCamera();
	bool Start();
	void Update();
	/////////////////////////////////////
	//メンバ変数
	/////////////////////////////////////
private:
	Player* m_Player;	//プレイヤー。
	Vector3 m_toCameraPos;	//注視点から視点に向かうベクトル。
	Quaternion m_quaternion;
	Game* game;
};
