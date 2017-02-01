#ifndef DIRECTORYENCODER_H
#define DIRECTORYENCODER_H

#include "directory.h"

/*!
 * \class DirectoryEncoder
 *
 * \brief wrapper class for orchestrating directory encoding
 *
 * NOTE: Number of threads is limited in order to prevent out-of-memory errors
 *       when encoding directories with a lot of files.
 *
 * NOTE: Measurements showed that the performace bottleneck lies with the CPU
 *       time and increasing number of threads yielded performance improvements,
 *       until the point when number of threads is greather than or equal to the
 *       number of CPU cores. Beyond that point, adding more threads doesn't
 *       doesn't impact performance (or the impact is within measurement noise).
 *
 *       Since there is no measurable benefit beyond this point, it is the value
 *       to which the max number of threads is initially set.
 */
class DirectoryEncoder
{
public:
	/*!
	 * \brief DirectoryEncoder constructor
	 */
	DirectoryEncoder();

	/*!
	 * \brief SetMaxThreads overrides default concurent threads limit
	 *
	 * \param max_threads new limit
	 */
	void SetMaxThreads(int max_threads);

	/*!
	 * \brief Encode encodes all .wav files in the directory
	 *
	 * \param directory directory to be encoded
	 *
	 * \return true if all files have been successfully encoded, false if some have failed
	 */
	bool Encode(const Directory& directory) const;

	/*!
	 * \brief Encode encodes single .wav file into .mp3 with the same basename
	 *
	 * \param wav file to be encoded
	 *
	 * \return true on success, false on failure
	 */
	static bool Encode(const FileName& wav);

private:
	unsigned int _max_threads;
};

#endif // DIRECTORYENCODER_H
