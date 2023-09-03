#include "pch.h"
#include "AxeDemon.h"
#include "Sprite.h"
#include "Level.h"
#include "Avatar.h"
#include "Axe.h"
#include "utils.h"
#include "SoundEffect.h"

AxeDemon::AxeDemon(const Point2f& point)
	: m_Velocity{ Vector2f{0, 0} }
	, m_Acceleration{ Vector2f{0, -981.0f / 3} }
	, m_ActingState{ ActingState::idle }
	, m_Scale{ 2.0f }
	, m_Path{ "Resources/Demon's crest/Enemy/AxeDemon.png" }
	, m_pAxeDemon{ new Sprite{ m_Path, 4.0f} }
	, m_Width{ 40.0f }
	, m_Height{ 49.0f }
	, m_Sign{ 1 }
	, m_JumpSpeed{ 150.0f }
	, m_pAxeSound{ new SoundEffect{"Resources/Demon's crest/Sounds/axe.mp3"} }
	, m_pFireSound{ new SoundEffect{"Resources/Demon's crest/Sounds/fire.mp3"} }

{
	const float factor{ 1.4f };
	m_Shape = Rectf{ point.x, point.y, (m_Width / factor) * m_Scale, (m_Height / factor) * m_Scale };
	m_pAxeDemon->SetFrameHeight(m_Height);
	m_pAxeDemon->SetFrameWidth(m_Width);
}

AxeDemon::~AxeDemon()
{
	delete m_pAxeDemon;
	m_pAxeDemon = nullptr;

	for (Axe* i : m_pAxeContainer)
	{
		delete i;
		i = nullptr;
	}

	delete m_pAxeSound;
	m_pAxeSound = nullptr;
	//
	delete m_pFireSound;
	m_pFireSound = nullptr;
}

void AxeDemon::Update(float elapsedSec, Level& level, const Avatar& avatar)
{
	m_pAxeDemon->Update(elapsedSec);

	// Reset to normal width and height.
	m_pAxeDemon->SetFrameHeight(m_Height);
	m_pAxeDemon->SetFrameWidth(m_Width);
	
	StateChanges(elapsedSec, level, avatar);

	for (Axe* i : m_pAxeContainer)
	{
		i->Update(elapsedSec, level);
	}

	// Calls the delete member for the container only when there is content in the container.
	if (m_pAxeContainer.size() != 0)
	{
		DeleteItem();
	}

	if (avatar.GetActingState() == Avatar::ActingState::dead || avatar.GetActingState() == Avatar::ActingState::death)
	{
		m_ActingState = ActingState::idle;
	}
}

void AxeDemon::Draw()
{
	//utils::DrawRect(m_Shape); //debug rect

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
		m_pAxeDemon->Draw(Point2f(0, 0), 1, bottom);
		glPopMatrix();
	}
	for (Axe* i : m_pAxeContainer)
	{
		i->Draw();
	}
}

AxeDemon::ActingState AxeDemon::GetActingState() const
{
	return m_ActingState;
}

void AxeDemon::StateChanges(float elapsedSec, Level& level, const Avatar& avatar)
{
	switch (m_ActingState)
	{
	case ActingState::jump:
	{
		m_pAxeDemon->SetCols(2);

		if (level.IsOnGround(m_Shape, m_Velocity))
		{
			m_Velocity.y = m_JumpSpeed;
			m_Sign *= -1;
		}
		if (m_Velocity.y > 0)
		{
			m_pAxeDemon->SetActFrame(1);
		}
		else
		{
			m_pAxeDemon->SetActFrame(0);
		}

		DistanceStateChange(avatar, ActingState::jump);

		const float jumpConstAtk{ 200.0f };
		float distance{ m_Shape.left - (avatar.GetShape().left + avatar.GetShape().width / 2) };
		if (distance < jumpConstAtk && distance > -jumpConstAtk)
		{
			m_ActingState = ActingState::attack;
		}

		float xMove{ 20.0f };

		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		if (keyState[SDL_SCANCODE_P])
		{
			m_ActingState = ActingState::death;
		}

		m_Velocity.x = xMove * m_Sign;
		m_Shape.left += m_Velocity.x * elapsedSec;

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		level.HandleCollision(m_Shape, m_Velocity);
	}
	break;

	case ActingState::attack:
	{
		m_pAxeDemon->SetCols(4);
		int frame{ m_pAxeDemon->GetActFrame() };
		float distance{};

		distance = DistanceStateChange(avatar, ActingState::attack);

		ChangeSign(avatar);

		static bool doOnce{ false };

		if (frame == 2)
		{
			if (!doOnce)
			{
				m_pAxeSound->Play(0);
				AddItem(Point2f(m_Shape.left, m_Shape.bottom + m_Shape.height), m_Sign, distance);
				doOnce = true;
			}
			
		}
		if (frame == 0)
		{
			doOnce = false;
		}

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		level.HandleCollision(m_Shape, m_Velocity);
	}
	break;
	
	case ActingState::idle:
	{
		m_pAxeDemon->SetCols(1);

		DistanceStateChange(avatar, ActingState::jump);
		//
		ChangeSign(avatar);

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		level.HandleCollision(m_Shape, m_Velocity);

	}
	break;
	case ActingState::death:
	{
		const float height{ 41.0f }, width{ 31.0f };
		m_pAxeDemon->SetFrameHeight(height);
		m_pAxeDemon->SetFrameWidth(width);

		m_pAxeDemon->SetCols(11);
		int frame{ m_pAxeDemon->GetActFrame() };

		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pAxeDemon->SetActFrame(0);
			m_pFireSound->Play(0);
			doOnce = true;
		}
		if (frame == 10)
		{
			m_ActingState = ActingState::dead;
			doOnce = false;
		}

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		level.HandleCollision(m_Shape, m_Velocity);
	}
	break;
	}
}

float AxeDemon::DistanceStateChange(const Avatar& avatar, ActingState actingState)
{

	float stateDistance{ };
	if (actingState == ActingState::jump)
	{
		stateDistance = 300.0f; // distance for jump state
	}
	else if(actingState == ActingState::attack)
	{
		stateDistance = 200.0f; // distance for attack state
	}
	float distance{ m_Shape.left - (avatar.GetShape().left + avatar.GetShape().width / 2) };
	if (distance < stateDistance && distance > -stateDistance)
	{
		m_ActingState = actingState;
	}
	else
	{
		m_ActingState = ActingState::idle;
	}
	return distance;
}

void AxeDemon::ChangeSign(const Avatar& avatar)
{
	float distance{ m_Shape.left - avatar.GetShape().left };
	if (distance > 0)
	{
		m_Sign = 1;
	}
	if (distance < 0)
	{
		m_Sign = -1;
	}
}

void AxeDemon::AddItem(const Point2f& center, int sign, float distance)
{
	Axe* pTempItem = new Axe{ center, sign, distance };
	m_pAxeContainer.push_back(pTempItem);
}

void AxeDemon::DeleteItem()
{
	int counter{ 0 };
	bool checkHit{ false };
	for (Axe* i : m_pAxeContainer)
	{
		if (i != nullptr && i->GetAxeLifetime() == false)
		{
			delete m_pAxeContainer.at(counter);
			m_pAxeContainer.at(counter) = nullptr;
			m_pAxeContainer.at(counter) = m_pAxeContainer.back();
			m_pAxeContainer.pop_back();
			++counter;

		}
	}
}

bool AxeDemon::IsOverlapping(const Rectf& rect)
{
	if (utils::IsOverlapping(rect, m_Shape))
	{
		m_ActingState = ActingState::death;
		return true;
	}
	return false;
}

bool AxeDemon::IsOverlappingAxe(const Rectf& rect)
{
	for (Axe* i : m_pAxeContainer)
	{
		if (i->IsOverlapping(rect))
		{
			return true;
		}
	}
	return false;
}
