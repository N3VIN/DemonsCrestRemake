#pragma once

#include "Vector2f.h" 
#include <vector>

class Sprite;
class Level;
class Avatar;
class Feather;
class Texture;
class Health;
class SoundEffect;

class Boss final
{
public:

	enum class ActingState
	{
		reset = 8,
		statue = 7,
		death = 6,
		activate = 5,
		hitHurt = 4,
		attack = 3,
		dash = 2,
		flying = 1,
		dead = 0
	};

	enum class FlyingState
	{
		goUp,
		attack,
		comeNear,
		idle,
		dash
	};

	explicit Boss(const Point2f& point);
	Boss(const Boss& Boss) = delete;
	Boss(Boss&& Boss) noexcept = delete;
	Boss& operator=(const Boss& Boss) = delete;
	Boss& operator=(Boss&& Boss) noexcept = delete;
	~Boss();

	void Update(float elapsedSec, Level& level, const Avatar& avatar);
	void Draw();
	ActingState GetActingState() const;
	bool IsOverlapping(const Rectf& rect);
	bool OverlappingRect(const Rectf& rect);
	bool IsOverlappingFeather(const Rectf& rect);
	void SetActingState(ActingState actingState);
	int GetHealth() const;
	int OverlapingRectHealth(const Rectf& rect);



private:
	Rectf m_Shape;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	ActingState m_ActingState;
	const float m_Scale;
	const std::string m_Path;
	Sprite* m_pBoss;
	const float m_FlyingSpeed;
	const float m_Width;
	const float m_Height;
	int m_Sign;
	float m_Time;
	const float m_DashSpeed;
	FlyingState m_FlyingState;
	float m_DistanceTravelledY;
	float m_TimeChange;
	float m_TimeAttack;
	std::vector<Feather*> m_pFeatherContainer;
	int m_Health;
	Texture* m_pBrokenStatue;
	std::vector<Health*> m_pHealth;
	SoundEffect* m_pDeathSound;
	SoundEffect* m_pDash;
	SoundEffect* m_pFeather;
	SoundEffect* m_pHurt;
	SoundEffect* m_pActivate;

	//
	void StateChanges(float elapsedSec, Level& level, const Avatar& avatar);
	void FlyingStateChanges(float elapsedSec, Level& level, const Avatar& avatar);
	void AddItem(const Point2f& center, int sign, float distance);
	void DeleteItem();



};

