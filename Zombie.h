#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Zombie
{
private:
	
	//Speed of zombies
	const float Bloater_Speed = 40;
	const float Chaser_Speed = 80;
	const float Crawer_Speed = 20;
	const int Max_Varriance = 30;//speed may vary
	const int Offset = 101 - Max_Varriance;

	//HP of zombies
	const float Bloater_Health = 3;
	const float Chaser_Health = 1;
	const float Crawler_Health = 2;

	Vector2f m_Position;
	Sprite m_Sprite;
	float m_Speed;
	float m_Health;
	bool m_Alive;

public:

	bool hit();
	bool isAlive();
	void Spawn(float startX, float startY, int type, int seed);
	FloatRect getPosition();
	Sprite getSprite();
	void Update(float elapsedTime, Vector2f playerLocation);

};