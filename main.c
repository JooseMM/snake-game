#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <stdio.h>

#define WIDTH 600
#define HEIGHT 400
#define FPS 60
#define FRAME_TARGET_TIME (1.0 / FPS)

int initialize(SDL_Window* window) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		printf("Unable to initialize SDL");
		return 1;
	}

	 window = SDL_CreateWindow(
		"SDL2 Test",
		WIDTH,
		HEIGHT, SDL_WINDOW_BORDERLESS);

	if (!window)
		return 1;

	return 0;
}

void handle_key_press(SDL_Event *e, int *exit) {
	switch(e->key.key) {
	case SDLK_ESCAPE:
		*exit = 1;
		break;
	case SDLK_UP:
		SDL_Log("UP");
		break;
	case SDLK_RIGHT:
		SDL_Log("RIGHT");
		break;
	case SDLK_LEFT:
		SDL_Log("LEFT");
		break;
	case SDLK_DOWN:
		SDL_Log("DOWN");
		break;
	}
}

int fps_counter() {

}

void game_loop(int *exit) {
	SDL_Event event;

	Uint64 fq = SDL_GetPerformanceFrequency();
	Uint64 fs, fe;
	double ft, dt;
	int fc;

	while(*exit != 1) {
		SDL_Log("%d", fc);

		fs = SDL_GetPerformanceCounter();

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_EVENT_QUIT:
				return;
			case SDL_EVENT_KEY_DOWN:
				handle_key_press(&event, exit);
				break;
			}
		}

		fe = SDL_GetPerformanceCounter();
		ft = (double)(fe - fs) / fq;

		dt = FRAME_TARGET_TIME - ft;

		if(dt > 0)
			SDL_Delay((Uint32) (dt * 1000.0));


		if(fc >= 60)
			fc = 0;
		else 
			fc++;
	}
}

typedef struct {
	struct snake_node *next;
	int x;
	int y;
	
} snake_node;

int main() {
	int exit = 0;
	SDL_Window *window;

	initialize(window);
	game_loop(&exit);

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

