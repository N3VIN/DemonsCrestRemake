#include "pch.h"
#include "SmallHealth.h"
#include "Texture.h"
#include "utils.h"
#include "SoundEffect.h"

SmallHealth::SmallHealth(const Point2f& point, int sign)
	: Health{ point, sign }
	, m_pSmallHealth{ new Texture{"Resources/Demon's crest/Item/SmallHealth.png"} }
{

	m_Shape = Rectf{ point.x, point.y, m_pSmallHealth->GetWidth() * m_Scale, m_pSmallHealth->GetHeight() * m_Scale };
	m_XVel = GetRand(30, 65); // Generates random val between 30 and 65.
}

SmallHealth::~SmallHealth()
{

	delete m_pSmallHealth;
	m_pSmallHealth = nullptr;
}

void SmallHealth::Draw()
{
	if (m_HealthLifetime)
	{
		m_pSmallHealth->Draw(m_Shape);
	}
}

int SmallHealth::IsOverlapping(const Rectf& rect)
{
	if (m_HealthLifetime)
	{
		if (utils::IsOverlapping(rect, m_Shape))
		{
			m_pHealthSound->Play(0);
			m_HealthLifetime = false;
			return 1;
		}
	}
	return 0;
}