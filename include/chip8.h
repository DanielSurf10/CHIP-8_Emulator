#ifndef CHIP_8
# define CHIP_8

# include <stdio.h>
# include <stdlib.h>
# include <SDL2/SDL.h>
# include <stdint.h>

# define WIDTH 64
# define HEIGHT 32
# define WINDOW_SCALE 10

typedef struct {
	uint8_t	*display;
} chip8;

#endif
