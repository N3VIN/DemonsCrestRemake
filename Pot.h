#pragma once
#include "Vector2f.h"
#include<vector>

class Level;
class Texture;
class Coin;
class Health;
class Rubble;


class Pot final
{
public:
	enum class ActingState
	{
		buffer = 2,
		notBroken = 1,
		broken = 0
	};

	explicit Pot(const Point2f& point, int numberRocks, std::string type, int number);
	Pot(const Pot& Pot) = delete;
	Pot(Pot&& Pot) noexcept = delete;
	Pot& operator=(const Pot& Pot) = delete;
	Pot& operator=(Pot&& Pot) noexcept = delete;
	~Pot();

	void Update(float elapsedSec, Level& level);
	void Draw();
	ActingState GetActingState() const;
	bool IsOverlapping(const Rectf& rect);
	void OverlapingRectCoins(const Rectf& rect);
	int OverlapingRectHealth(const Rectf& rect);

private:
	Rectf m_Shape;
	ActingState m_ActingState;
	const float m_Scale;
	const std::string m_Type;
	const int m_Number;
	Texture* m_pPot;
	std::vector<Coin*> m_pCoin;
	std::vector<Health*> m_pHealth;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	std::vector<Rubble*> m_pRubbleContainer;
	const int m_NumberOfRocks;

	//

	void StateChanges(float elapsedSec, Level& level);
	void AddItem(const Point2f& center);
	void AddRocksItem(const Point2f& center);
	void DeleteItem();
	int GetRand(int a, int b);
};

