#pragma once

typedef struct WaveHeader {
	unsigned char chunkID[4];      // big endian 
	unsigned char chunkSize[4];    // little endian
	unsigned char format[4];       // big endian
	unsigned char subchunk1ID[4];  // big endian
	unsigned char subchunk1Size[4];// little endian
	unsigned char audioFormat[2]; // little endian
	unsigned char numChannels[2];  // little endian
	unsigned char sampleRate[4];   // little endian 
	unsigned char byteRate[4];     // little endian
	unsigned char blockAlign[2];   // little endian
	unsigned char bitsPerSample[2];// little endian
	unsigned char subchunk2ID[4];  // big endian
	unsigned char subchunk2Size[4];// little endian
	unsigned char data[1];         // little endian
} WaveHeader;

class WaveFile
{
	friend class CWaveEditView;
	unsigned long numChannels;
	unsigned long sampleRate;
	unsigned long bitsPerSample;
	unsigned long bytesPerSample;
	unsigned int lastSample;
	unsigned int maxSamples;
	unsigned int fileSize;
	unsigned int dataSize;

	// Points to the wave file in memory
	WaveHeader * hdr;

public:

	// Getter functions
	unsigned long getSampleRate() { return sampleRate; }
	unsigned long getNumChannels() { return numChannels; }
	unsigned long getLastSample() { return lastSample; }
	unsigned long getbitsPerSample() { return bitsPerSample; }

public:
	// Create an empty wave file.
	WaveFile(void);

	// Create an empty wave file with these parameters.
	WaveFile(int numChannels, int sampleRate, int bitsPerSample);

	// Make a duplicate of a wave file
	static WaveFile* Copy(WaveFile wave);

	// Destructor
	~WaveFile(void);

	// Read a wave file from opened file f
	bool read(CFile * f);

	// Play wave file.
	void play();

	// Add a new sample at the end of the wave file.
	void add_sample(int sample);

	// Get ith sample
	int get_sample(int i);

	// Update the wave header in memory with the latest number of samples etc.
	// We need to call updateHeader before saving or playing the file after
	// the wave file has been updated.
	void updateHeader();

	// Save wave file in opened file f
	void save(CFile * f);

	// Create a tone with this frequency and this number of msecs.
	void tone(int frequency, int msecs);

	// Create a new wave file that is k times the frequency rate of the original up to durationms.
	// If durationms==0 then use the whole wave file.
	WaveFile * multiply_freq(double k, int durationms);

	// Similar to the other multiply_freq, but instead does it over a section selected
	WaveFile * multiply_freq(double k, int start, int end);

	// Append a wave file src to the end of this wave file. 
	void append_wave(WaveFile * src);

	// Create a new wavefile with echo from the original one.
	// echoAmount is a constant 0 to 1 with the amount of echo
	// delayms is the delay of the echo added to the original.
	WaveFile * echo(float echoAmount, float delayms);

	// Create a new wavefile that is a reverse copy of the original one
	WaveFile* reverse();

	// Speed up the sound of the WaveFile 
	// speed is a constant greater than 0 with the amount that it will be speed up
	WaveFile* speedUp(float speed);

	// Slow down the sound of the WaveFile
	// speed is a constant greater than 0 with the amount that it will be slowed down
	WaveFile* slowDown(float speed);

	// Get fragment of wavefile
	WaveFile* get_fragment(double startms, double endms);

	// Remove a fragment of wavefile
	WaveFile* remove_fragment(double start, double end);

	// Insert a frament fo a WaveFile into the this wave
	WaveFile* insert_fragment(WaveFile* toInsert, double start);
};
