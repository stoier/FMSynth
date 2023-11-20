/*
  ==============================================================================

    FMSynth.h
    Created: 24 Dec 2021 5:07:09am
    Author:  Benjamin Støier

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "WavetableOscillator.h"

class FMSynth{
public:
    void prepareToPlay(double sampleRate);
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);
private:
    double sampleRate;
    std::vector<WavetableOscillator> oscillators;
    std::vector<float> generateFmWavetable();
    
    void initOscillators();
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    float midiNoteToFrequency(int midiNote);
    void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
};
