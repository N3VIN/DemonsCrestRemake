#include "pch.h"
#include "ExtraHealth.h"
#include "Texture.h"
#include "utils.h"

ExtraHealth::ExtraHealth(const Point2f& point, int sign)
	: Health{ point, sign }
	, m_pExtraHealth{ new Texture{"Resources/Demon's crest/Item/ExtraHealth.png"} }
	
{
	m_Shape = Rectf{ point.x, point.y, m_pExtraHealth->GetWidth() * m_Scale, m_pExtraHealth->GetHeight() * m_Scale };
	m_XVel = GetRand(30, 65); // Generates rand va; between 30 and 65.
	m_TimeConst = 200000.0f;
}

ExtraHealth::~ExtraHealth()
{
	delete m_pExtraHealth;
	m_pExtraHealth = nullptr;
}

void ExtraHealth::Draw()
{
	if (m_HealthLifetime)
	{
		m_pExtraHealth->Draw(m_Shape);
	}
}

int ExtraHealth::IsOverlapping(const Rectf& rect)
{
	if (m_HealthLifetime)
	{
		if (utils::IsOverlapping(rect, m_Shape))
		{
			m_HealthLifetime = false;
			const int extraHealthSpecifier{ 3 };
			return extraHealthSpecifier;
		}
	}
	return 0;
}