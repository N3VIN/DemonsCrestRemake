#pragma once

class Texture;

class HUD final
{
public:
	explicit HUD(const Point2f& point, int totalPowerUps, const float& width, const float& height);
	HUD(const HUD&) = default;
	HUD& operator=(const HUD&) = default;
	HUD(HUD&&) noexcept = default;
	HUD& operator=(HUD&&) noexcept = default;
	~HUD();

	void DamageHit();
	void SmallHealthPickUp();
	void BigHealthPickUp();
	void ExtraHealthPickUp();

	void Draw();

private:
	Point2f m_BottomLeft;
	int m_TotalHealth;
	int m_HitDamage;
	Texture* m_pEmptyHealthTexture;
	Texture* m_pHealthTexture;
	Texture* m_pCrestsTexture;
	const float m_Width;
	const float m_Height;

	//
	void DrawEmptyHealth();
	void DrawHealth();
	void DrawCrests();
};

