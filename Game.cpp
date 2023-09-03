#include "pch.h"
#include "Game.h"
#include "Avatar.h"
#include "Camera.h"
#include "FireBall.h"
#include "Coin.h"
#include "Health.h"
#include "Boss.h"
#include "utils.h"
#include "SoundEffect.h"
#include "Texture.h"
#include "Core.h"

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_Level{}
	, m_pCamera{new Camera{m_Window.width, m_Window.height}}
	, m_Hud{ HUD{ Point2f{50.0f, m_Window.height - 55.0f}, 5, m_Window.width, m_Window.height } }
	, m_DamageCounter{0}
	, m_HealthPickupCheck{0}
	, m_pBoss{ new Boss{Point2f(3895.0f, 280.0f)} }
	, m_Transition{ 3470.0f }
	, m_Left{m_Transition - m_Window.width}
	, m_Bottom{m_Window.height / 2}
	, m_pBgSound{new SoundEffect{"Resources/Demon's crest/Sounds/Beyond the Colosseum.mp3"}}
	, m_pBreaking{new SoundEffect{"Resources/Demon's crest/Sounds/Breaking Sound.mp3"}}
	, m_pDevils{new SoundEffect{"Resources/Demon's crest/Sounds/Devils.mp3"}}
	, m_pDevilsIntro{new SoundEffect{"Resources/Demon's crest/Sounds/Devils intro.mp3"}}
	, m_pTreasure{new SoundEffect{"Resources/Demon's crest/Sounds/Treasure.mp3"}}
	, m_pDeath{new SoundEffect{"Resources/Demon's crest/Sounds/death.mp3"}}
	, m_Check{ false }
	, m_EndFlag{ false }
	, m_Time{0}
	, m_pEndScreen{new Texture{"Resources/Demon's crest/Ref/End.png"}}
	, m_pStart{new Texture{"Resources/Demon's crest/Ref/Start.png"}}
	, m_pSound{new Texture{"Resources/Demon's crest/Ref/Sound.png"}}
	, m_Quit{false}
	, m_MousePos{}
	, m_SoundPlay{true}
	, m_InRectSound{false}
	, m_InRectStart{false}
	, m_MainMenu{false}
	, m_SoundVol{100}

{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	PrintGameInfo();
	m_pBgSound->Play(0);
	m_pBgSound->SetVolume(m_SoundVol);

	m_pCamera->SetLevelBoundaries(Rectf(m_Level.GetBoundaries().left, m_Level.GetBoundaries().bottom, m_Transition, m_Level.GetBoundaries().height));
	m_Avatar.SetBounds(m_Transition);
	
	AddItems();
	
}

void Game::Cleanup( )
{
	delete m_pCamera;
	m_pCamera = nullptr;
	// 
	delete m_pBoss;
	m_pBoss = nullptr;
	//
	delete m_pBgSound;
	m_pBgSound = nullptr;
	//
	delete m_pBreaking;
	m_pBreaking = nullptr;
	//
	delete m_pDevils;
	m_pDevils = nullptr;
	//
	delete m_pDevilsIntro;
	m_pDevilsIntro = nullptr;
	//
	delete m_pTreasure;
	m_pTreasure = nullptr;
	//
	delete m_pDeath;
	m_pDeath = nullptr;
	// 
	delete m_pEndScreen;
	m_pEndScreen = nullptr;
	//
	delete m_pSound;
	m_pSound = nullptr;
	//
	delete m_pStart;
	m_pStart = nullptr;
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
	if (m_MainMenu)
	{

		m_Avatar.Update(elapsedSec, m_Level);
		m_Level.Update(elapsedSec);
		m_pBoss->Update(elapsedSec, m_Level, m_Avatar);
		m_PhantomManager.Update(elapsedSec, m_Level);
		m_AxeDemonManager.Update(elapsedSec, m_Level, m_Avatar);
		m_ItemsManager.Update(elapsedSec, m_Level);
		//
		if (m_PhantomManager.GetSize() != 0)
		{
			m_PhantomManager.DeleteItem();
		}

		if (m_AxeDemonManager.GetSize() != 0)
		{
			m_AxeDemonManager.DeleteItem();
		}

		bool temp{ m_pCamera->getTrackCheck() };
		m_Level.SetTrackCheck(temp);

		CreatePhantoms();

		//
		//collisions
		//
		DoCollisionTests(elapsedSec);
		const float newBounds{ 3340.0f };
		const float left{ 3350.0f };
		const float boundsOffset{ 175.0f };
		if (m_Avatar.GetShape().left >= newBounds && m_Avatar.GetShape().left <= newBounds + boundsOffset && m_AxeDemonManager.GetSize() == 0)
		{
			static bool doOnce{ false };
			if (!doOnce)
			{
				m_pBgSound->StopAll();
				m_pDevilsIntro->Play(0);
				doOnce = true;
			}
			m_pCamera->SetCheck(false);
			if (m_Left <= left)
			{
				m_Left++;
				if (m_Bottom >= 0)
				{
					m_Bottom--;
				}
			}

			Rectf newBoundries{ m_Left, m_Bottom, m_Level.GetBoundaries().width, m_Level.GetBoundaries().height };
			m_pCamera->SetLevelBoundaries(newBoundries);
			m_Avatar.SetUpdatePos(newBoundries.left);
			m_Avatar.SetBounds(m_Level.GetBoundaries().width);
		}
		else
		{
			m_pCamera->SetCheck(true);
		}
		// Ending camera movement.
		static bool endCheck{ false };

		const float cameraChange{ 3850.0f };
		if (m_Avatar.GetShape().left >= 3850.0f)
		{
			endCheck = true;
		}
		if (endCheck)
		{
			Rectf newBoundries{ m_Level.GetBoundaries().width - m_Window.width, m_Bottom, m_Level.GetBoundaries().width, m_Level.GetBoundaries().height };
			m_pCamera->SetLevelBoundaries(newBoundries);
			m_Avatar.SetUpdatePos(newBoundries.left);
			m_Avatar.SetBounds(newBoundries.width);
		}

		if (m_pBoss->GetActingState() == Boss::ActingState::death)
		{
			static bool doOnce{ false };
			if (!doOnce)
			{
				m_pDevils->StopAll();
				doOnce = true;
			}
		}

		if (m_Avatar.GetActingState() == Avatar::ActingState::death)
		{
			static bool doOnce{ false };
			if (!doOnce)
			{
				m_pBgSound->StopAll();
				m_pDeath->Play(0);
				doOnce = true;
			}
			m_Check = true;
		}
		//
		// 	Main Menu check.
		//

	}
}

void Game::Draw( )
{
	ClearBackground();

	if (!m_MainMenu)
	{
		DrawMenu();
	}
	else
	{

		glPushMatrix();
		m_pCamera->Translate(m_Avatar.GetShape());

		m_Level.DrawBackground();
		m_Level.DrawMidground();
		m_ItemsManager.Draw();
		m_PhantomManager.Draw();
		m_AxeDemonManager.Draw();
		m_pBoss->Draw();
		m_Avatar.Draw();
		m_Level.DrawForeGround();

		glPopMatrix();
		m_Hud.Draw();

		if (m_EndFlag)
		{
			m_pEndScreen->Draw(Rectf(0, 0, m_Window.width, m_Window.height));
		}
	}
	
}

bool Game::GetQuit()
{
	return m_Quit;
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	switch (e.keysym.sym)
	{
	case SDLK_i:
		PrintGameInfo();
		break;
	case SDLK_0:
		if (m_SoundVol < 100)
		{
			m_SoundVol += 10;
			m_pBgSound->SetVolume(m_SoundVol);
		}
		break;
	case SDLK_9:
		if (m_SoundVol > 0)
		{
			m_SoundVol -= 10;
			m_pBgSound->SetVolume(m_SoundVol);
		}
		break;
	case SDLK_8:
		m_pBgSound->ResumeAll();
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << m_Window.height - e.y << std::endl;
	m_MousePos.x = (float)e.x;
	m_MousePos.y = m_Window.height - (float)e.y;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		//std::cout << " left button " << std::endl;
		CheckStart();

		if (m_SoundPlay)
		{
			SoundCheckPause();
		}
		else
		{
			SoundCheckPlay();
		}
		break;
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::DamageHit()
{
	m_Avatar.SetHitHurtState();
	int maxHealth{ 5 };
	m_DamageCounter++;
	m_Hud.DamageHit();
	if (m_DamageCounter >= maxHealth)
	{
		m_Avatar.CheckDeath();
	}
}

void Game::SmallHealthPickup()
{
	if (m_DamageCounter > 0)
	{
		m_DamageCounter--;
		m_Hud.SmallHealthPickUp();
	}
}

void Game::BigHealthPickup()
{
	{
		m_DamageCounter = 0;
		m_Hud.BigHealthPickUp();
	}
}

void Game::ExtraHealthPickup()
{
	m_DamageCounter = -1;
	m_Hud.ExtraHealthPickUp();
}

void Game::DoCollisionTests(float elapsedSec)
{
	// Sees what the FireBall hits.
	for (FireBall* i : m_Avatar.GetFireballContainer())
	{
		// Phantom death check.
		i->SetFireBallState(m_PhantomManager.OverlapingRect(i->GetShape()));
		//
		// AxeEnemy death check
		//
		i->SetFireBallState(m_AxeDemonManager.OverlapingRect(i->GetShape()));
		//
		// Statue & Pot fireBall check
		//
		bool check{ true };
		check = m_ItemsManager.OverlapingRect(i->GetShape());
		i->SetFireBallState(check);
		if (!check)
		{
			m_pBreaking->Play(0);
		}
		//
		// Boss fireBall check
		//
		i->SetFireBallState(m_pBoss->IsOverlapping(i->GetShape()));
	}
	//
	// Statue & Pot HeadButt Hit check
	//
	if (m_Avatar.GetActingState() == Avatar::ActingState::headButt)
	{
		bool sound{true};
		sound = m_ItemsManager.OverlapingRect(m_Avatar.GetShape());
		if (!sound)
		{
			m_pBreaking->Play(0);
		}
	}
	//
	// Headbutt boss to break it.
	// 
	if (m_Avatar.GetActingState() == Avatar::ActingState::headButt && !m_pBoss->OverlappingRect(m_Avatar.GetShape()))
	{
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_pDevilsIntro->StopAll();
			m_pDevils->Play(3);
			doOnce = true;
		}

		if (m_pBoss->GetHealth() > 0)
		{
			m_pBoss->SetActingState(Boss::ActingState::activate);
		}
		else if (m_pBoss->GetHealth() < 0)
		{
			m_pBoss->SetActingState(Boss::ActingState::death);
		}
	}
	//
	// avatar health and coin pickup.
	//
	m_ItemsManager.OverlapingRectCoins(m_Avatar.GetShape()); // coins
	m_HealthPickupCheck =  m_ItemsManager.OverlapingRectHealth(m_Avatar.GetShape()); // Health
	
	const int smallHealth{ 1 };
	const int bigHealth{ 2 };
	const int extraHealth{ 3 };
	if (m_HealthPickupCheck == smallHealth)
	{
		SmallHealthPickup();
	}
	else if (m_HealthPickupCheck == bigHealth)
	{
		BigHealthPickup();
	}

	if (m_pBoss->OverlapingRectHealth(m_Avatar.GetShape()) == extraHealth)
	{
		static bool doonce{ false };
		if (!doonce)
		{
			m_pTreasure->Play(0);
			m_Time = 0;
			doonce = true;
		}
		m_Check = true;
		ExtraHealthPickup();
	}
	if (m_Check)
	{
		m_Time += elapsedSec;
		if (m_Time >= 4.0f)
		{
			m_EndFlag = true;
			m_Time = 0;
		}
	}
	if (m_EndFlag)
	{
		m_Time += elapsedSec;
		if (m_Time >= 2.0f)
		{
			m_Quit = true;
			m_Time = 0;
		}
	}
	// 
	// avatar Damage.
	//
	static bool doOnce{ false };
	if (!m_PhantomManager.OverlapingRect(m_Avatar.GetShape())) // Phantom.
	{
		if (!doOnce)
		{
			DamageHit();
			doOnce = true;
		}
	}
	else
	{
		doOnce = false;
	}
	//
	// 
	// 
	static bool doOnceBoss{ false };
	if (!m_pBoss->OverlappingRect(m_Avatar.GetShape()) && m_pBoss->GetActingState() == Boss::ActingState::dash) // Boss.
	{
		if (!doOnceBoss)
		{
			DamageHit();
			doOnceBoss = true;
		}
	}
	else
	{
		doOnceBoss = false;
	}
	// 
	//
	if (m_Avatar.GetActingState() != Avatar::ActingState::death && m_Avatar.GetActingState() != Avatar::ActingState::dead)
	{
		static bool doOnceAxe{ false };
		if (m_AxeDemonManager.OverlapingRectAxe(m_Avatar.GetShape())) // Axe.
		{
			if (!doOnceAxe)
			{
				DamageHit();
				doOnceAxe = true;
			}
		}
		else
		{
			doOnceAxe = false;
		}
	}
	//
	//
	if (m_Avatar.GetActingState() != Avatar::ActingState::death && m_Avatar.GetActingState() != Avatar::ActingState::dead)
	{
		static bool doOnceFeather{ false };
		if (m_pBoss->IsOverlappingFeather(m_Avatar.GetShape())) // Feather.
		{
			if (!doOnceFeather)
			{
				DamageHit();
				doOnceFeather = true;
			}
		}
		else
		{
			doOnceFeather = false;
		}
	}

}

void Game::CreatePhantoms()
{
	float avatarX{ m_Avatar.GetShape().left + 50 };
	const float point1{ 770.0f };
	const float point2{ 1182.0f };
	const float point3{ 1470.0f };
	const float point4{ 2225.0f };
	const float point5{ 2583.0f };
	const float point6{ 2868.0f };
	const float itemY{ 150.0f };

	if (avatarX > point1)
	{
		// create 2 phantoms
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_PhantomManager.AddItem(Point2f(point1 + 200.0f, itemY));
			m_PhantomManager.AddItem(Point2f(point1 + 300.0f, itemY));
			doOnce = true;
		}
	}
	//
	if (avatarX > point2)
	{
		// create 1 phantoms
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_PhantomManager.AddItem(Point2f(point2 + 150.0f, itemY));
			doOnce = true;
		}
	}
	//
	if (avatarX > point3)
	{
		// create 3 phantoms
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_PhantomManager.AddItem(Point2f(point3 + 200.0f, itemY));
			m_PhantomManager.AddItem(Point2f(point3 + 350.0f, itemY));
			m_PhantomManager.AddItem(Point2f(point3 + 600.0f, 350.0f));
			doOnce = true;
		}
	}
	if (avatarX > point4)
	{
		// create 1 phantoms
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_PhantomManager.AddItem(Point2f(point4 + 150.0f, itemY));
			doOnce = true;
		}
	}
	if (avatarX > point5)
	{
		// create 1 phantoms
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_PhantomManager.AddItem(Point2f(point5 + 100.0f, itemY));
			doOnce = true;
		}
	}
	if (avatarX > point6)
	{
		// create 2 phantoms
		static bool doOnce{ false };
		if (!doOnce)
		{
			m_PhantomManager.AddItem(Point2f(point6 + 150.0f, itemY));
			m_PhantomManager.AddItem(Point2f(point6 + 400.0f, itemY));
			doOnce = true;
		}
	}
}

void Game::PrintGameInfo()
{
	std::string gameInfo{};
	gameInfo = "-- Demon's Crest -- \nArrow Keys to move.\nSpaceBar to jump.\nTo fly have to jump then Up arrow\n'F' key for fireball\n'D' key for headbutt.\n'9' to reduce the volume.\n'0' to increase the volume.\n'8' to resume the sound. (If paused in the menu.)\n////--////\nReach to the end of the level\nHave to kill all the enemies to fight the boss.\n";
	std::cout << gameInfo << std::endl;
}

void Game::AddItems()
{
	m_AxeDemonManager.AddItem(Point2f(1182.0f, 300.0f));
	m_AxeDemonManager.AddItem(Point2f(1470.0f, 300.0f));
	m_AxeDemonManager.AddItem(Point2f(2235.0f, 500.0f));
	m_AxeDemonManager.AddItem(Point2f(2583.0f, 500.0f));
	m_AxeDemonManager.AddItem(Point2f(2868.0f, 500.0f));
	m_AxeDemonManager.AddItem(Point2f(3384.0f, 550.0f));
	//
	m_ItemsManager.AddStatueItem(Point2f(128.0f, 64.0f), 5);
	m_ItemsManager.AddStatueItem(Point2f(355.0f, 64.0f), 3, "small", 1);
	m_ItemsManager.AddStatueItem(Point2f(1162.0f, 300.0f), 4, "SmallHealth", 2);
	m_ItemsManager.AddStatueItem(Point2f(1440.0f, 300.0f), 4, "small", 4);
	m_ItemsManager.AddStatueItem(Point2f(2563.0f, 300.0f), 3, "SmallHealth", 1);
	m_ItemsManager.AddStatueItem(Point2f(2860.0f, 300.0f), 4, "big", 2);
	m_ItemsManager.AddStatueItem(Point2f(3364.0f, 350.0f), 5, "skull", 2);
	//
	m_ItemsManager.AddPotItem(Point2f(770.0f, 200.0f), 3, "skull", 1);
	m_ItemsManager.AddPotItem(Point2f(1220.0f, 100.0f), 5, "BigHealth", 1);
	m_ItemsManager.AddPotItem(Point2f(1761.0f, 200.0f), 2, "small", 2);
	m_ItemsManager.AddPotItem(Point2f(1790.0f, 200.0f), 1, "SmallHealth", 2);
	m_ItemsManager.AddPotItem(Point2f(3272.0f, 200.0f), 4, "BigHealth", 1);

}

void Game::DrawMenu()
{
	m_pEndScreen->Draw(Rectf(0, 0, m_Window.width, m_Window.height));
	
	Rectf sound{ m_Window.width - 60.0f,  m_Window.height - 60.0f, 50.0f, 50.0f };
	m_pSound->Draw(sound);
	if (utils::IsPointInRect(m_MousePos, sound))
	{
		utils::SetColor(Color4f(0, 1.0f, 0, 1.0f));
		utils::DrawRect(sound, 1.5f);
		m_InRectSound = true;
	}
	else
	{
		m_InRectSound = false;
	}
	//
	Rectf start{ (m_Window.width / 2) - (m_pStart->GetWidth() / 5) / 2,  (m_Window.height / 4.0f) - (m_pStart->GetHeight() / 5) / 2, m_pStart->GetWidth() / 5, m_pStart->GetHeight() / 5 };
	m_pStart->Draw(start);
	
	if (utils::IsPointInRect(m_MousePos, start))
	{
		utils::SetColor(Color4f(0, 1.0f, 0, 1.0f));
		utils::DrawRect(start, 1.5f);
		m_InRectStart = true;
	}
	else
	{
		m_InRectStart = false;
	}
}

void Game::SoundCheckPause()
{
	if (m_InRectSound)
	{
		m_pBgSound->PauseAll();
		m_SoundPlay = false;
	}
}

void Game::SoundCheckPlay()
{
	if (m_InRectSound)
	{
		m_pBgSound->ResumeAll();
		m_SoundPlay = true;
	}
}

void Game::CheckStart()
{
	if (m_InRectStart)
	{
		m_MainMenu = true;
	}
}
