#include "pch.h"
#include "RubbleSmall.h"
#include "Texture.h"

RubbleSmall::RubbleSmall(const Point2f& point, int sign)
	: Rubble{ point, sign }
	, m_pRubbleSmall{ new Texture{"Resources/Demon's crest/Item/RubbleSmall.png"} }
{
	m_Shape = Rectf{ point.x, point.y, m_pRubbleSmall->GetWidth() * m_Scale, m_pRubbleSmall->GetHeight() * m_Scale };
	m_XVel = GetRand(25, 40); // Generates random val between 25 and 40.
}

RubbleSmall::~RubbleSmall()
{
	delete m_pRubbleSmall;
	m_pRubbleSmall = nullptr;
}

void RubbleSmall::Draw()
{
	if (m_RubbleLifetime)
	{
		m_pRubbleSmall->Draw(m_Shape);
	}

}

