#include <stdio.h>
#include <SDL2/SDL.h>
#include <unistd.h>
#include <string.h>

// gcc game_of_life.c -lSDL2

#define WINDOW_HEIGHT 600 // 0 <= y < 600
#define WINDOW_WIDTH 800  // 0 <= x < 800

#define ALIVE 1
#define DEAD 0

int mod(int num, int mod) {
	int ret = num + mod;
	return ret % mod;
}

int calc_neighb(int y, int x) {
	return mod(y, WINDOW_HEIGHT) * WINDOW_WIDTH + mod(x, WINDOW_WIDTH);
}

void calc_frame(int* prev, int* next) {

	for (int x = 0; x < WINDOW_WIDTH; x++) {
		for (int y = 0; y < WINDOW_HEIGHT; y++) {

			int near_alive = 0;
			if (prev[calc_neighb(y - 1, x - 1)] == ALIVE) near_alive++;
			if (prev[calc_neighb(y - 1, x    )] == ALIVE) near_alive++;
			if (prev[calc_neighb(y - 1, x + 1)] == ALIVE) near_alive++;
			if (prev[calc_neighb(y    , x - 1)] == ALIVE) near_alive++;
			if (prev[calc_neighb(y    , x + 1)] == ALIVE) near_alive++;
			if (prev[calc_neighb(y + 1, x - 1)] == ALIVE) near_alive++;
			if (prev[calc_neighb(y + 1, x    )] == ALIVE) near_alive++;
			if (prev[calc_neighb(y + 1, x + 1)] == ALIVE) near_alive++;

			if (prev[y * WINDOW_WIDTH + x] == ALIVE) {

				if (near_alive > 3 || near_alive < 2) {
					next[y * WINDOW_WIDTH + x] = DEAD;
				} else {
					next[y * WINDOW_WIDTH + x] = ALIVE;
				}
			} else {
				if (near_alive > 2) {
					next[y * WINDOW_WIDTH + x] = ALIVE;
				} else {
					next[y * WINDOW_WIDTH + x] = DEAD;
				}
			}
		}
	}
}

int init_game(int* first_frame) {

	for (int i = WINDOW_WIDTH * 3; i < WINDOW_WIDTH * 7; i++) {

		if (i % 4 == 0) i += 2;
		first_frame[i] = ALIVE;
	}
	first_frame[WINDOW_HEIGHT / 2 * WINDOW_WIDTH + WINDOW_WIDTH / 2] = ALIVE;
	first_frame[(WINDOW_HEIGHT / 2 + 1) * WINDOW_WIDTH + WINDOW_WIDTH / 2 + 1] = ALIVE;
	first_frame[(WINDOW_HEIGHT / 2 + 2) * WINDOW_WIDTH + WINDOW_WIDTH / 2 - 1] = ALIVE;
	first_frame[(WINDOW_HEIGHT / 2 + 2) * WINDOW_WIDTH + WINDOW_WIDTH / 2] = ALIVE;
	first_frame[(WINDOW_HEIGHT / 2 + 2) * WINDOW_WIDTH + WINDOW_WIDTH / 2 + 1] = ALIVE;
}

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("Initialization failed\n");
		return 1;
    }

    SDL_Window* window = SDL_CreateWindow("The Game of Life [with feature]",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
            WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* rend = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	int prev_frame[WINDOW_HEIGHT * WINDOW_WIDTH] = {0};
	int next_frame[WINDOW_HEIGHT * WINDOW_WIDTH] = {0};

	init_game(next_frame);

    int quit = 0;
    SDL_Event event;

    while (!quit) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        for (int i = 0; i < WINDOW_HEIGHT * WINDOW_WIDTH; i++) {
        	prev_frame[i] = next_frame[i];
        }
        calc_frame(prev_frame, next_frame);

        SDL_RenderClear(rend);
        SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0x00, 0xFF);

		for (int x = 0; x < WINDOW_WIDTH; x++) {
			for (int y = 0; y < WINDOW_HEIGHT; y++) {
				if (prev_frame[y * WINDOW_WIDTH + x] == ALIVE)
					SDL_RenderDrawPoint(rend, x, y);
			}
		}
        SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderPresent(rend);
        usleep(10 * 1000);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(rend);
    SDL_Quit();
}