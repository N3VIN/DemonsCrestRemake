#pragma once
#include "Vector2f.h"

class Texture;
class Level;
class Avatar;

class Feather final
{
public:
	explicit Feather(const Point2f& point, int sign, float distance);
	~Feather();
	Feather(const Feather& Feather) = delete;
	Feather(Feather&& Feather) noexcept = delete;
	Feather& operator=(const Feather& Feather) = delete;
	Feather& operator=(Feather&& Feather) noexcept = delete;

	void Draw();
	void Update(float elapsedSec, Level& level);
	bool GetFeatherLifetime() const;
	bool IsOverlapping(const Rectf& rect);

private:
	int m_Sign;
	Texture* m_pFeather;
	Rectf m_Shape;
	float m_Scale;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	float m_LandAccSec;
	bool m_FeatherLifetime;
	float m_Distance;

	//
};

