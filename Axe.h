#pragma once
#include "Vector2f.h"

class Sprite;
class Level;
class Avatar;

class Axe final
{
public:
	explicit Axe(const Point2f& point, int sign, float distance);
	~Axe();
	Axe(const Axe& axe) = delete;
	Axe(Axe&& axe) noexcept = delete;
	Axe& operator=(const Axe& axe) = delete;
	Axe& operator=(Axe&& axe) noexcept = delete;

	void Draw();
	void Update(float elapsedSec, Level& level);
	bool GetAxeLifetime() const;
	bool IsOverlapping(const Rectf& rect);


private:
	int m_Sign;
	Sprite* m_pAxe;
	Rectf m_Shape;
	float m_Scale;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	float m_LandAccSec;
	bool m_AxeLifetime;
	float m_Distance;

	//
};

