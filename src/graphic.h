#include <SDL.h>
#include <SDL_image.h>
#include <string>

#ifndef _GRAPHIC_H_DEFINED
#define _GRAPHIC_H_DEFINED

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT= 720;

class Graphic
{
	public:

		//Constructor
		Graphic();

		//Destructor
		~Graphic();

		//Returns a pointer to the renderer
		SDL_Renderer* get_renderer();

		//Loads a texture and returns a pointer to the texture
		SDL_Texture* loadTexture(std::string path,bool transparency = false);


	private:

		//Window
		SDL_Window* window;

		//Renderer
		SDL_Renderer* renderer;


};

#endif
