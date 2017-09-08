#include "graphic.h"


Graphic::Graphic()
{
	/*
		Constructor
		Initialises the window and renderer variables
		Sets the renderer color to white
	*/
	window = SDL_CreateWindow("Chess game",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
}

Graphic::~Graphic()
{
	/*
		Destructor
		Deallocates memory for the internal variables
		And Closes window
	*/
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	SDL_Quit();
}

SDL_Renderer* Graphic::get_renderer()
{
	/*
		Returns a pointer to the renderer
	*/
	return renderer;
}

SDL_Texture* Graphic::loadTexture(std::string path, bool transparency)
{
	/*
		Loads a texture from the specified path and returns a pointer to the texture
		If Transparency is true, it will ignore white
	*/
	//Variable where the texture will be saved
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if(transparency)
	{
		Uint32 colorkey = SDL_MapRGB(loadedSurface->format,255,255,255);
		SDL_SetColorKey(loadedSurface, SDL_TRUE,colorkey);
	}

	newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

	SDL_FreeSurface(loadedSurface);

	return newTexture;
}