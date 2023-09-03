#include "pch.h"
#include "Avatar.h"
#include "Level.h"
#include "Utils.h"
#include "Texture.h"
#include "Sprite.h"
#include "FireBall.h"
#include <iostream>
#include "SoundEffect.h"

Avatar::Avatar()
	: m_HorSpeed{ 140.0f }
	, m_JumpSpeed{ 300.0f }
	, m_Velocity{ Vector2f{0, 0} }
	, m_Acceleration{ Vector2f{0, -981.0f} }
	, m_ActingState{ ActingState::walking }
	, m_Scale{ 2 }
	, m_pHero{ new Sprite{"Resources/Demon's crest/Hero/HeroSheet.png", 6.0f} }
	, m_LandAccSec{}
	, m_RectSize{ 62.933f }
	, m_FlyingSpeed{165.0f}
	, m_Sign{ 1 }
	, m_AirTime{0}
	, m_UpdatePos{0}
	, m_Bounds{}
	, m_Time{0}
	, m_pFireBallSound{ new SoundEffect{"Resources/Demon's crest/Sounds/FireBall.mp3"} }
	, m_pHurt{ new SoundEffect{"Resources/Demon's crest/Sounds/heroHurt.mp3"} }

{
	m_Shape = Rectf{ 50, 280, (m_RectSize / 2) * m_Scale, (m_RectSize / 1.4f) * m_Scale };
	//m_Shape = Rectf{ 3805.0f, 280, (m_RectSize / 2) * m_Scale, (m_RectSize / 1.4f) * m_Scale };
	m_pHero->SetFrameHeight(m_RectSize);
	m_pHero->SetFrameWidth(m_RectSize);
}

Avatar::~Avatar() 
{
	delete m_pHero;
	m_pHero = nullptr;

	for (FireBall* i : m_pFireBallContainer)
	{
		delete i;
		i = nullptr;
	}

	delete m_pFireBallSound;
	m_pFireBallSound = nullptr;
	//
	delete m_pHurt;
	m_pHurt = nullptr;
}

void Avatar::Update(float elapsedSec, Level& level) 
{
	m_pHero->Update(elapsedSec);
	m_pHero->SetFrameSec(1 / 6.0f);

	
	if (m_Shape.left <= m_UpdatePos)
	{
		m_Shape.left = m_UpdatePos;
	}
	if (m_Shape.left + m_Shape.width >= m_Bounds)
	{
		m_Shape.left = m_Bounds - m_Shape.width;
		level.SetTrackCheck(false);
	}
	StateChanges(elapsedSec, level);

	const Uint8* keyState = SDL_GetKeyboardState(nullptr);
	
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		m_Sign = 1;
	}
	if (keyState[SDL_SCANCODE_LEFT])
	{
		m_Sign = -1;
	}


	for (FireBall* i : m_pFireBallContainer)
	{
		i->Update(elapsedSec, level);
	}

	if (m_pFireBallContainer.size() != 0)
	{
		DeleteItem();
	}

	// Check AirTime. 
	const float airTimeConst{ 5.0f };
	if (m_AirTime > airTimeConst)
	{
		m_ActingState = ActingState::hitHurt;
		m_AirTime = 0;
	}

	//// debug
	//if (keyState[SDL_SCANCODE_P])
	//{
	//	m_ActingState = ActingState::hitHurt;

	//}

}

void Avatar::Draw() 
{
	
	float bottom = ((int)m_ActingState * m_RectSize);
	//utils::DrawRect(m_Shape); //debug rect

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
		m_pHero->Draw(Point2f(0, 0), 1, bottom);
		glPopMatrix();
	}
	
	for (FireBall* i : m_pFireBallContainer)
	{
		i->Draw();
	}

}


Rectf Avatar::GetShape() const 
{
	return m_Shape;
}


void Avatar::StateChanges(float elapsedSec, Level& level)
{

	switch (m_ActingState)
	{
	case ActingState::walking:
		//
		// setRows.
		m_pHero->SetCols(6);
		m_pHero->SetFrameSec(1 / 7.0f);

		if (level.IsOnGround(m_Shape, m_Velocity))
		{
			const Uint8* keyState = SDL_GetKeyboardState(nullptr);
			bool keyPressed{ false };

			if (keyState[SDL_SCANCODE_RIGHT]) 
			{
				m_Velocity.x = m_HorSpeed;
				keyPressed = true;
			}
			else if (keyState[SDL_SCANCODE_LEFT]) 
			{
				m_Velocity.x = -m_HorSpeed;

				keyPressed = true;
			}
			else
			{
				m_Velocity.x = 0;
			}

			if (keyState[SDL_SCANCODE_UP])
			{
				keyPressed = true;
				m_ActingState = ActingState::jump;
			}

			if (keyPressed == false) 
			{
				m_ActingState = ActingState::idle;
			}
		
		}
		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;
		
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		level.HandleCollision(m_Shape, m_Velocity);

		break;

	case ActingState::idle:
		level.SetTrackCheck(false);

		if (level.IsOnGround(m_Shape, m_Velocity))
		{
			const Uint8* keyState = SDL_GetKeyboardState(nullptr);
			if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_LEFT])
			{
				m_ActingState = ActingState::walking;
				m_Velocity.x = 0;
			}
			if (keyState[SDL_SCANCODE_UP])
			{
				m_ActingState = ActingState::jump;
			}
	
			if (keyState[SDL_SCANCODE_F])
			{
				m_ActingState = ActingState::attackGround;
			}
			if (keyState[SDL_SCANCODE_D])
			{
				m_ActingState = ActingState::headButt;
			}
		}
		//
		// setRows.
		m_pHero->SetCols(4);

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		level.HandleCollision(m_Shape, m_Velocity);

		break;
	case ActingState::headButt:
		//
	{
		m_pHero->SetFrameSec(1 / 7.0f);

		level.SetTrackCheck(false);

		m_pHero->SetCols(5);
		int frame{ m_pHero->GetActFrame() };
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pHero->SetActFrame(0);
			doOnce = true;
		}

		if (frame == 4)
		{
			m_ActingState = ActingState::idle;
			doOnce = false;
		}

	}
		break;
	case ActingState::jump:
	{
		// setRows.
		m_pHero->SetCols(3);
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pHero->SetActFrame(0);
			doOnce = true;
		}
	
		const Uint8* keyState = SDL_GetKeyboardState(nullptr);

		if (level.IsOnGround(m_Shape, m_Velocity))
		{
			if (keyState[SDL_SCANCODE_UP])
			{
				m_Velocity.y = m_JumpSpeed;
			}
		}
		
		if (keyState[SDL_SCANCODE_SPACE])
		{
			m_ActingState = ActingState::flying;
		}

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;
		level.HandleCollision(m_Shape, m_Velocity);

		if (level.IsOnGround(m_Shape, m_Velocity))
		{
			m_Velocity.x = 0;
			m_ActingState = ActingState::land;
		}		

	}
		break;
	case ActingState::hover:
	{
		m_pHero->SetFrameSec(1 / 7.0f);
		m_AirTime += elapsedSec;

		level.SetTrackCheck(false);

		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		if (keyState[SDL_SCANCODE_F])
		{
			m_ActingState = ActingState::attackAir;
		}

		if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_UP])
		{
			m_ActingState = ActingState::flying;
		}

		if (level.IsOnGround(m_Shape, m_Velocity))
		{
			m_ActingState = ActingState::land;
		}
	}
	level.HandleCollision(m_Shape, m_Velocity);

	//
	// setRows.
	m_pHero->SetCols(4);

	break;
	case ActingState::flying:		

		//
		{
		m_pHero->SetFrameSec(1 / 10.0f);

		m_AirTime += elapsedSec;

			const Uint8* keyState = SDL_GetKeyboardState(nullptr);
			bool keyPressed{ false };

			if (keyState[SDL_SCANCODE_RIGHT])
			{
				m_Velocity.x = m_FlyingSpeed;
				keyPressed = true;
			}
			else if (keyState[SDL_SCANCODE_LEFT]) 
			{
				m_Velocity.x = -m_FlyingSpeed;

				keyPressed = true;
			}
			else
			{
				m_Velocity.x = 0;
			}
			//
			if (keyState[SDL_SCANCODE_UP])
			{
				m_Velocity.y = m_FlyingSpeed;
				keyPressed = true;
			}
			else if (keyState[SDL_SCANCODE_DOWN])
			{
				m_Velocity.y = -m_FlyingSpeed;
				keyPressed = true;
			}
			else
			{
				m_Velocity.y = 0;
			}

			if (keyState[SDL_SCANCODE_F])
			{
				m_ActingState = ActingState::attackAir;
				keyPressed = true;
			}
			if (keyPressed == false) {
				m_ActingState = ActingState::hover;
			}

			if (level.IsOnGround(m_Shape, m_Velocity))
			{
				m_ActingState = ActingState::land;
			}

			m_Shape.bottom += m_Velocity.y * elapsedSec;
			m_Shape.left += m_Velocity.x * elapsedSec;

			level.HandleCollision(m_Shape, m_Velocity);

		}
		//
		// setRows.
		m_pHero->SetCols(7);

		break;
	case ActingState::attackAir:
	{
		level.SetTrackCheck(false);

		// setRows.
		m_pHero->SetCols(4);

		int frame{ m_pHero->GetActFrame() };
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pHero->SetActFrame(0);
			doOnce = true;
		}

		m_Time += elapsedSec;

		if (frame == 3)
		{
			Point2f point{ m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height / 2 };
			if (m_Sign == -1)
			{
				point.x = m_Shape.left - m_Shape.width / 2;
			}
			if (m_Time >= 0.1f)
			{
				m_pFireBallSound->Play(0);
				AddItem(point, m_Sign);
				m_Time = 0;
			}
			m_ActingState = ActingState::hover;
		}
		
		level.HandleCollision(m_Shape, m_Velocity);

	}
		break;
	case ActingState::attackGround:
	{
		level.SetTrackCheck(false);

		// setRows.
		m_pHero->SetCols(3);

		int frame{ m_pHero->GetActFrame() };
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pHero->SetActFrame(0);
			doOnce = true;
		}

		m_Time += elapsedSec;

		if (frame == 2)
		{
			Point2f point{ m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height / 2 };
			if (m_Sign == -1)
			{
				point.x = m_Shape.left - m_Shape.width / 2;
			}
			if (m_Time >= 0.1f)
			{
				m_pFireBallSound->Play(0);
				AddItem(point, m_Sign);
				m_Time = 0;
			}
			m_ActingState = ActingState::idle;
		}
	}
		break;
	case ActingState::land:
		// setRows.
		m_pHero->SetCols(1);
		{
			level.SetTrackCheck(false);

			const float timeConst{ 0.25f };
			m_LandAccSec += elapsedSec;
			if (m_LandAccSec >= timeConst)
			{
			m_ActingState = ActingState::idle;
				m_LandAccSec = 0;
			}
			level.SetTrackCheck(false);

		}
		break;
	case ActingState::hitHurt:
	{
		level.SetTrackCheck(false);

		//
		// setRows.
		m_pHero->SetCols(3);

		int frame{ m_pHero->GetActFrame() };
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pHero->SetActFrame(0);
			m_pHurt->Play(0);
			doOnce = true;
		}
		if (frame == 2)
		{
			m_ActingState = ActingState::idle;
			doOnce = false;
		}

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		level.HandleCollision(m_Shape, m_Velocity);
	}
		break;
	case ActingState::death:
	{
		level.SetTrackCheck(false);

		//
		// setRows.
		m_pHero->SetCols(8);
		int frame{ m_pHero->GetActFrame() };
		static bool doOnce{false};
		if (!doOnce) 
		{
			m_pHero->SetActFrame(0);
			doOnce = true;
		}
		if (frame == 7)
		{
			m_ActingState = ActingState::dead;
		}
		level.SetTrackCheck(false);

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		level.HandleCollision(m_Shape, m_Velocity);

	}
		break;

	case ActingState::dead:
		level.SetTrackCheck(false);
		break;
	}
}

void Avatar::CheckDeath()
{
	m_ActingState = ActingState::death;
}

std::vector<FireBall*> Avatar::GetFireballContainer() const
{
	return m_pFireBallContainer;
}

Avatar::ActingState Avatar::GetActingState() const
{
	return m_ActingState;
}

void Avatar::SetHitHurtState()
{
	m_ActingState = ActingState::hitHurt;
}

void Avatar::SetUpdatePos(float pos)
{
	m_UpdatePos = pos;
}

void Avatar::SetBounds(float pos)
{
	m_Bounds = pos;
}

void Avatar::AddItem(const Point2f& center, int sign)
{
	FireBall* pTempItem = new FireBall{ center, sign };
	m_pFireBallContainer.push_back(pTempItem);
}

void Avatar::DeleteItem()
{
	int counter{ 0 };
	bool checkHit{ false };
	for (FireBall* i : m_pFireBallContainer)
	{
		if (i != nullptr && i->GetFireBallLifetime() == false)
		{
			delete m_pFireBallContainer.at(counter);
			m_pFireBallContainer.at(counter) = nullptr;
			m_pFireBallContainer.at(counter) = m_pFireBallContainer.back();
			m_pFireBallContainer.pop_back();
			++counter;
		}
	}
}