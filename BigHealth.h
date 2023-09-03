#pragma once
#include "Health.h"

class Texture;

class BigHealth : public Health
{
public:
	explicit BigHealth(const Point2f& point, int sign);
	BigHealth(const BigHealth& BigHealth) = delete;
	BigHealth(BigHealth&& BigHealth) noexcept = delete;
	BigHealth& operator=(const BigHealth& BigHealth) = delete;
	BigHealth& operator=(BigHealth&& BigHealth) noexcept = delete;
	~BigHealth();

	virtual void Draw() override;
	virtual int IsOverlapping(const Rectf& rect) override;


private:
	Texture* m_BigHealth;
};

