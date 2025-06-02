#pragma once
#include <xaudio2.h>

class Audio
{
public:
	Audio(const wchar_t* filename);
	~Audio();

	void Play(bool loop);
	void Stop(bool play_tails = true, bool wait_for_buffer_to_unqueue = true);
	void Volume(float volume);
	bool Queuing();

private:
	WAVEFORMATEXTENSIBLE wf_xtensible = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
	IXAudio2SourceVoice* source_voice;
};