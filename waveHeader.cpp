/*
 * waveHeader.cpp
 *
 *  Created on: 17.10.2013
 *      Author: ivan
 */


#include "waveHeader.h"

WaveHeader::WaveHeader( int n_channels1, int sampleRate, long sampleSize, long dataSize)
{
	setNChannels(n_channels1);
	setSampleSize(sampleSize);
	setSampleRate(sampleRate);
	setDataSize(dataSize);
	setFormatTag(1);
	memcpy(header.riff, "RIFF",4);
	memcpy(header.wave, "WAVE",4);
	memcpy(header.data ,"data",4);
	memcpy(header.fmt, "fmt",4);
	std::cout << "we are here" << std::endl;
	std::cout << sizeof(header) << std::endl;
}

inline void WaveHeader::setNChannels(int n_channels1)
{
	header.channels = n_channels1;
	header.avebytespsec = header.rate * header.channels * (header.sample_size / 8);
	header.nBlockAlign = header.channels * (header.sample_size / 8);
}

/* Insert sample size, in bits, into the stored struct wav.  */
inline void WaveHeader::setSampleSize(int sample_size1)
{
	header.sample_size = sample_size1;
	header.avebytespsec = header.rate * header.channels * (header.sample_size / 8);
	header.nBlockAlign = header.channels * (header.sample_size / 8);
	header.datasize = header.samples * header.channels * (header.sample_size / 8);
}

/* Insert sampling rate in samples per second into the stored struct wav. */
inline void WaveHeader::setSampleRate(int sample_rate)
{
	header.rate = sample_rate;
	header.avebytespsec = sample_rate * header.channels * (header.sample_size / 8);
}

/* Set size of data area into the stored struct wav.  */
void WaveHeader::setDataSize(long n_samples)
{
	header.samples = n_samples;
	header.datasize = n_samples * header.channels
	    * (header.sample_size / 8);
	  if (header.fmtchunksize == 0)
		  header.fmtchunksize = 16;
	  header.chunksize = header.datasize + header.fmtchunksize + 20;
}

void WaveHeader::setFormatTag(int tag)
{
	header.fmttag = tag;
}

//Get n_channels from the stored struct wav
inline short WaveHeader::getNChannels() const
{
	return header.channels;
}

/* get sample size, in bits, from the stored struct wav.  */
inline short WaveHeader::getSampleSize() const
{
	return header.sample_size;
}

/* get sampling rate in samples per second from the stored struct wav. */
inline long WaveHeader::getSampleRate() const
{
	return header.rate;
}

/* Set size of data area into the stored struct wav.  */
inline long WaveHeader::getDataSize() const
{
	return header.datasize;
}

/* Return the length of the data area, measured in samples.  */
inline long WaveHeader::getNSamples() const
{
	return (header.datasize / (header.channels * (header.sample_size / 8)));
}

void WaveHeader::getHeader( wav *ptr)
{
	wav tempHeader = header;

	long riffBE = 0x5249464;
	long waveBE = 0x57415645;
	long dataBE = 0x64617461;
	long fmtBE =  0x666d7420;
	memcpy(tempHeader.riff,&riffBE,sizeof(long));
	memcpy(tempHeader.wave, &waveBE,sizeof(long));
	memcpy(tempHeader.data , &dataBE,sizeof(long));
	memcpy(tempHeader.fmt, &fmtBE,sizeof(long));

	memcpy(ptr, &tempHeader, sizeof(wav));

}

long WaveHeader::convertLitEnd2Big(long num)
{
	unsigned char c1, c2, c3, c4;

	c1 = num & 255;
	c2 = (num >> 8) & 255;
	c3 = (num >> 16) & 255;
	c4 = (num >> 24) & 255;

	return ((long) c1 << 24) + ((long) c2 << 16) + ((long) c3 << 8) + c4;

}
