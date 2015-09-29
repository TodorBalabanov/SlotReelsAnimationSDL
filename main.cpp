#include <ctime>
#include <vector>
#include <cstdlib>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

using namespace std;

static int last[5]={0, 0, 0, 0, 0};
static int stops[5];

static int reels[5][63] = {
	{9,7,12,5,7,3,4,11,9,7,12,6,7,4,11,10,4,3,11,12,7,5,10,9,7,5,9,10,8,9,7,4,9,10,11,5,10,3,9,10,3,9,4,8,7,5,11,9,12,6,3,5,7,9,11,10,6,7,3,5,10,8,4,},
	{11,5,4,7,6,8,4,9,7,8,3,11,6,5,11,7,12,5,8,6,10,9,5,6,8,7,12,11,5,6,10,3,4,5,9,12,8,9,6,5,12,8,9,12,7,8,5,10,12,7,11,3,4,8,7,4,5,9,8,6,12,4,6,},
	{10,4,5,8,6,7,5,9,6,7,8,4,6,5,11,3,9,8,7,11,12,6,8,5,4,8,6,12,9,6,5,11,3,7,4,8,7,3,10,9,5,6,4,3,9,12,10,8,9,6,3,9,10,6,7,5,6,8,4,11,9,7,8,},
	{9,3,6,5,3,10,6,9,5,12,4,8,11,10,7,6,5,9,7,3,8,6,12,4,5,7,3,12,10,6,9,7,8,5,6,4,9,6,11,5,8,12,6,7,3,6,10,3,7,5,10,8,9,6,12,4,7,9,5,6,8,3,10,},
	{5,3,9,4,6,12,10,5,11,4,8,7,10,5,9,11,4,6,7,3,6,4,8,5,11,8,5,10,8,11,5,10,8,3,7,4,10,11,5,7,8,9,5,11,6,8,10,3,7,9,3,8,10,12,6,8,10,11,7,10,8,11,6,},
};

static SDL_Window *window = NULL;
static SDL_Surface *canvas = NULL;
static const SDL_Surface *backgroundSurface = IMG_Load("./Background.png");
static SDL_Rect backgroundCoordinates = { 0, 0, 0, 0 };
static const SDL_Surface *symbolsSurface[] = {
	NULL,
	NULL,
	NULL,
	IMG_Load("./Symbol03.png"),
	IMG_Load("./Symbol04.png"),
	IMG_Load("./Symbol05.png"),
	IMG_Load("./Symbol06.png"),
	IMG_Load("./Symbol07.png"),
	IMG_Load("./Symbol08.png"),
	IMG_Load("./Symbol09.png"),
	IMG_Load("./Symbol10.png"),
	IMG_Load("./Symbol11.png"),
	IMG_Load("./Symbol12.png"),
	NULL,
	NULL,
	NULL,
	NULL,
};
static const SDL_Surface *reelsSurface[5] = {NULL, NULL, NULL, NULL, NULL};

static SDL_Rect symbolsCoordinates[5][3] = {
	{ { 298, 118, 0, 0 }, { 298, 266, 0, 0 }, { 298, 414, 0, 0 } },
	{ { 474, 118, 0, 0 }, { 474, 266, 0, 0 }, { 474, 414, 0, 0 } },
	{ { 651, 118, 0, 0 }, {	651, 266, 0, 0 }, { 651, 414, 0, 0 } },
	{ { 827, 118, 0, 0 }, { 827, 266, 0, 0 }, { 827, 414, 0, 0 } },
	{ { 1003, 118,	0, 0 }, { 1003, 266, 0, 0 }, {1003, 414, 0, 0 } },
};

static unsigned long animationStart = 0;
static unsigned long animationEnd = 0;
static bool stopped[5] = {false, false, false, false, false};
void draw() {
    static double y0 = 0;
    static double v0[5] = {-9.0, -9.2, -9.4, -9.6, -9.8};
    static long t = 0;
    static double a = 0.001;
    static int y = 0;

	SDL_BlitSurface((SDL_Surface*) backgroundSurface, NULL, canvas, &backgroundCoordinates);

    for (int i = 0; i < 5 && animationStart!=animationEnd; i++) {
        /*
         * y = y0 + v0*t + 1/2*at^2
         */
        y0 = last[i] * 140;
        t = (1000 * clock() / CLOCKS_PER_SEC) - animationStart;

        y = (int)(y0 + v0[i]*t + a*t*t/2) % (63*140);
        if(y < 0) {
            y += 63*140;
        }

        /*
         * Stop near to the target position.
         */
        if(i==0 && abs(y-stops[i]*140)<=140) {
            last[i] = stops[i];
            stopped[i] = true;
        }else if(stopped[i-1] == true && stopped[i] == false && abs(y-stops[i]*140)<=70) {
            last[i] = stops[i];
            stopped[i] = true;
        }

        if(stopped[i] == true) {
            SDL_SetSurfaceAlphaMod((SDL_Surface*) reelsSurface[i], 255);
            y = stops[i] * 140;
        } else {
            SDL_SetSurfaceAlphaMod((SDL_Surface*) reelsSurface[i], 191);
        }

        const SDL_Rect frame = {0, y, 140, 3*140};
        SDL_BlitSurface((SDL_Surface*) reelsSurface[i], &frame, canvas, &symbolsCoordinates[i][0]);
    }

    SDL_UpdateWindowSurface(window);
}

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);


    /*
     * Build long strips (two more images at the end).
     */
    for(int i=0, index; i<5; i++) {
        reelsSurface[i] = IMG_Load("./Reel.png");

        for(int j=0; j<(63+2); j++) {
            index = reels[i][j%63];

            SDL_Rect coordinates = {0, 140*j, 0, 0};
            SDL_BlitSurface((SDL_Surface*) symbolsSurface[index], NULL, (SDL_Surface*)reelsSurface[i], &coordinates);
        }
    }

    //window = SDL_CreateWindow("Slot Reels Animation", 0, 0, 1280, 1024, SDL_WINDOW_FULLSCREEN_DESKTOP);
    window = SDL_CreateWindow("Slot Reels Animation", 0, 0, 1440, 900, 0);
    canvas = SDL_GetWindowSurface(window);

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
					memset(stopped, false, 5*sizeof(bool));
                     animationStart = 1000 * clock() / CLOCKS_PER_SEC;

					for (int i = 0, r; i < 5; i++) {
						stops[i] = rand() % 63;
					}
					break;
				}
			}
		}

		draw();
	}


    SDL_DestroyWindow(window);

    for(int i=0; i<5; i++) {
        SDL_FreeSurface((SDL_Surface*)reelsSurface[i]);
    }

	SDL_Quit();

	return EXIT_SUCCESS;
}
