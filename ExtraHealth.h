#pragma once
#include "Health.h"

class Texture;

class ExtraHealth : public Health
{
public:
	explicit ExtraHealth(const Point2f& point, int sign);
	ExtraHealth(const ExtraHealth& ExtraHealth) = delete;
	ExtraHealth(ExtraHealth&& ExtraHealth) noexcept = delete;
	ExtraHealth& operator=(const ExtraHealth& ExtraHealth) = delete;
	ExtraHealth& operator=(ExtraHealth&& ExtraHealth) noexcept = delete;
	~ExtraHealth();

	virtual void Draw() override;
	virtual int IsOverlapping(const Rectf& rect) override;

private:
	Texture* m_pExtraHealth;
};

