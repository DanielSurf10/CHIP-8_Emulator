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

void	opcode_3(chip8 *chip8_data, uint16_t opcode) {
	skip_equal(chip8_data, (opcode >> 8) & 0xF, opcode & 0xFF);
}

void	opcode_4(chip8 *chip8_data, uint16_t opcode) {
	skip_not_equal(chip8_data, (opcode >> 8) & 0xF, opcode & 0xFF);
}

void	opcode_5(chip8 *chip8_data, uint16_t opcode) {
	// Tratar o último nibble caso não seja 0
	// Algo parecido com isso
	// if (opcode & 0xF != 0)
	// 	return;

	skip_equal_register(chip8_data, (opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
}

void	opcode_6(chip8 *chip8_data, uint16_t opcode) {
	copy_value_to_vx(chip8_data, (opcode >> 8) & 0xF, opcode & 0xFF);
}

void	opcode_7(chip8 *chip8_data, uint16_t opcode) {
	add_value_to_vx(chip8_data, (opcode >> 8) & 0xF, opcode & 0xFF);
}

void	opcode_8(chip8 *chip8_data, uint16_t opcode) {
	switch (opcode & 0xF) {
	case 0x0:
		copy_vx_from_vy(chip8_data, (opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
		break;

	case 0x1:
		binary_or_between_registers(chip8_data, (opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
		break;

	case 0x2:
		binary_and_between_registers(chip8_data, (opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
		break;

	case 0x3:
		binary_xor_between_registers(chip8_data, (opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
		break;

	case 0x4:
		add_vy_to_vx(chip8_data, (opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
		break;

	case 0x5:
		subtract_vy_from_vx(chip8_data, (opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
		break;

	case 0x6:
		shift_vx_right(chip8_data, (opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
		break;

	case 0x7:
		subtract_vx_from_vy(chip8_data, (opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
		break;

	case 0xE:
		shift_vx_left(chip8_data, (opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
		break;

	default:
		break;
	}
}

void	opcode_9(chip8 *chip8_data, uint16_t opcode) {
	// Tratar o último nibble caso não seja 0
	// Algo parecido com isso
	// if (opcode & 0xF != 0)
	// 	return;

	skip_not_equal_register(chip8_data, (opcode >> 8) & 0xF, (opcode >> 4) & 0xF);
}

void	opcode_A(chip8 *chip8_data, uint16_t opcode) {
	set_index_register_I(chip8_data, opcode & 0x0FFF);
}

void	opcode_D(chip8 *chip8_data, uint16_t opcode) {
	draw_sprite_xor(chip8_data, (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4, opcode & 0x000F);
}
