#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Pickup
{
private:
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;
	Sprite m_Sprite;
	IntRect m_Arena;
	int m_Value; //worth of pickup
	int m_Type; //1=health 2=ammo
	float m_SecondsSinceSpawn;
	float m_SecondsSicneDespawn;
	float m_SecondsToLive;
	float m_SecondsToWait;
	bool m_Spawned;

public:
	Pickup::Pickup(int type);
	void setArena(IntRect arena);
	void spawn();
	FloatRect getPosition();
	Sprite getSprite();
	void update(float elapsedTime);
	bool isSpawned();
	int gotIt();
	void upgrade();
};