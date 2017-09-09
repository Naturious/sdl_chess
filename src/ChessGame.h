#include "graphic.h"
#include "button.h"

#ifndef _CHESS_GAME_H_DEFINED
#define _CHESS_GAME_H_DEFINED

const int SQUARE = 65;

class ChessGame
{
	public:

		//Constructor
		ChessGame(Graphic *g);

		//Renders the frame
		void render();

		//Draws the outline, Squares, Letters and Numbers into the renderer
		void drawBoard();

		//Draws the states of the squares (possible move, check, capture..)
		void drawStates();

		//Draws the piece textures into the renderer
		void drawPieces();

		//Returns if the game ended or not
		bool ended();

		//Handles a clicka t the specified i,j coordinates
		void handleClick(const int &,const int &);

		//Handles the mouse event
		void handleEvent(SDL_Event* e);

		//Calculates the moves/captures possible by the current moving piece
		void calculateMoves();

		//Are the two pieces from the same camp?
		bool differentCamps(char,char);

		//Resets all square states to '-'
		void resetSquareStates();

		//Sets a square state to a character value
		void mark(const int &,const int &,char);

		//Turns the board when the player changes
		void turnBoard();

		//Checks if a king is in check
		int check();

		//Checks if there's a checkmate possible
		bool checkmate(bool player);

	private:

		//Graphics object where the game will be played
		Graphic* graph;

		//Renderer where we will render everything
		SDL_Renderer* renderer;

		//Has the game ended yet?
		bool game_ended;

		//Contains the curent state of the game
		//Black is lowercase, white is uppercase
		char chessboard[8][8] = {
			{'r','n','b','q','k','b','n','r'},
			{'p','p','p','p','p','p','p','p'},
			{'.','.','.','.','.','.','.','.'},
			{'.','.','.','.','.','.','.','.'},
			{'.','.','.','.','.','.','.','.'},
			{'.','.','.','.','.','.','.','.'},
			{'P','P','P','P','P','P','P','P'},
			{'R','N','B','Q','K','B','N','R'}
		};

		/*
			Contains the states of the squares:
			'-' if the square has nothing to do with anything
			'p' if the square is a possible move
			'k' if the square is a king at check
			'c' if the square is a capture
			'm' if the square contains the current moving piece
		*/
		char squareState[8][8] = {
			{'-','-','-','-','-','-','-','-'},
			{'-','-','-','-','-','-','-','-'},
			{'-','-','-','-','-','-','-','-'},
			{'-','-','-','-','-','-','-','-'},
			{'-','-','-','-','-','-','-','-'},
			{'-','-','-','-','-','-','-','-'},
			{'-','-','-','-','-','-','-','-'},
			{'-','-','-','-','-','-','-','-'}
		};

		//contains the current, selected piece
		int savedPiece[2];

		//Contains the buttons that represent each square
		LButton allButtons[8][8];

		//Contains the letters texture(0), numbers texture(1), and pieces_texture(2)
		SDL_Texture* boardComponents[3];

		//Are you moving any pieces now?
		bool moving;

		//Whose Turn is it
		bool white_turn;

		const SDL_Rect pieces[12]=
			{
				{0,0,50,55},
				{105,0,65,60},
				{220,0,55,60},
				{325,0,65,60},
				{440,0,60,60},
				{560,0,60,60},
			
				{0,95,50,55},
				{105,95,65,60},
				{220,95,65,60},
				{325,95,65,60},
				{440,95,65,60},
				{560,95,60,60}
			
			};

		const int startingPoint[2] = {100,20};
		const std::string piece_names = "kqrbnpKQRBNP";

		int moving_piece[2] = {0};

		bool castling[2][2] = {{true,true},{true,true}}; 
		// castling[0] is white and castling[1] is black
		// castling[x][0] is right and castling[x][1] is left
		// castling[x][y] contains whether player x can still castle to the y of his king

};

#endif