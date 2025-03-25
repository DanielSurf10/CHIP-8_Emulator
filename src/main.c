#include "chip8.h"

void	chip8_init(chip8 **chip8_data) {
	*chip8_data = malloc(sizeof(chip8));
	bzero(*chip8_data, sizeof(chip8));
}

void	draw_chip8_display(SDL_Renderer *renderer, chip8 *chip8_data) {
	SDL_Rect	rect = {.h = WINDOW_SCALE, .w = WINDOW_SCALE};

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (chip8_data->display[x + y * WIDTH]) {
				rect.x = x * WINDOW_SCALE;
				rect.y = y * WINDOW_SCALE;
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}

	SDL_RenderPresent(renderer);
}

int	init_window(SDL_Window **window, SDL_Renderer **renderer) {
	*window = NULL;
	*renderer = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return (0);

	*window = SDL_CreateWindow(
		"CHIP-8 Emulator",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH * WINDOW_SCALE,
		HEIGHT * WINDOW_SCALE,
		SDL_WINDOW_SHOWN
	);

	if (*window == NULL) {
		SDL_Quit();
		return (0);
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_WINDOW_SHOWN);
	if (*renderer == NULL) {
		SDL_DestroyWindow(*window);
		*window = NULL;
		SDL_Quit();
		return (0);
	}

	return (1);
}

int main(int argc, char *argv[]) {
	int				running;

	SDL_Window		*window;
	SDL_Renderer	*renderer;
	chip8			*chip8_data = NULL;

	(void) argc;
	(void) argv;
	// if (argc < 2) {
	// 	fprintf(stderr, "Usage: %s <rom>\n", argv[0]);
	// 	return (1);
	// }

	if (!init_window(&window, &renderer))
		return (-1);

	chip8_init(&chip8_data);
	chip8_data->display[0] = 1;

	int line = 0, column = 0;

	// Loop principal
	running = 1;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			}
		}

		// Atualize o estado do CHIP-8 aqui

		if (HEIGHT <= line)
			line = 0;
		else if (WIDTH > column)
			column++;
		else
		{
			line++;
			column = 0;
		}

		memset(chip8_data->display, 0, HEIGHT * WIDTH * sizeof(uint8_t));
		chip8_data->display[column + line * WIDTH] = 1;

		draw_chip8_display(renderer, chip8_data);
		SDL_Delay(1000 / 60);
	}

	free(chip8_data);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (0);
}
