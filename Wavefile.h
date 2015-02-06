//Wavefile class used for reading and writing wavefiles designed for easy use with Flowstones DLL component
//Currently only supports PCM wave data.
//Author Thomas Chatfield AKA Exo

#ifndef WAVEFILE
#define WAVEFILE

#include <iostream>

typedef struct 
{
    char        chunkID[4];
    int         chunkSize;
    short int   waveFormat; //PCM only
    short int   noOfChannels;
    int         sampleRate;
    int         byteRate;
    short int   blockAlign;
    short int   bitsPerSample; 
} Format; 

typedef struct 
{
    char        chunkID[4];
    int         chunkSize;
    char        formatType[4];
    Format      formatChunk;
    int         noOfSamples;
}Header;

typedef struct {
    long        identifier;
    long        type;
    long        start;
    long        end;
    long        fraction;
    long        playCount;
} SampleLoop;

typedef struct
{
    long         chunksize;
    long         manufacturer;
    long         product;
    long         samplePeriod;
    long         midiNote;
    long         midiPitchFrac;
    long         SMPTEFormat;
    long         SMPTEOffset;
    long         numSampleLoops;
    long         samplerData; 
    SampleLoop*  loopPoints;
}SamplerChunk;

class Wavefile
{
private:
	FILE* wave;
	bool isWavefile;
	int noOfSamples;
	short bitsPerSample;
	int noOfChannels;
	int seekIndex;
	int fileSize;
        short waveFormat;
	inline float bytesToFloat(BYTE firstByte, BYTE secondByte); //16bit
        inline float bytesToFloat(BYTE firstByte, BYTE secondByte, BYTE thirdByte); //24bit
        inline float bytesToFloat(BYTE firstByte, BYTE secondByte, BYTE thirdByte, BYTE fourthByte); //32bit
        
        inline int convertToFloatArray(BYTE* bytes, float* floatArray); 
        
        void readWaveData(float* waveData);
                
public:
	Wavefile();
	~Wavefile();

	void openWaveFile(char* path);
	void readHeader(Header& header);	
        int read(SamplerChunk& samplerChunk, int& waveData); //returns samples per channel
        
	void close(); //Must call this when done!
};

#endif
