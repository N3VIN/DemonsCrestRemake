#pragma once

#include <vector>
//#include "Statue.h"

class Level;
class Statue;
class Pot;

class ItemsManager final
{
public:
	explicit ItemsManager();
	~ItemsManager();
	ItemsManager(const ItemsManager&) = delete;
	ItemsManager& operator=(const ItemsManager&) = delete;
	ItemsManager(ItemsManager&&) noexcept = delete;
	ItemsManager& operator=(ItemsManager&&) noexcept = delete;

	void Update(float elapsedSec, Level& level);
	void Draw() const;

	size_t GetStatueItemSize() const;
	size_t GetPotItemSize() const;
	void AddStatueItem(const Point2f& center, int number, std::string type, int numberOfItems);
	void AddStatueItem(const Point2f& center, int number);
	void AddPotItem(const Point2f& center, int numberRocks, std::string type, int number);
	bool OverlapingRect(const Rectf& rect);
	void OverlapingRectCoins(const Rectf& rect);
	int OverlapingRectHealth(const Rectf& rect);
	
private:
	std::vector<Statue*> m_pStatueItems;
	std::vector<Pot*> m_pPotItems;

	// 
	void DeletePotItems();
};
