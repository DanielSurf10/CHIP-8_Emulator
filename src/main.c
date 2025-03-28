#include "chip8.h"

void	update_timers(chip8 *chip8_data) {
	if (chip8_data->delay_timer > 1)
		chip8_data->delay_timer--;
	if (chip8_data->sound_timer > 1)
	{
		chip8_data->sound_timer--;
		audio_play(chip8_data->audio);
	}
	else
		audio_stop(chip8_data->audio);
}

int main(int argc, char *argv[]) {

	SDL_Window		*window;
	SDL_Renderer	*renderer;
	chip8			*chip8_data = NULL;

	(void) argc;
	(void) argv;
	// if (argc < 2) {
	// 	fprintf(stderr, "Usage: %s <rom>\n", argv[0]);
	// 	return (1);
	// }

	if (!init_window(&window, &renderer))
		return (1);

	chip8_init(&chip8_data);
	int	num = 0, sec_count = 0;
	chip8_data->sound_timer = 75;

	// Loop principal
	int	running = 1;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			}
		}

		update_timers(chip8_data);

		for (int i = 0; i < 5; i++)
		{
			set_display_pixel(chip8_data->display, 0 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b10000000);
			set_display_pixel(chip8_data->display, 1 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b01000000);
			set_display_pixel(chip8_data->display, 2 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b00100000);
			set_display_pixel(chip8_data->display, 3 + 30, i + 12, chip8_data->memory[0x50 + i + num * 5] & 0b00010000);
		}

		sec_count++;

		if (sec_count >= 60)
		{
			if (num == 15)
				num = 0;
			else
				num++;
			sec_count = 0;
		}

		draw_chip8_display(renderer, chip8_data);
		SDL_Delay(1000 / DEFAULT_FPS);
	}

	audio_destroy(chip8_data->audio);
	free(chip8_data);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (0);
}
