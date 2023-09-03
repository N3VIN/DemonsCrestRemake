#include "pch.h"
#include "Sprite.h"
#include <iostream>
#include "Texture.h"

Sprite::Sprite( const std::string& path, int cols, int rows, float frameSec )
	: m_Cols( cols )
	, m_Rows( rows )
	, m_FrameSec( 1 / frameSec )
	, m_AccuSec{}
	, m_ActFrame{}
	, m_String{path}
	, m_pTexture{ new Texture( path ) }
	, m_FrameHeight{ m_pTexture->GetHeight() / m_Rows }
	, m_FrameWidth{ m_pTexture->GetWidth() / m_Cols }
	/*, m_FrameBottom{}
	, m_FrameLeft{}*/
{
}

Sprite::Sprite(const std::string& path, float frameSec)
	: m_Cols{1}
	, m_Rows{1}
	, m_FrameSec(1 / frameSec)
	, m_AccuSec{}
	, m_ActFrame{}
	, m_String{ path }
	, m_pTexture{ new Texture(path) }
	, m_FrameHeight{ m_pTexture->GetHeight() / m_Rows }
	, m_FrameWidth{ m_pTexture->GetWidth() / m_Cols }
	/*, m_FrameBottom{}
	, m_FrameLeft{}*/
{
}

Sprite::Sprite(const Sprite& sprite)
	: m_Cols{sprite.m_Cols}
	, m_Rows{sprite.m_Rows}
	, m_FrameSec{sprite.m_FrameSec}
	, m_AccuSec{sprite.m_AccuSec}
	, m_ActFrame{sprite.m_ActFrame}
	, m_String{sprite.m_String}
	, m_pTexture{ new Texture {sprite.m_String} }
	, m_FrameHeight{sprite.m_FrameHeight}
	, m_FrameWidth{sprite.m_FrameWidth}
	/*, m_FrameBottom{sprite.m_FrameBottom}
	, m_FrameLeft{sprite.m_FrameLeft}*/
{
	std::cout << "copy constructor called \n";
} // need to create the copy constructor.

Sprite& Sprite::operator=(const Sprite& sprite)
{
	std::cout << "copy assignment called \n";
	if (&sprite != this)
	{
		m_Cols = sprite.m_Cols;
		m_Rows = sprite.m_Rows;
		m_FrameSec = sprite.m_FrameSec;
		m_AccuSec = sprite.m_AccuSec;
		m_ActFrame = sprite.m_ActFrame;
		m_String = sprite.m_String;
		delete m_pTexture;
		m_pTexture = new Texture{sprite.m_String};
		m_FrameHeight = sprite.m_FrameHeight;
		m_FrameWidth = sprite.m_FrameWidth;
		/*m_FrameBottom = sprite.m_FrameBottom;
		m_FrameLeft = sprite.m_FrameLeft;*/
	}
	return *this;
}

Sprite::Sprite(Sprite&& sprite) noexcept
	: m_Cols{ sprite.m_Cols }
	, m_Rows{ sprite.m_Rows }
	, m_FrameSec{ sprite.m_FrameSec }
	, m_AccuSec{ sprite.m_AccuSec }
	, m_ActFrame{ sprite.m_ActFrame }
	, m_String{sprite.m_String}
	, m_pTexture{ sprite.m_pTexture }
	, m_FrameHeight{sprite.m_FrameHeight}
	, m_FrameWidth{sprite.m_FrameWidth}
	/*, m_FrameBottom{sprite.m_FrameBottom}
	, m_FrameLeft{sprite.m_FrameLeft}*/
{
	std::cout << "move constructor called \n";
	sprite.m_pTexture = nullptr;
	sprite.m_ActFrame = 0;
	sprite.m_AccuSec = 0;
	sprite.m_FrameSec = 0;
	sprite.m_Rows = 0;
	sprite.m_Cols = 0;
	sprite.m_String = "";
	sprite.m_FrameHeight = 0;
	sprite.m_FrameWidth = 0;
	/*sprite.m_FrameBottom = 0;
	sprite.m_FrameLeft = 0;*/
}

Sprite& Sprite::operator=(Sprite&& sprite) noexcept
{
	std::cout << "move assignment called \n";
	if (&sprite != this)
	{
		m_Cols = sprite.m_Cols;
		m_Rows = sprite.m_Rows;
		m_FrameSec = sprite.m_FrameSec;
		m_AccuSec = sprite.m_AccuSec;
		m_ActFrame = sprite.m_ActFrame;
		m_String = sprite.m_String;
		delete m_pTexture;
		m_pTexture = sprite.m_pTexture;
		m_FrameHeight = sprite.m_FrameHeight;
		m_FrameWidth = sprite.m_FrameWidth;
		/*m_FrameBottom = sprite.m_FrameBottom;
		m_FrameLeft = sprite.m_FrameLeft;*/
		sprite.m_pTexture = nullptr;
		sprite.m_Cols = 0;
		sprite.m_Rows = 0;
		sprite.m_FrameSec = 0;
		sprite.m_AccuSec = 0;
		sprite.m_ActFrame = 0;
		sprite.m_String = "";
		sprite.m_FrameHeight = 0;
		sprite.m_FrameWidth = 0;
		/*sprite.m_FrameBottom = 0;
		sprite.m_FrameLeft = 0;*/
	}
	return *this;
}


Sprite::~Sprite( )
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Sprite::Update( float elapsedSec )
{

	m_AccuSec += elapsedSec;

	if ( m_AccuSec > m_FrameSec )
	{
		// Go to next frame
		++m_ActFrame;
		if ( m_ActFrame >= m_Cols * m_Rows )
		{
			m_ActFrame = 0;
		}

		// Only keep the remaining time
		m_AccuSec -= m_FrameSec;
	}
}

void Sprite::Draw( const Point2f& pos, float scale )
{
	// frame dimensions
	int row = m_ActFrame / m_Cols;
	int col = m_ActFrame % m_Cols;

	Rectf srcRect;
	srcRect.height = m_FrameHeight;
	srcRect.width = m_FrameWidth;
	
	srcRect.left = m_ActFrame % m_Cols * m_FrameWidth;
	srcRect.bottom = m_ActFrame / m_Cols * m_FrameHeight + m_FrameHeight;
	/*srcRect.left = m_FrameLeft;
	srcRect.bottom = m_FrameBottom;*/
	Rectf destRect{ pos.x, pos.y,srcRect.width * scale,srcRect.height * scale };
	m_pTexture->Draw( destRect, srcRect );
}

void Sprite::Draw(const Point2f& pos, float scale, float bottom)
{
	int row = m_ActFrame / m_Cols;
	int col = m_ActFrame % m_Cols;

	Rectf srcRect;
	srcRect.height = m_FrameHeight;
	srcRect.width = m_FrameWidth;

	srcRect.left = m_ActFrame % m_Cols * m_FrameWidth;
	srcRect.bottom = bottom;
	/*srcRect.left = m_FrameLeft;
	srcRect.bottom = m_FrameBottom;*/
	Rectf destRect{ pos.x, pos.y,srcRect.width * scale,srcRect.height * scale };
	m_pTexture->Draw(destRect, srcRect);
}

float Sprite::GetFrameWidth( )
{
	return m_pTexture->GetWidth( ) / m_Cols;
}

float Sprite::GetFrameHeight( )
{
	return m_pTexture->GetHeight( ) / m_Rows;
}

int Sprite::GetActFrame()
{
	return m_ActFrame;
}

void Sprite::SetFrameHeight(float height)
{
	m_FrameHeight = height;
}

void Sprite::SetFrameWidth(float width)
{
	m_FrameWidth = width;
}

void Sprite::SetActFrame(int frame)
{
	m_ActFrame = frame;
}

void Sprite::SetFrameSec(float frameSec)
{
	m_FrameSec = frameSec;
}


void Sprite::SetRows(int row)
{
	m_Rows = row;
}

void Sprite::SetCols(int cols)
{
	m_Cols = cols;
}
