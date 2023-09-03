#include "pch.h"
#include "Axe.h"
#include "Sprite.h"
#include "Level.h"
#include "Avatar.h"
#include "utils.h"
#include <iostream>

Axe::Axe(const Point2f& point, int sign, float distance)
	: m_Sign{sign}
	, m_pAxe{ new Sprite{"Resources/Demon's crest/Enemy/Axe.png", 4, 1, 8.0f, } }
	, m_Shape{}
	, m_Scale{1.5f}
	, m_Velocity{ Vector2f{0, 125.0f} }
	, m_Acceleration{ Vector2f{0, -981.0f / 3} }
	, m_LandAccSec{0}
	, m_AxeLifetime{true}
	, m_Distance{distance}
{
	const int width{ 18 };
	m_pAxe->SetFrameWidth(width);
	m_Shape = Rectf{ point.x, point.y, m_pAxe->GetFrameWidth(), m_pAxe->GetFrameHeight() };
}

Axe::~Axe()
{
	delete m_pAxe;
	m_pAxe = nullptr;
}

void Axe::Draw()
{
	//utils::DrawRect(m_Shape); //debug rect

	glPushMatrix();
	const float widthFactor{ 2.5f };
	glTranslatef(m_Shape.left - m_Shape.width / widthFactor, m_Shape.bottom, 0);
	glScalef((float)m_Scale, (float)m_Scale, (float)m_Scale);
	if (m_Sign < 0.0f)
	{
		glScalef(-1, 1, 1);
		const float scaleFactor{ 1.75f };
		glTranslatef(-m_Shape.width / (m_Scale / scaleFactor), 0, 0);
	}
	m_pAxe->Draw(Point2f(0, 0), 1);
	glPopMatrix();

}

void Axe::Update(float elapsedSec, Level& level)
{
	m_pAxe->Update(elapsedSec);

	if (level.IsOnGround(m_Shape, m_Velocity))
	{
		m_pAxe->SetActFrame(2);
		const float timeConst{ 0.5f };
		m_LandAccSec += elapsedSec;
		if (m_LandAccSec >= timeConst)
		{
			m_LandAccSec = 0;
			m_AxeLifetime = false;
		}
	}
	else
	{
		const float distanceFactor{ 1.5f };
		m_Velocity.x = -m_Distance / distanceFactor;
		m_Shape.left += m_Velocity.x * elapsedSec;
	}

	m_Velocity.y += m_Acceleration.y * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	

	level.HandleCollision(m_Shape, m_Velocity);
}

bool Axe::GetAxeLifetime() const
{
	return m_AxeLifetime;
}

bool Axe::IsOverlapping(const Rectf& rect)
{
	if (utils::IsOverlapping(rect, m_Shape))
	{
		return true;
	}
	return false;
}

