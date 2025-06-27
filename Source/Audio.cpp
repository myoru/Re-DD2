#include "Audio.h"
#include "Graphics.h"
#include "misc.h"

HRESULT FindChunk(HANDLE hfile, DWORD fourcc, DWORD& chunk_size, DWORD& chunk_data_position)
{

	HRESULT hr = S_OK;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hfile, 0, NULL, FILE_BEGIN))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD chunk_type;
	DWORD chunk_data_size;
	DWORD riff_data_size = 0;
	DWORD file_type;
	DWORD bytes_read = 0;
	DWORD offset = 0;

	while (hr == S_OK)
	{
		DWORD number_of_bytes_read;
		if (0 == ReadFile(hfile, &chunk_type, sizeof(DWORD), &number_of_bytes_read, NULL))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (0 == ReadFile(hfile, &chunk_data_size, sizeof(DWORD), &number_of_bytes_read, NULL))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (chunk_type)
		{
		case 'FFIR'/*RIFF*/:
			riff_data_size = chunk_data_size;
			chunk_data_size = 4;
			if (0 == ReadFile(hfile, &file_type, sizeof(DWORD), &number_of_bytes_read, NULL))
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hfile, chunk_data_size, NULL, FILE_CURRENT))
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		offset += sizeof(DWORD) * 2;

		if (chunk_type == fourcc)
		{
			chunk_size = chunk_data_size;
			chunk_data_position = offset;
			return S_OK;
		}

		offset += chunk_data_size;

		if (bytes_read >= riff_data_size)
		{
			return S_FALSE;
		}
	}

	return S_OK;

}

HRESULT ReadChunkData(HANDLE hFile, LPVOID buffer, DWORD buffer_size, DWORD buffer_offset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, buffer_offset, NULL, FILE_BEGIN))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	DWORD number_of_bytes_read;
	if (0 == ReadFile(hFile, buffer, buffer_size, &number_of_bytes_read, NULL))
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
	}
	return hr;
}

Audio::Audio(const wchar_t* filename)
{
	Graphics& _graphics = Graphics::Instance();
	IXAudio2* _xaudio2 = _graphics.GetXaudio2();

	HRESULT hr;

	// Open the file
	HANDLE hfile = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hfile)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hfile, 0, NULL, FILE_BEGIN))
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	DWORD chunk_size;
	DWORD chunk_position;
	//check the file type, should be 'WAVE' or 'XWMA'
	FindChunk(hfile, 'FFIR'/*RIFF*/, chunk_size, chunk_position);
	DWORD filetype;
	ReadChunkData(hfile, &filetype, sizeof(DWORD), chunk_position);
	_ASSERT_EXPR(filetype == 'EVAW'/*WAVE*/, L"Only support 'WAVE'");

	FindChunk(hfile, ' tmf'/*FMT*/, chunk_size, chunk_position);
	ReadChunkData(hfile, &wfXtensible, chunk_size, chunk_position);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hfile, 'atad'/*DATA*/, chunk_size, chunk_position);
	BYTE* data = new BYTE[chunk_size];
	ReadChunkData(hfile, data, chunk_size, chunk_position);

	buffer.AudioBytes = chunk_size;  //size of the audio buffer in bytes
	buffer.pAudioData = data;  //buffer containing audio data
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	hr = _xaudio2->CreateSourceVoice(&sourceVoice, (WAVEFORMATEX*)&wfXtensible);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	sourceVoice->GetVolume(&volume);
}

Audio::~Audio()
{
	sourceVoice->DestroyVoice();
	delete[] buffer.pAudioData;
}

void Audio::Play(bool loop)
{
	HRESULT hr;

	XAUDIO2_VOICE_STATE voice_state = {};
	sourceVoice->GetState(&voice_state);

	if (voice_state.BuffersQueued)
	{

		return;
	}

	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
	hr = sourceVoice->SubmitSourceBuffer(&buffer);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = sourceVoice->Start(0);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void Audio::Stop(bool play_tails, bool wait_for_buffer_to_unqueue)
{
	XAUDIO2_VOICE_STATE voice_state = {};
	sourceVoice->GetState(&voice_state);
	if (!voice_state.BuffersQueued)
	{
		return;
	}

	HRESULT hr;
	hr = sourceVoice->Stop(play_tails ? XAUDIO2_PLAY_TAILS : 0);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = sourceVoice->FlushSourceBuffers();
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	while (wait_for_buffer_to_unqueue && voice_state.BuffersQueued)
	{
		sourceVoice->GetState(&voice_state);
	}
}

void Audio::SetVolume(float a_volume)
{
	volume = a_volume;
	HRESULT hr = sourceVoice->SetVolume(volume);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

bool Audio::Queuing()
{
	XAUDIO2_VOICE_STATE voice_state = {};
	sourceVoice->GetState(&voice_state);
	return voice_state.BuffersQueued;
}

bool Audio::IsPlaying()
{
	XAUDIO2_VOICE_STATE _voiceState = {};
	sourceVoice->GetState(&_voiceState);

	if (_voiceState.BuffersQueued)
	{
		return true;
	}
	else
	{
		return false;
	}
}
