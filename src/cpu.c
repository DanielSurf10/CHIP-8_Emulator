#include "chip8.h"

void	cycle(chip8 *chip8_data) {
	uint16_t	opcode;

	opcode = fetch(chip8_data);
	chip8_data->pc += 2;
	execute(chip8_data, opcode);
}

/*
 * Representação visual do processo de fetch:
 *
 * Memória CHIP-8:
 * +-------+-------+-------+-------+------+
 * | 0x200 | 0x201 | 0x202 | 0x203 | ...  |
 * +-------+-------+-------+-------+------+
 * | 0x61  | 0x2F  | 0xA1  | 0xF2  | ...  |
 * +-------+-------+-------+------+-------+
 *    ^       ^
 *    |       |
 *    PC      PC + 1
 *
 * Opcode
 * +-------+-------+
 * | 0x61  | 0x2F  | -> 0x612F
 * +-------+-------+
 *
 * O opcode é formado combinando os dois bytes:
 * opcode = (memory[PC] << 8) | memory[PC + 1]
 */

uint16_t	fetch(chip8 *chip8_data) {
	uint8_t		nibble1;
	uint8_t		nibble2;
	uint16_t	opcode;

	nibble1 = chip8_data->memory[chip8_data->pc];
	nibble2 = chip8_data->memory[chip8_data->pc + 1];
	opcode = (nibble1 << 8) | nibble2;

	return (opcode);
}

void	execute(chip8 *chip8_data, uint16_t opcode) {
	switch ((opcode & 0xF000) >> 12) {
	case 0x0:
		opcode_0(chip8_data, opcode);
		break;

	case 0x1:
		opcode_1(chip8_data, opcode);
		break;

	case 0x6:
		opcode_6(chip8_data, opcode);
		break;

	case 0x7:
		opcode_7(chip8_data, opcode);
		break;

	case 0xA:
		opcode_A(chip8_data, opcode);
		break;

	case 0xD:
		opcode_D(chip8_data, opcode);
		break;

	default:
		break;
	}
}

void	opcode_0(chip8 *chip8_data, uint16_t opcode) {
	switch (opcode & 0x0FFF) {
	case 0x0E0:
		clear_screen(chip8_data);
		break;

	// case 0x0EE:
	// 	ret(chip8_data);
	// 	break;

	default:
		break;
	}
}

void	opcode_1(chip8 *chip8_data, uint16_t opcode) {
	jump(chip8_data, opcode & 0x0FFF);
}

void	opcode_6(chip8 *chip8_data, uint16_t opcode) {
	set_register_vx(chip8_data, (opcode >> 8) & 0xF, opcode & 0xFF);
}

void	opcode_7(chip8 *chip8_data, uint16_t opcode) {
	add_value_to_register_vx(chip8_data, (opcode >> 8) & 0xF, opcode & 0xFF);
}

void	opcode_A(chip8 *chip8_data, uint16_t opcode) {
	set_index_register_I(chip8_data, opcode & 0x0FFF);
}

void	opcode_D(chip8 *chip8_data, uint16_t opcode) {
	draw_sprite_xor(chip8_data, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4, opcode & 0x000F);
}



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
