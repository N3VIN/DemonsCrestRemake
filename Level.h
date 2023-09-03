#pragma once
#include <vector>
#include "Vector2f.h"
//#include "Platform.h"

class Texture;
class Sprite;

class Level final
{
public:
	explicit Level();
	Level(const Level& level) = default;
	Level(Level&& level) noexcept = default;
	Level& operator=(const Level& level) = default;
	Level& operator=(Level&& level) noexcept = default;
	~Level();

	void DrawBackground();
	void DrawMidground() ;
	void DrawForeGround() ;
	void Update(float elapsedSec);
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) ;
	bool IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity);
	bool HitWall(Rectf& actorShape, const Vector2f& actorVelocity);
	Rectf GetBoundaries() const;
	void SetTrackCheck(bool trackCheck);


private:
	std::vector<Point2f> m_Vertices;
	std::vector<std::vector<Point2f>> m_LevelSVG;
	Texture* m_pLevel;
	Texture* m_pBackground;
	Texture* m_pForeGround;
	Sprite* m_pForeGroundTrees;
	Sprite* m_pBackGroundTrees;
	Sprite* m_pWater;
	Rectf m_Boundaries;
	float m_Xmovement;
	Vector2f m_Velocity;
	bool m_TrackCheck;

	//
	void DrawLevelTrees() const;
	void DrawBgTrees(const float& offset) const;


};

