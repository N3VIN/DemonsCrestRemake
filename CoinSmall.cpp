#include "pch.h"
#include "CoinSmall.h"
#include "Texture.h"
#include "utils.h"

CoinSmall::CoinSmall(const Point2f& point, int sign)
	: Coin{ point, sign }
	, m_pCoinSmall{ new Texture{"Resources/Demon's crest/Item/CoinSmall.png"} }
{
	m_Shape = Rectf{ point.x, point.y, m_pCoinSmall->GetWidth() * m_Scale, m_pCoinSmall->GetHeight() * m_Scale };
	m_XVel = GetRand(30, 75); // Generates random value between 30 and 75.
}

CoinSmall::~CoinSmall()
{
	delete m_pCoinSmall;
	m_pCoinSmall = nullptr;
}

void CoinSmall::Draw()
{
	//utils::DrawRect(m_Shape); //debug rect

	if (m_CoinLifetime)
	{
		m_pCoinSmall->Draw(m_Shape);
	}

}