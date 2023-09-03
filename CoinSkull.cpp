#include "pch.h"
#include "CoinSkull.h"
#include "Texture.h"

CoinSkull::CoinSkull(const Point2f& point, int sign)
	: Coin{ point, sign }
	, m_pCoinSkull{ new Texture{"Resources/Demon's crest/Item/CoinSkull.png"} }
{
	m_Shape = Rectf{ point.x, point.y, m_pCoinSkull->GetWidth() * m_Scale, m_pCoinSkull->GetHeight() * m_Scale };
	m_XVel = GetRand(30, 65);

}

CoinSkull::~CoinSkull()
{
	delete m_pCoinSkull;
	m_pCoinSkull = nullptr;
}

void CoinSkull::Draw()
{
	if (m_CoinLifetime)
	{
		m_pCoinSkull->Draw(m_Shape);
	}

}

