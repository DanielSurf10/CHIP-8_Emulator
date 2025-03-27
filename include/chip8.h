#ifndef CHIP_8
# define CHIP_8

// Includes

# include <stdio.h>
# include <stdlib.h>
# include <SDL2/SDL.h>
# include <stdint.h>

// Defines

# define WIDTH			64
# define HEIGHT			32
# define WINDOW_SCALE	10

// Structures

typedef struct chip8 {

	/*
	Memory Map:
	+----------------+= 0xFFF (4095) End of Chip-8 RAM
	|                |
	|                |
	| 0x200 to 0xFFF |
	|     Chip-8     |
	| Program / Data |
	|     Space      |
	|                |
	|                |
	+----------------+= 0x200 (512) Start of Chip-8 programs
	| 0x000 to 0x1FF |
	| Reserved for   |
	|  interpreter   |
	+----------------+= 0x000 (0) Start of Chip-8 RAM
	*/
	uint8_t	memory[4096];

	// 16 general purpose 8-bit registers
	uint8_t	V[16];

	// This register is generally used to store memory addresses
	uint16_t	I;

	// Points the the location in memory of the next Opcode - Program Counter
	uint16_t	pc;

	// Call Stack
	uint16_t	stack[16];

	// Call Stack Pointer
	uint8_t	sp;

	// Timers
	uint8_t	delay_timer;
	uint8_t	sound_timer;

	// Display
	uint8_t	display[WIDTH * HEIGHT];
} chip8;

// Functions

// Init functions
void	init_font(chip8 *chip8_data);
void	chip8_init(chip8 **chip8_data);
int		init_window(SDL_Window **window, SDL_Renderer **renderer);

// Display functions
void	draw_chip8_display(SDL_Renderer *renderer, chip8 *chip8_data);
void	set_display_pixel(uint8_t *display, int x, int y, int state);

#endif
