#include "chip8.h"

void	init_font(chip8 *chip8_data)
{
	// There are 16 characters (sprites) with 4 pixels width
	// and 5 pixels height
	const uint8_t	font_size = 16 * 5;
	const uint8_t	font_start_address = 0x50;
	const uint8_t	font[] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	memcpy(&chip8_data->memory[font_start_address], font, font_size);
}

void	chip8_init(chip8 **chip8_data) {
	*chip8_data = malloc(sizeof(chip8));
	bzero(*chip8_data, sizeof(chip8));
	init_font(*chip8_data);
	(*chip8_data)->sp = 0;
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
