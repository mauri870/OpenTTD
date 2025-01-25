/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file sdl3_s.cpp Playing sound via SDL3. */

#ifdef WITH_SDL3

#include "../stdafx.h"

#include "../mixer.h"
#include "sdl3_s.h"
#include <SDL3/SDL.h>

#include "../safeguards.h"

/** Factory for the SDL sound driver. */
static FSoundDriver_SDL3 iFSoundDriver_SDL3;

/**
 * Callback that handles the audio stream.
 * @param stream   The stream to put data into.
 * @param additional_amount The amount of samples.
 * @param total_amount The total amount of samples.
 */
static void CDECL audio_stream_callback(void *, SDL_AudioStream *stream, int additional_amount, int)
{
	if (additional_amount > 0) {
		Uint8 *data = SDL_stack_alloc(Uint8, additional_amount);
		if (data) {
			MxMixSamples(data, additional_amount / 4);
			SDL_PutAudioStreamData(stream, data, additional_amount);
			SDL_stack_free(data);
		}
	}
}

std::optional<std::string_view> SoundDriver_SDL3::Start(const StringList &parm)
{
	const SDL_AudioSpec spec = { SDL_AUDIO_S16, 2, GetDriverParamInt(parm, "hz", 44100) };

	/* Only initialise SDL if the video driver hasn't done it already */
	bool ret_code = false;
	if (SDL_WasInit(SDL_INIT_VIDEO) == 0) {
		ret_code = SDL_InitSubSystem(SDL_INIT_VIDEO);
	} else if (SDL_WasInit(SDL_INIT_AUDIO) == 0) {
		ret_code = SDL_InitSubSystem(SDL_INIT_AUDIO);
	}
	if (!ret_code) return SDL_GetError();

    SDL_AudioStream *stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, audio_stream_callback, NULL);
	if (stream == nullptr) return SDL_GetError();
	MxInitialize(spec.freq);
	SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(stream));
	return std::nullopt;
}

void SoundDriver_SDL3::Stop()
{
	SDL_CloseAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK);
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	if (SDL_WasInit(SDL_INIT_AUDIO) == 0) {
		SDL_Quit(); // If there's nothing left, quit SDL
	}
}

#endif /* WITH_SDL3 */
