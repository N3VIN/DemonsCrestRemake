#include "pch.h"
#include "Camera.h"
#include "utils.h"

Camera::Camera(float width, float height)
	: m_Width{ width }, m_Height{ height }, m_LevelBoundaries{}, m_TrackCheck{true}
	, m_Check{true}
	{
	
	}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::Translate(const Rectf& target) 
{
	const float offsetY{ 62.f };

	Point2f Target{ };

	if (m_Check)
	{
		Target = Track(target);
	}

	Clamp(Target);
	if (Target.x > m_LevelBoundaries.left)
	{
		m_TrackCheck = true;
	}
	else
	{
		m_TrackCheck = false;
	} 
	if (Target.x + m_Width >= m_LevelBoundaries.width)
	{
		m_TrackCheck = false;
	}

	glTranslatef(-Target.x, -Target.y, 0);
	
}

Point2f Camera::Track(const Rectf& target) 
{
	return Point2f((target.left + target.width / 2) - m_Width / 2, (target.bottom + target.height / 2) - m_Height / 2);
}

void Camera::Clamp(Point2f& bottomLeftPos) 
{
	
	// x Axis
	if (bottomLeftPos.x <= m_LevelBoundaries.left)
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}
	if (bottomLeftPos.x + m_Width >= m_LevelBoundaries.width)
	{
		bottomLeftPos.x = m_LevelBoundaries.width - m_Width;
	}

	// y axis
	if (bottomLeftPos.y <= m_LevelBoundaries.bottom)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}
	if (bottomLeftPos.y + m_Height >= m_LevelBoundaries.height)
	{
		bottomLeftPos.y = m_LevelBoundaries.height - m_Height;
	}
}

bool Camera::getTrackCheck() const
{
	return m_TrackCheck;
}

void Camera::SetCheck(bool check)
{
	m_Check = check;
}
