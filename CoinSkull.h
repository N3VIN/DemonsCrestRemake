#pragma once
#include "Coin.h"

class Texture;

class CoinSkull : public Coin
{
public:
	explicit CoinSkull(const Point2f& point, int sign);
	CoinSkull(const CoinSkull& CoinSkull) = delete;
	CoinSkull(CoinSkull&& CoinSkull) noexcept = delete;
	CoinSkull& operator=(const CoinSkull& CoinSkull) = delete;
	CoinSkull& operator=(CoinSkull&& CoinSkull) noexcept = delete;
	~CoinSkull();

	virtual void Draw() override;

private:
	Texture* m_pCoinSkull;
};

