#include "pch.h"
#include "BigHealth.h"
#include "Texture.h"
#include "utils.h"
#include "SoundEffect.h"


BigHealth::BigHealth(const Point2f& point, int sign)
	: Health{ point, sign }
	, m_BigHealth{ new Texture{"Resources/Demon's crest/Item/BigHealth.png"} }
{
	m_Shape = Rectf{ point.x, point.y, m_BigHealth->GetWidth() * m_Scale, m_BigHealth->GetHeight() * m_Scale };
	m_XVel = GetRand(30, 65); // generates Random value between 30 and 65.
}

BigHealth::~BigHealth()
{
	delete m_BigHealth;
	m_BigHealth = nullptr;
}

void BigHealth::Draw()
{
	if (m_HealthLifetime)
	{
		m_BigHealth->Draw(m_Shape);
	}

}

int BigHealth::IsOverlapping(const Rectf& rect)
{
	if (m_HealthLifetime)
	{
		if (utils::IsOverlapping(rect, m_Shape))
		{
			m_pHealthSound->Play(0);
			m_HealthLifetime = false;
			int bigHealthSpecifier{ 2 };
			return bigHealthSpecifier;
		}
	}
	return 0;
}