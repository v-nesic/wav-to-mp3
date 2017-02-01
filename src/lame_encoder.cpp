#include "lame_encoder.h"

#include <cstring>
#include <iostream>
#include <cmath>
#include <vector>

#include "filename.h"

using std::vector;

LameEncoder::LameEncoder(
		const FileName& wav,
		const FileName& mp3) :
	_wav_file(wav.FullPath()),
	_mp3_name(mp3),
	_frames_at_once(kFramesAtOnceDefault),
	_pcm_buffer(2*_frames_at_once), // LAME encoder always expects stereo input
	_mp3_buffer(std::ceil(1.25*_frames_at_once+7200)), // recomened by LAME encoder docs
	_pcm_right(0),
	_lame_flags(0),
	_mp3_file(0)
{
}

LameEncoder::~LameEncoder()
{
	CloseOutputFile();
	ResetEncoder();
}

bool LameEncoder::Encode()
{
	// Assert valid internal state, could be replaced with assert or throw
	if (_lame_flags || _mp3_file) return false;

	bool encoded = false;

	if (ConfigureEncoder())
	{
		encoded = EncodeMp3Output();
	}
	ResetEncoder();

	return encoded;
}

bool LameEncoder::EncodeMp3Output()
{
	bool encoded = false;

	if (InputValid() && RewindInput() && CreateOutputFile())
	{
		encoded = RunEncoder();

		if (encoded)
		{
			CloseOutputFile();
		}
		else
		{
			DeleteOutputFile();
		}
	}

	return encoded;
}

bool LameEncoder::RunEncoder()
{
	ConfigurePcmBuffer();

	for(sf_count_t processed_frames = 0, input_frames = InputFrames(); processed_frames < input_frames; )
	{
		sf_count_t frames_read = ReadInput();
		if (frames_read <= 0)
		{
			return false;
		}

		int mp3_bytes = EncodePcmBuffer(frames_read);
		if (mp3_bytes < 0 || !WriteOutputData(mp3_bytes))
		{
			return false;
		}

		processed_frames += frames_read;
	}

	int mp3_bytes = FlushEncoder();
	if (mp3_bytes < 0 || !WriteOutputData(mp3_bytes))
	{
		return false;
	}

	return true;
}

void LameEncoder::ConfigurePcmBuffer()
{
	if (1 == _wav_file.channels())
	{
		// We have allocated PCM buffer big enought to store full stereo data.
		// If the input file is mono, we can use 2nd half of the buffer as
		// all-zeros right-channel data.
		_pcm_right = &(_pcm_buffer.data()[_frames_at_once]);
		memset(_pcm_right, 0, sizeof(short)*_frames_at_once);
	}
	else
	{
		_pcm_right = 0;
	}
}

int LameEncoder::EncodePcmBuffer(int frame_count)
{
	// This logic has a goal to iron out differences between
	// libsndfile and libmp3lame data ordering.
	//
	// When reading from a stereo file, libsndfile will interleave the data,
	// forcing us to either copy the data or use lame_encode_buffer_interleaved,
	// and this method can't be used for mono files because LAME always expect
	// data for both channels.
	// To avoid copying the data in order to interleave it with zeros, we use
	// another method, lame_encode_buffer, and pass pre-filled buffer of
	// all zeros as the right channel.
	if (0 != _pcm_right)
	{
		return lame_encode_buffer(
					_lame_flags,
					_pcm_buffer.data(),
					_pcm_right,
					frame_count,
					_mp3_buffer.data(),
					_mp3_buffer.size());
	}
	else
	{
		return lame_encode_buffer_interleaved(
					_lame_flags,
					_pcm_buffer.data(),
					frame_count,
					_mp3_buffer.data(),
					_mp3_buffer.size());
	}
}

int LameEncoder::FlushEncoder()
{
	return lame_encode_flush(_lame_flags, _mp3_buffer.data(), _mp3_buffer.size());
}

bool LameEncoder::ConfigureEncoder()
{
	_lame_flags = lame_init();

	if ((NULL != _lame_flags) &&
		(-1 != lame_set_num_channels(_lame_flags, _wav_file.channels())) &&
		(-1 != lame_set_in_samplerate(_lame_flags, _wav_file.samplerate())) &&
		(-1 != lame_set_VBR(_lame_flags, vbr_default)) &&
		(-1 != lame_set_VBR_quality(_lame_flags, 0.0)) &&
		(-1 != lame_init_params(_lame_flags)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void LameEncoder::ResetEncoder()
{
	lame_close(_lame_flags);
	_lame_flags = NULL;
}

bool LameEncoder::RewindInput()
{
	return -1 != _wav_file.seek(0, SEEK_SET);
}

bool LameEncoder::InputValid() const
{
	return SF_ERR_NO_ERROR == _wav_file.error();
}

sf_count_t LameEncoder::ReadInput()
{
	return _wav_file.readf(_pcm_buffer.data(), _frames_at_once);
}

sf_count_t LameEncoder::InputFrames() const
{
	return _wav_file.frames();
}

bool LameEncoder::CreateOutputFile()
{
	_mp3_file = fopen(_mp3_name.FullPath().c_str(), "w+b");

	return NULL != _mp3_file;
}

void LameEncoder::CloseOutputFile()
{
	if (_mp3_file)
	{
		fclose(_mp3_file);
		_mp3_file = NULL;
	}
}

bool LameEncoder::DeleteOutputFile()
{
	CloseOutputFile();
	return _mp3_name.Unlink();
}

bool LameEncoder::WriteOutputData(size_t mp3_bytes)
{
	return fwrite(_mp3_buffer.data(), 1, mp3_bytes, _mp3_file) == mp3_bytes;
}
