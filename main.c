#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 1400
#define FLIPS 24

double map(int value, double oldMin, double oldMax, double newMin, double newMax)
{
    return newMin + (newMax - newMin) * ((value - oldMin) / (oldMax - oldMin));
}

/**

Algoritmo Mandelbrot

for each pixel (Px, Py) on the screen do
    x0 := scaled x coordinate of pixel (scaled to lie in the Mandelbrot X scale (-2.00, 0.47))
    y0 := scaled y coordinate of pixel (scaled to lie in the Mandelbrot Y scale (-1.12, 1.12))
    x := 0.0
    y := 0.0
    iteration := 0
    max_iteration := 1000
    while (x*x + y*y ≤ 2*2 AND iteration < max_iteration) do
        xtemp := x*x - y*y + x0
        y := 2*x*y + y0
        x := xtemp
        iteration := iteration + 1

    color := palette[iteration]
    plot(Px, Py, color)
**/
void drawMandelbrot(SDL_Window *window, SDL_Surface *surface, SDL_Renderer *renderer)
{
    int max_iterations = 1000;

    double xscale, yscale;

    // Para a escala do conjunto. Esses valores ficaram bons para a tela
    xscale = 3.5 / WINDOW_WIDTH;
    yscale = 1.5 / WINDOW_HEIGHT;

    // Play arround with these values
    double manMax = 2.5;  // max mandelbrot scale
    double manMin = -2.5; // min mandelbrot scale
    double navX = 0.0;   // x axis navigation
    double navY = 0.0;    // y axis navigation
    double zoom = 1.0;

    for (int Px = 0; Px < WINDOW_WIDTH; Px++)
    {
        for (int Py = 0; Py < WINDOW_HEIGHT; Py++)
        {

            double cx = (map(Px, 0, WINDOW_WIDTH, manMin, manMax) / zoom) + navX;
            double cy = (map(Py, 0, WINDOW_HEIGHT, manMin, manMax) / zoom) + navY;

            int n = 0;
            double x = 0.0, y = 0.0;

            while (n < max_iterations)
            {
                double xx = x * x - y * y; // real component
                double yy = 2.0 * x * y;   // imaginary component
                x = xx + cx;
                y = yy + cy;

                if (x * x + y * y > 4)
                {
                    break;
                }

                n++;
            }

            int r, g, b;

            int brightness = 0;
            if (n == max_iterations)
            {
                brightness = 255;
            }

            SDL_Rect rect;
            rect = (SDL_Rect){Px, Py, 1, 1};

            SDL_SetRenderDrawColor(renderer, (Uint8)brightness, (Uint8)brightness, (Uint8)brightness, 255);

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void main()
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    SDL_Window *window = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                                          WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window)
    {
        printf("Failed to create window\n");
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    SDL_Event event;
    bool quit = false;

    drawMandelbrot(window, surface, renderer);
    SDL_RenderPresent(renderer);
    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}