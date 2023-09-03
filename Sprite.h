#pragma once
#include <string>

class Texture;

class Sprite final
{
public:
	explicit Sprite( const std::string& filename, int nrCols = 1, int nrRows = 1, float frameSec = 0 );
	explicit Sprite(const std::string& filename, float frameSec = 0);
	Sprite(const Sprite& sprite);
	Sprite(Sprite&& Sprite) noexcept;
	Sprite& operator=(const Sprite& sprite);
	Sprite& operator=(Sprite&& sprite) noexcept;

	~Sprite( );
	void Update( float elapsedSec );
	void Draw( const Point2f& pos, float scale = 1.0f );
	void Draw(const Point2f& pos, float scale, float bottom);


	float GetFrameWidth( );
	float GetFrameHeight( );
	int GetActFrame();

	void SetFrameHeight(float height);
	void SetFrameWidth(float width);
	void SetActFrame(int frame);
	void SetFrameSec(float frameSec);
	
	void SetRows(int row);
	void SetCols(int cols);

private:
	Texture *m_pTexture;
	int m_Cols;
	int m_Rows;
	float m_FrameSec;
	float m_AccuSec;
	int   m_ActFrame;
	std::string m_String;
	float m_FrameWidth{  };
	float m_FrameHeight{  };
	
};


