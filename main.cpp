#include <vector>
#include <cstdlib>
#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

using namespace std;

static SDL_Surface* loadImageInDisplayFormat(const char *file);

static vector<vector<int> > view;

static int reels[5][63] = {
	{9,7,12,5,7,3,4,11,9,7,12,6,7,4,11,10,4,3,11,12,7,5,10,9,7,5,9,10,8,9,7,4,9,10,11,5,10,3,9,10,3,9,4,8,7,5,11,9,12,6,3,5,7,9,11,10,6,7,3,5,10,8,4,},
	{11,5,4,7,6,8,4,9,7,8,3,11,6,5,11,7,12,5,8,6,10,9,5,6,8,7,12,11,5,6,10,3,4,5,9,12,8,9,6,5,12,8,9,12,7,8,5,10,12,7,11,3,4,8,7,4,5,9,8,6,12,4,6,},
	{10,4,5,8,6,7,5,9,6,7,8,4,6,5,11,3,9,8,7,11,12,6,8,5,4,8,6,12,9,6,5,11,3,7,4,8,7,3,10,9,5,6,4,3,9,12,10,8,9,6,3,9,10,6,7,5,6,8,4,11,9,7,8,},
	{9,3,6,5,3,10,6,9,5,12,4,8,11,10,7,6,5,9,7,3,8,6,12,4,5,7,3,12,10,6,9,7,8,5,6,4,9,6,11,5,8,12,6,7,3,6,10,3,7,5,10,8,9,6,12,4,7,9,5,6,8,3,10,},
	{5,3,9,4,6,12,10,5,11,4,8,7,10,5,9,11,4,6,7,3,6,4,8,5,11,8,5,10,8,11,5,10,8,3,7,4,10,11,5,7,8,9,5,11,6,8,10,3,7,9,3,8,10,12,6,8,10,11,7,10,8,11,6,},
};

static SDL_Surface *canvas = NULL;
static const SDL_Surface *backgroundSurface = loadImageInDisplayFormat("./Background.png");
static SDL_Rect backgroundCoordinates = { 0, 0, 0, 0 };
static const SDL_Surface *symbolsSurface[] = {
	NULL,
	NULL,
	NULL, 
	loadImageInDisplayFormat("./Symbol03.png"),
	loadImageInDisplayFormat("./Symbol04.png"),
	loadImageInDisplayFormat("./Symbol05.png"),
	loadImageInDisplayFormat("./Symbol06.png"),
	loadImageInDisplayFormat("./Symbol07.png"),
	loadImageInDisplayFormat("./Symbol08.png"),
	loadImageInDisplayFormat("./Symbol09.png"),
	loadImageInDisplayFormat("./Symbol10.png"),
	loadImageInDisplayFormat("./Symbol11.png"),
	loadImageInDisplayFormat("./Symbol12.png"),
	NULL,
	NULL,
	NULL,
	NULL, 
};

static SDL_Rect symbolsCoordinates[5][3] = { 
	{ { 298, 118, 0, 0 }, { 298, 266, 0, 0 }, { 298, 414, 0, 0 } }, 
	{ { 474, 118, 0, 0 }, { 474, 266, 0, 0 }, { 474, 414, 0, 0 } }, 
	{ { 651, 118, 0, 0 }, {	651, 266, 0, 0 }, { 651, 414, 0, 0 } }, 
	{ { 827, 118, 0, 0 }, { 827, 266, 0, 0 }, { 827, 414, 0, 0 } }, 
	{ { 1003, 118,	0, 0 }, { 1003, 266, 0, 0 }, {1003, 414, 0, 0 } }, 
};

static SDL_Surface* loadImageInDisplayFormat(const char *file) {
	SDL_Surface *transformed = NULL;
	SDL_Surface *original = IMG_Load(file);
	return (original);
}

void draw() {
	SDL_BlitSurface((SDL_Surface*) backgroundSurface, NULL, canvas, &backgroundCoordinates);

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 5; i++) {
			SDL_BlitSurface((SDL_Surface*) symbolsSurface[view[i][j]], NULL, canvas, &symbolsCoordinates[i][j]);
		}
	}

	SDL_Flip(canvas);
}

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	view.resize(5);
	for (int i = 0; i < 5; i++) {
		view[i].resize(3);
		for (int j = 0; j < 3; j++) {
			view[i][j] = 0;
		}
	}

	canvas = SDL_SetVideoMode(1280, 1024, 32, SDL_FULLSCREEN | SDL_DOUBLEBUF);

	SDL_Event event;
	bool done = false;
	while (done == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				done = true;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {

				case SDLK_ESCAPE:
					done = true;
					break;

				case SDLK_RETURN:
					//startAnimation();
					for (int i = 0, r; i < view.size(); i++) {
						r = rand() % 63;
						for (int j = view[i].size() - 1; j >= 0; j--) {
							view[i][j] = reels[i][(r+j)%63];
						}
					}
					break;
				}
			}
		}

		draw();
	}

	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}

