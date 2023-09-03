#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include "Sprite.h"
#include "utils.h"
#include "SVGParser.h"
#include "Avatar.h"
#include <iostream>


Level::Level()
	: m_pLevel{ new Texture{ "Resources/Demon's crest/Level/LevelForeGround.png" } }
	, m_pForeGround{new Texture{"Resources/Demon's crest/Level/LevelForeGround2.png"}}
	, m_pBackground{ new Texture{ "Resources/Demon's crest/Level/LevelBackGround.png" } }
	, m_pForeGroundTrees{ new Sprite{ "Resources/Demon's crest/Level/ForeGroundTreeAnimation.png", 5, 1, 6.0f } }
	, m_pBackGroundTrees{ new Sprite{"Resources/Demon's crest/Level/BackGroundTreeAnimation.png", 3, 1, 6.0f } }
	, m_pWater{ new Sprite{ "Resources/Demon's crest/Level/LevelMist2.png", 1, 6, 4.0f} }
	, m_Xmovement{}
	, m_Velocity{  }
	, m_TrackCheck{}


{
	SVGParser::GetVerticesFromSvgFile("Resources/Demon's crest/Level/LevelSVG2.svg", m_LevelSVG);

	m_Boundaries = Rectf( 0, 0, m_pLevel->GetWidth(), m_pLevel->GetHeight());
}

Level::~Level()
{
	delete m_pLevel;
	m_pLevel = nullptr;
	delete m_pForeGroundTrees;
	m_pForeGroundTrees = nullptr;
	delete m_pBackGroundTrees;
	m_pBackGroundTrees = nullptr;
	delete m_pBackground;
	m_pBackground = nullptr;
	delete m_pForeGround;
	m_pForeGround = nullptr;
	delete m_pWater;
	m_pWater = nullptr;

}

void Level::Update(float elapsedSec)
{
	const float foreGroundWidth{ 63.0f }; // tree width
	const float foreGroundHeight{ 124.0f }; // tree height
	
	const float backGroundWidth{ 43.0f }; // tree width
	const float backGroundHeight{ 66.0f }; // tree height

	const float waterHeight{ 34.f }; // water height
	const float waterWidth{ m_pLevel->GetWidth() }; // water width


	m_pForeGroundTrees->SetFrameWidth(foreGroundWidth);
	m_pForeGroundTrees->SetFrameHeight(foreGroundHeight);
	m_pForeGroundTrees->Update(elapsedSec);

	m_pBackGroundTrees->SetFrameWidth(backGroundWidth);
	m_pBackGroundTrees->SetFrameHeight(backGroundHeight);
	m_pBackGroundTrees->Update(elapsedSec);

	m_pWater->SetFrameWidth(waterWidth);
	m_pWater->SetFrameHeight(waterHeight);
	m_pWater->Update(elapsedSec);

	const float paralaxMov{ 0.125f };

	if(m_Velocity.x > 0 && m_TrackCheck)
	{
		m_Xmovement += paralaxMov;
	}
	if (m_Velocity.x < 0 && m_TrackCheck)
	{
		m_Xmovement -= paralaxMov;
	}

}

void Level::DrawBackground() 
{
	const float offsetW{ 0.085f / 1.7f };
	const float bgOffset{((m_pBackground->GetWidth() / 2) * offsetW) * 2.0f};
	glPushMatrix();
	glTranslatef(m_Xmovement, 0, 0);
	const Point2f pos{-bgOffset,60.f};
	m_pBackground->Draw(Point2f(-bgOffset, 0));
	DrawBgTrees(bgOffset);
	m_pWater->Draw(pos, 2);
	glPopMatrix();
}

void Level::DrawMidground() 
{
	m_pLevel->Draw();
	DrawLevelTrees();	
}

void Level::DrawForeGround() 
{
	m_pForeGround->Draw();
	utils::SetColor(Color4f(1, 1, 1, 1));
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) 
{
	//vertical collision
	Point2f pMiddle{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	Point2f pBottom{ actorShape.left + actorShape.width / 2, actorShape.bottom };
	Point2f pTop{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };

	//side collision
	Point2f pTopMiddle{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };
	Point2f pTopRight{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height };
	Point2f pTopLeft{ actorShape.left, actorShape.bottom + actorShape.height };
	Point2f pRightMiddle{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2  };
	Point2f pLeftMiddle{ actorShape.left, actorShape.bottom + actorShape.height / 2 };
	utils::HitInfo hitInfo{};

	if (utils::Raycast(m_LevelSVG.at(0), pMiddle, pBottom, hitInfo))
	{
		actorShape.bottom = hitInfo.intersectPoint.y;
		actorVelocity.y = 0;
	}
	if (utils::Raycast(m_LevelSVG.at(0), pMiddle, pTop, hitInfo))
	{
		actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height;
		actorVelocity.y *= 0;
	}

	if (utils::Raycast(m_LevelSVG.at(0), pTopMiddle, pTopRight, hitInfo))
	{
		
		actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
		actorVelocity.x = 0;
		m_TrackCheck = false;

	}
	if (utils::Raycast(m_LevelSVG.at(0), pTopMiddle, pTopLeft, hitInfo))
	{
		actorShape.left = hitInfo.intersectPoint.x;
		actorVelocity.x = 0;
		m_TrackCheck = false;

	}
	if (utils::Raycast(m_LevelSVG.at(0), pMiddle, pRightMiddle, hitInfo))
	{

		actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
		actorVelocity.x = 0;
		m_TrackCheck = false;

	}
	if (utils::Raycast(m_LevelSVG.at(0), pMiddle, pLeftMiddle, hitInfo))
	{
		actorShape.left = hitInfo.intersectPoint.x;
		actorVelocity.x = 0;
		m_TrackCheck = false;

	}

}

bool Level::IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity)
{
	m_Velocity = actorVelocity;

	bool temp{ false };
	Point2f p1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	Point2f p2{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1 };
	utils::HitInfo hitInfo{};

	if (Raycast(m_LevelSVG.at(0), p1, p2, hitInfo))
	{
		temp = true;
	}
	else
	{
		temp = false;
	}

	return temp;
}

bool Level::HitWall(Rectf& actorShape, const Vector2f& actorVelocity)
{
	bool temp{ false };

	//side collision
	Point2f pMiddle{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	Point2f pRight{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2 };
	Point2f pLeft{ actorShape.left, actorShape.bottom + actorShape.height / 2 };
	//
	Point2f pTopMiddle{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };
	Point2f pTopRight{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height };
	Point2f pTopLeft{ actorShape.left, actorShape.bottom + actorShape.height };
	utils::HitInfo hitInfo{};

	if (utils::Raycast(m_LevelSVG.at(0), pMiddle, pRight, hitInfo)) 
	{
		actorShape.left = hitInfo.intersectPoint.x - (actorShape.width + 1.0f);
		return true;
	}
	else if (utils::Raycast(m_LevelSVG.at(0), pMiddle, pLeft, hitInfo))
	{
		actorShape.left = hitInfo.intersectPoint.x + 1.0f;
		return true;
	}
	else if (utils::Raycast(m_LevelSVG.at(0), pTopMiddle, pTopRight, hitInfo))
	{

		actorShape.left = hitInfo.intersectPoint.x - (actorShape.width + 1.0f);
		return true;
	}
	else if (utils::Raycast(m_LevelSVG.at(0), pTopMiddle, pTopLeft, hitInfo))
	{
		actorShape.left = hitInfo.intersectPoint.x + 1.0f;
		return true;
	}
	else
	{
		return false;
	}
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

void Level::DrawLevelTrees() const
{
	const float posY{ 31.0f * 2 };

	const Point2f tree1{ 16.0f * 2, posY }; // position for the tree 1.
	const Point2f tree2{ 224.0f * 2, posY }; // position for the tree 2.
	const Point2f tree3{ 433.0f * 2, posY + 32.f }; // position for the tree 3.
	const Point2f tree4{ 672.0f * 2, posY }; // position for the tree 4.
	const Point2f tree5{ 1344.0f * 2, posY }; // position for the tree 5.
	const Point2f tree6{ 1568.0f * 2, posY }; // position for the tree 6.
	const Point2f tree7{ 1776.0f * 2, posY }; // position for the tree 7.
	const Point2f tree8{ 2000.0f * 2, posY }; // position for the tree 8.

	float scale{ 2 };
	m_pForeGroundTrees->Draw(tree1, scale); // tree1
	m_pForeGroundTrees->Draw(tree2, scale); // tree2
	m_pForeGroundTrees->Draw(tree3, scale); // tree3
	m_pForeGroundTrees->Draw(tree4, scale); // tree4
	m_pForeGroundTrees->Draw(tree5, scale); // tree5
	m_pForeGroundTrees->Draw(tree6, scale); // tree6
	m_pForeGroundTrees->Draw(tree7, scale); // tree7
	m_pForeGroundTrees->Draw(tree8, scale); // tree8
}

void Level::DrawBgTrees(const float& offset) const
{

	const float posY{ 73.0f * 2 };
	const Point2f tree1{ 98.0f * 2 - offset, posY }; // position for the tree 1.
	const Point2f tree2{ 306.0f * 2 - offset, posY }; // position for the tree 2.
	const Point2f tree3{ 514.0f * 2 - offset, posY }; // position for the tree 3.
	const Point2f tree4{ 722.0f * 2 - offset, posY }; // position for the tree 4.
	const Point2f tree5{ 866.0f * 2 - offset, posY }; // position for the tree 5.
	const Point2f tree6{ 1138.0f * 2 - offset, posY }; // position for the tree 6.
	const Point2f tree7{ 1331.0f * 2 - offset, posY }; // position for the tree 7.
	const Point2f tree8{ 1539.0f * 2 - offset, posY }; // position for the tree 8.
	const Point2f tree9{ 1747.0f * 2 - offset, posY }; // position for the tree 9.
	const Point2f tree10{ 1955.0f * 2 - offset, posY }; // position for the tree 10.


	float scale{ 2 };
	m_pBackGroundTrees->Draw(tree1, scale); // tree1
	m_pBackGroundTrees->Draw(tree2, scale); // tree2
	m_pBackGroundTrees->Draw(tree3, scale); // tree3
	m_pBackGroundTrees->Draw(tree4, scale); // tree4
	m_pBackGroundTrees->Draw(tree5, scale); // tree5
	m_pBackGroundTrees->Draw(tree6, scale); // tree6
	m_pBackGroundTrees->Draw(tree7, scale); // tree7
	m_pBackGroundTrees->Draw(tree8, scale); // tree8
	m_pBackGroundTrees->Draw(tree9, scale); // tree9
	m_pBackGroundTrees->Draw(tree10, scale); // tree10

}

void Level::SetTrackCheck(bool trackCheck)
{
	m_TrackCheck = trackCheck;
}