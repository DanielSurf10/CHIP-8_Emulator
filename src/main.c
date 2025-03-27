#include "chip8.h"

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

void	set_display_pixel(uint8_t *display, int x, int y, int state) {

	////////////////////////////
	// Não sei qual precisa usar

	// if (x * y > WIDTH * HEIGHT)
	// return ;

	// Vou deixar esse aqui
	if (x >= WIDTH || y >= HEIGHT)
		return ;
	////////////////////////////

	display[WIDTH * y + x] = state;
}

void	update_timers(chip8 *chip8_data) {
	if (chip8_data->delay_timer > 1)
		chip8_data->delay_timer--;
	if (chip8_data->sound_timer > 1)
		chip8_data->sound_timer--;
}

int main(int argc, char *argv[]) {

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
	int	num = 0;

	// Loop principal
	int	running = 1;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			}
		}

		update_timers(chip8_data);

		for (int i = 0; i < 5; i++)
		{
			set_display_pixel(chip8_data->display, 0 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b10000000);
			set_display_pixel(chip8_data->display, 1 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b01000000);
			set_display_pixel(chip8_data->display, 2 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b00100000);
			set_display_pixel(chip8_data->display, 3 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b00010000);
		}


		if (num == 15)
			num = 0;
		else
			num++;

		draw_chip8_display(renderer, chip8_data);
		SDL_Delay(1000 / 2);
	}

	free(chip8_data);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (0);
}
