#pragma once
#include "Coin.h"

class Texture;

class CoinSmall : public Coin
{
public:
	explicit CoinSmall(const Point2f& point, int sign);
	CoinSmall(const CoinSmall& CoinSmall) = delete;
	CoinSmall(CoinSmall&& CoinSmall) noexcept = delete;
	CoinSmall& operator=(const CoinSmall& CoinSmall) = delete;
	CoinSmall& operator=(CoinSmall&& CoinSmall) noexcept = delete;
	~CoinSmall();

	virtual void Draw() override;


private:
	Texture* m_pCoinSmall;
};

