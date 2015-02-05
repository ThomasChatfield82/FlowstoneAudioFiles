#include "stdafx.h"
#include "Wavefile.h"
#include "Waveform.h"
#include "math.h"
#include <iostream>
#include <stdio.h>
#include "FileException.h"
#include <limits.h>

Wavefile::Wavefile()
{
}

Wavefile::~Wavefile()
{
}

void Wavefile::openWaveFile(char* path)
{
    if(path)
    {
          wave = fopen(path,"rb");
          if(wave ==NULL){
              throw FileException("File failed to load!!");
          } 
          
    }
}
void Wavefile::read(SamplerChunk& samplerChunk, float* waveData)
{
    if(wave)
    {
              //look for data chunk 
              char name[4] = {'d','a','t','a'};	
              char smpl[4] = {'s','m','p','l'};
              char dataID[4];
              char chunkID[4];
              samplerChunk.numSampleLoops=0; // initialise to zero for checking
              
              while(true)
	    {
		//This will loop looking for data chunk until we reach the end of the file....
		fread(dataID,sizeof(BYTE),4,wave);
	        if (!strncmp(dataID,name,4))
	        { //found data chunk
		    int noOfBytes = 0;
	            fread(&noOfBytes,sizeof(DWORD),1,wave); //get number of bytes
                    noOfSamples = header.noOfSamples = (noOfBytes / (bitsPerSample/8))/noOfChannels; //Store for use in readWaveData method
                    
		    readWaveData(waveData);
	        }
                
                else if(!strncmp(chunkID,smpl,4))
            { //Found Sampler chunk
                fread(&samplerChunk.chunksize,sizeof(DWORD),1,wave); //Get size of chunk in bytes
                fseek(wave,sizeof(long)*8, SEEK_CUR); //Skip straight to number of loop points
                fread(&samplerChunk.numSampleLoops,sizeof(DWORD),1,wave);
                
                samplerChunk.loopPoints = new SampleLoop[samplerChunk.numSampleLoops];
                
                for(int i=0; i< samplerChunk.numSampleLoops; i++)
                {
                    fread(samplerChunk.loopPoints[i],sizeof(SampleLoop),1,wave);
                }
                
                }
		else
                   {
		        //Not cue chunk, so just get the size of this chunk and skip to the end...
			int size=0;
			fread(&size, sizeof(DWORD), 1, wave); //read in 32bit chunksize value
			fseek(wave,(long)size,SEEK_CUR); //Skip this chunk!
		   }

		   seekIndex = ftell(wave); //Get current seekIndex 

		   if(seekIndex>=fileSize) //Reached end of file
		   {
                       
			break;
		   }
	    }    
    }
}

void Wavefile::readHeader(Header& header)
{
    if(wave)
    {
        this->header= header;
       fread(header.chunkID, sizeof(BYTE), 4, wave); //read in first four bytes
       
       if (!strncmp(header.chunkID, "RIFF",4))
       {  
           fread(&header.chunkSize, sizeof(DWORD), 1, wave); //read in 32bit chunksize value
	     fileSize = header.chunkSize;
           fread(header.formatType, sizeof(BYTE), 4, wave); //read in 4 byte string FormatType
           
           if (!strncmp(header.formatType,"WAVE",4)) 
           { 
	        isWavefile=true;
              fread(header.formatChunk.chunkID, sizeof(BYTE), 4, wave); //read in 4 bytes "fmt ";
              fread(&header.formatChunk.chunkSize, sizeof(DWORD),1,wave);
              fread(&header.formatChunk.waveFormat, sizeof(short), 1, wave); //PCM / float - Only supporting PCM for now
              waveFormat = header.formatChunk.waveFormat;
              fread(&header.formatChunk.noOfChannels, sizeof(short),1,wave);
              noOfChannels = header.formatChunk.noOfChannels;
	        
              fread(&header.formatChunk.sampleRate, sizeof(DWORD), 1, wave);
              fread(&header.formatChunk.byteRate, sizeof(DWORD), 1, wave);
              fread(&header.formatChunk.blockAlign, sizeof(short), 1, wave);
              
              fread(&header.formatChunk.bitsPerSample, sizeof(short), 1, wave);
              bitsPerSample = header.formatChunk.bitsPerSample;
		/**
              //look for data chunk 
              char name[4] = {'d','a','t','a'};	
              char smpl[4] = {'s','m','p','l'};
              char dataID[4];
              char chunkID[4];
              while(true)
	    {
		//This will loop looking for data chunk until we reach the end of the file....
		fread(dataID,sizeof(BYTE),4,wave);
	        if (!strncmp(dataID,name,4))
	        { //found data chunk
		    int noOfBytes = 0;
	            fread(&noOfBytes,sizeof(DWORD),1,wave); //get number of bytes
                    noOfSamples = header.noOfSamples = (noOfBytes / (bitsPerSample/8))/noOfChannels; //Store for use in readWaveData method
		    break;
	        }
                
                else if(!strncmp(chunkID,smpl,4))
            { //Found Sampler chunk
                fread(&sampler.chunksize,sizeof(DWORD),1,wave); //Get size of chunk in bytes
                fseek(wave,28L, SEEK_CUR); //Skip straight to number of loop points
                fread(&sampler.numSampleLoops,sizeof(DWORD),1,wave);
                }
		else
                   {
		        //Not cue chunk, so just get the size of this chunk and skip to the end...
			int size=0;
			fread(&size, sizeof(DWORD), 1, wave); //read in 32bit chunksize value
			fseek(wave,(long)size,SEEK_CUR); //Skip this chunk!
		   }

		   seekIndex = ftell(wave); //Get current seekIndex 

		   if(seekIndex>=fileSize) //Reached end of file
		   {
			
			break;
		   }
	    }
	      */
	   }
       }
    }
}


void Wavefile::readWaveData(float* waveData)
{
    if(wave)
    {
	if(isWavefile)
	{
	    int size = ((noOfSamples*noOfChannels)*(bitsPerSample/8)); //Get size of byte array
	    BYTE* bytes = new BYTE[size];
		
	    fread(bytes,sizeof(BYTE),size,wave); 
	
            convertToFloatArray(bytes, waveData);
            
            
	    this->waveData = waveData;

	    delete[] bytes;		
	}
    }
			
}


void Wavefile::readCueChunk(CueChunk& cueChunk)
{
    if(wave)
    {
	if(isWavefile)
	{
	    char name[4] = {'c','u','e',' '};
	    
	    while(true)
	    {
		//This will loop looking for cue chunk until we reach the end of the file....
		fread(cueChunk.chunkID,sizeof(BYTE),4,wave);
	        if (!strncmp(cueChunk.chunkID,name,4))
	        { //found Cue chunk
		    fread(&cueChunk.chunkSize, sizeof(DWORD), 1, wave); //read in 32bit chunksize value
		    fread(&cueChunk.noOfCuePoints, sizeof(DWORD), 1, wave); //Read number of cuepoints

		    cueChunk.cuePoints = new CuePoint[sizeof(CuePoint) * cueChunk.noOfCuePoints]; //cuePoint Array

		    for(int cueIndex=0; cueIndex<cueChunk.noOfCuePoints; cueIndex++)
		    {
			fread(&cueChunk.cuePoints[cueIndex],sizeof(CuePoint),1,wave);
		    }

		    break;
	        }
		else
                   {
		        //Not cue chunk, so just get the size of this chunk and skip to the end...
			int size=0;
			fread(&size, sizeof(DWORD), 1, wave); //read in 32bit chunksize value
			fseek(wave,(long)size,SEEK_CUR); //Skip this chunk!
		   }

		   seekIndex = ftell(wave); //Get current seekIndex 

		   if(seekIndex>=fileSize) //Reached end of file
		   {
			cueChunk.noOfCuePoints=0;
			break;
		   }
	    }
	}
    }
}

int Wavefile::convertToFloatArray(BYTE* bytes, float* floatArray)
{
    
    if(bitsPerSample==16)
    {
        int bytesPerSample = (bitsPerSample/8);
        int byteIndex=0;
        //This algorithm is adding channels one after the other instead of interleaving channels
        //This makes playback much easier...
        for(int j = 0; j < noOfChannels; j++)
        {
           byteIndex=(j*(bytesPerSample)); //index for reading from byte array
           for(int i=0; i<noOfSamples;i++) 
	   {
	        floatArray[i + (j*noOfSamples)] =  bytesToFloat(bytes[byteIndex],bytes[byteIndex+1]); 
                byteIndex = byteIndex +(bytesPerSample*noOfChannels);
           }
        }
    }
    
    else if(bitsPerSample==24)
    {
        int bytesPerSample = (bitsPerSample/8);
        int byteIndex=0;
        //This algorithm is adding channels one after the other instead of interleaving channels
        //This makes playback much easier...
        for(int j = 0; j < noOfChannels; j++)
        {
           byteIndex=(j*(bytesPerSample)); //index for reading from byte array
           for(int i=0; i<noOfSamples;i++) 
	   {
	        floatArray[i + (j*noOfSamples)] =  bytesToFloat(bytes[byteIndex],bytes[byteIndex+1], bytes[byteIndex+2]); 
                byteIndex = byteIndex +(bytesPerSample*noOfChannels);
           }
        }
        
    }
    else if(bitsPerSample==32)
    {
        int bytesPerSample = (bitsPerSample/8);
        int byteIndex=0;
        //This algorithm is adding channels one after the other instead of interleaving channels
        //This makes playback much easier...
        for(int j = 0; j < noOfChannels; j++)
        {
           byteIndex=(j*(bytesPerSample)); //index for reading from byte array
           for(int i=0; i<noOfSamples;i++) 
	   {
	        floatArray[i + (j*noOfSamples)] =  bytesToFloat(bytes[byteIndex],bytes[byteIndex+1], bytes[byteIndex+2], bytes[byteIndex+3]); 
                byteIndex = byteIndex +(bytesPerSample*noOfChannels);
           }
        }
    }
    else{
        return -1; 
    }
}



//16bit bytes to float
float Wavefile::bytesToFloat(BYTE firstByte, BYTE secondByte) 
{
    // Convert 2 bytes to one short, little endian.
    short s = (secondByte << 8) | firstByte;
    
    return s / 32767.0; //Divide by max short value to get a -1 to 1 float value
}
//24bit bytes to float , not tested this might not work because int is 32bits
float Wavefile::bytesToFloat(BYTE firstByte, BYTE secondByte, BYTE thirdByte) 
{
    // Convert 3 bytes to one int, little endian.
    int s = (thirdByte << 24) | (secondByte << 16) | firstByte<<8;
    
    return s / (float)(INT_MAX - 256); //Divide by max 24bit int value to get a -1 to 1 float value
}
//32bit bytes to float
float Wavefile::bytesToFloat(BYTE firstByte, BYTE secondByte, BYTE thirdByte, BYTE fourthByte) 
{
    if(waveFormat==1) //PCM (int)
    {
    // Convert 4 bytes to one int, little endian. 
    int s = (fourthByte << 24) | (thirdByte << 16) | (secondByte << 8) | firstByte;
    
    return s / (float)INT_MAX; //Divide by max int value to get a -1 to 1 float value
    
    }else if(waveFormat==3)//Float
    { 
        float f = (fourthByte << 24) | (thirdByte << 16) | (secondByte << 8) | firstByte;
        return f;
    }
}

void Wavefile::close()
{
	fclose(wave);    
}
