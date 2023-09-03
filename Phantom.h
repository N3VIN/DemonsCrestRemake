#pragma once
#include "Vector2f.h" 

class Sprite;
class Level;
class SoundEffect;

class Phantom final
{
public:

	enum class ActingState
	{
		death = 4,
		turn = 3,
		move = 2,
		spawn = 1,
		dead = 0	
	};

	explicit Phantom(const Point2f& point);
	Phantom(const Phantom& phantom) = delete;
	Phantom(Phantom&& phantom) noexcept = delete;
	Phantom& operator=(const Phantom& phantom) = delete;
	Phantom& operator=(Phantom&& phantom) noexcept = delete;
	~Phantom();

	void Update(float elapsedSec, Level& level);
	void Draw();
	ActingState GetActingState() const;
	bool IsOverlapping(const Rectf& rect);

private:
	Rectf m_Shape;
	const float m_HorSpeed;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	ActingState m_ActingState;
	const float m_Scale;
	const std::string m_Path;
	Sprite* m_pPhantom;
	const float m_Width;
	const float m_Height;
	int m_Sign;
	SoundEffect* m_pFireSound;

	//
	void StateChanges(float elapsedSec, Level& level);

};

