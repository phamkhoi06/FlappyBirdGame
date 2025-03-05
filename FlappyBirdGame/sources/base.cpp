#include "../headers/base.h"
#include <iostream>


SDL_Window* BaseTexture::gWindow = NULL;
SDL_Renderer* BaseTexture::gRenderer = NULL;
SDL_Event BaseTexture::event;
bool BaseTexture::quit = false;

BaseTexture::BaseTexture()
{
	Texture = NULL;
	tWidth = 0;
	tHeight = 0;
}

void BaseTexture::free()
{
	if (Texture != NULL)
	{
		SDL_DestroyTexture(Texture);
		Texture = NULL;
		tWidth = 0;
		tHeight = 0;
	}
}



bool BaseTexture::initGraphic()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		gWindow = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);


				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

void BaseTexture::releaseGraphic()
{
	if (gRenderer) {
		SDL_DestroyRenderer(gRenderer);
		gRenderer = NULL;
	}
	if (gWindow) {
		SDL_DestroyWindow(gWindow);
		gWindow = NULL;
	}

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}


bool BaseTexture::Load(string path, double scale) { return false; }
void BaseTexture::Render(int x, int y, int angle, SDL_Rect* clip, SDL_RendererFlip flip) {}
int BaseTexture::getWidth() { return tWidth; }
int BaseTexture::getHeight() { return tHeight; }
bool BaseTexture::isNULL() { return Texture == NULL; }


void position::getPos(const int x, const int y) { this->x = x; this->y = y; }