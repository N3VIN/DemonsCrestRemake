#pragma once

#include <vector>
#include "AxeDemon.h"

class Level;

class AxeDemonManager final
{
public:
	explicit AxeDemonManager();
	~AxeDemonManager();
	AxeDemonManager(const AxeDemonManager&) = delete;
	AxeDemonManager& operator=(const AxeDemonManager&) = delete;
	AxeDemonManager(AxeDemonManager&&) noexcept = delete;
	AxeDemonManager& operator=(AxeDemonManager&&) noexcept = delete;

	void AddItem(const Point2f& center);
	void Update(float elapsedSec, Level& level, const Avatar& avatar);
	void Draw() const;

	size_t GetSize() const;
	void DeleteItem();
	bool OverlapingRect(const Rectf& rect);
	bool OverlapingRectAxe(const Rectf& rect);


private:
	std::vector<AxeDemon*> m_pItems;
};

