/* 
 * File:   Waveform.h
 * Author: Thomas
 *
 * Created on 20 January 2015, 22:04
 */

#ifndef WAVEFORM_H
#define	WAVEFORM_H

class Waveform
{
private:
    float* wave;
    int noOfSamples;
public:
    Waveform();
    ~Waveform();
    void setWaveform(float* wave,int size);
    void minMaxForDisplay(float* minDisplay, float* maxDisplay, int size);
    void minMaxForDisplay(float* minDisplay, float* maxDisplay, float scroll, float zoom, int size);
    
};

#endif	/* WAVEFORM_H */

