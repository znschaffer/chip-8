#include "SDL2/SDL.h"
#include "core.h"
SDL_Window *window;
SDL_Renderer *render;

void draw_screen(struct Core *state)
{
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);

	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	bool *screen = get_screen(state);
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		bool pixel = screen[i];
		if (pixel) {
			uint8_t x = (i % SCREEN_WIDTH);
			uint8_t y = (i / SCREEN_WIDTH);

			SDL_Rect *rect = &(SDL_Rect){ .x = x * SCALE,
						      .y = y * SCALE,
						      .w = SCALE,
						      .h = SCALE };
			SDL_RenderFillRect(render, rect);
		}
	}
	SDL_RenderPresent(render);
}

int main(int argc, char *args[])
{
	(void)argc;
	FILE *file = fopen(args[1], "rb");
	if (file == NULL) {
		printf("error: Couldn't open %s\n", args[1]);
		exit(1);
	}

	struct Core *state = new_core();
	load_rom(state, file);

	// setup our window
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL couldn't be initialized");
	} else {
		window = SDL_CreateWindow("chip-8", SDL_WINDOWPOS_UNDEFINED,
					  SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
					  WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("window couldn't be initialized");
		} else {
			render = SDL_CreateRenderer(window, -1,
						    SDL_RENDERER_ACCELERATED);
		}
	}

	// start the game loop
	while (true) {
		// Get the next event
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				// Break out of the loop on quit
				break;
			}
		}
		tick(state);
		draw_screen(state);
	}

	free(state);
	return 0;
}
