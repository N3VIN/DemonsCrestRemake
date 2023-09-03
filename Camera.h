#pragma once
class Camera final
{
public:
	explicit Camera(float width, float height);
	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Translate(const Rectf& target) ;
	bool getTrackCheck() const;
	void SetCheck(bool check);

private:
	Point2f Track(const Rectf& target)  ;
	void Clamp(Point2f& bottomLeftPos) ;

	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;
	bool m_TrackCheck;
	bool m_Check;

};

