/*
  ==============================================================================

    FMSynth.cpp
    Created: 24 Dec 2021 5:07:09am
    Author:  Benjamin Støier

  ==============================================================================
*/

#include "FMSynth.h"

std::vector<float> FMSynth::generateFmWavetable()
{
    constexpr auto  WAVETABLE_LENGTH = 64;
    
    std::vector<float> fmWavetable(WAVETABLE_LENGTH);
    
    const auto PI = std::atanf(1.f) *4;
    
    for (auto i=0; i <WAVETABLE_LENGTH; ++i)
    {
        fmWavetable[i] = std::sinf(2 * PI * static_cast<float>(i)/static_cast<float>(WAVETABLE_LENGTH));
    }
    
    return fmWavetable;
}

void FMSynth::initOscillators()
{
    //We have 128 MIDI notes available
    constexpr auto OSC_COUNT = 128;
    
    const auto wavetable = generateFmWavetable();
    
    oscillators.clear();
    for (auto i=0; i < OSC_COUNT; ++i)
    {
        oscillators.emplace_back(wavetable, sampleRate);
    }
    
}

void FMSynth::prepareToPlay(double sampleRate)
{
    //store the sample rate from PluginProcessor
    this->sampleRate = sampleRate;
    initOscillators();
}

void FMSynth::processBlock(juce::AudioBuffer<float> & buffer, juce::MidiBuffer & midiMessages)
{
    //Code is from WolfSound
    //It handles ..
    auto currentSample = 0;

    for (const auto midiMessage : midiMessages)
    {
        const auto midiEvent = midiMessage.getMessage();
        const auto midiEventSample= static_cast<int>(midiEvent.getTimeStamp());

        render(buffer, currentSample, midiEventSample);
        currentSample = midiEventSample;
        handleMidiEvent(midiEvent);
    }

    render(buffer, currentSample, buffer.getNumSamples());
}

void FMSynth::render(juce::AudioBuffer<float> &buffer, int startSample, int endSample)
{
    auto* firstChannel = buffer.getWritePointer(0);
    
    for (auto& oscillator: oscillators)
    {
        if (oscillator.isPlaying())
        {
            for ( auto sample = startSample; sample < endSample; ++sample)
            {
                firstChannel[sample] += oscillator.getSample();
            }
        }
    }
    
    for (auto channel = 1; channel < buffer.getNumChannels(); ++channel)
    {
        std::copy(firstChannel + startSample, firstChannel + endSample, buffer.getWritePointer(channel) + startSample);
    }
}

void FMSynth::handleMidiEvent(const juce::MidiMessage &midiEvent)
{
    if(midiEvent.isNoteOn())
    {
        const auto noteId =midiEvent.getNoteNumber();
        const auto frequency = midiNoteToFrequency(noteId);
        oscillators[noteId].setFrequency(frequency);
    }
    if(midiEvent.isNoteOff())
    {
        const auto noteId = midiEvent.getNoteNumber();
        oscillators[noteId].stop();
    }
    if(midiEvent.isAllNotesOff())
    {
        for (auto& oscillator : oscillators)
        {
            oscillator.stop();
        }
        
    }
}

float FMSynth::midiNoteToFrequency(int midiNote)
{
    return 440.f * std::powf(2.f, (midiNote-69.f) / 12.f);
}
