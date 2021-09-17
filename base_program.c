#include <SDL2/SDL.h>
typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;
int init(SDL_Instance *instance)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return (1);
	instance->window = SDL_CreateWindow("testeando", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 700, 0);
	if (!instance->window)
	{
		SDL_Quit();
		return (1);
	}
	instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!instance->renderer)
	{
		SDL_DestroyWindow(instance->window);
		SDL_Quit();
		return (1);
	}
	return (0);
}
void draw(SDL_Instance instance)
{
	/* draw here */
}
int main(void)
{
	SDL_Instance instance;
	SDL_Event event;
	int quit = 0;
	if (init(&instance) != 0)
		return (1);
	while (!quit)
	{
		SDL_SetRenderDrawColor(instance.renderer, 255, 255, 255, 255);
		SDL_RenderClear(instance.renderer);
		
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				quit = 1;
		}
		
		draw(instance);
		
		SDL_RenderPresent(instance.renderer);
	}
	SDL_DestroyRenderer(instance.renderer);
	SDL_DestroyWindow(instance.window);
	SDL_Quit;
	return (0);	
}
