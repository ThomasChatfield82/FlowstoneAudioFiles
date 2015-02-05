#include "stdafx.h"
#include <iostream>
#include <Unknwn.h>  
#include <windows.h>
#include <math.h>
#include "Wavefile.h"
#include <stdio.h>
#include "FileException.h"
#include "Waveform.h"

//////////////////////////////////////////////////////////////////////////////////
// Helper Macros - use as you please

#define GETFLOAT(p) *((float*)&p)
#define GETBOOL(p) *((bool*)&p)
#define GETINT(p) p
#define GETSTRING(p) *((char**)&p)
#define GETFLOATARRAY(p) p ? ((float*)p+1) : 0
#define GETINTARRAY(p) p ? ((int*)p+1) : 0
#define GETSTRINGARRAY(p) p ? ((char**)p+1) : 0
#define GETARRAYSIZE(p) p ? *((int*)p) : 0
#define GETFRAME(p) p ? ((float*)p+1) : 0
#define GETFRAMESIZE(p) p ? *((int*)p) : 0
#define GETBITMAPWIDTH(p) p ? *((int*)p) : 0
#define GETBITMAPHEIGHT(p) p ? *((int*)p+1) : 0
#define GETBITMAPCHANNELS(p) p ? *((int*)p+2) : 0
#define GETBITMAPDATA(p) p ? ((BYTE*)p+12) : 0
#define GETBITMAPBYTES(p) p ? *((int*)p) * *((int*)p+1) * *((int*)p+2) : 0
#define NEWINTARRAY(p,n) if(n>0) { *((int**)&p)=new int[n+1]; ((int*)p)[0]=n; }
#define NEWFLOATARRAY(p,n) if(n>0) { *((float**)&p)=new float[n+1]; ((int*)p)[0]=n; }
#define NEWSTRINGARRAY(p,n) if(n>0) { *((char***)&p)=new char*[n+1]; ((int*)p)[0]=n; }
#define DELETESTRING(p) if(p) { delete *((char**)&p); p=0; }
#define DELETEINTARRAY(p) if(p) { delete *((int**)&p); p=0; }
#define DELETEFLOATARRAY(p) if(p) { delete *((float**)&p); p=0; }
#define DELETESTRINGARRAY(p) if(p) { for( int j=0; j<*((int*)p); j++ ) { if( ((char**)p+1)[j] ) delete ((char**)p+1)[j]; } delete *((char***)&p); p=0; }

//////////////////////////////////////////////////////////////////////////////////


extern "C" __declspec(dllexport) void loadWaveFile(int nParams, int* pIn, int* pOut)
{
	if( pIn && pOut && nParams >= 2 )
	{
	    char* path = GETSTRING(pIn[0]);
            try{
            //Currently supports 16,24,32bit PCM and 32bit float
	    Wavefile wavefile;
	    wavefile.openWaveFile(path);

	    //HEADER
	    Header head;
	    wavefile.readHeader(head);

	    //Output the Header info of interest
	    GETINT(pOut[1]) = head.formatChunk.waveFormat;
	    GETINT(pOut[2]) = head.formatChunk.noOfChannels;
	    GETINT(pOut[3]) = head.formatChunk.sampleRate;
	    GETINT(pOut[4]) = head.noOfSamples;

	    //DATA
	    NEWFLOATARRAY(pOut[5],head.noOfSamples*head.formatChunk.noOfChannels);
	    float* waveData = GETFLOATARRAY(pOut[5]);
	    //Read the audio data into a float array
	    wavefile.readWaveData(waveData);

	    //CUE
            //I'm assuming that the CueChunk always comes after the dataChunk, but it is possible in rare cases
            //that the cueChunk comes first so need to deal with that possibility at some point....
	    CueChunk cueChunk;
	    wavefile.readCueChunk(cueChunk); //If no cueChunk "noOfCuePoints" is set to zero which we can check against
			  
	    if(cueChunk.noOfCuePoints>0)
            {
		NEWINTARRAY(pOut[6],cueChunk.noOfCuePoints);
		int* cuePoints = GETINTARRAY(pOut[6]);
		for(int i =0; i< cueChunk.noOfCuePoints; i++)
		{
		    cuePoints[i] = cueChunk.cuePoints[i].sampleOffset;
		}

		delete[] cueChunk.cuePoints; //TODO Refactor wavefile class so this is deleted in that class
	    }

            wavefile.close();
            
            }catch(FileException& ex){
                
            }
	}
}

//TODO 
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
