#include "pch.h"
#include "FireBall.h"
#include "Sprite.h"
#include "Level.h"
#include "utils.h"

FireBall::FireBall(const Point2f& point, int sign)
	: m_Sign{ sign }
	, m_pFireBall{ new Sprite{"Resources/Demon's crest/Hero/FireBall.png", 4, 1, 7.0f, } }
	, m_pFireRing{ new Sprite{"Resources/Demon's crest/Hero/FireBall.png", 3, 1, 6.0f, } }
	, m_Shape{}
	, m_Shape2{}
	, m_Scale{ 1.5f }
	, m_Velocity{ Vector2f{0, 125.0f} }
	, m_FireBallLifetime{ true }
	, m_FireRingLifetime{ true }
	, m_ActingState{ActingState::moving}
	, m_AccuSec{}
{
	const float width{ 100.0f / 4.0f };
	const float height{ 50.0f / 3.0f };

	m_pFireRing->SetFrameWidth(width);
	m_pFireRing->SetFrameHeight(height);
	m_pFireBall->SetFrameHeight(height);
	m_pFireBall->SetFrameWidth(width);

	const float widthFactor{ 1.5f };
	const float heightFactor{ 3.0f };
	m_Shape = Rectf{ point.x, point.y, m_pFireBall->GetFrameWidth()/widthFactor * m_Scale, m_pFireBall->GetFrameHeight()/heightFactor * m_Scale};
	m_Shape2 = Rectf{ point.x, point.y, m_pFireRing->GetFrameWidth()/widthFactor * m_Scale, m_pFireRing->GetFrameHeight()/heightFactor * m_Scale};
}

FireBall::~FireBall()
{
	delete m_pFireBall;
	m_pFireBall = nullptr;

	delete m_pFireRing;
	m_pFireRing = nullptr;
}

void FireBall::Draw()
{

	float bottom = ((int)m_ActingState * (50.0f / 3.0f));

	if (m_ActingState != ActingState::buffer)
	{
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
		m_pFireBall->Draw(Point2f(0, 0), 1, bottom);
		glPopMatrix();
	}
	// 
	//
	if (m_FireRingLifetime)
	{
		glPushMatrix();
		const float widthFactor{ 2.5f };
		glTranslatef(m_Shape2.left - m_Shape2.width / widthFactor, m_Shape2.bottom, 0);
		glScalef((float)m_Scale, (float)m_Scale, (float)m_Scale);
		if (m_Sign < 0.0f)
		{
			glScalef(-1, 1, 1);
			const float scaleFactor{ 1.75f };
			glTranslatef(-m_Shape2.width / (m_Scale / scaleFactor), 0, 0);
		}
		m_pFireRing->Draw(Point2f(0, 0), 1);
		glPopMatrix();
	}

}

void FireBall::Update(float elapsedSec, Level& level)
{
	m_pFireBall->Update(elapsedSec);
	m_pFireRing->Update(elapsedSec);


	switch (m_ActingState)
	{
		case ActingState::moving:
		{
			m_pFireBall->SetFrameSec(1.0f / 7.0f);

			m_pFireBall->SetCols(5);

			static bool doOnce{ false };
			if (!doOnce)
			{
				m_pFireBall->SetActFrame(0);
				doOnce = true;
			}

			if (!level.HitWall(m_Shape, m_Velocity))
			{
				const float xVel{ 300.0f };
				m_Velocity.x = xVel * m_Sign;
				m_Shape.left += m_Velocity.x * elapsedSec;
			}
			else
			{
				m_ActingState = ActingState::hit;
			}

			m_AccuSec += elapsedSec;
			const float accuSecLimit{ 1.5f };
			if (m_AccuSec >= accuSecLimit)
			{
				m_ActingState = ActingState::buffer;
				m_AccuSec = 0;
			}

			level.HandleCollision(m_Shape, m_Velocity);
		}
		break;

		case ActingState::hit:
		{
			m_pFireBall->SetFrameSec(1.0f / 4.0f);

			m_pFireBall->SetCols(5);

			static bool doOnce{ false };
			if (!doOnce)
			{
				m_pFireBall->SetActFrame(0);
				doOnce = true;
			}

			int frame{ m_pFireBall->GetActFrame() };
			if (frame == 4)
			{
				m_ActingState = ActingState::buffer;
				m_FireBallLifetime = false;
				doOnce = false;
			}
		}
		break;

		case ActingState::buffer:
		{
			m_FireBallLifetime = false;
		}
		break;

	}

	m_pFireRing->SetCols(4);
	static bool doOnce{ false };
	if (!doOnce)
	{
		m_pFireRing->SetActFrame(0);
		doOnce = true;
	}

	int frame{ m_pFireRing->GetActFrame() };
	if (frame == 3)
	{
		m_FireRingLifetime = false;
	}

}

bool FireBall::GetFireBallLifetime() const
{
	return m_FireBallLifetime;
}

void FireBall::SetFireBallState(bool Lifetime)
{
	if (Lifetime == false)
	{
		m_ActingState = ActingState::hit;
	}
}

Rectf FireBall::GetShape()
{
	return m_Shape;
}
