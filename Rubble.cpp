#include "pch.h"
#include "Rubble.h"
#include "Level.h"

Rubble::Rubble(const Point2f& point, int sign)
	: m_Scale{ 2.0f }
	, m_Velocity{ Vector2f{0, 225.0f} }
	, m_Acceleration{ Vector2f{0, -981.0f / 3} }
	, m_Shape{}
	, m_LandAccSec{ 0 }
	, m_RubbleLifetime{ true }
	, m_Sign{ sign }
	, m_XVel{}
{

}


void Rubble::Update(float elapsedSec, Level& level)
{
	if (level.IsOnGround(m_Shape, m_Velocity))
	{
		const float timeConst{ 0.75f };
		m_LandAccSec += elapsedSec;
		if (m_LandAccSec >= timeConst)
		{
			m_LandAccSec = 0;
			m_RubbleLifetime = false;
		}
	}
	else
	{
		m_Velocity.x = (float)m_XVel * m_Sign;
		m_Shape.left += m_Velocity.x * elapsedSec;
	}

	m_Velocity.y += m_Acceleration.y * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	level.HandleCollision(m_Shape, m_Velocity);
}

bool Rubble::GetRubbleLifetime() const
{
	return m_RubbleLifetime;
}

int Rubble::GetRand(int a, int b)
{
	int random{};

	random = rand() % (b - a + 1) + a;
	return random;
}