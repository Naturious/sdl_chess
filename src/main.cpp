#include "ChessGame.h"
#include "graphic.h"

int main(int argc, char** args)
{
	bool quit = false;

	SDL_Event e;

	Graphic g;

	ChessGame ch(&g);

	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT)
			{
				quit = true;
			}
			if(!ch.ended())
			{
				ch.handleEvent(&e);
			}
		}
		ch.render();
	}

	return 0;

}