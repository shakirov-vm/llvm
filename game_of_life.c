#include "sim.h"
// clang game_of_life.c sim.c -lSDL2 -I"/usr/include/SDL2/"
// clang -Xclang -load -Xclang ./libPass.so game_of_life.c log.c sim.c -lSDL2 -I"/usr/include/SDL2/"

#define ALIVE 1
#define DEAD 0

int mod(int num, int mod) {
	int ret = num + mod;
	return ret % mod;
}

int calc_neighb(int y, int x) {
	return mod(y, SIM_Y_SIZE) * SIM_X_SIZE + mod(x, SIM_X_SIZE);
}

void calc_frame(int* prev, int* next) {

	for (int x = 0; x < SIM_X_SIZE; x++) {
		for (int y = 0; y < SIM_Y_SIZE; y++) {

			int near_alive = 0;
			if (prev[calc_neighb(y - 1, x - 1)] == ALIVE) near_alive++;
			if (prev[calc_neighb(y - 1, x    )] == ALIVE) near_alive++;
			if (prev[calc_neighb(y - 1, x + 1)] == ALIVE) near_alive++;
			if (prev[calc_neighb(y    , x - 1)] == ALIVE) near_alive++;
			if (prev[calc_neighb(y    , x + 1)] == ALIVE) near_alive++;
			if (prev[calc_neighb(y + 1, x - 1)] == ALIVE) near_alive++;
			if (prev[calc_neighb(y + 1, x    )] == ALIVE) near_alive++;
			if (prev[calc_neighb(y + 1, x + 1)] == ALIVE) near_alive++;

			if (prev[y * SIM_X_SIZE + x] == ALIVE) {

				if (near_alive > 3 || near_alive < 2) {
					next[y * SIM_X_SIZE + x] = DEAD;
				} else {
					next[y * SIM_X_SIZE + x] = ALIVE;
				}
			} else {
				if (near_alive > 2) {
					next[y * SIM_X_SIZE + x] = ALIVE;
				} else {
					next[y * SIM_X_SIZE + x] = DEAD;
				}
			}
		}
	}
}

void init_game(int* first_frame) {

	for (int i = SIM_X_SIZE * 3; i < SIM_X_SIZE * 7; i++) {

		if (i % 4 == 0) i += 2;
		first_frame[i] = ALIVE;
	}
	first_frame[SIM_Y_SIZE / 2 * SIM_X_SIZE + SIM_X_SIZE / 2] = ALIVE;
	first_frame[(SIM_Y_SIZE / 2 + 1) * SIM_X_SIZE + SIM_X_SIZE / 2 + 1] = ALIVE;
	first_frame[(SIM_Y_SIZE / 2 + 2) * SIM_X_SIZE + SIM_X_SIZE / 2 - 1] = ALIVE;
	first_frame[(SIM_Y_SIZE / 2 + 2) * SIM_X_SIZE + SIM_X_SIZE / 2] = ALIVE;
	first_frame[(SIM_Y_SIZE / 2 + 2) * SIM_X_SIZE + SIM_X_SIZE / 2 + 1] = ALIVE;
}

void app() {

	int prev_frame[SIM_Y_SIZE * SIM_X_SIZE] = {0};
	int next_frame[SIM_Y_SIZE * SIM_X_SIZE] = {0};

	init_game(next_frame);

	for(int i = 0; i < 1000; i++) {

        for (int i = 0; i < SIM_Y_SIZE * SIM_X_SIZE; i++)
        	prev_frame[i] = next_frame[i];

        calc_frame(prev_frame, next_frame);

		for (int x = 0; x < SIM_X_SIZE; x++) {
			for (int y = 0; y < SIM_Y_SIZE; y++) {
				if (prev_frame[y * SIM_X_SIZE + x] == ALIVE)
					simPutPixel(x, y, 0x00FFFF00); // Yellow
				else
					simPutPixel(x, y, 0x00000000); // Black
			}
		}
		simFlush();
    }
}