#pragma once
#include <xaudio2.h>

class Audio
{
public:
	Audio(const wchar_t* filename);
	~Audio();

	void Play(bool loop);
	void Stop(bool play_tails = true, bool wait_for_buffer_to_unqueue = true);
	void SetVolume(float a_volume);
	float GetVolume() { return volume; }
	bool Queuing();
	bool IsPlaying();

private:
	WAVEFORMATEXTENSIBLE wfXtensible = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
	IXAudio2SourceVoice* sourceVoice;
	float volume = 0.0f;
};