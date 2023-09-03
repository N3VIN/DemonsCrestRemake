#pragma once
#include "rubble.h"

class Texture;

class RubbleSmall : public Rubble
{
public:
	explicit RubbleSmall(const Point2f& point, int sign);
	RubbleSmall(const RubbleSmall& RubbleSmall) = delete;
	RubbleSmall(RubbleSmall&& RubbleSmall) noexcept = delete;
	RubbleSmall& operator=(const RubbleSmall& RubbleSmall) = delete;
	RubbleSmall& operator=(RubbleSmall&& RubbleSmall) noexcept = delete;
	~RubbleSmall();

	virtual void Draw() override;

private:
	Texture* m_pRubbleSmall;
};

