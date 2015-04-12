#include "CImg.h"
#include "pieceTest.h"
#include "pawnTest.cpp"

#ifndef IMAGEPATH
#define IMAGEPATH "res/"
#endif

#define BLANK 10
#define WHITEKING 11
#define WHITEQUEEN 12
#define WHITEKNIGHT 13
#define WHITEBISHOP 14
#define WHITEROOK 15
#define WHITEPAWN 16
#define BLACKKING 17
#define BLACKQUEEN 18
#define BLACKKNIGHT 19
#define BLACKBISHOP 20
#define BLACKROOK 21
#define BLACKPAWN 22

#define PAWN 0
#define BISHOP 1
#define KNIGHT 2
#define ROOK 3
#define QUEEN 4
#define KING 5

using namespace cimg_library;

//each piece is 60px by 60px chessboard is 480px by 480px or 60px * 8
CImg<unsigned char> chessboard(IMAGEPATH "chessboard.png");
CImg<unsigned char> wking(IMAGEPATH "Chess_klt60.png");
CImg<unsigned char> bking(IMAGEPATH "Chess_kdt60.png");
CImg<unsigned char> wqueen(IMAGEPATH "Chess_qlt60.png");
CImg<unsigned char> bqueen(IMAGEPATH "Chess_qdt60.png");
CImg<unsigned char> wknight(IMAGEPATH "Chess_nlt60.png");
CImg<unsigned char> bknight(IMAGEPATH "Chess_ndt60.png");
CImg<unsigned char> wbishop(IMAGEPATH "Chess_blt60.png");
CImg<unsigned char> bbishop(IMAGEPATH "Chess_bdt60.png");
CImg<unsigned char> wrook(IMAGEPATH "Chess_rlt60.png");
CImg<unsigned char> brook(IMAGEPATH "Chess_rdt60.png");
CImg<unsigned char> wpawn(IMAGEPATH "Chess_plt60.png");
CImg<unsigned char> bpawn(IMAGEPATH "Chess_pdt60.png");
CImg<unsigned char> dsquare(IMAGEPATH "dsq.png");
CImg<unsigned char> lsquare(IMAGEPATH "lsq.png");
CImg<unsigned char> graveyard(500,400,1,3,0);
	
//Displays (each display is a new window)
CImgDisplay main_disp(chessboard,"Chess");
CImgDisplay graveyard_disp(graveyard,"Graveyard");

//Boardarray 
int boardarray[8][8];
Piece piecearray[8][8];

//last coordinates start outside board
int lastx = 8;
int lasty = 8;
int lastp = 0;

//to write to screen/debug
const unsigned char green[] = { 0,255,0 };


//Board Methods
void updateboard()
{
	chessboard.display(main_disp);//update board
	graveyard.display(graveyard_disp);
}

int returnpiece(int x, int y)//returns piece at spot on board given
{
	return piecearray[x][y].getType();
}

void setup()
{
	
	//setup board
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 8; j++)
		{
			boardarray[i][j] = 0;
			//piecearray[i][j].changeType(BLANK);//TODO FIX
		}
	//white
	chessboard.draw_image(0,420,wrook);
	boardarray[0][7] = WHITEROOK;
	chessboard.draw_image(60,420,wknight);
	boardarray[1][7] = WHITEKNIGHT;
	chessboard.draw_image(120,420,wbishop);
	boardarray[2][7] = WHITEBISHOP;
	chessboard.draw_image(180,420,wqueen);
	boardarray[3][7] = WHITEQUEEN;
	chessboard.draw_image(240,420,wking);
	boardarray[4][7] = WHITEKING;
	chessboard.draw_image(300,420,wbishop);
	boardarray[5][7] = WHITEBISHOP;
	chessboard.draw_image(360,420,wknight);
	boardarray[6][7] = WHITEKNIGHT;
	chessboard.draw_image(420,420,wrook);
	boardarray[7][7] = WHITEROOK;
	for(int i = 0; i <=7; i++)
	{
		chessboard.draw_image(i*60,360,wpawn);
		boardarray[i][6] = WHITEPAWN;
	}
		
	//black
	chessboard.draw_image(0,0,brook);
	boardarray[0][0] = BLACKROOK;
	chessboard.draw_image(60,0,bknight);
	boardarray[1][0] = BLACKKNIGHT;
	chessboard.draw_image(120,0,bbishop);
	boardarray[2][0] = BLACKBISHOP;
	chessboard.draw_image(180,0,bqueen);
	boardarray[3][0] = BLACKQUEEN;
	chessboard.draw_image(240,0,bking);
	boardarray[4][0] = BLACKKING;
	chessboard.draw_image(300,0,bbishop);
	boardarray[5][0] = BLACKBISHOP;
	chessboard.draw_image(360,0,bknight);
	boardarray[6][0] = BLACKKNIGHT;
	chessboard.draw_image(420,0,brook);
	boardarray[7][0] = BLACKROOK;
	for(int i = 0; i <=7; i++)
	{
		chessboard.draw_image(i*60,60,bpawn);
		boardarray[i][1] = BLACKPAWN;
	}
	updateboard();
}

void movedraw(CImg<unsigned char> piece, int srcx, int srcy, int destx, int desty, bool special)//x and y are 0-7
{	
	if(!special)//a special move is the en passant or castling or queening
	{
		chessboard.draw_image(destx*60, desty*60, piece);
		//determine if square is light or dark
		if(srcx%2 == 0 && srcy%2 == 0)
			chessboard.draw_image(srcx*60,srcy*60,lsquare);
		else if(srcx%2 == 0 && srcy != 0)
			chessboard.draw_image(srcx*60,srcy*60,dsquare);
		else if(srcx%2 != 0 && srcy%2 == 0)
			chessboard.draw_image(srcx*60,srcy*60,dsquare);
		else if(srcx%2 != 0 && srcy%2 != 0)
			chessboard.draw_image(srcx*60,srcy*60,lsquare);
		
		
		updateboard();	
		boardarray[destx][desty] = boardarray[srcx][srcy];
		boardarray[srcx][srcx] = BLANK;
	}
	else//Assume that if command for special move is passed the requirements are met
	{
		//Castling
		if(piece == bking || piece == wking)
		{
			//move rook to spot on side of king
			//left
			if(srcx < destx)
			{
				//
			}
			//right
			else if(srcx > destx)
			{
				//
			}
		}
	}
}

void handleclick(int p, int x, int y, bool b)//kinda working
{
	if(b && (lastp != BLANK))
	{
		//check if valid move
		CImg<unsigned char> piece;
		if(lastp == WHITEKING)
			piece = wking;
		if(lastp == WHITEQUEEN)
			piece = wqueen;
		if(lastp == WHITEKNIGHT)
			piece = wknight;
		if(lastp == WHITEBISHOP)
			piece = wbishop;
		if(lastp == WHITEROOK)
			piece = wrook;
		if(lastp == WHITEPAWN)
			piece = wpawn;
		if(lastp == BLACKKING)
			piece = bking;
		if(lastp == BLACKQUEEN)
			piece = bqueen;
		if(lastp == BLACKKNIGHT)
			piece = bknight;
		if(lastp == BLACKBISHOP)
			piece = bbishop;
		if(lastp == BLACKROOK)
			piece = brook;
		if(lastp == BLACKPAWN)
			piece = bpawn;
		movedraw(piece,lastx,lasty,x,y,false);
	}
	else
	{
		lastx = x;
		lasty = y;
		lastp = p;
	}
}
