#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 800
#define FPS 60
#define FRAME_TARGET_TIME (1.0 / FPS)
#define SNAKE_SIZE 800 * 0.05

typedef struct {
	struct snake_node *next;
	SDL_FRect body;
} snake_node;

int initialize(SDL_Window** w, SDL_Renderer** r) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		printf("Unable to initialize SDL");
		return 1;
	}

	 *w = SDL_CreateWindow(
		"SDL2 Test",
		WIDTH,
		HEIGHT, SDL_WINDOW_BORDERLESS);

	if (!*w) {
		SDL_Log("Unable to create a window: %s", SDL_GetError());
		return 1;
	}


	*r = SDL_CreateRenderer(*w, NULL);
	if(!*r) {
		SDL_Log("Unable to create a renderer: %s", SDL_GetError());
		return 1;
	}

	return 0;
}

void handle_key_press(SDL_Event *e, int *exit, snake_node* snake) {
	float n;
	float ns = SNAKE_SIZE * 0.5;
	float h = HEIGHT - SNAKE_SIZE;
	float w = WIDTH - SNAKE_SIZE;

	switch(e->key.key) {
	case SDLK_ESCAPE:
		*exit = 1;
		break;
	case SDLK_W:
		n = snake->body.y - ns;
		if(n < 0) 
			snake->body.y = 0;
		else 
			snake->body.y = n;
		break;
	case SDLK_D:
		n = snake->body.x + ns;
		if(n > w)
			snake->body.x = w;
		else
			snake->body.x = n;
		break;
	case SDLK_A:
		n = snake->body.x - ns;
		if(n < 0 + SNAKE_SIZE)
			snake->body.x = 0;
		else 
			snake->body.x = n;
		break;
	case SDLK_S:
		n = snake->body.y + ns;
		if(n > h) 
			snake->body.y = h;
		else 
			snake->body.y = n;
		break;
	}
}

void handle_fps(Uint64 *fs, int *fc) {
	Uint64 fq = SDL_GetPerformanceFrequency();
	Uint64 fe = SDL_GetPerformanceCounter();
	double ft = (double)(fe - *fs) / fq;
	double dt = FRAME_TARGET_TIME - ft;

	if(dt > 0)
		SDL_Delay((Uint32) (dt * 1000.0));


	if(*fc >= 60)
		*fc = 0;
	else 
		*fc += 1;
}


void handle_drawing(SDL_Renderer *r, snake_node *snake) {
	SDL_SetRenderDrawColor(r, 40, 40, 40, 255);
	SDL_RenderClear(r);

	SDL_SetRenderDrawColor(r, 184, 187, 38, 255);
	SDL_RenderFillRect(r, &snake->body);
	SDL_RenderPresent(r);
}

void game_loop(int *exit, SDL_Renderer *r) {
	int fc;
	Uint64 fs, fe;
	SDL_Event event;
	snake_node snake = {
		.next = NULL,
		.body = { .x = 10.0f, .y = 10.0f, .h = SNAKE_SIZE, .w = SNAKE_SIZE }
	};

	while(*exit != 1) {
		fs = SDL_GetPerformanceCounter();

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_EVENT_QUIT:
				return;
			case SDL_EVENT_KEY_DOWN:
				handle_key_press(&event, exit, &snake);
				break;
			}
		}

		handle_drawing(r, &snake);
		handle_fps(&fs, &fc);
	}
}

int main() {
	int exit = 0;
	SDL_Window *window;
	SDL_Renderer *render;

	initialize(&window, &render);
	game_loop(&exit, render);

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

