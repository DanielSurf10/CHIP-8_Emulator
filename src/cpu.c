#include "chip8.h"

void	cycle(chip8 *chip8_data) {
	uint16_t	opcode;

	opcode = fetch(chip8_data);
	chip8_data->pc += 2;
	decode_and_execute(chip8_data, opcode);
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

void	decode_and_execute(chip8 *chip8_data, uint16_t opcode) {
	switch ((opcode & 0xF000) >> 12) {
	case 0x0:
		opcode_0(chip8_data, opcode);
		break;

	case 0x1:
		opcode_1(chip8_data, opcode);
		break;

	case 0x2:
		opcode_2(chip8_data, opcode);
		break;

	case 0x3:
		opcode_3(chip8_data, opcode);
		break;

	case 0x4:
		opcode_4(chip8_data, opcode);
		break;

	case 0x5:
		opcode_5(chip8_data, opcode);
		break;

	case 0x6:
		opcode_6(chip8_data, opcode);
		break;

	case 0x7:
		opcode_7(chip8_data, opcode);
		break;

	case 0x9:
		opcode_9(chip8_data, opcode);
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
