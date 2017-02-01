## Synopsis
WavToMp3 is a tool for converting PCM (.wav) files to MP3.

It uses libsndfile for processing input files and encodes MP3 using LAME mp3 encoder.

## Design
* Encoding is done using multiple threads, one per each CPU core - no gains measured beyond this number.
* Input is read in realtively small chunks (28*1154 bytes) - larger buffers failed to produce measureable gains:
 * Up to ~100kB there was no difference in performance.
 * Starting with ~100kB there were occasional congections which led to drops in CPU utilizations.
 
## Project Oganization
* WavToMp3.pro - qmake project file for building the main tool
* libraries.pri - qmake include file, listing libary prequisites
* src/ - tool's source code
* tests/tests.pro - qmake project file for building unit tests
* tests/ - unit tests

## Tools and Platforms
WavToMp3 has been compiled and tested using:
* Windows7 64bit, Qt Creator v3.6.1 and MinGw v5.4.2 (32bit)
* Ubuntu 16.04.LTS, Qt Creator v4.2.1 and MinGw v5.7.1 GCC (64bit)

## Open Source Libraries
WavToMp3 tool depends on two libraries:
* libmp3lame (LAME Encoder, included in the source tree) and
* libsndfile (PCM-format reader, must be installed separately)
* tpunit++ (minimalistic unit-testing framework)

## Installing Prequistes (libsndfile)
In order to build and run WavToMp3 too, libsndfile must be installed and placed in a location from which it can be used - directory listed in ld.path.conf file on Linux or PATH environment variable on Windows.

If the library is installed on a non-standard location or on 32bit Windows system, it's path should be updated in libraries.pri.

Source tarball and binary installer can be found in libsndfile directory or downloaded from http://www.mega-nerd.com/libsndfile

## Testing
The project includes a set of unit-test which validate behaviour of implemented classes.

Automating end-to-end and audio quality testing hasn't been done.
