#include "chip8.h"

/**
 * @brief 00E0 - CLS - Clears the display by setting all pixels to the "off" state
 *
 * The 00E0 instruction is used to clear the screen.
 * When executed, it resets the entire display memory,
 * effectively erasing any graphics currently being displayed.
 *
 * This instruction is commonly used at the beginning of a program or
 * before rendering a new frame to ensure a clean display.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 */
void	clear_screen(chip8 *chip8_data) {
	bzero(chip8_data->display, WIDTH * HEIGHT);
}

/**
 * @brief 00EE - RET - Return from a subroutine
 *
 * This instruction is used to return from a subroutine. It pops the
 * address from the top of the stack and sets the program counter (PC)
 * to that address. The stack pointer (SP) is decremented to reflect
 * the removal of the address from the stack.
 *
 * Instruction: 00EE
 * - Decrements the stack pointer (SP).
 * - Sets the program counter (PC) to the address stored at the top of the stack.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state structure.
 *
 * @note Ensure that the stack is not empty before calling this function
 *       to avoid stack underflow errors.
 */
void	ret(chip8 *chip8_data) {
	// Verificar se a stack está vazia
	// Tratar esse erro

	chip8_data->sp -= 1;
	chip8_data->pc = chip8_data->stack[chip8_data->sp];
}

/**
 * @brief 1nnn - JP addr - Sets the PC to the address nnn
 *
 * This instruction performs a jump operation by setting the program counter (PC)
 * to the address nnn (specified by `location`). This effectively
 * transfers control to the specified memory location.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param nnn The 12-bit address to jump to
 *            (specified by the lower 12 bits of the instruction).
 */
void	jump(chip8 *chip8_data, uint16_t location) {
	chip8_data->pc = location;
}

/**
 * @brief 2nnn - CALL addr - Call subroutine at nnn
 *
 * This function handles the 2nnn instruction, which is used to call a subroutine
 * located at the specified memory address (nnn). The current program counter (PC)
 * is pushed onto the stack before jumping to the subroutine, allowing the program
 * to return to the calling location later.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state structure.
 * @param location The memory address (nnn) of the subroutine to call.
 *
 * @note Ensure that the stack is not full before calling this function to avoid
 *       stack overflow.
 */
void	call(chip8 *chip8_data, uint16_t location) {
	// Verificaar se a pilha estiver cheio
	// Tratar esse erro - stack overflow

	chip8_data->stack[chip8_data->sp] = chip8_data->pc;
	chip8_data->sp += 1;
	chip8_data->pc = location;
}

/**
 * @brief 6xkk - LD Vx, byte - Sets Vx to the immediate value kk
 *
 * This function sets the value of the register Vx (specified by `register_v`)
 * to the immediate value `kk` (specified by `value`).
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param register_v The index of the V register (0x0 to 0xF) to be updated.
 * @param value The immediate value (kk) to set in the specified register Vx.
 */
void	set_register_vx(chip8 *chip8_data, uint8_t register_v, uint8_t value) {
	chip8_data->V[register_v] = value;
}

/**
 * @brief 7xkk - ADD Vx, byte - ADD Vx, kk
 *
 * Adds the immediate value `kk` (specified by `value`) to the value in
 * register Vx (specified by `register_v`) and stores the result back in Vx.
 * This operation does not affect the carry flag (VF).
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param register_v The index of the V register (0x0 to 0xF) to be updated.
 * @param value The immediate value (kk) to be added to the register Vx.
 */
void	add_value_to_register_vx(chip8 *chip8_data, uint8_t register_v, uint8_t value) {
	chip8_data->V[register_v] = chip8_data->V[register_v] + value;
}

/**
 * @brief Ann - LD I, addr - Sets the index register (I) to the immediate value (kk)
 *
 * Sets the index register (I) to the specified 12-bit address.
 *
 * The Annn instruction updates the I register with the immediate value
 * provided in the `value` parameter. This is commonly used to set the
 * memory address for subsequent operations involving the I register.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator data structure.
 * @param value The 12-bit address to set the index register I to.
 */
void	set_index_register_I(chip8 *chip8_data, uint16_t value) {
	chip8_data->I = value;
}

/**
 * @brief Dxyn - DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I at (Vx, Vy)
 *
 * This function draws a sprite at the coordinates specified by the registers
 * Vx and Vy. The sprite is `sprite_height` bytes tall and always 8 pixels wide.
 * Each byte in memory starting at the address stored in the I register represents
 * a row of the sprite, with each bit corresponding to a pixel (1 for on, 0 for off).
 *
 * The sprite is drawn using XOR logic, meaning that if a pixel is already set
 * and the sprite sets it again, the pixel is turned off. If this operation causes
 * any pixels to be turned off, the VF register is set to 1; otherwise, it is set to 0.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param register_vx Index of the Vx register, which holds the x-coordinate.
 * @param register_vy Index of the Vy register, which holds the y-coordinate.
 * @param sprite_height Height of the sprite in bytes.
 */
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
