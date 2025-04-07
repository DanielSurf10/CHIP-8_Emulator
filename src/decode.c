#include "chip8.h"

void	opcode_0(chip8 *chip8_data, uint16_t opcode) {
	switch (opcode & 0x0FFF) {
	case 0x0E0:
		clear_screen(chip8_data);
		break;

	case 0x0EE:
		ret(chip8_data);
		break;

	default:
		break;
	}
}

void	opcode_1(chip8 *chip8_data, uint16_t opcode) {
	jump(chip8_data, opcode & 0x0FFF);
}

void	opcode_2(chip8 *chip8_data, uint16_t opcode) {
	call(chip8_data, opcode & 0x0FFF);
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
