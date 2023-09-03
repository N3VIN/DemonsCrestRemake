#pragma once
#include "Level.h"
#include "Avatar.h"
#include "PhantomManager.h"
#include "AxeDemonManager.h"
#include "ItemsManager.h"
#include "HUD.h"

class Camera;
class Statue;
class Boss;
class SoundEffect;

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( );
	bool GetQuit();

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;
	PhantomManager m_PhantomManager;
	AxeDemonManager m_AxeDemonManager;
	ItemsManager m_ItemsManager;
	Level m_Level;
	Avatar m_Avatar;
	Camera* m_pCamera;
	HUD m_Hud;
	float m_DamageCounter;
	int m_HealthPickupCheck;
	Boss* m_pBoss;
	const float m_Transition;
	float m_Left;
	float m_Bottom;
	SoundEffect* m_pBgSound;
	SoundEffect* m_pBreaking;
	SoundEffect* m_pDevils;
	SoundEffect* m_pDevilsIntro;
	SoundEffect* m_pTreasure;
	SoundEffect* m_pDeath;
	bool m_Check;
	bool m_EndFlag;
	float m_Time;
	Texture* m_pEndScreen;
	Texture* m_pStart;
	Texture* m_pSound;
	bool m_Quit;
	Point2f m_MousePos;
	bool m_SoundPlay;
	bool m_InRectSound;
	bool m_InRectStart;
	bool m_MainMenu;
	int m_SoundVol;


	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
	void DamageHit();
	void SmallHealthPickup();
	void BigHealthPickup();
	void ExtraHealthPickup();
	void DoCollisionTests(float elapsedSec);
	void CreatePhantoms();
	void PrintGameInfo();
	void AddItems();
	void DrawMenu();
	void SoundCheckPause();
	void SoundCheckPlay();
	void CheckStart();
};