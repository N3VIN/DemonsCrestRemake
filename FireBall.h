#pragma once
#include "Vector2f.h"

class Sprite;
class Level;

class FireBall final
{
public:

	enum class ActingState
	{
		buffer = 0,
		moving = 2,
		hit = 3
	};

	explicit FireBall(const Point2f& point, int sign);
	~FireBall();
	FireBall(const FireBall& FireBall) = delete;
	FireBall(FireBall&& FireBall) noexcept = delete;
	FireBall& operator=(const FireBall& FireBall) = delete;
	FireBall& operator=(FireBall&& FireBall) noexcept = delete;

	void Draw();
	void Update(float elapsedSec, Level& level);
	bool GetFireBallLifetime() const;
	void SetFireBallState(bool Lifetime);
	Rectf GetShape();

private:
	int m_Sign;
	Sprite* m_pFireBall;
	Sprite* m_pFireRing;
	Rectf m_Shape;
	Rectf m_Shape2;
	float m_Scale;
	Vector2f m_Velocity;
	bool m_FireBallLifetime;
	bool m_FireRingLifetime;
	ActingState m_ActingState;
	float m_AccuSec;

	//

};

