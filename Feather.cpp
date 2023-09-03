#include "pch.h"
#include "Feather.h"
#include "Texture.h"
#include "Level.h"
#include "Avatar.h"
#include "utils.h"
#include <iostream>

Feather::Feather(const Point2f& point, int sign, float distance)
	: m_Sign{ sign }
	, m_pFeather{ new Texture{"Resources/Demon's crest/Enemy/BossFeather.png" } }
	, m_Shape{}
	, m_Scale{ 2.25f }
	, m_Velocity{ Vector2f{0, 0} }
	, m_Acceleration{ Vector2f{0, -981.0f} }
	, m_LandAccSec{ 0 }
	, m_FeatherLifetime{ true }
	, m_Distance{ distance }
{
	
	m_Shape = Rectf{ point.x, point.y, m_pFeather->GetWidth() * m_Scale, m_pFeather->GetHeight() * m_Scale };
}

Feather::~Feather()
{
	delete m_pFeather;
	m_pFeather = nullptr;
}

void Feather::Draw()
{
	//utils::DrawRect(m_Shape); //debug rect

	glPushMatrix();
	glTranslatef(m_Shape.left , m_Shape.bottom, 0);
	glScalef((float)m_Scale, (float)m_Scale, (float)m_Scale);
	if (m_Sign < 0.0f)
	{
		glScalef(-1, 1, 1);
		glTranslatef(-m_Shape.width / 2, 0, 0);
	}
	m_pFeather->Draw();
	glPopMatrix();

}

void Feather::Update(float elapsedSec, Level& level)
{

	if (level.IsOnGround(m_Shape, m_Velocity))
	{
		const float timeConst{ 1.5f };
		m_LandAccSec += elapsedSec;
		if (m_LandAccSec >= timeConst)
		{
			m_LandAccSec = 0;
			m_FeatherLifetime = false;
		}
	}
	else
	{
		m_Velocity.x = -m_Distance * m_Sign;
		m_Shape.left += m_Velocity.x * elapsedSec;
	}

	m_Velocity.y += m_Acceleration.y * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;



	level.HandleCollision(m_Shape, m_Velocity);
}

bool Feather::GetFeatherLifetime() const
{
	return m_FeatherLifetime;
}

bool Feather::IsOverlapping(const Rectf& rect)
{
	if (utils::IsOverlapping(rect, m_Shape))
	{
		return true;
	}
	return false;
}

