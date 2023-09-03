#include "pch.h"
#include "CoinBig.h"
#include "Texture.h"

CoinBig::CoinBig(const Point2f& point, int sign)
	: Coin{point, sign}
	, m_pCoinBig{ new Texture{"Resources/Demon's crest/Item/CoinBig.png"} }
{
	m_Shape = Rectf{ point.x, point.y, m_pCoinBig->GetWidth() * m_Scale, m_pCoinBig->GetHeight() * m_Scale };
	m_XVel = GetRand(30, 65);
}

CoinBig::~CoinBig()
{
	delete m_pCoinBig;
	m_pCoinBig = nullptr;
}

void CoinBig::Draw()
{
	if (m_CoinLifetime)
	{
		m_pCoinBig->Draw(m_Shape);
	}
	
}

