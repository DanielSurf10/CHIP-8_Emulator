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

chip8	*chip8_init(void) {
	chip8	*chip8_data = malloc(sizeof(chip8));

	bzero(chip8_data, sizeof(chip8));
	init_font(chip8_data);
	chip8_data->sp = 0;
	chip8_data->pc = PC_START;
	chip8_data->audio = audio_create(DEFAULT_FREQUENCY, DEFAULT_SAMPLE_RATE, DEFAULT_AMP);

	return (chip8_data);
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
