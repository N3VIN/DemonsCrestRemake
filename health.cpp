#include "pch.h"
#include "Health.h"
#include "Level.h"
#include "utils.h"
#include "SoundEffect.h"

Health::Health(const Point2f& point, int sign)
	: m_Scale{ 2 }
	, m_Velocity{ Vector2f{0, 150.0f} }
	, m_Acceleration{ Vector2f{0, -981.0f / 3} }
	, m_Shape{}
	, m_LandAccSec{ 0 }
	, m_HealthLifetime{ true }
	, m_Sign{ sign }
	, m_XVel{}
	, m_pHealthSound{ new SoundEffect{"Resources/Demon's crest/Sounds/Health.mp3"} }
	, m_TimeConst{ 2.25f }
{
}

Health::~Health()
{

	delete m_pHealthSound;
	m_pHealthSound = nullptr;
}

void Health::Update(float elapsedSec, Level& level)
{
	if (level.IsOnGround(m_Shape, m_Velocity))
	{
		m_LandAccSec += elapsedSec;
		if (m_LandAccSec >= m_TimeConst)
		{
			m_LandAccSec = 0;
			m_HealthLifetime = false;
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

bool Health::GetHealthLifetime() const
{
	return m_HealthLifetime;
}

int Health::GetRand(int a, int b)
{
	int random{};

	random = rand() % (b - a + 1) + a;
	return random;
}

void Health::SetHealthLifetime(bool lifeTime)
{
	m_HealthLifetime = lifeTime;
}
