#pragma once
#include <vector>
#include "Phantom.h"

class Level;

class PhantomManager final
{
public:
	explicit PhantomManager();
	~PhantomManager();
	PhantomManager(const PhantomManager&) = delete;
	PhantomManager& operator=(const PhantomManager&) = delete;
	PhantomManager(PhantomManager&&) noexcept = delete;
	PhantomManager& operator=(PhantomManager&&) noexcept = delete;

	void AddItem(const Point2f& center);
	void Update(float elapsedSec, Level& level);
	void Draw() const;

	size_t GetSize() const;
	void DeleteItem();
	bool OverlapingRect(const Rectf& rect);

private:
	std::vector<Phantom*> m_pItems;


};

