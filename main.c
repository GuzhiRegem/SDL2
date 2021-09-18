#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;
int init(SDL_Instance *instance);
typedef struct point3D
{
	double x;
	double y;
	double z;
} p3D;
typedef struct point2D
{
	double x;
	double y;
} p2D;
typedef struct line
{
	int idx1;
	int idx2;
} line;
typedef struct state_s
{
	int r_key;
	int l_key;
	double d_ang;
	double angle;
} state_t;

p3D dots[] = {
{0,0,0},{50,0,0},{100,0,0},{150,0,0},{200,0,0},{250,0,0},{300,0,0},{350,0,0},
{0,50,0},{50,50,0},{100,50,0},{150,50,0},{200,50,0},{250,50,0},{300,50,0},{350,50,0},
{0,100,0},{50,100,0},{100,100,0},{150,100,0},{200,100,0},{250,100,0},{300,100,0},{350,100,0},
{0,150,0},{50,150,0},{100,150,0},{150,150,0},{200,150,0},{250,150,0},{300,150,0},{350,150,0},
{0,200,0},{50,200,0},{100,200,0},{150,200,0},{200,200,0},{250,200,0},{300,200,0},{350,200,0},
{0,250,0},{50,250,0},{100,250,0},{150,250,0},{200,250,0},{250,250,0},{300,250,0},{350,250,0},
{0,300,0},{50,300,0},{100,300,0},{150,300,0},{200,300,0},{250,300,0},{300,300,0},{350,300,0},
{0,350,0},{50,350,0},{100,350,0},{150,350,0},{200,350,0},{250,350,0},{300,350,0},{350,350,0}
};
line lines[] = {
{0,8},{0,1},{1,9},{1,2},{2,10},{2,3},{3,11},{3,4},{4,12},{4,5},{5,13},{5,6},{6,14},{6,7},{7,15},
{8,16},{8,9},{9,17},{9,10},{10,18},{10,11},{11,19},{11,12},{12,20},{12,13},{13,21},{13,14},{14,22},{14,15},{15,23},
{16,24},{16,17},{17,25},{17,18},{18,26},{18,19},{19,27},{19,20},{20,28},{20,21},{21,29},{21,22},{22,30},{22,23},{23,31},
{24,32},{24,25},{25,33},{25,26},{26,34},{26,27},{27,35},{27,28},{28,36},{28,29},{29,37},{29,30},{30,38},{30,31},{31,39},
{32,40},{32,33},{33,41},{33,34},{34,42},{34,35},{35,43},{35,36},{36,44},{36,37},{37,45},{37,38},{38,46},{38,39},{39,47},
{40,48},{40,41},{41,49},{41,42},{42,50},{42,43},{43,51},{43,44},{44,52},{44,45},{45,53},{45,46},{46,54},{46,47},{47,55},
{48,56},{48,49},{49,57},{49,50},{50,58},{50,51},{51,59},{51,52},{52,60},{52,53},{53,61},{53,54},{54,62},{54,55},{55,63},
{56,57},{57,58},{58,59},{59,60},{60,61},{61,62},{62,63}
	};

void draw(SDL_Instance instance, double time, state_t state)
{
	int i;
	p3D pnt1 = {0, 0, 0};
	p3D pnt2 = {0, 0, 0};
	p2D dot1 = {0, 0};
	p2D dot2 = {0, 0};
	double inclination = 0.7;
	double a = (state.angle + sin(time)) * M_PI / 180;
	double chang = (sin(time) + 1) / 2;
	int offs[] = {350 + sin(time*0.4)*5, 250 + cos(time*0.6)*5}, num = 0;

	SDL_SetRenderDrawColor(instance.renderer, 255, 255, 255, 255);
	for (i = 0; i < 201; i++)
	{
		pnt1.x = dots[lines[i].idx1].x;
		pnt1.y = dots[lines[i].idx1].y;
		pnt1.z = dots[lines[i].idx1].z * chang;
		pnt2.x = dots[lines[i].idx2].x;
		pnt2.y = dots[lines[i].idx2].y;
		pnt2.z = dots[lines[i].idx2].z * chang;

		dot1.x = (inclination*pnt1.x)-(inclination*pnt1.y);
		dot1.y = (1 - inclination) * pnt1.x + (1 - inclination) * pnt1.y - pnt1.z;
		dot2.x = (inclination*pnt2.x)-(inclination*pnt2.y); 
		dot2.y = (1 - inclination) * pnt2.x + (1 - inclination) * pnt2.y - pnt2.z;

		SDL_RenderDrawLine(instance.renderer, 
		(dot1.x * cos(a) - dot1.y * sin(a)) + offs[0], 
		(dot1.x * sin(a) + dot1.y * cos(a)) + offs[1], 
		(dot2.x * cos(a) - dot2.y * sin(a)) + offs[0], 
		(dot2.x * sin(a) + dot2.y * cos(a)) + offs[1]
		);
	}
}
int main(int argc, char**argv)
{
	SDL_Instance instance;
	SDL_Event event;
	int quit = 0, fd, chars, idx = 0, sig = 1, i;
	char *buffer; 
	double time;
	state_t state = {0,0,0,0};

	if (argc != 2)
	{
		printf("syntax: ./terrain <filename>");
		return (1);
	}
	buffer = malloc(1024);
	if (!buffer)
		return (1);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		printf("error reading file\n");
		return (1);
	}
	chars = read(fd, buffer, 1024);
	for (i = 0; i < chars; i++)
	{
		if (buffer[i] <= '9' && buffer[i] >=  '0')
			dots[idx].z = (dots[idx].z * 10) + (buffer[i] - '0') * sig * 0.5;
		else if (buffer[i] == ' ' || buffer[i] == '\n')
		{
			idx++;
			sig = 1;
		}
		else if (buffer[i] == '-')
			sig = -1;
	}
	if (idx != 64)
	{
		printf("wrong file syntax\n");
		return (1);
	}
	if (init(&instance) != 0)
		return (1);
	while (!quit)
	{
		time = (double)SDL_GetTicks() / 1000;
		SDL_SetRenderDrawColor(instance.renderer, 
		30 + (30 * sin((time + 2) * 1.2)), 
		30 + (30 * sin((time + 5) * 0.6)), 
		30 + (30 * sin((time + 9) * 1.9)), 
		255);
		SDL_RenderClear(instance.renderer);
		
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				quit = 1;
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_RIGHT)
					state.r_key = 1;
				else if (event.key.keysym.sym == SDLK_LEFT)
					state.l_key = 1;
			}
			else if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_RIGHT)
					state.r_key = 0;
				else if (event.key.keysym.sym == SDLK_LEFT)
					state.l_key = 0;
			}
		}
		state.d_ang += state.r_key - state.l_key;
		state.angle += (double)(state.d_ang - state.angle) / 25;
		printf("left:%d right:%d\n", state.l_key, state.r_key);	
		draw(instance, time, state);
		
		SDL_RenderPresent(instance.renderer);
	}
	SDL_DestroyRenderer(instance.renderer);
	SDL_DestroyWindow(instance.window);
	SDL_Quit;
	return (0);	
}





/* Init function */


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
