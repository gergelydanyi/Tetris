#pragma once

typedef struct sTriColor
{
	COLORREF colour[3];
}tTriColor;

typedef struct sTileParts
{
	POINT p[4];
} tTileParts;

typedef struct sTile
{
	tTileParts parts;
	short w;
	short h;
	short left;
	short bottom;
	short lmin, lmax, bottommax;
	int MoveLeft();
	int MoveRight();
	int MoveDown();
	int Rotate();
} tTile;

int tTile::MoveLeft()
{
	if (left > lmin)
	{
		left--;
	}
	return 0;
}

int tTile::MoveRight()
{
	if (left < lmax - w)
	{
		left++;
	}
	return 0;
}

int tTile::MoveDown()
{
	if (bottom < bottommax)
	{
		bottom++;
	}
	return 0;
}

int tTile::Rotate()
{
	return 0;
}

typedef struct sGameBoard
{
	int width, height;
	long left, top, borderWidth;
} tGameBoard;

typedef struct sGameInfo
{
	int TickInterval;
	unsigned short int BlockSize;
	sGameBoard GameBoard;
	tTile Tiles[7] = {};
	tTriColor TriColor[7] = { RGB(10, 54,235),RGB(6, 40,174),RGB(7, 47,205),
							 RGB(24,225, 19),RGB(18,167, 14),RGB(21,196, 17),
							 RGB(227, 17, 17),RGB(168, 13, 13),RGB(198, 15, 15),
							 RGB(227, 17, 17),RGB(168, 13, 13),RGB(198, 15, 15),
							 RGB(227, 17,175),RGB(168, 13,130),RGB(198, 15,152),
							 RGB(227, 17,175),RGB(168, 13,130),RGB(21,196, 17),
							 RGB(244,165,  0),RGB(181,122,  0),RGB(213,143,  0),
	};
	tTriColor GBColor = { RGB(238,238,238),RGB(102,102,102),RGB(153,153,153) };
	//POINT TilePos = { 6,0 };
	HDC hdc;
	HBRUSH bgBrush;
	tTile ActiveTile = Tiles[0];
	void Init();
} tGameInfo;

void tGameInfo::Init()
{
	TickInterval = 500;
	bgBrush = (HBRUSH)COLOR_BACKGROUND;
	BlockSize = 30;
	GameBoard.width = 10;
	GameBoard.height = 20;
	GameBoard.left = 50;
	GameBoard.top = 50;
	GameBoard.borderWidth = 8;
	// ■
	// ■
	// ■
	// ■
	Tiles[0].parts = { 0, 0, 0,-1, 0,-1, 0,-1 };
	Tiles[0].w = 1;
	Tiles[0].h = 4;
	Tiles[0].left = 6;
	Tiles[0].bottom = -1;
	Tiles[0].lmin = 0;
	Tiles[0].lmax = 9;
	Tiles[0].bottommax = 19;

	// ■ ■
	// ■ ■
	Tiles[1].parts = { 0, 0, 0,-1, 1, 0, 0, 1 };	
	Tiles[1].w = 2;
	Tiles[1].h = 2;
	Tiles[1].left = 5;
	Tiles[1].bottom = -1;
	Tiles[1].lmin = 0;
	Tiles[1].lmax = 9;
	Tiles[1].bottommax = 19;

	// ■
	// ■
	// ■ ■
	Tiles[2].parts = { 1, 0,-1, 0, 0,-1, 0,-1 };	
	Tiles[2].w = 2;
	Tiles[2].h = 3;
	Tiles[2].left = 5;
	Tiles[2].bottom = -1;
	Tiles[2].lmin = 0;
	Tiles[2].lmax = 9;
	Tiles[2].bottommax = 19;

	//   ■
	//   ■
	// ■ ■
	Tiles[3].parts = { 0, 0, 1, 0, 0,-1, 0,-1 };	
	Tiles[3].w = 2;
	Tiles[3].h = 3;
	Tiles[3].left = 5;
	Tiles[3].bottom = -1;
	Tiles[3].lmin = 0;
	Tiles[3].lmax = 9;
	Tiles[3].bottommax = 19;

	//   ■ ■
	// ■ ■
	Tiles[4].parts = { 0, 0, 1, 0, 0,-1, 1, 0 };	
	Tiles[4].w = 3;
	Tiles[4].h = 2;
	Tiles[4].left = 4;
	Tiles[4].bottom = -1;
	Tiles[4].lmin = 0;
	Tiles[4].lmax = 9;
	Tiles[4].bottommax = 19;

	// ■ ■
	//   ■ ■
	Tiles[5].parts = { 0,-1, 1, 0, 0, 1, 1, 0 };	
	Tiles[5].w = 3;
	Tiles[5].h = 2;
	Tiles[5].left = 4;
	Tiles[5].bottom = -1;
	Tiles[5].lmin = 0;
	Tiles[5].lmax = 9;
	Tiles[5].bottommax = 19;

	// ■ ■ ■
	//   ■
	Tiles[6].parts = {-1,-1, 1, 0, 1, 0,-1, 1 };	
	Tiles[6].w = 3;
	Tiles[6].h = 2;
	Tiles[6].left = 4;
	Tiles[6].bottom = -1;
	Tiles[6].lmin = 0;
	Tiles[6].lmax = 9;
	Tiles[6].bottommax = 19;

	ActiveTile = Tiles[0];
}