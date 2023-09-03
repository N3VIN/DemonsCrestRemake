#include "pch.h"
#include "AxeDemonManager.h"

AxeDemonManager::AxeDemonManager()
{

}

AxeDemonManager::~AxeDemonManager()
{
	for (AxeDemon* i : m_pItems)
	{
		delete i;
		i = nullptr;
	}
}

void AxeDemonManager::AddItem(const Point2f& center)
{
	AxeDemon* pTempItem = new AxeDemon{ center };
	m_pItems.push_back(pTempItem);
}

void AxeDemonManager::Update(float elapsedSec, Level& level, const Avatar& avatar)
{
	for (AxeDemon* i : m_pItems)
	{
		i->Update(elapsedSec, level, avatar);
	}
}

void AxeDemonManager::Draw() const
{
	for (AxeDemon* i : m_pItems)
	{
		i->Draw();
	}
}

size_t AxeDemonManager::GetSize() const
{
	return m_pItems.size();
}

void AxeDemonManager::DeleteItem()
{
	int counter{ 0 };
	bool checkHit{ false };
	for (AxeDemon* i : m_pItems)
	{
		if (i != nullptr && (int)i->GetActingState() == 0)
		{
			delete m_pItems.at(counter);
			m_pItems.at(counter) = nullptr;
			m_pItems.at(counter) = m_pItems.back();
			m_pItems.pop_back();
		}
		++counter;
	}
}

bool AxeDemonManager::OverlapingRect(const Rectf& rect)
{
	for (AxeDemon* i : m_pItems)
	{
		if (i->IsOverlapping(rect))
		{
			return false;
		}
	}
	return true;
}

bool AxeDemonManager::OverlapingRectAxe(const Rectf& rect)
{
	for (AxeDemon* i : m_pItems)
	{
		if (i->IsOverlappingAxe(rect))
		{
			return true;
		}
	}
	return false;
}
