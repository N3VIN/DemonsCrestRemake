#pragma once
#include "rubble.h"

class Texture;

class RubbleBig : public Rubble
{
public:
	explicit RubbleBig(const Point2f& point, int sign);
	RubbleBig(const RubbleBig& RubbleBig) = delete;
	RubbleBig(RubbleBig&& RubbleBig) noexcept = delete;
	RubbleBig& operator=(const RubbleBig& RubbleBig) = delete;
	RubbleBig& operator=(RubbleBig&& RubbleBig) noexcept = delete;
	~RubbleBig();

	virtual void Draw() override;

private:
	Texture* m_pRubbleBig;
};

