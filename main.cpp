#include <iostream>
#include <ostream>
#include <cmath>
#include <fstream>
#include <vector>

template <typename T>
void write(std::ofstream& stream, const T& t) {
  stream.write((const char*)&t, sizeof(T));
}

template <typename SampleType>
void writeWAVData(const char* outFile, /*SampleType **buf*/std::vector< SampleType* > *buf, size_t bufSize, int sampleRate, short channels)
{
  std::ofstream stream(outFile, std::ios_base::binary | std::ios_base::trunc);
  stream.write("RIFF", 4);
  write<int>(stream, 36 + bufSize);
  stream.write("WAVE", 4);
  stream.write("fmt ", 4);
  write<int>(stream, 16);
  write<short>(stream, 1);                                        // Format (1 = PCM)
  write<short>(stream, channels);                                 // Channels
  write<int>(stream, sampleRate);                                 // Sample Rate
  write<int>(stream, sampleRate * channels * sizeof(SampleType)); // Byterate
  write<short>(stream, channels * sizeof(SampleType));            // Frame size
  write<short>(stream, 8 * sizeof(SampleType));                   // Bits per sample
  stream.write("data", 4);
  stream.write((const char*)&bufSize, 4);
  //stream.write((const char*)buf, bufSize);
  //std::cout << sizeof(SampleType) << std::endl;
  for(int i = 0; i < bufSize ; i++){
	  for(int j = 0; j < channels; j++){
		  //stream.write((char*)&(ptr[j]),sizeof(SampleType));
		  SampleType *ptr = (*buf)[j];
		  write<short>(stream, ptr[i]);
		  std::cout << ptr[i] << std::endl;
	  }
  }
  stream.close();
}

using namespace std;

short convDoubleTo16bitInt(double num)
{
    long r;

    // clamp values:
    if (num > 1.0) { num = 1.0; }
    else if (num < -1.0) { num = -1.0; }

    num += 1.0;      // remap from [-1.0,1.0] to [0.0,2.0]
    num /= 2;        // remap from [0.0,2.0] to [0.0,1.0]
    r = (num * 65535.); // remap from [0.0,1.0] to [0,65535]
    return r - 32768.;  // remap from [0,65535] to [-32768,32767]
}

int main( int argc, char *argv[])
{

	double freqd = 44100;
	double timeSec = 1;
	double freq1 = 5000;
	double freq2 = 755;
	double alpha1 = (2.0* M_PI)*freq1;
	double alpha2 = (2.0* M_PI)*freq2;
	double ampl = 1.0;
	//double freqs[2] = {504, 755};
	//double alphs[2] = {(2.0* M_PI)*freqs[0], (2.0* M_PI)*freqs[1]};

	short sig1[(int)(freqd*timeSec)];
	short sig2[(int)(freqd*timeSec)];
	vector<short*> signals(2);

	for(int i = 0; i < (int)(freqd*timeSec); i++){
		sig1[i] = convDoubleTo16bitInt(ampl*sin(alpha1*i/freqd));
		sig2[i] = convDoubleTo16bitInt(ampl*sin(alpha2*i/freqd));
		//cout << i << endl;
	}
	signals[0] = &sig1[0];
	signals[1] = &sig2[0];
	writeWAVData<short>("testSignal.wav", &signals, freqd*timeSec, 44100, 2);


	return 0;
}
