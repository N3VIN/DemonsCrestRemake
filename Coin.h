#pragma once
#include<vector>
#include "Vector2f.h"

class Level;
class SoundEffect;

class Coin
{
public:
	
	explicit Coin(const Point2f& point, int sign);
	Coin(const Coin& Coin) = delete;
	Coin(Coin&& Coin) noexcept = delete;
	Coin& operator=(const Coin& Coin) = delete;
	Coin& operator=(Coin&& Coin) noexcept = delete;
	virtual ~Coin();

	void Update(float elapsedSec, Level& level);
	virtual void Draw() = 0;
	//
	bool GetCoinLifetime() const;
	bool IsOverlapping(const Rectf& rect);
	void SetCoinLifetime(bool lifeTime);

protected:
	const float m_Scale;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	Rectf m_Shape;
	float m_LandAccSec;
	bool m_CoinLifetime;
	int m_Sign;
	int m_XVel;
	SoundEffect* m_pCoin;


	//
	int GetRand(int a, int b);
};

