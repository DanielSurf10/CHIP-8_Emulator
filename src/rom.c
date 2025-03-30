#include "chip8.h"

static int	check_file(char *path_to_rom) {
	struct stat	file_stat;

	if (access(path_to_rom, F_OK) == -1)
	{
		fprintf(stderr, "%s: ROM file does not exist\n", path_to_rom);
		return (0);
	}
	if (access(path_to_rom, R_OK) == -1)
	{
		fprintf(stderr, "%s: Permission denied to read ROM file\n", path_to_rom);
		return (0);
	}

	if (stat(path_to_rom, &file_stat) == -1)
	{
		fprintf(stderr, "%s: Error checking file\n", path_to_rom);
		return (0);
	}

	if (!S_ISREG(file_stat.st_mode))
	{
		fprintf(stderr, "%s: Not a file, let alone a ROM\n", path_to_rom);
		return (0);
	}

	if (file_stat.st_size == 0)
	{
		fprintf(stderr, "%s: Empty file, therefore not a ROM\n", path_to_rom);
		return (0);
	}

	if (file_stat.st_size > ROM_SIZE)
	{
		fprintf(stderr, "%s: File too large to be a chip8 ROM (%ld bytes)\n", path_to_rom, file_stat.st_size);
		return (0);
	}

	return (1);
}

static FILE	*open_rom_file(char *path_to_rom) {
	FILE	*rom_file;

	rom_file = fopen(path_to_rom, "rb");
	if (rom_file == NULL)
	{
		fprintf(stderr, "Error opening ROM: %s\n", path_to_rom);
		return (NULL);
	}

	return (rom_file);
}

int	read_rom(char *path_to_rom, uint8_t *memory) {
	int		rom_size;
	FILE	*rom_file;
	uint8_t	*rom_buffer;

	if (check_file(path_to_rom) == 0)
		return (0);

	rom_file = open_rom_file(path_to_rom);
	if (rom_file == NULL)
		return (0);

	rom_buffer = malloc(ROM_SIZE);
	if (rom_buffer == NULL)
	{
		fprintf(stderr, "Error allocating memory for ROM buffer\n");
		fclose(rom_file);
		return (0);
	}

	rom_size = fread(rom_buffer, sizeof(uint8_t), ROM_SIZE, rom_file);
	memcpy(&memory[PC_START], rom_buffer, rom_size);

	free(rom_buffer);
	fclose(rom_file);

	return (1);
}
