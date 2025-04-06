#include "chip8.h"

void	clear_screen(chip8 *chip8_data) {
	bzero(chip8_data->display, WIDTH * HEIGHT);
}

void	jump(chip8 *chip8_data, uint16_t location) {
	chip8_data->pc = location;
}

void	set_register_vx(chip8 *chip8_data, uint8_t register_v, uint8_t value) {
	chip8_data->V[register_v] = value;
}

void	add_value_to_register_vx(chip8 *chip8_data, uint8_t register_v, uint8_t value) {
	chip8_data->V[register_v] = chip8_data->V[register_v] + value;
}

void	set_index_register_I(chip8 *chip8_data, uint16_t value) {
	chip8_data->I = value;
}

void	draw_sprite_xor(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy, uint8_t sprite_height) {
	uint8_t			x_cord;
	uint8_t			y_cord;
	uint8_t			pixel_state;
	const uint8_t	sprite_width = 8;

	x_cord = chip8_data->V[register_vx] % WIDTH;
	y_cord = chip8_data->V[register_vy] % HEIGHT;
	chip8_data->V[0xF] = 0;

	for (int row = 0; row < sprite_height; row++) {
		for (int column = 0; column < sprite_width; column++) {
			pixel_state = get_display_pixel(chip8_data->display, x_cord + column, y_cord + row);

			if (chip8_data->memory[chip8_data->I + row] & (1 << (sprite_width - column - 1))) {
				if (pixel_state)
				{
					pixel_state = 0;
					chip8_data->V[0xF] = 1;
				} else {
					pixel_state = 1;
				}
			}

			set_display_pixel(chip8_data->display, x_cord + column, y_cord + row, pixel_state);
		}
	}
}
