#pragma once

#include "Vector2f.h" 
#include <vector>

class Sprite;
class Level;
class Avatar;
class Axe;
class SoundEffect;

class AxeDemon final
{
public:

	enum class ActingState
	{
		death = 4,
		idle = 3,
		jump = 2,
		attack = 1,
		dead = 0
	};

	explicit AxeDemon(const Point2f& point);
	AxeDemon(const AxeDemon& AxeDemon) = delete;
	AxeDemon(AxeDemon&& AxeDemon) noexcept = delete;
	AxeDemon& operator=(const AxeDemon& AxeDemon) = delete;
	AxeDemon& operator=(AxeDemon&& AxeDemon) noexcept = delete;
	~AxeDemon();

	void Update(float elapsedSec, Level& level, const Avatar& avatar);
	void Draw();
	ActingState GetActingState() const;
	bool IsOverlapping(const Rectf& rect);
	bool IsOverlappingAxe(const Rectf& rect);


private:
	Rectf m_Shape;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	ActingState m_ActingState;
	const float m_Scale;
	const std::string m_Path;
	Sprite* m_pAxeDemon;
	const float m_Width;
	const float m_Height;
	int m_Sign;
	float m_JumpSpeed;
	std::vector<Axe*> m_pAxeContainer;
	SoundEffect* m_pAxeSound;
	SoundEffect* m_pFireSound;

	//
	void StateChanges(float elapsedSec, Level& level, const Avatar& avatar);
	float DistanceStateChange(const Avatar& avatar, ActingState actingState);
	void ChangeSign(const Avatar& avatar);
	void AddItem(const Point2f& center, int sign, float distance);
	void DeleteItem();



};

