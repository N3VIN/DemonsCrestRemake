#include "pch.h"
#include "Coin.h"
#include "Level.h"
#include "utils.h"
#include "SoundEffect.h"

Coin::Coin(const Point2f& point, int sign)
	: m_Scale{2}
	, m_Velocity{ Vector2f{0, 150.0f} }
	, m_Acceleration{ Vector2f{0, -981.0f / 3} }
	, m_Shape{}
	, m_LandAccSec{0}
	, m_CoinLifetime{true}
	, m_Sign{sign}
	, m_XVel{}
	, m_pCoin{ new SoundEffect{"Resources/Demon's crest/Sounds/Coin.mp3"} }
{

}

Coin::~Coin()
{
	delete m_pCoin;
	m_pCoin = nullptr;
}

void Coin::Update(float elapsedSec, Level& level)
{
	if (level.IsOnGround(m_Shape, m_Velocity))
	{
		const float timeConst{ 2.25f };
		m_LandAccSec += elapsedSec;
		if (m_LandAccSec >= timeConst)
		{
			m_LandAccSec = 0;
			m_CoinLifetime = false;
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

bool Coin::GetCoinLifetime() const
{
	return m_CoinLifetime;
}

int Coin::GetRand(int a, int b)
{
	int random{};

	random = rand() % (b - a + 1) + a;
	return random;
}

bool Coin::IsOverlapping(const Rectf& rect)
{
	if(m_CoinLifetime)
	{
		if (utils::IsOverlapping(rect, m_Shape))
		{
			m_pCoin->Play(0);
			m_CoinLifetime = false;
			return true;
		}
	}
	return false;
}

void Coin::SetCoinLifetime(bool lifeTime)
{
	m_CoinLifetime = lifeTime;
}
