#pragma once
#include "Vector2f.h"
#include <vector>

class Level;
class Texture;
class Sprite;
class FireBall;
class SoundEffect;

class Avatar final
{
public:

	enum class ActingState 
	{
		death = 15,
		hitHurt = 14,
		headbuttAir = 13,
		headButt = 12,
		attackWall = 11,
		attackAir = 10,
		attackGround = 9,
		wallCling = 8,
		land = 7,
		flying = 6,
		hover = 5,
		hover2 = 4,
		jump = 3,
		walking = 2,
		idle = 1,
		dead = 0
	};

	explicit Avatar();
	Avatar (const Avatar& avatar) = default;
	Avatar(Avatar&& avatar) noexcept = default;
	Avatar& operator=(const Avatar& avatar) = default;
	Avatar& operator=(Avatar&& avatar) noexcept = default;

	void Update(float elapsedSec, Level& level);

	void Draw() ;
	Rectf GetShape() const;
	void CheckDeath();
	std::vector<FireBall*> GetFireballContainer() const;
	ActingState GetActingState() const;
	void SetHitHurtState();
	void SetUpdatePos(float pos);
	void SetBounds(float pos);

	~Avatar();

private:
	Rectf m_Shape;
	const float m_HorSpeed;
	float m_JumpSpeed;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	ActingState m_ActingState;
	const float m_Scale;
	Sprite* m_pHero;
	float m_LandAccSec;
	const float m_RectSize;
	const float m_FlyingSpeed;
	int m_Sign;
	std::vector<FireBall*> m_pFireBallContainer;
	float m_AirTime;
	float m_UpdatePos;
	float m_Bounds;
	float m_Time;
	SoundEffect* m_pFireBallSound;
	SoundEffect* m_pHurt;

	// 

	void StateChanges(float elapsedSec, Level& level);
	void AddItem(const Point2f& center, int sign);
	void DeleteItem();
	//

};

