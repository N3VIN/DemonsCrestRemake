#include "pch.h"
#include "Phantom.h"
#include "Sprite.h"
#include "Level.h"
#include "utils.h"
#include "SoundEffect.h"


Phantom::Phantom(const Point2f& point)
	: m_HorSpeed{ -50.0f }
	, m_Velocity{ Vector2f{0, 0} }
	, m_Acceleration{ Vector2f{0, -981.0f / 3} }
	, m_ActingState{ ActingState::spawn }
	, m_Scale{ 2.0f }
	, m_Path{ "Resources/Demon's crest/Enemy/Phantom.png" }
	, m_pPhantom{ new Sprite{ m_Path, 4.0f} }
	, m_Width{ 31.0f }
	, m_Height{ 41.0f }
	, m_Sign{1}
	, m_pFireSound{ new SoundEffect{"Resources/Demon's crest/Sounds/fire.mp3"} }

{
	m_Shape = Rectf{ point.x, point.y, (m_Width / 2) * m_Scale, (m_Height / 1.1f) * m_Scale };
	m_pPhantom->SetFrameHeight(m_Height);
	m_pPhantom->SetFrameWidth(m_Width);
}

Phantom::~Phantom()
{
	delete m_pPhantom;
	m_pPhantom = nullptr;
	//
	delete m_pFireSound;
	m_pFireSound = nullptr;
}

void Phantom::Update(float elapsedSec, Level& level)
{
	m_pPhantom->Update(elapsedSec);
	StateChanges(elapsedSec, level);

}

void Phantom::Draw()
{
	float bottom = ((int)m_ActingState * m_Height);
	if (m_ActingState != ActingState::dead)
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
		m_pPhantom->Draw(Point2f(0, 0), 1, bottom);
		glPopMatrix();
	}
}

Phantom::ActingState Phantom::GetActingState() const
{
	return m_ActingState;
}

void Phantom::StateChanges(float elapsedSec, Level& level)
{
	switch (m_ActingState)
	{
	case ActingState::move:
	{
		m_pPhantom->SetCols(4);
		if (level.HitWall(m_Shape, m_Velocity))
		{
			m_ActingState = ActingState::turn;
		}

		m_Velocity.x = m_HorSpeed * m_Sign;
		m_Shape.left += m_Velocity.x * elapsedSec;

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		level.HandleCollision(m_Shape, m_Velocity);
	}
	break;

	case ActingState::turn:
	{
		m_pPhantom->SetCols(5);
		int frame{ m_pPhantom->GetActFrame() };

		if (frame == 4)
		{
			m_Sign *= -1;
			m_ActingState = ActingState::move;
		}

		level.HandleCollision(m_Shape, m_Velocity);
	}
	break;

	case ActingState::spawn:
	{
		m_pPhantom->SetCols(9);

		int frame{ m_pPhantom->GetActFrame() };
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pPhantom->SetActFrame(0);
			doOnce = true;
		}
		if (frame == 8)
		{
			m_ActingState = ActingState::move;
		}

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		level.HandleCollision(m_Shape, m_Velocity);

	}
	break;
	case ActingState::death:
	{
		m_pPhantom->SetCols(11);
		int frame{ m_pPhantom->GetActFrame() };

		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pFireSound->Play(0);
			m_pPhantom->SetActFrame(0);
			doOnce = true;
		}
		if (frame == 10)
		{
			m_ActingState = ActingState::dead;
			doOnce = false;
		}

		level.HandleCollision(m_Shape, m_Velocity);
	}
	break;
	}
}

bool Phantom::IsOverlapping(const Rectf& rect)
{
	if (utils::IsOverlapping(rect, m_Shape))
	{
		m_ActingState = ActingState::death;
		return true;
	}
	return false;
}