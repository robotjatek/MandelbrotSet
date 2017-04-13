#include <iostream>
#include <complex>
#include <cstdint>
#include <vector>

#include <SDL.h>

#ifdef _WIN32
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#endif

#define SCREEN_SIZE_X 1600
#define SCRREN_SIZE_Y 900
/*struct Color
{
uint8_t r, g, b;
};

std::vector<Color> colors;*/
uint8_t* screen = new uint8_t[SCREEN_SIZE_X * 3 * SCRREN_SIZE_Y];

SDL_Window* wnd;
SDL_Renderer* renderer;
SDL_Texture* screentex;
SDL_Event s_event;

bool InsideMandelbrot(const std::complex<double>& Z)
{
	return std::abs(Z) < 2 ? true : false;
}

std::complex<double> CalculateComplex(const std::complex<double>& C, unsigned int Iteration)
{
	std::complex<double> ret(0, 0);
	for (unsigned int i = 0; i < Iteration; i++)
	{
		ret = ret*ret + C;
	}
	return ret;
}

uint8_t NumOfIterations(const std::complex<double>& num, unsigned int MaxIterations)
{
	unsigned int i = 1;
	while (InsideMandelbrot(CalculateComplex(num, i++)) && i < MaxIterations);
	return i - 1;
}

void InitScreen(unsigned int Iterations)
{
	const double stepx = 4.0 / SCREEN_SIZE_X;
	const double stepy = 4.0 / SCRREN_SIZE_Y;
	static int callnumber = 0;

	for (int i = 0; i < SCREEN_SIZE_X; i++)
	{
		for (int j = 0; j < SCRREN_SIZE_Y; j++)
		{
			uint8_t nit = NumOfIterations(std::complex<double>(stepx*i - 2, stepy*j - 2), Iterations);
			/*if (colors.size() < nit)
			{
			Color c;
			c.r =callnumber+=5;
			c.g = rand() % 256;
			c.b = rand() % 256;
			colors.push_back(c);
			}*/
			screen[i * 3 + j * SCREEN_SIZE_X * 3] = nit * 10;
			screen[i * 3 + j * SCREEN_SIZE_X * 3 + 1] = 0;
			screen[i * 3 + j * SCREEN_SIZE_X * 3 + 2] = 0;
		}
	}
}

void HandleKeys(SDL_Event& ev, bool& exit)
{
	if (ev.type == SDL_KEYDOWN)
	{
		if (ev.key.keysym.sym == SDLK_ESCAPE)
		{
			exit = true;
		}
	}
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	wnd = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE_X, SCRREN_SIZE_Y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	screentex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, SCREEN_SIZE_X, SCRREN_SIZE_Y);

	bool exit = false;
	SDL_Event ev;
	while (!exit)
	{
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			{
				exit = true;
			}

			HandleKeys(ev, exit);
		}
		static unsigned int i = 1;
		//std::cout << i << "\n";
		if (i > 28)
		{
			i = 28;
		}
		InitScreen(i++);
		SDL_UpdateTexture(screentex, nullptr, screen, SCREEN_SIZE_X * 3);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, screentex, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(screentex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(wnd);
	SDL_Quit();
	delete[] screen;
	return 0;
}