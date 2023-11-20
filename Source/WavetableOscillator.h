/*
  ==============================================================================

    WavetableOscillator.h
    Created: 25 Dec 2021 4:08:20am
    Author:  Benjamin Støier

  ==============================================================================
*/

#pragma once
#include <vector>

class WavetableOscillator
{
public:
    WavetableOscillator(std::vector<float> wavetable, double sampleRate);
    
    void setFrequency(float frequency);
    float getSample();
    void stop();
    bool isPlaying();
    
private:
    float interpolateLinearly();
    std::vector<float> wavetable;
    double sampleRate;
    float index = 0.f;
    float indexIncrement = 0.f;
    
};
