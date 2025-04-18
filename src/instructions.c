#include "chip8.h"

/**
 * @brief 00E0 - CLS - Clears the display by setting all pixels to the "off" state
 *
 * Clears the screen by resetting the display memory, erasing all graphics.
 * Often used at the start of a program or before rendering a new frame.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 */
void	clear_screen(chip8 *chip8_data) {
	bzero(chip8_data->display, WIDTH * HEIGHT);
}

/**
 * @brief 00EE - RET - Return from a subroutine
 *
 * Return from a subroutine. Pops the address from the stack and sets the PC to it.
 * Decrements the stack pointer (SP) to reflect the removal of the address.
 *
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
 * Performs a jump by setting the program counter (PC) to the address `nnn`
 * (specified by `location`), transferring control to that memory location.
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
 * Calls a subroutine at address `nnn` (specified by `location`). Saves the current PC
 * on the top of the stack for later return, then jumps to the subroutine.
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
 * @brief 3xkk - SE Vx, byte - Skip next instruction if Vx is equal to kk
 *
 * Compares the value in register `Vx` (specified by `register_v`) with the
 * immediate value `kk` (specified by `value`).
 * If they are equal, increments the program counter (pc) by 2 to skip
 * the next instruction.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state structure.
 * @param register_v The index of the V register to compare (0x0 to 0xF).
 * @param value The immediate value (kk) to compare against the register.
 */
void	skip_equal(chip8 *chip8_data, uint8_t register_v, uint8_t value) {
	if (chip8_data->V[register_v] == value)
		chip8_data->pc += 2;
}

/**
 * @brief 4xkk - SNE Vx, byte - Skip next instruction if Vx is not equal to kk
 *
 * Compares the value in register `Vx` (specified by `register_v`) with the
 * immediate value `kk` (specified by `value`).
 * If they are different, increments the program counter (pc) by 2 to skip
 * the next instruction.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state structure.
 * @param register_v The index of the register Vx to compare (0x0 to 0xF).
 * @param value The immediate value (kk) to compare against.
 */
void	skip_not_equal(chip8 *chip8_data, uint8_t register_v, uint8_t value) {
	if (chip8_data->V[register_v] != value)
		chip8_data->pc += 2;
}

/**
 * @brief 5xy0 - SE Vx, Vy - Skip the next instruction if Vx is equal to Vy.
 *
 * This function checks if the values stored in the registers `Vx` and `Vy`
 * (specified by `register_vx` and `register_vy`) are equal. If they are
 * equal, the program counter (pc) is incremented by 2, effectively
 * skipping the next instruction in memory.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state structure.
 * @param register_vx Index of the Vx register (0x0 to 0xF).
 * @param register_vy Index of the Vy register (0x0 to 0xF).
 */
void	skip_equal_register(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy) {
	if (chip8_data->V[register_vx] == chip8_data->V[register_vy])
		chip8_data->pc += 2;
}

/**
 * @brief 6xkk - LD Vx, byte - Sets Vx to the immediate value kk
 *
 * This function sets the value of the register `Vx` (specified by `register_v`)
 * to the immediate value `kk` (specified by `value`).
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param register_v The index of the V register (0x0 to 0xF) to be updated.
 * @param value The immediate value (kk) to set in the specified register Vx.
 */
void	copy_value_to_vx(chip8 *chip8_data, uint8_t register_v, uint8_t value) {
	chip8_data->V[register_v] = value;
}

/**
 * @brief 7xkk - ADD Vx, byte - ADD Vx, kk
 *
 * Adds the immediate value `kk` (specified by `value`) to the value in
 * register `Vx` (specified by `register_v`) and stores the result back in `Vx`.
 * This operation does not affect the carry flag (VF).
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param register_v The index of the V register (0x0 to 0xF) to be updated.
 * @param value The immediate value (kk) to be added to the register Vx.
 */
void	add_value_to_vx(chip8 *chip8_data, uint8_t register_v, uint8_t value) {
	chip8_data->V[register_v] = chip8_data->V[register_v] + value;
}

/**
 * @brief 8xy0 - LD Vx, Vy - Set Vx to the value of Vy
 *
 * Copies the value from register `Vy` (specified by `register_vy`) into
 * register `Vx` (specified by `register_vx`). This does not affect any
 * flags or other registers.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state structure.
 * @param register_vx Index of the destination register Vx (0x0 to 0xF).
 * @param register_vy Index of the source register Vy (0x0 to 0xF).
 */
void	copy_vx_from_vy(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy) {
	chip8_data->V[register_vx] = chip8_data->V[register_vy];
}

/**
 * @brief 8xy1 - OR Vx, Vy - Performs a bitwise OR operation between Vx and Vy
 *
 * Performs a bitwise OR operation between the values in registers `Vx`
 * (specified by `register_vx`) and `Vy` (specified by `register_vy`),
 * and stores the result in `Vx`.
 * The value in `Vy` remains unchanged.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param register_vx Index of the Vx register (0x0 to 0xF).
 * @param register_vy Index of the Vy register (0x0 to 0xF).
 */
void	binary_or_between_registers(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy) {
	chip8_data->V[register_vx] = chip8_data->V[register_vx] | chip8_data->V[register_vy];
}

/**
 * @brief 8xy2 - AND Vx, Vy - Performs a bitwise AND operation between Vx and Vy
 *
 * This instruction performs a bitwise AND operation between the values in
 * registers `Vx` (specified by `register_vx`) and `Vy` (specified by `register_vy`),
 * and stores the result in `Vx`. The value in `Vy` remains unchanged.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param register_vx Index of the Vx register (0x0 to 0xF).
 * @param register_vy Index of the Vy register (0x0 to 0xF).
 */
void	binary_and_between_registers(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy) {
	chip8_data->V[register_vx] = chip8_data->V[register_vx] & chip8_data->V[register_vy];
}

/**
 * @brief 8xy3 - XOR Vx, Vy - Performs a bitwise XOR operation between Vx and Vy
 *
 * Performs a bitwise XOR operation between the values in registers `Vx`
 * (specified by `register_vx`) and `Vy` (specified by `register_vy`),
 * and stores the result in `Vx`.
 * The value in `Vy` remains unchanged.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param register_vx Index of the Vx register (0x0 to 0xF).
 * @param register_vy Index of the Vy register (0x0 to 0xF).
 */
void	binary_xor_between_registers(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy) {
	chip8_data->V[register_vx] = chip8_data->V[register_vx] ^ chip8_data->V[register_vy];
}

/**
 * @brief 8xy4 - ADD Vx, Vy - Add the value stored in the registers
 *
 * Adds the value in register `Vy` (specified by `register_vy`) to the value
 * in register `Vx` (specified by `register_vx`), storing the result in `Vx`.
 * If the result exceeds 8 bits (255), the carry flag (VF) is set to 1;
 * otherwise, it is set to 0.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param register_vx Index of the Vx register (0x0 to 0xF).
 * @param register_vy Index of the Vy register (0x0 to 0xF).
 */
void	add_vy_to_vx(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy) {
	// Talvez precise mudar o nome da função
	// a função deveria ter um nome onde vx aparece primeiro que vy
	// por causa da ordem dos parâmetros
	// algo como `adicionar_vx_em_vy_e_guardar_em_vx`
	int	sum;

	sum = chip8_data->V[register_vx] + chip8_data->V[register_vy];

	chip8_data->V[0xF] = 0;
	if (sum > 0xFF)
		chip8_data->V[0xF] = 1;

	chip8_data->V[register_vx] = sum;
}

/**
 * @brief 8xy5 - SUB Vx, Vy - Subtract Vy from Vx
 *
 * Subtracts the value of register `Vy` (specified by `register_vy`) from
 * register `Vx` (specified by `register_vx`) and stores the result in `Vx`.
 * The VF register (carry flag) is updated as follows:
 * - VF = 1 if `Vy` > `Vx` (no borrow occurs).
 * - VF = 0 if `Vy` <= `Vx` (a borrow occurs).
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param register_vx Index of the Vx register (0x0 to 0xF).
 * @param register_vy Index of the Vy register (0x0 to 0xF).
 *
 * @note Difference between 8xy7 (SUBN Vx, Vy) and 8xy5 (SUB Vx, Vy):
 * - 8xy7 computes Vy - Vx and stores the result in Vx.
 * - 8xy5 computes Vx - Vy and stores the result in Vx.
 * - The borrow condition and VF flag behavior are inverted between the two instructions.
 */
void	subtract_vy_from_vx(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy) {
	chip8_data->V[0xF] = 0;
	if (chip8_data->V[register_vx] > chip8_data->V[register_vy])
		chip8_data->V[0xF] = 1;

	chip8_data->V[register_vx] = chip8_data->V[register_vx] - chip8_data->V[register_vy];
}

/**
 * @brief 8xy6 - SHR Vx {, Vy} - Shifts the value in register Vx one bit to the right
 *
 * Shifts the value in `Vx` (specified by `register_vx`) one bit to the right.
 * The least significant bit (LSB) of `Vx` before the shift is stored in the
 * VF register (carry flag).
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state structure.
 * @param register_vx The index of the Vx register to be shifted (0x0 to 0xF).
 * @param register_vy The index of the Vy register (0x0 to 0xF) (ignored in this implementation).
 *
 * @note: The `Vy` (specified by `register_vy`) parameter is ignored in
 * this implementation, as some CHIP-8 interpreters do not use it for
 * this instruction. The operation is performed directly on `Vx`.
 */
void	shift_vx_right(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy) {
	// Talvez isso mude
	// Porque no CHIP-48 e no SUPER-CHIP isso não tem
	// e em alguns interpretadores de CHIP8 tem
	// Se não tem a operação é feita direta no Vx
	// Talvez seja necessário deixar isso opcional
	// chip8_data->V[register_vx] = chip8_data->V[register_vy];

	(void) register_vy;
	chip8_data->V[0xF] = chip8_data->V[register_vx] & 0x1;
	chip8_data->V[register_vx] = chip8_data->V[register_vx] >> 1;
}

/**
 * @brief 8xy7 - SUBN Vx, Vy - Subtract Vx from Vy
 *
 * Subtracts the value of register `Vx` (specified by `register_vx`) from
 * register `Vy` (specified by `register_vy`) and stores the result in `Vx`.
 * The VF register (carry flag) is updated as follows:
 * - VF = 1 if `Vy` > `Vx` (no borrow occurs).
 * - VF = 0 if `Vy` <= `Vx` (a borrow occurs).
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state.
 * @param register_vx Index of the Vx register (0x0 to 0xF).
 * @param register_vy Index of the Vy register (0x0 to 0xF).
 *
 * @note Difference between 8xy7 (SUBN Vx, Vy) and 8xy5 (SUB Vx, Vy):
 * - 8xy7 computes Vy - Vx and stores the result in Vx.
 * - 8xy5 computes Vx - Vy and stores the result in Vx.
 * - The borrow condition and VF flag behavior are inverted between the two instructions.
 */
void	subtract_vx_from_vy(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy) {
	chip8_data->V[0xF] = 0;
	if (chip8_data->V[register_vy] > chip8_data->V[register_vx])
		chip8_data->V[0xF] = 1;

	chip8_data->V[register_vx] = chip8_data->V[register_vy] - chip8_data->V[register_vx];
}

/**
 * @brief 8xyE - SHL Vx {, Vy} - Shifts the value in register Vx one bit to the left
 *
 * Shifts the value in `Vx` (specified by `register_vx`) one bit to the left.
 * The least significant bit (LSB) of `Vx` before the shift is stored in the
 * VF register (carry flag).
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state structure.
 * @param register_vx The index of the Vx register to be shifted (0x0 to 0xF).
 * @param register_vy The index of the Vy register (0x0 to 0xF) (ignored in this implementation).
 *
 * @note: The `Vy` (specified by `register_vy`) parameter is ignored in
 * this implementation, as some CHIP-8 interpreters do not use it for
 * this instruction. The operation is performed directly on `Vx`.
 */
void	shift_vx_left(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy) {
	// Talvez isso mude
	// Porque no CHIP-48 e no SUPER-CHIP isso não tem
	// e em alguns interpretadores de CHIP8 tem
	// Se não tem a operação é feita direta no Vx
	// Talvez seja necessário deixar isso opcional
	// chip8_data->V[register_vx] = chip8_data->V[register_vy];

	(void) register_vy;
	chip8_data->V[0xF] = chip8_data->V[register_vx] >> 7;
	chip8_data->V[register_vx] = chip8_data->V[register_vx] << 1;
}

/**
 * @brief 9xy0 - SNE Vx, Vy - Skip the next instruction if Vx is equal to Vy
 *
 * This function checks if the values stored in the registers `Vx` and `Vy`
 * (specified by `register_vx` and `register_vy`) are not equal. If they are
 * different, the program counter (pc) is incremented by 2, effectively
 * skipping the next instruction in memory.
 *
 * @param chip8_data Pointer to the CHIP-8 emulator state structure.
 * @param register_vx Index of the Vx register to compare.
 * @param register_vy Index of the Vy register to compare.
 */
void	skip_not_equal_register(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy) {
	if (chip8_data->V[register_vx] != chip8_data->V[register_vy])
		chip8_data->pc += 2;
}

/**
 * @brief Ann - LD I, addr - Sets the index register (I) to the immediate value (kk)
 *
 * Sets the index register (I) to the immediate value `kk` (specified by `value`).
 * Commonly used to set the memory address for operations involving the I register.
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
