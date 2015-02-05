#include "Waveform.h"
#include "math.h"
#include <iostream>
#include <stdio.h>

Waveform::Waveform()
{
    
}

Waveform::~Waveform()
{
    
}

void Waveform::setWaveform(float* wave, int noOfSamples)
{
    this->wave=wave;
    this->noOfSamples=noOfSamples;   
}

void Waveform::minMaxForDisplay(float* minA, float* maxA, int size)
{
    int step=0;
	int frameSize = (noOfSamples)/size;
	float mn = 1;
	float mx = -1;
	int x=0;
        //Just read from first channel
	for(int i=0; i < noOfSamples; i++) 
	{
	    step++;
		 
            mx = wave[i] > mx ? wave[i] : mx;
	    mn = wave[i] < mn ? wave[i] : mn;
		
            if(step>=frameSize)
	   {
		minA[std::min(x,size-1)] = mn;
		maxA[std::min(x,size-1)] = mx;
		
		step = 0;
		mx = -1;
		mn = 1;
		x++;		
	    }
	}
}

void Waveform::minMaxForDisplay(float* minA, float* maxA, float scroll, float zoom, int size)
{
    int step=0;
	
	float mn = 1;
	float mx = -1;
	int x=0;
        int length = noOfSamples*zoom;
        int frameSize = length/size;
        int scrollSamples = std::min((noOfSamples  * scroll), (float)(noOfSamples-length));
        
        //Just read from first channel
	for(int i=scrollSamples; i < std::min(length + scrollSamples,noOfSamples); i++) 
	{
	    step++;
		 
            mx = wave[i] > mx ? wave[i] : mx;
	    mn = wave[i] < mn ? wave[i] : mn;
		
            if(step>=frameSize)
	   {
		minA[std::min(x,size-1)] = mn;
		maxA[std::min(x,size-1)] = mx;
		
		step = 0;
		mx = -1;
		mn = 1;
		x++;		
	    }
	}
}