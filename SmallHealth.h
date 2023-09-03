#pragma once
#include "Health.h"

class Texture;

class SmallHealth : public Health
{
public:
	explicit SmallHealth(const Point2f& point, int sign);
	SmallHealth(const SmallHealth& SmallHealth) = delete;
	SmallHealth(SmallHealth&& SmallHealth) noexcept = delete;
	SmallHealth& operator=(const SmallHealth& SmallHealth) = delete;
	SmallHealth& operator=(SmallHealth&& SmallHealth) noexcept = delete;
	~SmallHealth();

	virtual void Draw() override;
	virtual int IsOverlapping(const Rectf& rect) override;

private:
	Texture* m_pSmallHealth;
};

