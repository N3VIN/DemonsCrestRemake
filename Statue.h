#pragma once
#include "Vector2f.h"
#include<vector>

class Level;
class Texture;
class Rubble;
class Coin;
class Health;

class Statue final
{
public:
	enum class ActingState
	{
		buffer = 2,
		notBroken = 1,
		broken = 0
	};

	explicit Statue(const Point2f& point, int number);
	explicit Statue(const Point2f& point, int number, std::string type, int numberOfItems);
	Statue(const Statue& Statue) = delete;
	Statue(Statue&& Statue) noexcept = delete;
	Statue& operator=(const Statue& Statue) = delete;
	Statue& operator=(Statue&& Statue) noexcept = delete;
	~Statue();

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
	Texture* m_pNotBroken;
	Texture* m_pBroken;
	std::vector<Rubble*> m_pRubbleContainer;
	const int m_Number;
	const std::string m_Type;
	const int m_NumberOfItems;
	std::vector<Coin*> m_pCoin;
	std::vector<Health*> m_pHealth;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;

	//
	void StateChanges(float elapsedSec, Level& level);
	void AddRocksItem(const Point2f& center);
	void AddPickupItem(const Point2f& center);
	void DeleteItem();
	int GetRand(int a, int b);

};

