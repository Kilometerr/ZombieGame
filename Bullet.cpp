#include "stdafx.h"
#include "Bullet.h"

using namespace sf;

Bullet::Bullet()
{
	m_BulletShape.setSize(Vector2f(4, 4));
}

void Bullet::shoot(float startX, float startY, float targetX, float targetY)
{
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	float gradient = (startX - targetX) / (startY - targetY); //calculate flight path
	
	if (gradient < 0)
	{
		gradient *= -1;
	}

	float ratioXY = m_bulletSpeed / (1 + gradient);

	//Set the speed
	m_bulletDistanceY = ratioXY;
	m_bulletDistanceX = ratioXY * gradient;

	//aim direction of bullet
	if (targetX < startX)
	{
		m_bulletDistanceX *= -1;
	}

	if (targetY < startY)
	{
		m_bulletDistanceY *= -1;
	}

	//max range
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	m_BulletShape.setPosition(m_Position);

}

void Bullet::stop()
{
	m_InFlight = false;
}

bool Bullet::isInFlight()
{
	return m_InFlight;
}

FloatRect Bullet::getPosition()
{
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}

void Bullet::update(float elapsedTime)
{
	m_Position.x += m_bulletDistanceX * elapsedTime;
	m_Position.y += m_bulletDistanceY * elapsedTime;

	// Move the bullet
	m_BulletShape.setPosition(m_Position);

	// Has the bullet gone out of range?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}
}