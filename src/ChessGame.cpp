#include "ChessGame.h"

ChessGame::ChessGame(Graphic* g)
{

	//Initialises graphics variables
	graph = g;
	renderer = g->get_renderer();

	//Initialises Textures
	boardComponents[0] = g->loadTexture("img/letters.bmp");
	boardComponents[1] = g->loadTexture("img/numbers.bmp");
	boardComponents[2] = g->loadTexture("img/chess_pieces.bmp",1);

	//Initialises buttons
	for(int i = 0;i<8;i++)
	{
		for(int j = 0;j<8;j++)
		{
			allButtons[i][j].setPosition(j*SQUARE+startingPoint[0],i*SQUARE+startingPoint[1]);
		}
	}

	//Initialises internal variables
	moving = false;
	white_turn = true;
	game_ended = false;

}

bool ChessGame::ended()
{
	return game_ended;
}

void ChessGame::drawBoard()
{
	/*
		Draws the numbers
		Draws The letters
		Draws the squares
	*/

	//Draws the numbers
	SDL_Rect numbers = {startingPoint[0]-SQUARE,startingPoint[1],SQUARE,SQUARE*8};
	SDL_RenderCopy(renderer,boardComponents[1],NULL,&numbers);

	//Draws the letters
	SDL_Rect letters = {startingPoint[0],startingPoint[1]+(SQUARE*8),SQUARE*8,((int)(SQUARE*1.43))};
	SDL_RenderCopy(renderer,boardComponents[0],NULL,&letters);

	//Draws the outline square
	SDL_Rect outline = {startingPoint[0],startingPoint[1],SQUARE*8,SQUARE*8};
	SDL_SetRenderDrawColor(renderer,0,0,0,0xFF);
	SDL_RenderDrawRect(renderer,&outline);

	//Draws the board squares
	SDL_SetRenderDrawColor(renderer,64,64,64,255);
	for(int i = 0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			//j is for x coordinates
			//i is for y coordinates
			
			if((i+j)%2!=0)
			{
				SDL_Rect sq = {startingPoint[0]+j*SQUARE,startingPoint[1]+i*SQUARE,SQUARE,SQUARE};
				SDL_RenderFillRect(renderer,&sq);
			}
		}
	}

}

void ChessGame::drawStates()
{
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	for(int i = 0;i<8;i++)
	{
		for(int j = 0;j<8;j++)
		{
			//j is for x coordinates
			//i is for y coordinates
			/*
				Square color are indicated by the squareState array:
				'-': do nothing, render it as it is
				'p': render it blue
				'k': render it red
				'c': render it yellow
			*/
			if(squareState[i][j] == '-'){
				continue;
			}
			SDL_Rect sq = {startingPoint[0]+j*SQUARE,startingPoint[1]+i*SQUARE,SQUARE,SQUARE};
			if(squareState[i][j] == 'p')
			{
				SDL_SetRenderDrawColor(renderer,0,0,127,127);
			}
			if(squareState[i][j] == 'k')
			{
				SDL_SetRenderDrawColor(renderer,127,0,0,127);
			}
			if(squareState[i][j] == 'c')
			{
				SDL_SetRenderDrawColor(renderer,127,127,0,127);
			}
			SDL_RenderFillRect(renderer,&sq);
		}
	}
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);
}

void ChessGame::drawPieces()
{
	for(int i = 0;i<8;i++)
	{
		for(int j = 0;j<8;j++)
		{
			if(chessboard[i][j] == '.')
			{
				continue;
			}
			SDL_Rect sq = {startingPoint[0]+j*SQUARE,startingPoint[1]+i*SQUARE,SQUARE,SQUARE};
			SDL_Rect cut = pieces[piece_names.find(chessboard[i][j])];
			SDL_RenderCopy(renderer,boardComponents[2],&cut,&sq);
		}
	}
}

void ChessGame::render()
{

	SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
	SDL_RenderClear(renderer);

	drawBoard();
	drawStates();
	drawPieces();

	SDL_RenderPresent(renderer);
}

void ChessGame::handleEvent(SDL_Event* e)
{
	for(int i = 0;i<8;i++)
	{
		for(int j = 0;j<8;j++)
		{
			allButtons[i][j].handleEvent(e);
			if(allButtons[i][j].getSprite() == BUTTON_SPRITE_MOUSE_DOWN)
			{
				handleClick(i,j);
			}
		}
	}

}

void ChessGame::handleClick(const int &i,const int &j)
{
	char piece = chessboard[i][j];
	std::string black_pieces = "rnbkqp";
	std::string white_pieces = "RNBKQP";

	if(!moving)
	{
		if(white_turn)
		{
			
			if(white_pieces.find(piece)!=-1)
			{
				moving_piece[0] = i;
				moving_piece[1] = j;
				calculateMoves();
				moving = true;
			}
		}else{
			if(black_pieces.find(piece)!=-1)
			{
				moving_piece[0] = i;
				moving_piece[1] = j;
				calculateMoves();
				moving = true;
			}
		}

	}else{

		//IMLEMENT CHECK AND PLAYER CHANGE

		if((squareState[i][j] == 'p') || (squareState[i][j] == 'c')) //Clicked square is a possible move or a capture
		{
			char saved = chessboard[moving_piece[0]][moving_piece[1]];
			chessboard[i][j] = chessboard[moving_piece[0]][moving_piece[1]];
			chessboard[moving_piece[0]][moving_piece[1]] = '.';
			if( (check()+white_turn)!=2 ){ //It is a safe move to make
				white_turn = !white_turn;
				turnBoard();
				/*YOU SHOULD CHECK IF CHECKMATE HERE --------------------------------------------

				azaz
				eraz
				er
				azer
				aze
				raze
				geehtryterutey
				ez
				zer
				zery
				Just trying to prove my point here.


				Just saying.
				*/
				checkmate(!white_turn);

			}else{
				chessboard[moving_piece[0]][moving_piece[1]] = chessboard[i][j];
				chessboard[i][j] = '.';
			}
		}

		resetSquareStates();

		moving = false;

	}
}

void ChessGame::resetSquareStates()
{
	for(int i = 0;i<8;i++)
	{
		for(int j = 0;j<8;j++)
		{
			squareState[i][j] = '-';
		}
	}
}

void ChessGame::calculateMoves()
{
	/*
		Takes moving_piece as an implicit argument
		Checks the type of the piece,
		calculates the possible moves that that piece can do:
			respecting the moving rules of chess
			without putting the king into check (which itself needs a function checkCheck() but I'll write that after this)
		marks the free, possible squares with 'p', the 
	*/
	int r = moving_piece[0],c = moving_piece[1];
	char piece = chessboard[r][c];

	if(toupper(piece) == 'P')
	{
		if(chessboard[r-1][c] == '.')
		{
			mark(r-1,c,'p');
			if(r == 6 && chessboard[r-2][c] == '.')
			{
				mark(r-2,c,'p');
			}
		}
		if(differentCamps(piece,chessboard[r-1][c+1]) && chessboard[r-1][c+1]!='.')
		{
			mark(r+1,c+1,'c');
		}
		if(differentCamps(piece,chessboard[r-1][c-1]) && chessboard[r-1][c-1]!='.')
		{
			mark(r+1,c-1,'c');
		}
		return;
	}

	if(toupper(piece) == 'R')
	{
		//Calculate possible moves on the right
		for(int j = c+1;j<8;j++)
		{
			if(chessboard[r][j] == '.')
			{
				mark(r,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[r][j]))
				{
					mark(r,j,'c');
				}
				break;
			}
		}

		//Calculate possible moves on the left
		for(int j = c-1;j>=0;j--)
		{
			if(chessboard[r][j] == '.')
			{
				mark(r,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[r][j]))
				{
					mark(r,j,'c');
				}
				break;
			}
		}

		//Calculate possible moves downwards
		for(int i = r+1;i<8;i++)
		{
			if(chessboard[i][c] == '.')
			{
				mark(i,c,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][c]))
				{
					mark(i,c,'c');
				}
				break;
			}
		}

		//Calculate possible moves upwards
		for(int i = r-1;i>=0;i--)
		{
			if(chessboard[i][c] == '.')
			{
				mark(i,c,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][c]))
				{
					mark(i,c,'c');
				}
				break;
			}
		}
		return;
	}

	if(toupper(piece) == 'B')
	{
		//Checks for moves downwards right
		for(int i = r+1,j=c+1;(i<8&&j<8);i++,j++)
		{
			if(chessboard[i][j] == '.')
			{
				mark(i,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][j]))
				{
					mark(i,j,'c');
				}
				break;
			}
		}
		//Checks for moves downwards left
		for(int i = r+1,j=c-1;(i<8&&j>=0);i++,j--)
		{
			if(chessboard[i][j] == '.')
			{
				mark(i,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][j]))
				{
					mark(i,j,'c');
				}
				break;
			}
		}
		//checks for moves upwards right
		for(int i = r-1,j=c+1;(i>=0&&j<8);i--,j++)
		{
			if(chessboard[i][j] == '.')
			{
				mark(i,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][j]))
				{
					mark(i,j,'c');
				}
				break;
			}
		}
		//checks fo moves upwards left
		for(int i = r-1,j=c-1;(i>=0&&j>=0);i--,j--)
		{
			if(chessboard[i][j] == '.')
			{
				mark(i,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][j]))
				{
					mark(i,j,'c');
				}
				break;
			}
		}

		return;
	}

	if(toupper(piece) == 'Q')
	{
		//Calculate possible moves on the right
		for(int j = c+1;j<8;j++)
		{
			if(chessboard[r][j] == '.')
			{
				mark(r,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[r][j]))
				{
					mark(r,j,'c');
				}
				break;
			}
		}

		//Calculate possible moves on the left
		for(int j = c-1;j>=0;j--)
		{
			if(chessboard[r][j] == '.')
			{
				mark(r,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[r][j]))
				{
					mark(r,j,'c');
				}
				break;
			}
		}

		//Calculate possible moves downwards
		for(int i = r+1;i<8;i++)
		{
			if(chessboard[i][c] == '.')
			{
				mark(i,c,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][c]))
				{
					mark(i,c,'c');
				}
				break;
			}
		}

		//Calculate possible moves upwards
		for(int i = r-1;i>=0;i--)
		{
			if(chessboard[i][c] == '.')
			{
				mark(i,c,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][c]))
				{
					mark(i,c,'c');
				}
				break;
			}
		}

		//Checks for moves downwards right
		for(int i = r+1,j=c+1;(i<8&&j<8);i++,j++)
		{
			if(chessboard[i][j] == '.')
			{
				mark(i,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][j]))
				{
					mark(i,j,'c');
				}
				break;
			}
		}
		//Checks for moves downwards left
		for(int i = r+1,j=c-1;(i<8&&j>=0);i++,j--)
		{
			if(chessboard[i][j] == '.')
			{
				mark(i,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][j]))
				{
					mark(i,j,'c');
				}
				break;
			}
		}
		//checks for moves upwards right
		for(int i = r-1,j=c+1;(i>=0&&j<8);i--,j++)
		{
			if(chessboard[i][j] == '.')
			{
				mark(i,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][j]))
				{
					mark(i,j,'c');
				}
				break;
			}
		}
		//checks fo moves upwards left
		for(int i = r-1,j=c-1;(i>=0&&j>=0);i--,j--)
		{
			if(chessboard[i][j] == '.')
			{
				mark(i,j,'p');
				continue;
			}else{
				if(differentCamps(piece,chessboard[i][j]))
				{
					mark(i,j,'c');
				}
				break;
			}
		}
		return;
	}

	if(toupper(piece) == 'N')
	{
		//Check moves on the right
		int j = c + 2;
		if(j<8)
		{
			if(r<7)
			{
				if(chessboard[r+1][j] == '.')
				{
					mark(r+1,j,'p');
				}else if(differentCamps(piece,chessboard[r+1][j]))
				{
					mark(r+1,j,'c');
				}
			}
			if(r>0)
			{
				if(chessboard[r-1][j] == '.')
				{
					mark(r-1,j,'p');
				}else if(differentCamps(piece,chessboard[r-1][j]))
				{
					mark(r-1,j,'c');
				}
			}
		}
		//Check moves on the left

		j = c-2;
		if(j>=0)
		{
			if(r<7)
			{
				if(chessboard[r+1][j] == '.')
				{
					mark(r+1,j,'p');
				}else if(differentCamps(piece,chessboard[r+1][j]))
				{
					mark(r+1,j,'c');
				}
			}
			if(r>0)
			{
				if(chessboard[r-1][j] == '.')
				{
					mark(r-1,j,'p');
				}else if(differentCamps(piece,chessboard[r-1][j]))
				{
					mark(r-1,j,'c');
				}
			}
		}

		//Check moves upwards

		int i = r+2;
		if(i<8)
		{
			if(c<7)
			{
				if(chessboard[i][c+1] == '.')
				{
					mark(i,c+1,'p');
				}else if(differentCamps(piece,chessboard[i][c+1]))
				{
					mark(i,c+1,'c');
				}
			}
			if(c>0)
			{
				if(chessboard[i][c-1] == '.')
				{
					mark(i,c-1,'p');
				}else if(differentCamps(piece,chessboard[i][c-1]))
				{
					mark(i,c-1,'c');
				}
			}
		}
		//Check moves downwards
		i = r - 2;
		if(i>=0)
		{
			if(c<7)
			{
				if(chessboard[i][c+1] == '.')
				{
					mark(i,c+1,'p');
				}else if(differentCamps(piece,chessboard[i][c+1]))
				{
					mark(i,c+1,'c');
				}
			}
			if(c>0)
			{
				if(chessboard[i][c-1] == '.')
				{
					mark(i,c-1,'p');
				}else if(differentCamps(piece,chessboard[i][c-1]))
				{
					mark(i,c-1,'c');
				}
			}
		}
		return;
	}

	if(toupper(piece) == 'K')
	{
		for(int i = r-1;i<r+2;i++)
		{
			for(int j = c-1;j<c+2;j++)
			{
				if(i>7||i<0||j>7||j<0)
				{
					continue;
				}
				if(chessboard[i][j] == '.')
				{
					mark(i,j,'p');
				}else if(differentCamps(chessboard[i][j],piece))
				{
					mark(i,j,'c');
				}
			}
		}
		return;
	}
}

bool ChessGame::differentCamps(char a,char b)
{
	return (a & 32)!=(b & 32);
}

void ChessGame::mark(const int &i,const int&j,char ch)
{
	squareState[i][j] = ch;
}

void ChessGame::turnBoard()
{
	char temp[8];
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<8;j++)
		{
			temp[7-j] = chessboard[i][j];
		}
		for(int j = 0;j<8;j++)
		{
			chessboard[i][7-j] = chessboard[7-i][j];
		}
		for(int j = 0;j<8;j++)
		{
			chessboard[7-i][j] = temp[j];
		}
	}
}

int ChessGame::check()
{
	/*
		Checks if any king is at check:
			Returns 0 if no king is at check
			Returns 1 if the white king is at check
			Returns 2 if the black king is at check
	*/
	int kings[2][2];
	for(int i = 0;i<8;i++)
	{
		for(int j = 0;j<8;j++)
		{
			if(chessboard[i][j] == 'K')
			{
				kings[0][0] = i;
				kings[0][1] = j;
			}
			if(chessboard[i][j] == 'k')
			{
				kings[1][0] = i;
				kings[1][1] = j;
			}
		}
	}

	for(int pos = 0;pos<2;pos++)
	{
		char piece = (pos==0)?'K':'k';
		int r = kings[pos][0];
		int c = kings[pos][1];

		/*
			CHECKS IF A KNIGHT IS ATTACKING THE KING
		*/

		//Check moves on the right
		int k = c + 2;
		if(k<8)
		{
			if(r<7)
			{
				if(differentCamps(piece,chessboard[r+1][k]) && toupper(chessboard[r+1][k]) == 'N')
				{
					return pos+1;
				}
			}
			if(r>0)
			{
				if(differentCamps(piece,chessboard[r-1][k]) && toupper(chessboard[r-1][k])=='N')
				{
					return pos+1;
				}
			}
		}
		//Check moves on the left

		k = c-2;
		if(k>=0)
		{
			if(r<7)
			{
				if(differentCamps(piece,chessboard[r+1][k]) && toupper(chessboard[r+1][k]) == 'N')
				{
					return pos+1;
				}
			}
			if(r>0)
			{
				if(differentCamps(piece,chessboard[r-1][k]) && toupper(chessboard[r-1][k]) == 'N')
				{
					return pos+1;
				}
			}
		}

		//Check moves upwards

		int l = r+2;
		if(l<8)
		{
			if(c<7)
			{
				if(differentCamps(piece,chessboard[l][c+1]) && toupper(chessboard[l][c+1]) == 'N')
				{
					return pos+1;
				}
			}
			if(c>0)
			{
				if(differentCamps(piece,chessboard[l][c-1]) && toupper(chessboard[l][c-1]) == 'N')
				{
					return pos+1;
				}
			}
		}
		//Check moves downwards
		l = r - 2;
		if(l>=0)
		{
			if(c<7)
			{
				if(differentCamps(piece,chessboard[l][c+1]) && toupper(chessboard[l][c+1])=='N')
				{
					return pos+1;
				}
			}
			if(c>0)
			{
				if(differentCamps(piece,chessboard[l][c-1]) && toupper(chessboard[l][c-1])=='N')
				{
					return pos+1;
				}
			}
		}

		/*
			END KNIGHT CHECK
		*/

		/*
			CHECKS FOR THREAT IN THE COLUMNS AND ROWS
		*/

					//Check on the right
		for(int j = c+1;j<8;j++)
		{
			if(!differentCamps(piece,chessboard[r][j]) && chessboard[r][j]!='.')break;
			if(differentCamps(piece,chessboard[r][j])&&(toupper(chessboard[r][j])=='N'||toupper(chessboard[r][j])=='Q'))
			{
				return pos+1;
			}
		}

		//Check on the left
		for(int j = c-1;j>=0;j--)
		{
			if(!differentCamps(piece,chessboard[r][j]) && chessboard[r][j]!='.')break;
			if(differentCamps(piece,chessboard[r][j])&&(toupper(chessboard[r][j])=='N'||toupper(chessboard[r][j])=='Q'))
			{
				return pos+1;
			}
		}

		//Check downwards
		for(int i = r+1;i<8;i++)
		{			
			if(!differentCamps(piece,chessboard[i][c]) && chessboard[i][c]!='.')break;
			if(differentCamps(piece,chessboard[i][c])&&(toupper(chessboard[i][c])=='N'||toupper(chessboard[i][c])=='Q') )
			{
				return pos+1;
			}

		}

		//Check upwards
		for(int i = r-1;i>=0;i--)
		{
				if(!differentCamps(piece,chessboard[i][c]) && chessboard[i][c]!='.')break;
				if(differentCamps(piece,chessboard[i][c]) &&(toupper(chessboard[i][c])=='N'||toupper(chessboard[i][c])=='Q'))
				{
					return pos+1;
				}

		}

		/*
			END COLUMNS AND ROWS CHECK
		*/

		/*
			CHECK FOR DIAGONALS
		*/

			//Checks for moves downwards right
		for(int i = r+1,j=c+1;(i<8&&j<8);i++,j++)
		{
			if(!differentCamps(piece,chessboard[i][j]) && chessboard[i][j]!='.')break;
			if(differentCamps(piece,chessboard[i][j]) && (toupper(chessboard[i][j])=='B'||toupper(chessboard[i][j])=='Q'))
				{
					return pos+1;
				}
		}
		//Checks for moves downwards left
		for(int i = r+1,j=c-1;(i<8&&j>=0);i++,j--)
		{
			if(!differentCamps(piece,chessboard[i][j]) && chessboard[i][j]!='.')break;
			if(differentCamps(piece,chessboard[i][j]) && (toupper(chessboard[i][j])=='B'||toupper(chessboard[i][j])=='Q'))
				{
					return pos+1;
				}
		}
		//checks for moves upwards right
		for(int i = r-1,j=c+1;(i>=0&&j<8);i--,j++)
		{
			if(!differentCamps(piece,chessboard[i][j]) && chessboard[i][j]!='.')break;
				if(differentCamps(piece,chessboard[i][j]) && (toupper(chessboard[i][j])=='B'||toupper(chessboard[i][j])=='Q'))
				{
					return pos+1;
				}
		}
		//checks fo moves upwards left
		for(int i = r-1,j=c-1;(i>=0&&j>=0);i--,j--)
		{
			if(!differentCamps(piece,chessboard[i][j]) && chessboard[i][j]!='.')break;
				if(differentCamps(piece,chessboard[i][j])&& (toupper(chessboard[i][j])=='B'||toupper(chessboard[i][j])=='Q'))
				{
					return pos+1;
				}
		}

		/*
			END CHECK FOR DIAGONALS
		*/
	}

	return 0;

}
bool ChessGame::checkmate(bool player)
{
	/*
	*	bool player: true if the player is white, false if the player is black (no racism intended)
	*	Reminder: Black is lowercase, White is uppercase
	*/
	/*
	*	WHAT IS DOES:
	*	Checks if the given player is in a checkmate
	*	And if:
	*	he isn't: does nothing, returns false
	*	he is:Declares the winner, ends the game, returns true
	*	(and optionally, mark the pieces that make the checkmate happen)
	*/
	/*
	*	HOW IT DOES IT:
	*	Iterates through all the pieces on the board and if they are of that player:
	*		Assume that piece is the moving_piece ()
	*		Calculate its moves
	*		Try those moves: Iterate through the possible moves, if "p" or "c"
	*			If one of them is not a check: return false
	*	//If after all this, it hasn't returned false, it means there are no possible moves:
	*	Declares the winner, ends the game, returns true
	*/
	
	for(int i = 0;i<8;i++)
	{
		for(int j = 0;j<8;j++)
		{
			if(isupper(chessboard[i][j])==player) //The piece belongs to that player
			{
				moving_piece[0] = i;
				moving_piece[1] = j;
				calculateMoves();
				//It should now try the moves it found!
				for(int poss_it_i = 0;poss_it_i<8;poss_it_i++)
				{
					for(int poss_it_j = 0;poss_it_j<8;poss_it_j++)
					{
						if((squareState[poss_it_i][poss_it_j] == 'p') || (squareState[poss_it_i][poss_it_j] == 'c'))
						{
							bool there_s_a_possible_move = false;
							char saved = chessboard[poss_it_i][poss_it_j];
							chessboard[poss_it_i][poss_it_j] = chessboard[moving_piece[0]][moving_piece[1]];
							chessboard[moving_piece[0]][moving_piece[1]] = '.';
							if((check()+player)!=2)//NO CHECK for that player! ORRRRR NO KING ON THE BOARD XDDDD
								/*
								THIS SHOULD GET FIXED W2222222222222222222222ZJRETKLZJERLKTJA
								EALZEJRLAKZEJ
								RAZETKA
								MZLEKT
								MALZEKRM
								LAKEZ
								MRLKAZE
								MLRKA
								ZEMLRKAMZELKR
								MLK
								*/
							{
								there_s_a_possible_move = true;
								mark(poss_it_i,poss_it_j,'c');
							}
								chessboard[moving_piece[0]][moving_piece[1]] = chessboard[poss_it_i][poss_it_j];
								chessboard[poss_it_i][poss_it_j] = saved;
								if(there_s_a_possible_move){return false;}
						}
					}
				}
			}
		}
	}
	game_ended=true;
	return true;
}