#include "chip8.h"

void	update_timers(chip8 *chip8_data) {
	if (chip8_data->delay_timer > 0)
		chip8_data->delay_timer--;
	if (chip8_data->sound_timer > 0)
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
	chip8			*chip8_data;

	(void) argc;
	(void) argv;
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <rom>\n", argv[0]);
		return (1);
	}

	chip8_data = chip8_init(argv[1]);

	if (!chip8_data || !init_window(&window, &renderer))
		return (1);


	// Verificar se o audio funcionou
	chip8_data->audio = audio_create(DEFAULT_FREQUENCY, DEFAULT_SAMPLE_RATE, DEFAULT_AMP);

	chip8_data->sound_timer = 60;

	// Loop principal
	int	running = 1;
	int last_instruction_time = SDL_GetTicks();

	while (running) {
		int current_time = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			}
		}

		if ((current_time - last_instruction_time) > 1000 / DEFAULT_FPS)
		{
			draw_chip8_display(renderer, chip8_data);
			update_timers(chip8_data);
			last_instruction_time = current_time;
		}

		SDL_Delay(1000 / INSTRUCTIONS_PER_SECOND);
	}

	audio_destroy(chip8_data->audio);
	free(chip8_data);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (0);
}
