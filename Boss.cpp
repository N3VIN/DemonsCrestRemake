#include "pch.h"
#include "Boss.h"
#include "Sprite.h"
#include "Level.h"
#include "Avatar.h"
#include "Feather.h"
#include <iostream>
#include "utils.h"
#include "Texture.h"
#include "health.h"
#include "ExtraHealth.h"
#include "SoundEffect.h"

Boss::Boss(const Point2f& point)
	: m_Velocity{ Vector2f{0, 0} }
	, m_Acceleration{ Vector2f{0, -981.0f / 3} }
	, m_ActingState{ ActingState::statue }
	, m_Scale{ 2.0f }
	, m_Path{ "Resources/Demon's crest/Enemy/Boss.png" }
	, m_pBoss{ new Sprite{ m_Path, 6.0f} }
	, m_FlyingSpeed{ 100.0f }
	, m_Width{ 97.0f }
	, m_Height{ 100.0f }
	, m_Sign{ 1 }
	, m_Time{0}
	, m_DashSpeed{200.0f}
	, m_FlyingState{FlyingState::goUp}
	, m_DistanceTravelledY{0}
	, m_TimeChange{0}
	, m_TimeAttack{0}
	, m_Health{11}
	, m_pBrokenStatue{new Texture{"Resources/Demon's crest/Enemy/StatueBroken.png"}}
	, m_pDeathSound{new SoundEffect{"Resources/Demon's crest/Sounds/deathBoss.mp3"}}
	, m_pDash{new SoundEffect{"Resources/Demon's crest/Sounds/dash.mp3"}}
	, m_pFeather{new SoundEffect{"Resources/Demon's crest/Sounds/feather.mp3"}}
	, m_pHurt{new SoundEffect{"Resources/Demon's crest/Sounds/demonHurt.mp3"}}
	, m_pActivate{new SoundEffect{"Resources/Demon's crest/Sounds/activate.mp3"}}
{
	const float factor{ 1.4f };
	m_Shape = Rectf{ point.x, point.y, (m_Width / factor) * m_Scale, (m_Height / factor) * m_Scale };
	m_pBoss->SetFrameHeight(m_Height);
	m_pBoss->SetFrameWidth(m_Width);
}

Boss::~Boss()
{
	delete m_pBoss;
	m_pBoss = nullptr;

	for (Feather* i : m_pFeatherContainer)
	{
		delete i;
		i = nullptr;
	}

	delete m_pBrokenStatue;
	m_pBrokenStatue = nullptr;

	for (Health* i : m_pHealth)
	{
		delete i;
		i = nullptr;
	}
	//
	delete m_pDeathSound;
	m_pDeathSound = nullptr;
	//
	delete m_pDash;
	m_pDash = nullptr;
	//
	delete m_pFeather;
	m_pFeather= nullptr;
	//
	delete m_pHurt;
	m_pHurt = nullptr;
	//
	delete m_pActivate;
	m_pActivate = nullptr;
	//
}

void Boss::Update(float elapsedSec, Level& level, const Avatar& avatar)
{
	m_pBoss->Update(elapsedSec);

	if (avatar.GetActingState() == Avatar::ActingState::dead || avatar.GetActingState() == Avatar::ActingState::death)
	{
		m_FlyingState = FlyingState::idle;
	}

	StateChanges(elapsedSec, level, avatar);

	for (Feather* i : m_pFeatherContainer)
	{
		i->Update(elapsedSec, level);
	}

	if (m_pFeatherContainer.size() != 0)
	{
		DeleteItem();
	}

	if (m_Health == 0)
	{
		m_ActingState = ActingState::reset;
	}
	if ((m_ActingState == ActingState::flying && m_Health == 6) || (m_ActingState == ActingState::dash && m_Health == 6))
	{
		m_ActingState = ActingState::reset;
	}

	for (Health* i : m_pHealth)
	{
		i->Update(elapsedSec, level);
	}
	//
	//	Clamping the boss within the screen.
	//
	const float height{ 448.0f };
	const float width{ 512.0f };
	const float offset{ 512.0f };
	if (m_Shape.left + m_Shape.width / 3 >= level.GetBoundaries().width)
	{
		m_Shape.left = level.GetBoundaries().width - m_Shape.width / 3;
	}
	if (m_Shape.left + m_Shape.width / 3 <= level.GetBoundaries().width - width)
	{
		m_Shape.left = (level.GetBoundaries().width - width) + m_Shape.width / 3;
	}
	if (m_Shape.bottom + m_Shape.height / 3 >= height)
	{
		m_Shape.bottom = height - m_Shape.height / 3;
	}
	//
}

void Boss::Draw()
{

	if (m_ActingState == ActingState::death)
	{
		const float widthFactor{ 30.f };
		m_pBrokenStatue->Draw(Rectf(m_Shape.left - m_Shape.width / widthFactor, m_Shape.bottom, m_pBrokenStatue->GetWidth() * m_Scale, m_pBrokenStatue->GetHeight() * m_Scale));
	}

	for (Health* i : m_pHealth)
	{
		i->Draw();
	}

	float bottom = ((int)m_ActingState * m_Height);

	if (m_ActingState != ActingState::dead)
	{
		glPushMatrix();
		const float widthFactor{ 2.5f };
		glTranslatef(m_Shape.left - m_Shape.width / widthFactor, m_Shape.bottom, 0);
		if (m_ActingState == ActingState::death)
		{
			glTranslatef(m_pBrokenStatue->GetWidth() / 2.0f , 0, 0);
		}
		glScalef((float)m_Scale, (float)m_Scale, (float)m_Scale);
		if (m_Sign < 0.0f)
		{
			glScalef(-1, 1, 1);
			const float scaleFactor{ 1.75f };
			glTranslatef(-m_Shape.width / (m_Scale / scaleFactor), 0, 0);
		}
		m_pBoss->Draw(Point2f(0, 0), 1, bottom);
		
		glPopMatrix();
	}

	for (Feather* i : m_pFeatherContainer)
	{
		i->Draw();
	}

}

Boss::ActingState Boss::GetActingState() const
{
	return m_ActingState;
}

void Boss::StateChanges(float elapsedSec, Level& level, const Avatar& avatar)
{
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);
	const float height{ 448.0f };
	const float width{ 512.0f };
	switch (m_ActingState)
	{
	case ActingState::activate:
	{

		m_pBoss->SetCols(22);
		m_pBoss->SetFrameWidth(60.0f);
		m_Shape.width = 60.0f;
		m_Shape.height = 100.0f;

		int frame{ m_pBoss->GetActFrame() };
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pBoss->SetActFrame(0);
			m_pActivate->Play(0);
			doOnce = true;
		}

		if (frame == 21)
		{
			m_ActingState = ActingState::flying;
			doOnce = false;
		}

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		level.HandleCollision(m_Shape, m_Velocity);

	}
	break;
	case ActingState::attack:
	{

		m_pBoss->SetCols(1);
		m_Time += elapsedSec;
		if (m_Time >= 0.15f)
		{
			float distanceBig{ 450.0f };
			float distanceMedium{ 300.0f };
			float distanceSmall{ 150.0f };

			float width{ 20.0f };
			if (m_Sign == 1)
			{
				m_pFeather->Play(0);
				AddItem(Point2f(m_Shape.left - width, m_Shape.bottom + width / 2), m_Sign, distanceBig);
				AddItem(Point2f(m_Shape.left + width, m_Shape.bottom), m_Sign, distanceMedium);
				AddItem(Point2f(m_Shape.left + width * 2, m_Shape.bottom), m_Sign, distanceSmall);
			}
			else if (m_Sign == -1)
			{
				m_pFeather->Play(0);
				AddItem(Point2f(m_Shape.left, m_Shape.bottom), m_Sign, distanceSmall);
				AddItem(Point2f(m_Shape.left + width, m_Shape.bottom), m_Sign, distanceMedium);
				AddItem(Point2f(m_Shape.left + width * 2, m_Shape.bottom + width / 2), m_Sign, distanceBig);
			}

			m_ActingState = ActingState::flying;
			m_Time = 0;
			m_FlyingState = FlyingState::idle;
		}
	}
	break;
	case ActingState::dash:
	{
		m_pBoss->SetCols(4);

		int frame{ m_pBoss->GetActFrame() };
		static bool doOnce{ false };

		if (!doOnce)
		{
			m_pBoss->SetActFrame(0);
			m_pDash->Play(0);

			doOnce = true;
		}

		if (frame == 2)
		{
			m_Velocity.x = m_DashSpeed * -m_Sign;
		}

		if (frame == 3)
		{
			m_pBoss->SetActFrame(2);
		}
		m_Shape.left += m_Velocity.x * elapsedSec;

		m_Time += elapsedSec;
		const float widthOffset{ 25.0f };
		if (m_Shape.left <= level.GetBoundaries().width - (width - widthOffset) || m_Shape.left + m_Shape.width >= level.GetBoundaries().width - widthOffset)
		{
			m_Sign *= -1;
			m_ActingState = ActingState::flying;
			m_FlyingState = FlyingState::goUp;
			m_Time = 0;
			doOnce = false;
		}

	}
	break;
	case ActingState::death:
	{

		m_pBoss->SetCols(11);
		m_pBoss->SetFrameWidth(31.0f);
		m_pBoss->SetFrameHeight(100.0f);
		m_Shape.width = 60.0f;
		m_Shape.height = 100.0f;
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pDeathSound->Play(0);
			doOnce = true;
		}

		int frame{ m_pBoss->GetActFrame() };
		if (frame == 10)
		{
			m_ActingState = ActingState::dead;
			Health* pTempItem = new ExtraHealth{ Point2f(m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2), m_Sign };
			m_pHealth.push_back(pTempItem);
		}
	}
	break;
	case ActingState::flying:
	{
		m_pBoss->SetFrameWidth(97.0f);
		m_pBoss->SetCols(4);
		m_Shape.width = 97.0f;
		m_Shape.height = 100.0f;


		FlyingStateChanges(elapsedSec, level, avatar);

		level.HandleCollision(m_Shape, m_Velocity);
	}
	break;
	case ActingState::hitHurt:
	{

		m_pBoss->SetCols(1);
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pHurt->Play(0);
			doOnce = true;
		}
		m_Time += elapsedSec;
		const float timeConst{ 0.15f };
		if (m_Time >= timeConst)
		{
			m_ActingState = ActingState::flying;
			m_Time = 0;
			doOnce = false;
		}
	}
	break;
	case ActingState::statue:
	{
		m_pBoss->SetCols(1);
		m_Shape.width = 60.0f;
		m_Shape.height = 100.0f;

		m_Sign = 1;

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		level.HandleCollision(m_Shape, m_Velocity);
	}
	break;

	case ActingState::reset:
	{
		m_pBoss->SetCols(22);
		m_pBoss->SetFrameWidth(60.0f);
		m_Shape.width = 60.0f;
		m_Shape.height = 100.0f;

		m_Sign = -1;

		int frame{ m_pBoss->GetActFrame() };
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pBoss->SetActFrame(0);
			m_pActivate->Play(0);
			doOnce = true;
		}
		if (m_Health > 0)
		{
			m_Health = 5;

		}
		else if (m_Health == 0)
		{
			m_Health--;
		}

		if (frame == 21)
		{
			m_ActingState = ActingState::statue;
			doOnce = false;
		}


		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		level.HandleCollision(m_Shape, m_Velocity);
	}
	break;
	}
}

void Boss::FlyingStateChanges(float elapsedSec, Level& level, const Avatar& avatar)
{
	Rectf avatarShape{ avatar.GetShape() };
	const float height{ 448.0f };
	const float width{ 512.0f };

	switch (m_FlyingState)
	{
	case FlyingState::goUp:
		{
		m_DistanceTravelledY += m_Velocity.y;
		const float offset{ 10000.0f };
		if (m_DistanceTravelledY <= offset)
		{
			m_Velocity.y = m_FlyingSpeed;
		}
		else
		{
			m_DistanceTravelledY = 0;
			m_TimeChange = 0;
			m_FlyingState = FlyingState::idle;
		}

		m_Shape.bottom += m_Velocity.y * elapsedSec;
		}
		break;
	case FlyingState::comeNear:
		{
			if (m_Shape.bottom > avatarShape.bottom && m_Shape.bottom + m_Shape.height / 2 >= height / 2)
			{
				m_Velocity.y = -m_FlyingSpeed;
				m_Velocity.x = m_FlyingSpeed * (float)-m_Sign;
			}
			else
			{
				m_FlyingState = FlyingState::dash;
			}

			m_Shape.bottom += m_Velocity.y * elapsedSec;
			m_Shape.left += m_Velocity.x * elapsedSec;

		}
		break;
	case FlyingState::dash:
		{
			m_ActingState = ActingState::dash;
		}
		break;
	case FlyingState::attack:
		{
			m_Velocity.x = m_FlyingSpeed * m_Sign;
			m_Velocity.y = m_FlyingSpeed;
			const float Offset{ 10.0f };
			if (m_Shape.left + m_Shape.width >= width - Offset && m_Shape.bottom + m_Shape.height >= height - Offset
				|| m_Shape.left <= level.GetBoundaries().width - (width + Offset) && m_Shape.bottom + m_Shape.height >= height - Offset)
			{
				m_ActingState = ActingState::attack;
			}
			m_Shape.bottom += m_Velocity.y * elapsedSec;
			m_Shape.left += m_Velocity.x * elapsedSec;
		}
		break;
	case FlyingState::idle:
		{
			const float limit{ 350.0f };
			m_TimeChange += elapsedSec;
			if (m_TimeChange >= 0.75f && m_Shape.bottom + m_Shape.height <= limit)
			{
				m_FlyingState = FlyingState::goUp;
				m_TimeChange = 0;
				m_TimeAttack = 0;
			}
			
			if (avatarShape.bottom + avatarShape.height / 2 >= height / 2)
			{
				m_FlyingState = FlyingState::comeNear;
				m_TimeAttack = 0;
			}

			m_TimeAttack += elapsedSec;
			const float timeConst{ 2.5f };
			if ((avatar.GetActingState() != Avatar::ActingState::dead && m_TimeAttack >= timeConst)|| (avatar.GetActingState() != Avatar::ActingState::death && m_TimeAttack >= timeConst))
			{
				m_FlyingState = FlyingState::attack;
				m_TimeAttack = 0;
			}
			
		}
		break;
	}
}

void Boss::AddItem(const Point2f& center, int sign, float distance)
{
	Feather* pTempItem = new Feather{ center, sign, distance };
	m_pFeatherContainer.push_back(pTempItem);
}

void Boss::DeleteItem()
{
	int counter{ 0 };
	bool checkHit{ false };
	for (Feather* i : m_pFeatherContainer)
	{
		if (i != nullptr && i->GetFeatherLifetime() == false)
		{
			delete m_pFeatherContainer.at(counter);
			m_pFeatherContainer.at(counter) = nullptr;
			m_pFeatherContainer.at(counter) = m_pFeatherContainer.back();
			m_pFeatherContainer.pop_back();
			++counter;
		}
	}
}


bool Boss::IsOverlapping(const Rectf& rect)
{
	static bool doOnce{ false };
	if ((utils::IsOverlapping(rect, m_Shape) && m_ActingState == ActingState::flying) || (utils::IsOverlapping(rect, m_Shape) && m_ActingState == ActingState::dash) || 
		(utils::IsOverlapping(rect, m_Shape) && m_ActingState == ActingState::hitHurt))
	{
		if (!doOnce)
		{
			m_ActingState = ActingState::hitHurt;
			--m_Health;
			doOnce = true;
			return false;
		}
	}
	else
	{
		doOnce = false;
	}
	return true;
}

bool Boss::OverlappingRect(const Rectf& rect)
{
	static bool doOnce{ false };
	if (utils::IsOverlapping(rect, m_Shape))
	{
		return false;
	}
	return true;
}

bool Boss::IsOverlappingFeather(const Rectf& rect)
{
	for (Feather* i : m_pFeatherContainer)
	{
		if (i->IsOverlapping(rect))
		{
			return true;
		}
	}
	return false;
}

void Boss::SetActingState(ActingState actingState)
{
	m_ActingState = actingState;
}

int Boss::GetHealth() const
{
	return m_Health;
}

int Boss::OverlapingRectHealth(const Rectf& rect)
{
	for (Health* i : m_pHealth)
	{
		int overLap{ i->IsOverlapping(rect) };
		if (overLap != 0)
		{
			i->SetHealthLifetime(false);
			return overLap;
		}
	}
	return 0;
}