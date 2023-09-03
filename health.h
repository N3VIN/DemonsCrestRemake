#pragma once
#include<vector>
#include "Vector2f.h"

class Level;
class SoundEffect;

class Health
{
public:

	explicit Health(const Point2f& point, int sign);
	Health(const Health& Health) = delete;
	Health(Health&& Health) noexcept = delete;
	Health& operator=(const Health& Health) = delete;
	Health& operator=(Health&& Health) noexcept = delete;
	virtual ~Health();

	void Update(float elapsedSec, Level& level);
	virtual void Draw() = 0;
	//
	bool GetHealthLifetime() const;
	void SetHealthLifetime(bool lifeTime);
	virtual int IsOverlapping(const Rectf& rect) = 0;

protected:
	const float m_Scale;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	Rectf m_Shape;
	float m_LandAccSec;
	bool m_HealthLifetime;
	int m_Sign;
	int m_XVel;
	SoundEffect* m_pHealthSound;
	float m_TimeConst;


	//
	int GetRand(int a, int b);
};
