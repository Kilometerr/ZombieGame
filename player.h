#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Player
{

private:
	const float startSpeed = 200;
	const float startHealth = 100;
	
	//Player pos sprite and texture
	Vector2f m_Position;
	Sprite m_Sprite;
	Texture m_Texture;
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;
	int m_Health;
	int m_MaxHealth;
	Time m_LastHit;
	float m_Speed;

	//Screen resolution
	Vector2f m_Resolution;

	//Current area size
	IntRect m_Arena;
	int m_TileSize;

public:

	//Constructor and func def
	Player();
	
	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	void resetPlayerStats();
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void stopUp();
	void stopDown();
	void stopLeft();
	void stopRight();
	void Update(float elapsedTime, Vector2i mousePosition);
	void upgradeSpeed();
	void upgradeHealth();
	void increseHealthLevel(int amount);

	bool hit(Time timeHit);
	
	Time getLastHitTime();

	FloatRect getPosition();

	Vector2f getCenter();

	float getRotation();

	Sprite getSprite();

	int getHealth();

};
