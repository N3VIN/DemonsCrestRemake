#include "pch.h"
#include "ItemsManager.h"
#include "Statue.h"
#include "Pot.h"
#include "Coin.h"
#include "Health.h"

ItemsManager::ItemsManager()

{

}

ItemsManager::~ItemsManager()
{
	for (Statue* i : m_pStatueItems)
	{
		delete i;
		i = nullptr;
	}

	for (Pot* i : m_pPotItems)
	{
		delete i;
		i = nullptr;
	}
}

void ItemsManager::AddStatueItem(const Point2f& center, int number)
{
	Statue* pTempItem = new Statue{ center, number };
	m_pStatueItems.push_back(pTempItem);
}

void ItemsManager::AddStatueItem(const Point2f& center, int number, std::string type, int numberOfItems)
{
	Statue* pTempItem = new Statue{ center, number, type, numberOfItems };
	m_pStatueItems.push_back(pTempItem);
}

void ItemsManager::AddPotItem(const Point2f& center, int numberRocks, std::string type, int number)
{
	Pot* pTempItem = new Pot{ center, numberRocks, type, number};
	m_pPotItems.push_back(pTempItem);

}

void ItemsManager::DeletePotItems()
{
	int counter{ 0 };
	bool checkHit{ false };
	for (Pot* i : m_pPotItems)
	{
		if (i != nullptr && (int)i->GetActingState() == 2)
		{
			delete m_pPotItems.at(counter);
			m_pPotItems.at(counter) = nullptr;
			m_pPotItems.at(counter) = m_pPotItems.back();
			m_pPotItems.pop_back();
			++counter;
		}
	}
}

void ItemsManager::Update(float elapsedSec, Level& level)
{
	for (Statue* i : m_pStatueItems)
	{
		i->Update(elapsedSec, level);
	}

	for (Pot* i : m_pPotItems)
	{
		i->Update(elapsedSec, level);
	}
}

void ItemsManager::Draw() const
{
	for (Statue* i : m_pStatueItems)
	{
		i->Draw();
	}

	for (Pot* i : m_pPotItems)
	{
		i->Draw();
	}
}

size_t ItemsManager::GetStatueItemSize() const
{
	return m_pStatueItems.size();
}

size_t ItemsManager::GetPotItemSize() const
{
	return m_pPotItems.size();
}

bool ItemsManager::OverlapingRect(const Rectf& rect)
{
	for (Statue* i : m_pStatueItems)
	{
		if (i->IsOverlapping(rect))
		{
			return false;
		}
	}
	//
	for (Pot* i : m_pPotItems)
	{
		if (i->IsOverlapping(rect))
		{
			return false;
		}
	}

	return true;
}

void ItemsManager::OverlapingRectCoins(const Rectf& rect)
{

	for (Statue* i : m_pStatueItems)
	{
		i->OverlapingRectCoins(rect);
	}

	for (Pot* i : m_pPotItems)
	{
		i->OverlapingRectCoins(rect);
	}
}

int ItemsManager::OverlapingRectHealth(const Rectf& rect)
{
	for (Statue* i : m_pStatueItems)
	{
		int overLap{ i->OverlapingRectHealth(rect) };

		if (overLap != 0)
		{
			return overLap;
		}
	}

	for (Pot* i : m_pPotItems)
	{
		int overLap{ i->OverlapingRectHealth(rect) };

		if (overLap != 0)
		{
			return overLap;
		}
	}

	return 0;
}
