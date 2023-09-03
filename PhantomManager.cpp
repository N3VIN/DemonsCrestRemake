#include "pch.h"
#include "PhantomManager.h"

PhantomManager::PhantomManager()
{

}

PhantomManager::~PhantomManager()
{
	for (Phantom* i : m_pItems)
	{
		delete i;
		i = nullptr;
	}
}

void PhantomManager::AddItem(const Point2f& center)
{
	Phantom* pTempItem = new Phantom{ center };
	m_pItems.push_back(pTempItem);
}

void PhantomManager::Update(float elapsedSec, Level& level)
{
	for (Phantom* i : m_pItems)
	{
		i->Update(elapsedSec, level);
	}
}

void PhantomManager::Draw() const
{
	for (Phantom* i : m_pItems)
	{
		i->Draw();
	}
}

size_t PhantomManager::GetSize() const
{
	return m_pItems.size();
}

void PhantomManager::DeleteItem()
{
	int counter{ 0 };
	//size_t position{ 0 };
	bool checkHit{ false };
	for (Phantom* i : m_pItems)
	{
		if ((int)i->GetActingState() == 0)
		{
			delete m_pItems.at(counter);
			m_pItems.at(counter) = nullptr;
			m_pItems.at(counter) = m_pItems.back(); 
			m_pItems.pop_back();
			++counter;
		}
	}
}

bool PhantomManager::OverlapingRect(const Rectf& rect)
{
	for (Phantom* i : m_pItems)
	{
		if (i->IsOverlapping(rect))
		{
			return false;
		}
	}
	return true;
}
