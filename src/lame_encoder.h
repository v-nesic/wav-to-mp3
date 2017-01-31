#ifndef LAMEENCODER_H
#define LAMEENCODER_H

#include <vector>

#include <lame.h>
#include <sndfile.hh>

#include "filename.h"

/*!
 * \class LameEncoder
 *
 * \brief encodes input wav file int output mp3 file using LAME encoder
 */
class LameEncoder
{
public:
	/*!
	 * \brief LameEncoder constructor, allocates memory and captures file names
	 *
	 * \param wav input file
	 * \param mp3 output file
	 */
	LameEncoder(const FileName& wav, const FileName& mp3);

	/*!
	 * \brief LameEncoder destructor, releases memory
	 */
	~LameEncoder();

	/*!
	 * \brief Encode encodes intput wav file into output mp3 file
	 *
	 * \return true if the encoding is done, false if it encoutred an error
	 */
	bool Encode();

private:
	static const int kFramesAtOnceDefault = 28*1152;

	bool EncodeMp3Output();
	bool RunEncoder();
	void ConfigurePcmBuffer();
	int EncodePcmBuffer(int frame_count);
	int FlushEncoder();
	bool ConfigureEncoder();
	void ResetEncoder();
	bool RewindInput();
	bool InputValid() const;
	sf_count_t ReadInput();
	sf_count_t InputFrames() const;
	bool CreateOutputFile();
	void CloseOutputFile();
	bool DeleteOutputFile();
	bool WriteOutputData(size_t mp3_bytes);

	SndfileHandle _wav_file;
	FileName _mp3_name;
	unsigned int _frames_at_once;
	std::vector<short int> _pcm_buffer;
	std::vector<unsigned char> _mp3_buffer;
	lame_global_flags* _lame_flags;
	FILE* _mp3_file;
	short int* _pcm_right;
};


#endif // LAMEENCODER_H
