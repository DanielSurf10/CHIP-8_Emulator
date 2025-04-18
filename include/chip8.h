#ifndef CHIP_8
# define CHIP_8

// Includes

# include <stdio.h>
# include <stdlib.h>
# include <SDL2/SDL.h>
# include <stdint.h>
# include <math.h>
# include <sys/stat.h>
# include <unistd.h>

// Defines

# define WIDTH						64
# define HEIGHT						32
# define WINDOW_SCALE				10
# define DEFAULT_FPS				60
# define INSTRUCTIONS_PER_SECOND	700

# define MEMORY_SIZE				4096
# define PC_START					0x200
# define ROM_SIZE					MEMORY_SIZE - PC_START

# define DEFAULT_FREQUENCY			500
# define DEFAULT_SAMPLE_RATE		44100
# define DEFAULT_AMP				28000
# define DEFAULT_INTERVAL			(1000 / DEFAULT_FPS)
# define SOUND_CLOCK				(1000 / 60)

// Structures

typedef struct {
	int32_t	sample_rate;
	int32_t	frequency;
	int32_t	amplitude;
	int32_t	sample_index;

	SDL_AudioSpec		have;
	SDL_AudioSpec		want;
	SDL_AudioDeviceID	device;
}	Audio;

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
	uint8_t	memory[MEMORY_SIZE];

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

	// Sound
	Audio	*audio;
}	chip8;

// Functions

// Init functions
chip8		*chip8_init(char *path_to_rom);
void		init_font(chip8 *chip8_data);
int			init_window(SDL_Window **window, SDL_Renderer **renderer);
int			read_rom(char *path_to_rom, uint8_t *memory);

// Display functions
void		draw_chip8_display(SDL_Renderer *renderer, chip8 *chip8_data);
void		set_display_pixel(uint8_t *display, int x, int y, int state);
uint8_t		get_display_pixel(uint8_t *display, int x, int y);

// Audio functions
Audio		*audio_create(int32_t freq, int32_t rate, int32_t amp);
void		audio_play(Audio *audio);
void		audio_stop(Audio *audio);
void		audio_destroy(Audio *audio);

// CPU functions
void		cycle(chip8 *chip8_data);
uint16_t	fetch(chip8 *chip8_data);
void		decode_and_execute(chip8 *chip8_data, uint16_t opcode);

void	opcode_0(chip8 *chip8_data, uint16_t opcode);
void	opcode_1(chip8 *chip8_data, uint16_t opcode);
void	opcode_2(chip8 *chip8_data, uint16_t opcode);
void	opcode_3(chip8 *chip8_data, uint16_t opcode);
void	opcode_4(chip8 *chip8_data, uint16_t opcode);
void	opcode_5(chip8 *chip8_data, uint16_t opcode);
void	opcode_6(chip8 *chip8_data, uint16_t opcode);
void	opcode_7(chip8 *chip8_data, uint16_t opcode);
void	opcode_8(chip8 *chip8_data, uint16_t opcode);
void	opcode_9(chip8 *chip8_data, uint16_t opcode);
void	opcode_A(chip8 *chip8_data, uint16_t opcode);
void	opcode_D(chip8 *chip8_data, uint16_t opcode);

void	clear_screen(chip8 *chip8_data);
void	ret(chip8 *chip8_data);
void	jump(chip8 *chip8_data, uint16_t location);
void	call(chip8 *chip8_data, uint16_t location);
void	skip_equal(chip8 *chip8_data, uint8_t register_v, uint8_t value);
void	skip_not_equal(chip8 *chip8_data, uint8_t register_v, uint8_t value);
void	skip_equal_register(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy);
void	copy_value_to_vx(chip8 *chip8_data, uint8_t register_v, uint8_t value);
void	add_value_to_vx(chip8 *chip8_data, uint8_t register_v, uint8_t value);
void	copy_vx_from_vy(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy);
void	binary_or_between_registers(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy);
void	binary_and_between_registers(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy);
void	binary_xor_between_registers(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy);
void	add_vy_to_vx(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy);
void	subtract_vy_from_vx(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy);
void	shift_vx_right(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy);
void	subtract_vx_from_vy(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy);
void	shift_vx_left(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy);
void	skip_not_equal_register(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy);
void	set_index_register_I(chip8 *chip8_data, uint16_t value);
void	draw_sprite_xor(chip8 *chip8_data, uint8_t register_vx, uint8_t register_vy, uint8_t sprite_height);

#endif
