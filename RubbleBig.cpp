#include "pch.h"
#include "RubbleBig.h"
#include "Texture.h"

RubbleBig::RubbleBig(const Point2f& point, int sign)
	: Rubble{ point, sign }
	, m_pRubbleBig{ new Texture{"Resources/Demon's crest/Item/RubbleBig.png"} }
{
	m_Shape = Rectf{ point.x, point.y, m_pRubbleBig->GetWidth() * m_Scale, m_pRubbleBig->GetHeight() * m_Scale };
	m_XVel = GetRand(30, 65); // Generates random val between 30, 65.
}

RubbleBig::~RubbleBig()
{
	delete m_pRubbleBig;
	m_pRubbleBig = nullptr;
}

void RubbleBig::Draw()
{
	if (m_RubbleLifetime)
	{
		m_pRubbleBig->Draw(m_Shape);
	}

}

