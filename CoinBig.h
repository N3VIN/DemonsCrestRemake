#pragma once
#include "Coin.h"

class Texture;

class CoinBig : public Coin
{
public:
	explicit CoinBig(const Point2f& point, int sign);
	CoinBig(const CoinBig& CoinBig) = delete;
	CoinBig(CoinBig&& CoinBig) noexcept = delete;
	CoinBig& operator=(const CoinBig& CoinBig) = delete;
	CoinBig& operator=(CoinBig&& CoinBig) noexcept = delete;
	~CoinBig();

	virtual void Draw() override;

private:
	Texture* m_pCoinBig;
};

