/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 25 Dec 2021 4:08:20am
    Author:  Benjamin Støier

  ==============================================================================
*/

#include "WavetableOscillator.h"
#include <cmath>


WavetableOscillator::WavetableOscillator(std::vector<float> wavetable, double sampleRate)
: wavetable{ std::move(wavetable) },
   sampleRate{ sampleRate }
{}

void WavetableOscillator::setFrequency(float frequency)
{
    // static_cast converts types to float
    indexIncrement = frequency * static_cast<float>(wavetable.size())/ static_cast<float>(sampleRate);
}

float WavetableOscillator::getSample()
{
    const auto sample = interpolateLinearly();
    index += indexIncrement;
    index = std::fmod(index, static_cast<float>(wavetable.size()));
    return sample;
}

float WavetableOscillator::interpolateLinearly()
{
    //truncates index by converting floating point to integer
    const auto truncatedIndex = static_cast<int>(index);
    const auto nextIndex = (truncatedIndex + 1) % static_cast<int>(wavetable.size());
    const auto nextIndexWeight = index -  static_cast<float>(truncatedIndex);
    const auto truncatedIndexWeight = 1.f - nextIndexWeight;
    
    return truncatedIndexWeight * wavetable[truncatedIndex] + nextIndexWeight * wavetable[nextIndex];
}

//resets both index and indexIncrement to zero
void WavetableOscillator::stop()
{
    index = 0;
    indexIncrement = 0;
}

//The oscillator is playing if the indexIncrement is not equal to zero
bool WavetableOscillator::isPlaying()
{
    return indexIncrement != 0;
}
