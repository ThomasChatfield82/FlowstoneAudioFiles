#include "stdafx.h"
#include <iostream>
#include <Unknwn.h>  
#include <windows.h>
#include <math.h>
#include "Wavefile.h"
#include <stdio.h>
#include "FileException.h"
#include "Waveform.h"
#include "FlowstoneMacros.h"

extern "C" __declspec(dllexport) void loadWaveFile(int nParams, int* pIn, int* pOut)
{
	if( pIn && pOut && nParams >= 2 )
	{
	    char* path = GETSTRING(pIn[0]);
            try{
            //Currently supports 16,24,32bit PCM
	    Wavefile wavefile;
	    wavefile.openWaveFile(path);
            
	   //HEADER
	    Header head;
            SamplerChunk samplerChunk;
            
            int noOfSamplesPerChannel=0;
            noOfSamplesPerChannel = wavefile.read(head, samplerChunk, pOut[5]);
            
            //Output the Header info of interest
	    GETINT(pOut[1]) = head.formatChunk.waveFormat;
	    GETINT(pOut[2]) = head.formatChunk.noOfChannels;
	    GETINT(pOut[3]) = head.formatChunk.sampleRate;
            GETINT(pOut[4]) = noOfSamplesPerChannel; 
            GETBOOL(pOut[8]) = wavefile.hasSamplerChunk();
            GETINT(pOut[9]) = samplerChunk.numSampleLoops;
            
            if(wavefile.hasSamplerChunk()) 
            {
                if(pOut[7])
                {
                    DELETEINTARRAY(pOut[6]);
                    DELETEINTARRAY(pOut[7]);
                }
                
                NEWINTARRAY(pOut[6], samplerChunk.numSampleLoops);
                NEWINTARRAY(pOut[7], samplerChunk.numSampleLoops);
                
                int* start = GETINTARRAY(pOut[6]);
                int* end = GETINTARRAY(pOut[7]);
                
                for(int i=0; i < samplerChunk.numSampleLoops; i++)
                {
                    start[i] = (int)samplerChunk.loopPoints[i].start;
                    end[i] = (int)samplerChunk.loopPoints[i].end;
                }
                
                delete [] samplerChunk.loopPoints;
            }
            
            wavefile.close();
            
            }catch(FileException& ex){
                
            }
	}
}

extern "C" __declspec(dllexport) void minMax(int nParams, int* pIn, int* pOut)
{
            float* wave = GETFLOATARRAY(pIn[0]);
            int size = pIn[1];
	    //MIN/MAX ARRAYS FOR DISPLAY 
	    int displaySize=300; //This is hard coded as a reasonable amount of sample points to display a waveform. 
           
	    //Create output arrays once
            if(pOut[1]==NULL)
            {
                NEWFLOATARRAY(pOut[2],displaySize);
	        NEWFLOATARRAY(pOut[3],displaySize);
                NEWFLOATARRAY(pOut[4],displaySize);
	        NEWFLOATARRAY(pOut[5],displaySize);
            }

	    float* min = GETFLOATARRAY(pOut[2]);
	    float* max = GETFLOATARRAY(pOut[3]);
            //Do min max but with no scroll, needed for full display of waveform regardless of scroll or zoom
            float* minStatic = GETFLOATARRAY(pOut[4]);
	    float* maxStatic = GETFLOATARRAY(pOut[5]);
            
            float scroll = GETFLOAT(pIn[6]);
            float zoom = GETFLOAT(pIn[7]); 
            
            Waveform waveform;
            
            waveform.setWaveform(wave, size);
            
            waveform.minMaxForDisplay(min,max,scroll,zoom,displaySize);
            
            waveform.minMaxForDisplay(minStatic,maxStatic,displaySize);
            
}

extern "C" __declspec(dllexport) void writeWavefile(int nParams, int* pIn, int* pOut)
{
    
}