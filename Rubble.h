#pragma once
#include<vector>
#include "Vector2f.h"

class Level;

class Rubble
{
public:

	explicit Rubble(const Point2f& point, int sign);
	Rubble(const Rubble& Rubble) = delete;
	Rubble(Rubble&& Rubble) noexcept = delete;
	Rubble& operator=(const Rubble& Rubble) = delete;
	Rubble& operator=(Rubble&& Rubble) noexcept = delete;
	virtual ~Rubble() = default;

	void Update(float elapsedSec, Level& level);
	virtual void Draw() = 0;
	//
	bool GetRubbleLifetime() const;

protected:
	const float m_Scale;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	Rectf m_Shape;
	float m_LandAccSec;
	bool m_RubbleLifetime;
	int m_Sign;
	int m_XVel;


	//
	int GetRand(int a, int b);
};

