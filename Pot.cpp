#include "pch.h"
#include "Pot.h"
#include "Texture.h"
#include "CoinBig.h"
#include "CoinSmall.h"
#include "CoinSkull.h"
#include "Coin.h"
#include "health.h"
#include "BigHealth.h"
#include "SmallHealth.h"
#include "Rubble.h"
#include "RubbleSmall.h"
#include "RubbleBig.h"
#include "Level.h"
#include "utils.h"

Pot::Pot(const Point2f& point, int numberRocks, std::string type, int number)
	: m_ActingState{ ActingState::notBroken }
	, m_Scale{2}
	, m_pPot{ new Texture{"Resources/Demon's crest/Item/Pot.png"} }
	, m_Type{type}
	, m_Number{number}
	, m_Velocity{ Vector2f{0, 0} }
	, m_Acceleration{ Vector2f{0, -981.0f} }
	, m_NumberOfRocks{numberRocks}

{
	m_Shape = Rectf{ point.x, point.y, m_pPot->GetWidth() * m_Scale, m_pPot->GetHeight() * m_Scale };

}

Pot::~Pot()
{
	delete m_pPot;
	m_pPot = nullptr;

	for (Coin* i : m_pCoin)
	{
		delete i;
		i = nullptr;
	}

	for (Health* i : m_pHealth)
	{
		delete i;
		i = nullptr;
	}

	for (Rubble* i : m_pRubbleContainer)
	{
		delete i;
		i = nullptr;
	}
}

void Pot::Update(float elapsedSec, Level& level)
{
	StateChanges(elapsedSec, level);

	for (Rubble* i : m_pRubbleContainer)
	{
		i->Update(elapsedSec, level);
	}

	for (Coin* i : m_pCoin)
	{
		i->Update(elapsedSec, level);
	}
	for (Health* i : m_pHealth)
	{
		i->Update(elapsedSec, level);
	}
}

void Pot::Draw()
{

	if (m_ActingState == ActingState::notBroken)
	{
		m_pPot->Draw(m_Shape);
	}

	for (Coin* i : m_pCoin)
	{
		i->Draw();
	}

	for (Health* i : m_pHealth)
	{
		i->Draw();
	}

	for (Rubble* i : m_pRubbleContainer)
	{
		i->Draw();
	}
}


void Pot::StateChanges(float elapsedSec, Level& level)
{

	switch (m_ActingState)
	{
	case ActingState::broken:
	{
		for (int i = 0; i < m_NumberOfRocks; i++)
		{
			AddRocksItem(Point2f(m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2));
		} 

		for (int i = 0; i < m_Number; i++)
		{
			AddItem(Point2f(m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2));
		}
		m_ActingState = ActingState::buffer;
	}
	break;
	case ActingState::notBroken:
	{
		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		level.HandleCollision(m_Shape, m_Velocity);
	}
	break;
	
	}
}

void Pot::AddRocksItem(const Point2f& center)
{
	int rand{ GetRand(1,10) };
	if (rand <= 5)
	{
		rand = -1;
	}
	else
	{
		rand = 1;
	}
	Rubble* pTempItem = new RubbleSmall{ center, rand };
	m_pRubbleContainer.push_back(pTempItem);
}


void Pot::AddItem(const Point2f& center)
{
	int rand{ GetRand(1,10) };
	if (rand <= 5)
	{
		rand = -1;
	}
	else
	{
		rand = 1;
	}
	//
	if (m_Type == "small")
	{
		Coin* pTempItem = new CoinSmall{ center, rand };
		m_pCoin.push_back(pTempItem);
	}
	else if (m_Type == "big")
	{
		Coin* pTempItem = new CoinBig{ center, rand };
		m_pCoin.push_back(pTempItem);
	}
	else if (m_Type == "skull")
	{
		Coin* pTempItem = new CoinSkull{ center, rand };
		m_pCoin.push_back(pTempItem);
	}
	else if(m_Type == "BigHealth")
	{
		Health* pTempItem = new BigHealth{ center, rand };
		m_pHealth.push_back(pTempItem);
	}
	else if (m_Type == "SmallHealth")
	{
		Health* pTempItem = new SmallHealth{ center, rand };
		m_pHealth.push_back(pTempItem);
	}
}

void Pot::DeleteItem()
{
	int counter{ 0 };
	bool checkHit{ false };
	for (Coin* i : m_pCoin)
	{
		if (i->GetCoinLifetime() == false)
		{
			delete m_pCoin.at(counter);
			m_pCoin.at(counter) = nullptr;
			m_pCoin.at(counter) = m_pCoin.back();
			m_pCoin.pop_back();
			++counter;
		}
	}
}

int Pot::GetRand(int a, int b)
{
	int random{};

	random = rand() % (b - a + 1) + a;
	return random;
}

Pot::ActingState Pot::GetActingState() const
{
	return m_ActingState;
}

bool Pot::IsOverlapping(const Rectf& rect)
{
	if (m_ActingState != ActingState::buffer)
	{
		if (utils::IsOverlapping(rect, m_Shape))
		{
			m_ActingState = ActingState::broken;
			return true;
		}
	}
	return false;
}

void Pot::OverlapingRectCoins(const Rectf& rect)
{
	for (Coin* i : m_pCoin)
	{
		if (i->IsOverlapping(rect))
		{
			i->SetCoinLifetime(false);
		}
	}
}

int Pot::OverlapingRectHealth(const Rectf& rect)
{
	for (Health* i : m_pHealth)
	{
		int overLap{ i->IsOverlapping(rect) };
		if (overLap != 0)
		{
			i->SetHealthLifetime(false);
			return overLap;
		}
	}
	return 0;
}