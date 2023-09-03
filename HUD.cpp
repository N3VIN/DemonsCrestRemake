#include "pch.h"
#include "HUD.h"
#include "Texture.h"

HUD::HUD(const Point2f& point, int TotalHealth, const float& width, const float& height)
	: m_BottomLeft{ point }
	, m_TotalHealth{ TotalHealth }
	, m_HitDamage{ 0 }
	, m_pEmptyHealthTexture{ new Texture {"Resources/Demon's crest/Level/EmptyHealth.png"} }
	, m_pHealthTexture{ new Texture {"Resources/Demon's crest/Level/Health.png"} }
	, m_pCrestsTexture{ new Texture {"Resources/Demon's crest/Level/Crests.png"} }
	, m_Width{ width }
	, m_Height{ height }
{
}


HUD::~HUD()
{
	delete m_pEmptyHealthTexture; m_pEmptyHealthTexture = nullptr;
	delete m_pCrestsTexture; m_pCrestsTexture = nullptr;
	delete m_pHealthTexture; m_pHealthTexture = nullptr;
}

void HUD::DamageHit()
{
	m_HitDamage++;
}

void HUD::SmallHealthPickUp()
{
	m_HitDamage--;

}

void HUD::BigHealthPickUp()
{
	m_HitDamage = 0;
}

void HUD::ExtraHealthPickUp()
{
	m_TotalHealth = 6;
	m_HitDamage = 0;
}

void HUD::Draw()
{
	DrawEmptyHealth();
	DrawHealth();
	DrawCrests();
}

void HUD::DrawEmptyHealth()
{
	for (int i{}; i < m_TotalHealth; ++i)
	{
		Rectf srcRect{};
		srcRect.width = m_pEmptyHealthTexture->GetWidth();
		srcRect.height = m_pEmptyHealthTexture->GetHeight();
		srcRect.bottom = 0.0f;
		srcRect.left = 0.0f;
		const float scale{ 2.5f };
		Rectf destRect{};
		destRect.width = srcRect.width * scale;
		destRect.height = srcRect.height * scale;
		destRect.bottom = m_BottomLeft.y;
		destRect.left = m_BottomLeft.x + (i * (destRect.width + 5.0f));
		m_pEmptyHealthTexture->Draw(destRect, srcRect);
	}
}

void HUD::DrawHealth()
{
	for (int i{}; i < m_TotalHealth - m_HitDamage; ++i)
	{
		Rectf srcRect{};
		srcRect.width = m_pHealthTexture->GetWidth();
		srcRect.height = m_pHealthTexture->GetHeight();
		srcRect.bottom = 0.0f;
		srcRect.left = 0.0f;
		const float scale{ 2.5f };
		Rectf destRect{};
		destRect.width = srcRect.width * scale;
		destRect.height = srcRect.height * scale;
		destRect.bottom = m_BottomLeft.y;
		destRect.left = m_BottomLeft.x + (i * (destRect.width + 5.0f));
		m_pHealthTexture->Draw(destRect, srcRect);
	}
}

void HUD::DrawCrests()
{
	m_pCrestsTexture->Draw(Rectf(m_Width - m_pCrestsTexture->GetWidth() * 2.f, m_Height - m_pCrestsTexture->GetHeight() * 2.f, m_pCrestsTexture->GetWidth() * 1.5f, m_pCrestsTexture->GetHeight() * 1.5f));
}

