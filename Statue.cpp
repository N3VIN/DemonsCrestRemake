#include "pch.h"
#include "Statue.h"
#include "Texture.h"
#include "Rubble.h"
#include "RubbleBig.h"
#include "RubbleSmall.h"
#include "CoinBig.h"
#include "CoinSmall.h"
#include "CoinSkull.h"
#include "Coin.h"
#include "health.h"
#include "BigHealth.h"
#include "SmallHealth.h"
#include "Level.h"
#include "utils.h"

Statue::Statue(const Point2f& point, int number)
	: m_ActingState{ActingState::notBroken}
	, m_Scale{2}
	, m_pNotBroken{ new Texture{"Resources/Demon's crest/Item/StatueNB.png"} }
	, m_pBroken{ new Texture{"Resources/Demon's crest/Item/BrokenStatue.png"} }
	, m_Number{ number }
	, m_Type{}
	, m_NumberOfItems{}
	, m_Velocity{ Vector2f{0, 0} }
	, m_Acceleration{ Vector2f{0, -981.0f} }

{
	m_Shape = Rectf{ point.x, point.y, m_pNotBroken->GetWidth() * m_Scale, m_pNotBroken->GetHeight() * m_Scale };

}

Statue::Statue(const Point2f& point, int number, std::string type, int numberOfItems)
	:m_ActingState{ ActingState::notBroken }
	, m_Scale{2}
	, m_pNotBroken{ new Texture{"Resources/Demon's crest/Item/StatueNB.png"} }
	, m_pBroken{ new Texture{"Resources/Demon's crest/Item/BrokenStatue.png"} }
	, m_Number{ number }
	, m_Type{ type }
	, m_NumberOfItems{numberOfItems}
	, m_Velocity{ Vector2f{0, 0} }
	, m_Acceleration{ Vector2f{0, -981.0f} }
{
	m_Shape = Rectf{ point.x, point.y, m_pNotBroken->GetWidth() * m_Scale, m_pNotBroken->GetHeight() * m_Scale };

}

Statue::~Statue()
{
	delete m_pNotBroken;
	m_pNotBroken = nullptr;
	delete m_pBroken;
	m_pBroken = nullptr;

	for (Rubble* i : m_pRubbleContainer)
	{
		delete i;
		i = nullptr;
	}

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
}

void Statue::Update(float elapsedSec, Level& level)
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

void Statue::Draw()
{
	//utils::DrawRect(m_Shape); //debug rect

	if (m_ActingState == ActingState::broken || m_ActingState == ActingState::buffer)
	{
		m_pBroken->Draw(m_Shape);
	}
	else
	{
		m_pNotBroken->Draw(m_Shape);
	}

	for (Rubble* i : m_pRubbleContainer)
	{
		i->Draw();
	}

	for (Coin* i : m_pCoin)
	{
		i->Draw();
	}

	for (Health* i : m_pHealth)
	{
		i->Draw();
	}
}

void Statue::StateChanges(float elapsedSec, Level& level)
{

	switch (m_ActingState)
	{
	case ActingState::broken:
	{
		for (int i = 0; i < m_Number; i++)
		{
			AddRocksItem(Point2f(m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2));
		}
		// creates pickup items.
		for (int i = 0; i < m_NumberOfItems; i++)
		{
			AddPickupItem(Point2f(m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2));
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

void Statue::AddRocksItem(const Point2f& center)
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

	int rand2{ GetRand(2,11) };
	if (rand2 <= 6)
	{

		Rubble* pTempItem = new RubbleBig{ center, rand };
		m_pRubbleContainer.push_back(pTempItem);

	}
	else
	{
		Rubble* pTempItem = new RubbleSmall{ center, rand };
		m_pRubbleContainer.push_back(pTempItem);

	}
}

void Statue::AddPickupItem(const Point2f& center)
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
	else if (m_Type == "BigHealth")
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

void Statue::DeleteItem()
{
	int counter{ 0 };
	bool checkHit{ false };
	for (Rubble* i : m_pRubbleContainer)
	{
		if (i->GetRubbleLifetime() == false)
		{
			delete m_pRubbleContainer.at(counter);
			m_pRubbleContainer.at(counter) = nullptr;
			m_pRubbleContainer.at(counter) = m_pRubbleContainer.back();
			m_pRubbleContainer.pop_back();
			++counter;
		}
	}
}

int Statue::GetRand(int a, int b)
{
	int random{};

	random = rand() % (b - a + 1) + a;
	return random;
}

Statue::ActingState Statue::GetActingState() const
{
	return m_ActingState;
}

bool Statue::IsOverlapping(const Rectf& rect)
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

void Statue::OverlapingRectCoins(const Rectf& rect)
{
	for (Coin* i : m_pCoin)
	{
		if (i->IsOverlapping(rect))
		{
			i->SetCoinLifetime(false);
		}
	}
}

int Statue::OverlapingRectHealth(const Rectf& rect)
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

