#ifndef WAVEHEADER_H
#define WAVEHEADER_H

#include <string.h>
#include <iostream>

#pragma pack(push)
#pragma pack (1)

struct wav {
	char riff[4]; /* ASCII "RIFF"  */
	long chunksize; /* datasize + 8 + 16 + 12  */
	char wave[4]; /* ASCII "WAVE" */
	char fmt[4]; /* ASCII "fmt " */
	long fmtchunksize; /* fmt chunk size 16 */
	short fmttag; /* 1 = wave format PCM */
	short channels;
	long rate;
	long avebytespsec; /* rate * channels * samsize / 8  */
	short nBlockAlign; /* channels * samsize / 8  */
	short sample_size; /* bits per sample of one channel. */
	char data[4]; /* ASCII "data"  */
	long datasize; /* data chunk size  channels * samples * (samsize / 8)  */
	long samples;
};

#pragma pack(pop)

class WaveHeader{

protected:

	wav header;

public:

	WaveHeader( int n_channels1 = 2, int sampleRate = 44100, long sampleSize = 16, long dataSize = 0);
	//Insert n_channels into the stored struct wav
	inline void setNChannels(int n_channels1);
	/* Insert sample size, in bits, into the stored struct wav.  */
	inline void setSampleSize(int sample_size1);
	/* Insert sampling rate in samples per second into the stored struct wav. */
	inline void setSampleRate(int sample_rate);
	/* Set size of data area into the stored struct wav.  */
	void setDataSize(long n_samples);
	/* Set the tag value representing the encoding method.
	   Refer to the struct fmt formats, above.  */
	void setFormatTag(int tag);

	//Get n_channels from the stored struct wav
	inline short getNChannels() const;
	/* get sample size, in bits, from the stored struct wav.  */
	inline short getSampleSize() const;
	/* get sampling rate in samples per second from the stored struct wav. */
	inline long getSampleRate()const;
	/* Set size of data area into the stored struct wav.  */
	inline long getDataSize() const;
	/* Return the length of the data area, measured in samples.  */
	inline long getNSamples() const;
	//copy wav header to destanation memory
	void getHeader( wav *ptr);

private:
	long convertLitEnd2Big(long num);
};


#endif
