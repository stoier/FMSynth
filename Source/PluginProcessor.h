/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "FMSynth.h"
#include "FMVoice.h"
//==============================================================================
/**
*/

class FMSynthMiniProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FMSynthMiniProjectAudioProcessor();
    ~FMSynthMiniProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    /* Adding parameter control [6]: Create a "setter" function
    
            Rather than making the member variables of your class public, it is good practice to make them private and access them with public "getter/setter" functions.
     
            In order for the slider in the Editor to change the private "gain" variable of this class, a "setter" function must be created.
     */
    void setModulatorRatio (double modulatorRatioToSet) { modulatorRatio = modulatorRatioToSet; };
    void setModulatorFeedback (double modulatorFeedbackToSet) {modulatorFeedback = modulatorFeedbackToSet;};
    void setFmIndexOne (double fmIndexOneToSet) {fmIndexOne = fmIndexOneToSet;};
    void setFmIndexTwo (double fmIndexTwoToSet) {fmIndexTwo = fmIndexTwoToSet;};
    
    void setCarrierTwoTune(double carrierTwoTuneToSet) { carrierTwoTune = carrierTwoTuneToSet; };
    void setCarrierTwoAmpScale(double carrierTwoAmpScaleToSet) { carrierTwoAmpScale = carrierTwoAmpScaleToSet; };
    void setCarrierTwoIndexScale(double carrierTwoIndexScaleToSet) { carrierTwoIndexScale = carrierTwoIndexScaleToSet; };
    
    void setFilterCutoff(double filterCutoffToSet){filterCutoff = filterCutoffToSet;};
    void setFilterResonance(double filterResonanceToSet){filterResonance = filterResonanceToSet;};
    void setFilterEnvelopeAmount(double filterEnvelopeAmountToSet){filterEnvelopeAmount = filterEnvelopeAmountToSet;};
    
    void setAmpAttack(double ampAttackToSet) {ampAttack = ampAttackToSet;};
    void setAmpDecay(double ampDecayToSet) {ampDecay = ampDecayToSet;};
    void setAmpSustain(double ampSustainToSet) {ampSustain = ampSustainToSet;};
    void setAmpRelease(double ampReleaseToSet) {ampRelease = ampReleaseToSet;};
    
    void setFmAttack(double fmAttackToSet) {fmAttack = fmAttackToSet;};
    void setFmDecay(double fmDecayToSet) {fmDecay = fmDecayToSet;};
    void setFmSustain(double fmSustainToSet) {fmSustain = fmSustainToSet;};
    void setFmRelease(double fmReleaseToSet) {fmRelease = fmReleaseToSet;};
    
    void setFilterAttack(double filterAttackToSet) {filterAttack = filterAttackToSet;};
    void setFilterDecay(double filterDecayToSet) {filterDecay = filterDecayToSet;};
    void setFilterSustain(double filterSustainToSet) {filterSustain = filterSustainToSet;};
    void setFilterRelease(double filterReleaseToSet) {filterRelease = filterReleaseToSet;};
    
    void setPitchAttack(double pitchAttackToSet) {pitchAttack = pitchAttackToSet;};
    void setPitchDecay(double pitchDecayToSet) {pitchDecay = pitchDecayToSet;};
    void setPitchSustain(double pitchSustainToSet) {pitchSustain = pitchSustainToSet;};
    void setPitchRelease(double pitchReleaseToSet) {pitchRelease = pitchReleaseToSet;};
    void setPitchEnvelopeAmount(double pitchEnvelopeAmountToSet){pitchEnvelopeAmount = pitchEnvelopeAmountToSet;};
    
    void setLfoRate (double lfoRateToSet) { lfoRate = lfoRateToSet; };
    void setLfoAmount (double lfoAmountToSet) { lfoAmount = lfoAmountToSet; };
    
    void setVolume (double volumeToSet) { volume = volumeToSet; };
    
    void setLfoMapCutoff (bool lfoMapCutoffToSet) {lfoMapCutoff = lfoMapCutoffToSet;};
    void setLfoMapPitch (bool lfoMapPitchToSet) {lfoMapPitch = lfoMapPitchToSet;};
    void setLfoMapAmplitude (bool lfoMapAmplitudeToSet) {lfoMapAmplitude = lfoMapAmplitudeToSet;};
    void setLfoMapIndex (bool lfoMapIndexToSet) {lfoMapIndex = lfoMapIndexToSet;};
    void setLfoMapModRatio (bool lfoMapModRatioToSet) {lfoMapModRatio = lfoMapModRatioToSet;};
    void setLfoMapLfoRate (bool lfoMapLfoRateToSet) {lfoMapLfoRate = lfoMapLfoRateToSet;};
    

    double volume = 0.5;
    double modulatorRatio = 2;
    double modulatorFeedback = 10;
    double carrierTwoTune = 12;
    double carrierTwoAmpScale = 0.5;
    double carrierTwoIndexScale = 0.5;
    double filterCutoff = 20000;
    double filterResonance = 0.707;
    double filterEnvelopeAmount = 0;
    int filterMenuId = 1;
    int filterMenuSlopeId = 1;
    double fmIndexOne = 5;
    double fmIndexTwo = 15;
    double ampAttack= 50, ampDecay= 50, ampSustain=1, ampRelease=100;
    double fmAttack= 1000, fmDecay= 200, fmSustain=0.5, fmRelease=100;
    double filterAttack=50, filterDecay=50, filterSustain=1, filterRelease=100;
    double pitchAttack=50, pitchDecay=50, pitchSustain=1, pitchRelease=100;
    double pitchEnvelopeAmount = 0;
    double lfoRate = 10;
    double lfoAmount = 0;
    bool lfoMapCutoff = false;
    bool lfoMapPitch = false;
    bool lfoMapAmplitude = false;
    bool lfoMapIndex = false;
    bool lfoMapModRatio = false;
    bool lfoMapLfoRate = false;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSynthMiniProjectAudioProcessor)
    
    double fs; // to be obtained from the "prepareToPlay()" function
    
    // Sample index for the sinusoid
    unsigned long n = 0;
    
    //FMSynth FMSynth;
    juce::Synthesiser fmSynth;
    FMVoice fmVoice;
};
