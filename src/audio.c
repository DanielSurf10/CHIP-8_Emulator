#include "chip8.h"

static uint16_t square_wave(uint32_t time, int32_t rate, int32_t freq, double amp) {
	uint32_t	ticks = rate / freq;
	double		cycle = time % ticks;

	return (cycle < (double)ticks / 2 ? (uint16_t)(INT16_MAX * amp) : 0);
}

static void audio_callback(void *data, uint8_t *buffer, int length) {
	Audio		*current = (Audio *)data;
	uint16_t	*stream = (uint16_t *)buffer;

	for (int i = 0; i < length / 2; i++, current->sample_index++) {
		stream[i] = square_wave(current->sample_index, current->sample_rate, current->frequency, current->amplitude);
	}
}

Audio	*audio_create(int32_t freq, int32_t rate, int32_t amp) {
	Audio	*audio = malloc(sizeof(Audio));

	if (!audio) {
		fprintf(stderr, "Failed to allocate memory for audio.\n");
		return (NULL);
	}

	audio->frequency = freq;
	audio->sample_rate = rate;
	audio->amplitude = amp;
	audio->sample_index = 0;

	SDL_zero(audio->have);
	SDL_zero(audio->want);
	audio->want.freq = rate;
	audio->want.format = AUDIO_U16;
	audio->want.channels = 1;
	audio->want.samples = 512;
	audio->want.callback = audio_callback;
	audio->want.userdata = audio;

	audio->device = SDL_OpenAudioDevice(NULL, 0, &audio->want, &audio->have, 0);
	if (!audio->device) {
		fprintf(stderr, "Failed to open audio. Reason: %s\n", SDL_GetError());
		free(audio);
		return (NULL);
	}

	return (audio);
}

void audio_play(Audio *audio) {
	if (audio) {
		SDL_PauseAudioDevice(audio->device, 0);
	}
}

void audio_stop(Audio *audio) {
	if (audio) {
		SDL_PauseAudioDevice(audio->device, 1);
	}
}

void audio_destroy(Audio *audio) {
	if (audio) {
		SDL_CloseAudioDevice(audio->device);
		free(audio);
	}
}
