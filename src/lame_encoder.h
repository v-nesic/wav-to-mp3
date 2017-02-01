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
	 * Implementation: configures mp3 encoder and calls EncodeMp3Output)
	 *
	 * \return true if the encoding is done, false if it encoutred an error
	 */
	bool Encode();

private:
	static const int kFramesAtOnceDefault = 28*1152;

	/*!
	 * \brief EncodeMp3Output encodes input wav file into output mp3 file
	 *
	 * Implementation validates input file, creates output file and runs mp3 encoder
	 *
	 * \return true on success, false in case of an error
	 */
	bool EncodeMp3Output();

	/*!
	 * \brief RunEncoder main mp3 encoder loop
	 *
	 * \return true on success, false in case of an error
	 */
	bool RunEncoder();

	/*!
	 * \brief ConfigurePcmBuffer adjusts allocated buffers for mono or stereo input
	 */
	void ConfigurePcmBuffer();

	/*!
	 * \brief EncodePcmBuffer calls appropriate lame_encode_buffer... function
	 *
	 * \param frame_count number of input samples
	 *
	 * \return number of bytes placed in the output buffer
	 */
	int EncodePcmBuffer(int frame_count);

	/*!
	 * \brief FlushEncoder runs encoding pipeline until all input data is encoded
	 *
	 * \return number of bytes placed in the output buffer
	 */
	int FlushEncoder();

	/*!
	 * \brief ConfigureEncoder configures lame encoder (VBR with default settings)
	 *
	 * NOTE: number of chanells and sample rate are read from the input file
	 *
	 * \return true on success, false in case of an error
	 */
	bool ConfigureEncoder();

	/*!
	 * \brief ResetEncoder releases encoder settings, allowing new encoding session
	 */
	void ResetEncoder();

	/*!
	 * \brief RewindInput rewinds input file read position to the first sample
	 *
	 * \return true on success, false in case of an error
	 */
	bool RewindInput();

	/*!
	 * \brief InputValid checks if the file can be read by libsndfile or not
	 *
	 * \return true on success, false in case of an error
	 */
	bool InputValid() const;

	/*!
	 * \brief ReadInput read input samples
	 *
	 * \return number of input samples read
	 */
	sf_count_t ReadInput();

	/*!
	 * \brief InputFrames input file size
	 *
	 * \return number of samples which should be read from the input
	 */
	sf_count_t InputFrames() const;

	/*!
	 * \brief CreateOutputFile creates/overwrites output file
	 *
	 * \return true on success, false in case of an error
	 */
	bool CreateOutputFile();

	/*!
	 * \brief CloseOutputFile releases output file handle
	 */
	void CloseOutputFile();

	/*!
	 * \brief DeleteOutputFile removes output file (called in case of an error)
	 *
	 * \return true on success, false in case of an error
	 */
	bool DeleteOutputFile();

	/*!
	 * \brief WriteOutputData writes output buffer to the output file
	 *
	 * \param mp3_bytes number of bytes which should be written
	 *
	 * \return true on success (all bytes written), false in case of an error
	 */
	bool WriteOutputData(size_t mp3_bytes);

	// libsndfile object responsible for reading from input file
	SndfileHandle _wav_file;

	// output filename
	FileName _mp3_name;

	// number of frames to be processed in one chunk
	unsigned int _frames_at_once;

	// input buffer
	std::vector<short int> _pcm_buffer;

	// output buffer
	std::vector<unsigned char> _mp3_buffer;

	// pointer to the second half of the _pcm_buffer, used for feeding zeros to
	// the encoder's right channel in case of mono input
	short int* _pcm_right;

	// lame encoder configuration
	lame_global_flags* _lame_flags;

	// output file handle
	FILE* _mp3_file;
};


#endif // LAMEENCODER_H
