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

// for (int i = 0; i < 5; i++)
// {
// 	set_display_pixel(chip8_data->display, 0 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b10000000);
// 	set_display_pixel(chip8_data->display, 1 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b01000000);
// 	set_display_pixel(chip8_data->display, 2 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b00100000);
// 	set_display_pixel(chip8_data->display, 3 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b00010000);
// }
