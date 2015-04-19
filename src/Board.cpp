#include "Board.h"


void Board::setupPieceArray()
{
	for(int index = 0; index < 32; index++)
		{	
			if(index < 8)
				pieceArray[index] = new Pawn(true, index, 6);								//white pawns
			else if(index < 16)
				pieceArray[index] = new Pawn(false, index - 8, 1);							//black pawns
			else if(index < 20)
			{
				if(index % 2 == 0)
					pieceArray[index] = new Bishop(true, (3*((index/2)-7))-1, 7);			//white bishops
				else
					pieceArray[index] = new Bishop(false, (3*(((index-1)/2)-7))-1, 0);		//black bishops
			}
			else if(index < 24)
			{
				if(index % 2 == 0)
					pieceArray[index] = new Knight(true, (5*((index/2)-10)) + 1 , 7);		//white knight
				else
					pieceArray[index] = new Knight(false, (5*(((index-1)/2)-10)) + 1 , 0);	//black knight
			}
			else if(index < 28)
			{
				if(index % 2 == 0)
					pieceArray[index] = new Rook(true, (7*((index/2)-12)), 7);				//white rook
				else
					pieceArray[index] = new Rook(false, (7*(((index-1)/2)-12)), 0);			//black rook
			}
			else if(index == 28) pieceArray[index] = new Queen(true, 3, 7);					//white queen
			else if(index == 29) pieceArray[index] = new Queen(false, 3, 0);				//black queen
			else if(index == 30) pieceArray[index] = new King(true, 4, 7);					//white king
			else if(index == 31) pieceArray[index] = new King(false, 4, 0);					//black king
		}
}
bool Board::collision(int moveX, int moveY, int positionX, int positionY) { //Method that returns free as true or false
	

	int moveSpacesHor = moveX - positionX;
	int moveSpacesVer = moveY - positionY;
	bool free = true;
	if (abs(moveSpacesHor) == abs(moveSpacesVer)) { //Checks if diagnol
		if (moveSpacesHor == moveSpacesVer && moveSpacesHor > 0) {	//diagnol + +
			for ( int i = 1; i<abs(moveSpacesHor); i++) {
				if (returnPiece(positionX + i, positionY + i) != -1) {
					free = false;
				}
			}
		} else if (moveSpacesHor == moveSpacesVer && moveSpacesHor < 0) { //diagnol - -
			for ( int i = 1; i<abs(moveSpacesHor); i++) {
				if (returnPiece(positionX - i, positionY - i) != -1) {
					free = false;
				}
			}
		} else if (abs(moveSpacesHor) == moveSpacesVer) { //diagnol - +
			for ( int i = 1; i<abs(moveSpacesHor); i++) {
				if (returnPiece(positionX - i, positionY + i) != -1) {
					free = false;
				}
			}
		} else if (moveSpacesHor == abs(moveSpacesVer)) { //diagnol + -
			for ( int i = 1; i<abs(moveSpacesHor); i++) {
				if (returnPiece(positionX + i, positionY - i) != -1) {
					free = false;
				}
			}
		}
	} else if ((moveSpacesHor == 0) && (moveSpacesVer > 0)) { //Vertical +
		for ( int i = 1; i<moveSpacesVer; i++) {
			if (returnPiece(positionX, positionY + i) != -1) {
					free = false;
				}
			}
	} else if ((moveSpacesHor == 0) && (moveSpacesVer < 0)) { //Vertical -
		for ( int i = 1; i<abs(moveSpacesVer); i++) {
			if (returnPiece(positionX, positionY - i) != -1) {
					free = false;
				}
			}
	} else if ((moveSpacesVer == 0) && (moveSpacesHor > 0)) { //Horizontal +
		for ( int i = 1; i<moveSpacesHor; i++) {
			if (returnPiece(positionX + 1, positionY) != -1) {
					free = false;
				}
			}
	} else if ((moveSpacesVer == 0) && (moveSpacesHor < 0)) { //Horizontal -
		for ( int i = 1; i<abs(moveSpacesHor); i++) {
			if (returnPiece(positionX - 1, positionY) != -1) {
				free = false;
				}
			}
	}
	
	return free;
}

bool Board::isCheck(int kingX, int kingY, int newX, int newY, int oldX, int oldY)
{
	if(newX == -2 && turn) //original array, white is in check
	{
		for(int i = 0; i < 32; i++)
		{
			if((i >= 8 && i <= 15) || i == 25 || i == 21 || i == 17 || i == 29 || i == 31 || i == 19 || i == 23 || i == 27)
			{
				if(pieceArray[i]->moveLegal(kingX, kingY, -2) && collision(kingX, kingY, pieceArray[i]->getX(), pieceArray[i]->getY()))
				{
					//isCheckmate();
					return true;
				}
			}
		}
	}
	
	else if(newX == -2 && !turn) //original array, black is in check
	{
		for(int i = 0; i < 32; i++)
		{
			if((i >= 0 && i <= 7) || i == 24 || i == 20 || i == 16 || i == 28 || i == 30 || i == 18 || i == 22 || i == 26)
			{
				if(pieceArray[i]->moveLegal(kingX, kingY, -2) && collision(kingX, kingY, pieceArray[i]->getX(), pieceArray[i]->getY()))
				{
					//isCheckmate();
					return true;
				}
			}
		}
	}
	
	else if(newX != -2 && turn) //hypo array, white is in check
	{
		if(boardarray[newX][newY] != -1)
		{
			int queenIndex; //return to newX if fail
			int knightIndex; //return to oldX if fail
			for(int i = 0; i < 32; i++) //gets rid of queen
			{
				if(pieceArray[i]->getX() == newX && pieceArray[i]->getY() == newY)
				{
					queenIndex = i;
					pieceArray[i]->setPosition(-10, -100);
					break;
				}
			}
			for(int i = 0; i < 32; i++) //puts knight in queen spot
			{
				if(pieceArray[i]->getX() == oldX && pieceArray[i]->getY() == oldY)
				{
					knightIndex = i;
					pieceArray[i]->setPosition(newX, newY);
					break;
				}
			}
			int knightBoardIndex;
			int queenBoardIndex;
			knightBoardIndex = boardarray[oldX][oldY];
			queenBoardIndex = boardarray[newX][newY];
			boardarray[newX][newY] = knightBoardIndex;
			boardarray[oldX][oldY] = -1;
			for(int i = 0; i < 32; i++)
			{
				if(((i >= 8 && i <= 15) || i == 25 || i == 21 || i == 17 || i == 29 || i == 31 || i == 19 || i == 23 || i == 27) && i != queenIndex && knightIndex != 30)
				{
					if(pieceArray[i]->moveLegal(kingX, kingY, -2) && collision(kingX, kingY, pieceArray[i]->getX(), pieceArray[i]->getY()))
					{
						/*pieceArray[queenIndex]->setPosition(newX, newY);
						pieceArray[knightIndex]->setPosition(oldX,oldY);
						boardarray[oldX][oldY] = knightBoardIndex;
						boardarray[newX][newY] = queenBoardIndex;*/
						return true;
					}
				}
				else if(((i >= 8 && i <= 15) || i == 25 || i == 21 || i == 17 || i == 29 || i == 31 || i == 19 || i == 23 || i == 27) && i != queenIndex && knightIndex == 30)
				{
					if(pieceArray[i]->moveLegal(newX, newY, -2) && collision(newX, newY, pieceArray[i]->getX(), pieceArray[i]->getY()))
					{ 
						/*pieceArray[queenIndex]->setPosition(newX, newY);
						pieceArray[knightIndex]->setPosition(oldX,oldY);
						boardarray[oldX][oldY] = knightBoardIndex;
						boardarray[newX][newY] = queenBoardIndex;*/
						return true;
					}
				}
			}
			pieceArray[queenIndex]->setPosition(newX, newY);
			pieceArray[knightIndex]->setPosition(oldX,oldY);
			boardarray[oldX][oldY] = knightBoardIndex;
			boardarray[newX][newY] = queenBoardIndex;
		}
		
		
		
		else
		{
			int pieceIndex;
			for(int i = 0; i < 32; i++)
			{
				if(pieceArray[i]->getX() == oldX && pieceArray[i]->getY() == oldY)
				{
					pieceIndex = i;
					pieceArray[i]->setPosition(newX, newY);
					
				}
			}
			int boardIndex;
			boardIndex = boardarray[oldX][oldY];
			boardarray[newX][newY] = pieceIndex;
			boardarray[oldX][oldY] = -1;
			for(int i = 0; i < 32; i++)
			{
				if(((i >= 8 && i <= 15) || i == 25 || i == 21 || i == 17 || i == 29 || i == 31 || i == 19 || i == 23 || i == 27) && pieceIndex != 30)
				{
					if(pieceArray[i]->moveLegal(kingX, kingY, -2) && collision(kingX, kingY, pieceArray[i]->getX(), pieceArray[i]->getY()))
					{
						/*pieceArray[pieceIndex]->setPosition(oldX, oldY);
						boardarray[oldX][oldY] = boardIndex;
						boardarray[newX][newY] = -1;*/
						return true;
					}
				}
				else if(((i >= 8 && i <= 15) || i == 25 || i == 21 || i == 17 || i == 29 || i == 31 || i == 19 || i == 23 || i == 27) && pieceIndex == 30)
				{
					if(pieceArray[i]->moveLegal(newX, newY, -2) && collision(newX, newY, pieceArray[i]->getX(), pieceArray[i]->getY()))
					{
						/*pieceArray[pieceIndex]->setPosition(oldX, oldY);
						boardarray[oldX][oldY] = boardIndex;
						boardarray[newX][newY] = -1;*/
						return true;
					}
				}
			}
			pieceArray[pieceIndex]->setPosition(oldX, oldY);
			boardarray[oldX][oldY] = boardIndex;
			boardarray[newX][newY] = -1;

		}
		
		
	}

	else if(newX != -2 && !turn) //hypo array, black is in check
	{
		if(boardarray[newX][newY] != -1)
		{

			int queenIndex; //return to newX if fail
			int knightIndex; //return to oldX if fail
			for(int i = 0; i < 32; i++) //gets rid of queen
			{
				if(pieceArray[i]->getX() == newX && pieceArray[i]->getY() == newY)
				{
					queenIndex = i;
					pieceArray[i]->setPosition(-10, -100);
					break;
				}
			}
			for(int i = 0; i < 32; i++) //puts knight in queen spot
			{
				if(pieceArray[i]->getX() == oldX && pieceArray[i]->getY() == oldY)
				{
					knightIndex = i;
					pieceArray[i]->setPosition(newX, newY);
					break;
				}
			}
			int knightBoardIndex;
			int queenBoardIndex;
			knightBoardIndex = boardarray[oldX][oldY];
			queenBoardIndex = boardarray[newX][newY];
			boardarray[newX][newY] = knightBoardIndex;
			boardarray[oldX][oldY] = -1;
			for(int i = 0; i < 32; i++)
			{
				if(((i >= 0 && i <= 7) || i == 24 || i == 20 || i == 16 || i == 28 || i == 30 || i == 18 || i == 22 || i == 26) && i != queenIndex && knightIndex != 31)
				{
					if(pieceArray[i]->moveLegal(kingX, kingY, -2) && collision(kingX, kingY, pieceArray[i]->getX(), pieceArray[i]->getY()))
					{
						pieceArray[queenIndex]->setPosition(newX, newY);
						pieceArray[knightIndex]->setPosition(oldX,oldY);
			
						boardarray[oldX][oldY] = knightBoardIndex;
						boardarray[newX][newY] = queenBoardIndex;
						return true;
					}
				}
				else if(((i >= 0 && i <= 7) || i == 24 || i == 20 || i == 16 || i == 28 || i == 30 || i == 18 || i == 22 || i == 26) && i != queenIndex && knightIndex == 31)
				{
					if(pieceArray[i]->moveLegal(newX, newY, -2) && collision(newX, newY, pieceArray[i]->getX(), pieceArray[i]->getY()))
					{
						pieceArray[queenIndex]->setPosition(newX, newY);
						pieceArray[knightIndex]->setPosition(oldX,oldY);
			
						boardarray[oldX][oldY] = knightBoardIndex;
						boardarray[newX][newY] = queenBoardIndex;
						return true;
					}
				}
			}
			pieceArray[queenIndex]->setPosition(newX, newY);
			pieceArray[knightIndex]->setPosition(oldX,oldY);
			boardarray[oldX][oldY] = knightBoardIndex;
			boardarray[newX][newY] = queenBoardIndex;
		}
		else
		{
		
			int pieceIndex;
			for(int i = 0; i < 32; i++)
			{
				if(pieceArray[i]->getX() == oldX && pieceArray[i]->getY() == oldY)
				{
					pieceIndex = i;
					pieceArray[i]->setPosition(newX, newY);
					break;
				}
			}
			int boardIndex;
			boardIndex = boardarray[oldX][oldY];
			boardarray[newX][newY] = pieceIndex;
			boardarray[oldX][oldY] = -1;
			for(int i = 0; i < 32; i++)
			{
				if(((i >= 0 && i <= 7) || i == 24 || i == 20 || i == 16 || i == 28 || i == 30 || i == 18 || i == 22 || i == 26) && pieceIndex != 31)
				{
					if(pieceArray[i]->moveLegal(kingX, kingY, -2) && collision(kingX, kingY, pieceArray[i]->getX(), pieceArray[i]->getY()))
					{
						pieceArray[pieceIndex]->setPosition(oldX, oldY);
						boardarray[oldX][oldY] = boardIndex;
						boardarray[newX][newY] = -1;
						return true;
					}
				}
				else if(((i >= 0 && i <= 7) || i == 24 || i == 20 || i == 16 || i == 28 || i == 30 || i == 18 || i == 22 || i == 26) && pieceIndex != 31)
				{
					if(pieceArray[i]->moveLegal(newX, newY, -2) && collision(newX, newY, pieceArray[i]->getX(), pieceArray[i]->getY()))
					{
						pieceArray[pieceIndex]->setPosition(oldX, oldY);
						boardarray[oldX][oldY] = boardIndex;
						boardarray[newX][newY] = -1;
						return true;
					}
				}
			}
			
			
			
			pieceArray[pieceIndex]->setPosition(oldX, oldY);
			boardarray[oldX][oldY] = boardIndex;
			boardarray[newX][newY] = -1;
		}
		
	}


	return false;
}

//Board Methods defined
void Board::updateBoard()
{
	chessboard.display(main_disp);//update board
	debugbox.display(debug_disp);

	if(turn == true) //was blacks turn
	{
		if(isCheck(pieceArray[30]->getX(), pieceArray[30]->getY(), -2, -2, -2, -2))
			check = true;
		
	}
	else if(turn == false) //was whites turn
	{
		if(isCheck(pieceArray[31]->getX(), pieceArray[31]->getY(), -2, -2, -2, -2))
			check = true;
	}
}

int Board::returnPiece(int x, int y)//returns piece at spot on board given
{
	return boardarray[x][y];
}


CImg<unsigned char> Board::parseDraw(int xx, int yy, int pp)//draw light pieces on light squares, dark pieces on dark squares
{
	bool color = true; //true is light, false is dark
	//determine if square is light or dark
	if(xx%2 == 0 && yy%2 == 0)
		color = true;
	else if(xx%2 == 0 && yy != 0)
		color = false;
	else if(xx%2 != 0 && yy%2 == 0)
		color = false;
	else if(xx%2 != 0 && yy%2 != 0)
		color = true;
		
	//assign image to variables
	CImg<unsigned char> piece;
	if(color)
	{
		if(pp == WHITEKING)
		piece = wlking;
		if(pp == WHITEQUEEN)
		piece = wlqueen;
		if(pp == WHITEKNIGHT || pp == WHITEKNIGHTTWO)
		piece = wlknight;
		if(pp == WHITEBISHOP || pp == WHITEBISHOPTWO)
		piece = wlbishop;
		if(pp == WHITEROOK || pp == WHITEROOKTWO)
		piece = wlrook;
		if(pp == WHITEPAWN || pp == WHITEPAWNTWO || pp == WHITEPAWNTHREE || pp == WHITEPAWNFOUR || pp == WHITEPAWNFIVE || pp == WHITEPAWNSIX || pp == WHITEPAWNSEVEN || pp == WHITEPAWNEIGHT)
		piece = wlpawn;
		if(pp == BLACKKING)
		piece = blking;
		if(pp == BLACKQUEEN)
		piece = blqueen;
		if(pp == BLACKKNIGHT || pp == BLACKKNIGHTTWO)
		piece = blknight;
		if(pp == BLACKBISHOP || pp == BLACKBISHOPTWO)
		piece = blbishop;
		if(pp == BLACKROOK || pp == BLACKROOKTWO)
		piece = blrook;
		if(pp == BLACKPAWN || pp == BLACKPAWNTWO || pp == BLACKPAWNTHREE || pp == BLACKPAWNFOUR || pp == BLACKPAWNFIVE || pp == BLACKPAWNSIX || pp == BLACKPAWNSEVEN || pp == BLACKPAWNEIGHT)
		piece = blpawn;
		if(pp == BLANK)
		piece = lblank;
	}
	else
	{
		if(pp == WHITEKING)
		piece = wdking;
		if(pp == WHITEQUEEN)
		piece = wdqueen;
		if(pp == WHITEKNIGHT || pp == WHITEKNIGHTTWO)
		piece = wdknight;
		if(pp == WHITEBISHOP || pp == WHITEBISHOPTWO)
		piece = wdbishop;
		if(pp == WHITEROOK || pp == WHITEROOKTWO)
		piece = wdrook;
		if(pp == WHITEPAWN || pp == WHITEPAWNTWO || pp == WHITEPAWNTHREE || pp == WHITEPAWNFOUR || pp == WHITEPAWNFIVE || pp == WHITEPAWNSIX || pp == WHITEPAWNSEVEN || pp == WHITEPAWNEIGHT)
		piece = wdpawn;
		if(pp == BLACKKING)
		piece = bdking;
		if(pp == BLACKQUEEN)
		piece = bdqueen;
		if(pp == BLACKKNIGHT || pp == BLACKKNIGHTTWO)
		piece = bdknight;
		if(pp == BLACKBISHOP || pp == BLACKBISHOPTWO)
		piece = bdbishop;
		if(pp == BLACKROOK || pp == BLACKROOKTWO)
		piece = bdrook;
		if(pp == BLACKPAWN || pp == BLACKPAWNTWO || pp == BLACKPAWNTHREE || pp == BLACKPAWNFOUR || pp == BLACKPAWNFIVE || pp == BLACKPAWNSIX || pp == BLACKPAWNSEVEN || pp == BLACKPAWNEIGHT)
		piece = bdpawn;
		if(pp == BLANK)
		piece = dblank;
	}
	return(piece);
}

void Board::setup()
{
	
	//setup board
	//for painting
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 8; j++)
			boardarray[i][j] = BLANK;
	
	//white
	chessboard.draw_image(0,7*PIXELSQUARESIZE,wdrook);
	boardarray[0][7] = WHITEROOK;
	chessboard.draw_image(1*PIXELSQUARESIZE,7*PIXELSQUARESIZE,wlknight);
	boardarray[1][7] = WHITEKNIGHT;
	chessboard.draw_image(2*PIXELSQUARESIZE,7*PIXELSQUARESIZE,wdbishop);
	boardarray[2][7] = WHITEBISHOP;
	chessboard.draw_image(3*PIXELSQUARESIZE,7*PIXELSQUARESIZE,wlqueen);
	boardarray[3][7] = WHITEQUEEN;
	chessboard.draw_image(4*PIXELSQUARESIZE,7*PIXELSQUARESIZE,wdking);
	boardarray[4][7] = WHITEKING;
	chessboard.draw_image(5*PIXELSQUARESIZE,7*PIXELSQUARESIZE,wlbishop);
	boardarray[5][7] = WHITEBISHOPTWO;
	chessboard.draw_image(6*PIXELSQUARESIZE,7*PIXELSQUARESIZE,wdknight);
	boardarray[6][7] = WHITEKNIGHTTWO;
	chessboard.draw_image(7*PIXELSQUARESIZE,7*PIXELSQUARESIZE,wlrook);
	boardarray[7][7] = WHITEROOKTWO;
	chessboard.draw_image(0*PIXELSQUARESIZE,6*PIXELSQUARESIZE,parseDraw(0,6,WHITEPAWN));
	boardarray[0][6] = WHITEPAWN;
	chessboard.draw_image(1*PIXELSQUARESIZE,6*PIXELSQUARESIZE,parseDraw(1,6,WHITEPAWN));
	boardarray[1][6] = WHITEPAWNTWO;
	chessboard.draw_image(2*PIXELSQUARESIZE,6*PIXELSQUARESIZE,parseDraw(2,6,WHITEPAWN));
	boardarray[2][6] = WHITEPAWNTHREE;
	chessboard.draw_image(3*PIXELSQUARESIZE,6*PIXELSQUARESIZE,parseDraw(3,6,WHITEPAWN));
	boardarray[3][6] = WHITEPAWNFOUR;
	chessboard.draw_image(4*PIXELSQUARESIZE,6*PIXELSQUARESIZE,parseDraw(4,6,WHITEPAWN));
	boardarray[4][6] = WHITEPAWNFIVE;
	chessboard.draw_image(5*PIXELSQUARESIZE,6*PIXELSQUARESIZE,parseDraw(5,6,WHITEPAWN));
	boardarray[5][6] = WHITEPAWNSIX;
	chessboard.draw_image(6*PIXELSQUARESIZE,6*PIXELSQUARESIZE,parseDraw(6,6,WHITEPAWN));
	boardarray[6][6] = WHITEPAWNSEVEN;
	chessboard.draw_image(7*PIXELSQUARESIZE,6*PIXELSQUARESIZE,parseDraw(7,6,WHITEPAWN));
	boardarray[7][6] = WHITEPAWNEIGHT;
		
	//black
	chessboard.draw_image(0,0,blrook);
	boardarray[0][0] = BLACKROOK;
	chessboard.draw_image(1*PIXELSQUARESIZE,0,bdknight);
	boardarray[1][0] = BLACKKNIGHT;
	chessboard.draw_image(2*PIXELSQUARESIZE,0,blbishop);
	boardarray[2][0] = BLACKBISHOP;
	chessboard.draw_image(3*PIXELSQUARESIZE,0,bdqueen);
	boardarray[3][0] = BLACKQUEEN;
	chessboard.draw_image(4*PIXELSQUARESIZE,0,blking);
	boardarray[4][0] = BLACKKING;
	chessboard.draw_image(5*PIXELSQUARESIZE,0,bdbishop);
	boardarray[5][0] = BLACKBISHOPTWO;
	chessboard.draw_image(6*PIXELSQUARESIZE,0,blknight);
	boardarray[6][0] = BLACKKNIGHTTWO;
	chessboard.draw_image(7*PIXELSQUARESIZE,0,bdrook);
	boardarray[7][0] = BLACKROOKTWO;
	chessboard.draw_image(0*PIXELSQUARESIZE,1*PIXELSQUARESIZE,parseDraw(0,1,BLACKPAWN));
	boardarray[0][1] = BLACKPAWN;
	chessboard.draw_image(1*PIXELSQUARESIZE,1*PIXELSQUARESIZE,parseDraw(1,1,BLACKPAWN));
	boardarray[1][1] = BLACKPAWNTWO;
	chessboard.draw_image(2*PIXELSQUARESIZE,1*PIXELSQUARESIZE,parseDraw(2,1,BLACKPAWN));
	boardarray[2][1] = BLACKPAWNTHREE;
	chessboard.draw_image(3*PIXELSQUARESIZE,1*PIXELSQUARESIZE,parseDraw(3,1,BLACKPAWN));
	boardarray[3][1] = BLACKPAWNFOUR;
	chessboard.draw_image(4*PIXELSQUARESIZE,1*PIXELSQUARESIZE,parseDraw(4,1,BLACKPAWN));
	boardarray[4][1] = BLACKPAWNFIVE;
	chessboard.draw_image(5*PIXELSQUARESIZE,1*PIXELSQUARESIZE,parseDraw(5,1,BLACKPAWN));
	boardarray[5][1] = BLACKPAWNSIX;
	chessboard.draw_image(6*PIXELSQUARESIZE,1*PIXELSQUARESIZE,parseDraw(6,1,BLACKPAWN));
	boardarray[6][1] = BLACKPAWNSEVEN;
	chessboard.draw_image(7*PIXELSQUARESIZE,1*PIXELSQUARESIZE,parseDraw(7,1,BLACKPAWN));
	boardarray[7][1] = BLACKPAWNEIGHT;

	updateBoard();
}




void Board::moveDraw(CImg<unsigned char> piece, int srcx, int srcy, int destx, int desty)//x and y are 0-7 only call if your move is valid you dummy
{
	/*//Queening a pawn
	else if((piece == blpawn && desty == 7) || (piece == bdpawn && desty == 0))
	{
		//
	}
	else if((piece == wlpawn && desty == 0) || (piece == wdpawn && desty == 0))
	{
		//
	}*/
	chessboard.draw_image(destx*PIXELSQUARESIZE, desty*PIXELSQUARESIZE, piece);
	
	chessboard.draw_image(srcx*PIXELSQUARESIZE, srcy*PIXELSQUARESIZE,parseDraw(srcx,srcy,BLANK));
		
	
	boardarray[destx][desty] = boardarray[srcx][srcy];//move piece to new position
	boardarray[srcx][srcy] = BLANK;//override old one
	updateBoard();
}

void Board::enPassant(bool *typeMoveLegal, int srcx, int srcy, int destx, int desty, int srcPiece, bool whiteColorsrc, int srcPieceColor){
			
	if(typeMoveLegal && abs(srcy - desty) == 2)
	{
		pieceArray[srcPiece]->setPassantCount(moveCount + 1);
	}
	else if(!*typeMoveLegal)
	{
		int val1 = -1;
		if(whiteColorsrc) val1 = 1;
		if(boardarray[destx][desty+val1] != -1)
		{
			int whiteBack = -1;
			bool whetherWhite = true;
			whetherWhite = pieceArray[boardarray[destx][desty + val1]]->getWhite();
			if(whetherWhite)
				whiteBack = 1;
			else
				whiteBack = 2;

			if((srcy - desty == val1) && abs(destx - srcx) == 1)
			{
				if(boardarray[destx][desty + val1] >= 0 || boardarray[destx][desty + val1] <= 17)
				{
					if(moveCount == pieceArray[boardarray[destx][desty + val1]]->getPassantCount() && (whiteBack != srcPieceColor))
					{
						*typeMoveLegal = true;
						moveDraw(dblank, destx, desty + val1, destx, desty + val1);
					}
				}
			}
		}
	}
			
}	

void Board::castling(bool *typeMoveLegal, int srcx, int srcy, int destx, int desty, int srcPiece){

	if(srcPiece == 31 && srcx == 4 && (destx == 6 || destx == 2) && pieceArray[srcPiece]->getFirstMoved()){
		if(boardarray[5][srcy] == -1 && boardarray[6][srcy] == -1 && boardarray[7][srcy] == BLACKROOKTWO && pieceArray[BLACKROOKTWO]->getFirstMoved()){
			pieceArray[BLACKROOKTWO]->setPosition(5, 0);
			moveDraw(bdrook, 7, 0, 5, 0);
			*typeMoveLegal = true;
		}
		if(boardarray[2][srcy] == -1 && boardarray[3][srcy] == -1 && boardarray[0][srcy] == BLACKROOK && pieceArray[BLACKROOK]->getFirstMoved()){
			pieceArray[BLACKROOK]->setPosition(3, 0);
			moveDraw(bdrook, 0, 0, 3, 0);
			*typeMoveLegal = true;
		}
	}
	//for white castle
	if(srcPiece == 30 && srcx == 4 && (destx == 6 || destx == 2) && pieceArray[srcPiece]->getFirstMoved()){
		if(boardarray[5][7] == -1 && boardarray[6][7] == -1 && boardarray[7][7] == WHITEROOKTWO && pieceArray[WHITEROOKTWO]->getFirstMoved()){
			pieceArray[WHITEROOKTWO]->setPosition(srcx, 5);
			moveDraw(wlrook, 7, 7, 5, 7);
			*typeMoveLegal = true;
		}
		if(boardarray[2][srcy] == -1 && boardarray[3][srcy] == -1 && boardarray[0][srcy] == WHITEROOK && pieceArray[WHITEROOK]->getFirstMoved()){
			pieceArray[WHITEROOK]->setPosition(3, 7);
			moveDraw(wlrook, 0, 7, 3, 7);
			*typeMoveLegal = true;
		}
	}
}

bool Board::validMove(int srcx, int srcy, int destx, int desty)
{
		bool collisionLegal = false;
		bool typeMoveLegal = false;
		bool sameTeam = true;
		int srcPiece = boardarray[srcx][srcy];
		int destPiece = boardarray[destx][desty];
		int srcPieceColor = 0;
		int destPieceColor = 0;
		bool whiteColorsrc = pieceArray[srcPiece]->getWhite();
		bool whiteColorDest = false;
		bool checkMove = true;
		int colorTurn = 0;

		if(turn)
			colorTurn = 1;
		else
			colorTurn = 2;

		if(whiteColorsrc) srcPieceColor = 1;
			else srcPieceColor = 2;

		if(colorTurn == srcPieceColor){					//checks for whose turn it is

			
			if(destPiece != -1){						//fixes OutofIndex error
				whiteColorDest = pieceArray[destPiece]->getWhite();

				if(whiteColorDest) destPieceColor = 1;		//checks for friendly fire
				else destPieceColor = 2;
			}
			else destPieceColor = 3;

			if(srcPieceColor == destPieceColor)
				sameTeam = false;
			
			typeMoveLegal = pieceArray[srcPiece]->moveLegal(destx, desty, destPiece); 
			collisionLegal = collision(destx, desty, srcx, srcy);		

			//CASTLING
			if(srcPiece == 31 || srcPiece == 30)		
				castling(&typeMoveLegal, srcx, srcy, destx, desty, srcPiece);

		
			if(typeMoveLegal && collisionLegal && check && turn) //legal move, in check, white turn
			{
				if(isCheck(pieceArray[WHITEKING]->getX(), pieceArray[WHITEKING]->getY(), destx, desty, srcx, srcy))
				{
					checkMove = false;
				}
				else
					check = false;
			}
			else if(typeMoveLegal && collisionLegal && check && !turn) //legal move, in check, black turn
			{
				if(isCheck(pieceArray[BLACKKING]->getX(), pieceArray[BLACKKING]->getY(), destx, desty, srcx, srcy))
				{
					checkMove = false;
				}
				else
					check = false;
			}	

		
			//EN PASSANT
			if((srcPiece >= 0 && srcPiece <= 17))
			//if((srcPiece >= 0 || srcPiece <= 17) && collisionLegal && sameTeam && checkMove)
			{
				enPassant(&typeMoveLegal, srcx, srcy, destx, desty, srcPiece, whiteColorsrc, srcPieceColor);
			}

			if(srcPiece >= 20 && srcPiece <= 23)
				collisionLegal = true;
	
			if(typeMoveLegal && collisionLegal && sameTeam && checkMove)
			{
				if(srcPiece == 30 || srcPiece == 31 || (srcPiece <= 27 && srcPiece >= 24) || (srcPiece >= 0 && srcPiece <= 15))
					pieceArray[srcPiece]->setFirstMoved();	
				moveCount++;
				pieceArray[srcPiece]->setPosition(destx, desty);
				turn = !turn;
				return true;
			}
		}

		return false;
	
		
}

bool Board::handleClick(int p, int x, int y, bool select)
{
		if(lastp == p)
	{	
		CImg<unsigned char> originalPiece = parseDraw(lastx,lasty,lastPieceP);
		lastp = BLANK;
		chessboard.draw_image(lastx*PIXELSQUARESIZE, lasty*PIXELSQUARESIZE, originalPiece);
		debugbox.fill(0).draw_text(0, 0, "PIECE DESELECTED.", green);
		return false;
	}
	else if(select && lastp != BLANK)
	{
		//check what piece to draw
		CImg<unsigned char> piecetodraw = parseDraw(x,y,lastp);//Last p is the piece to move
		lastp = BLANK;//overwrite last piece to prevent double drawing
		
		//CHECK VALID MOVE FIRST
		if(validMove(lastx,lasty,x,y))
		{
			moveDraw(piecetodraw,lastx,lasty,x,y);
			lastPieceP = BLANK;
		}
		
		else
		{
			debugbox.fill(0).draw_text(0, 0, "INVALID MOVE.\n MOVE AGAIN.", green);
			CImg<unsigned char> originalPiece = parseDraw(lastx,lasty,lastPieceP);
			chessboard.draw_image(lastx*PIXELSQUARESIZE, lasty*PIXELSQUARESIZE, originalPiece);
			//debugbox.fill(0).draw_text(0, 0, "PIECE DESELECTED.", green);
		}
	}
	else
	{
		if(p != BLANK){
		lastx = x;
		lasty = y;
		
			lastPieceP = p;
		lastp = p;

		CImg<unsigned char> outlines;
		outlines = selection;
		chessboard.draw_image(lastx*PIXELSQUARESIZE, lasty*PIXELSQUARESIZE, outlines);
		chessboard.draw_image(lastx*PIXELSQUARESIZE + 95, lasty*PIXELSQUARESIZE, outlines);
		debugbox.fill(0).draw_text(0, 0, "PIECE SELECTED.", green);
		}
	}
	return true;
}
