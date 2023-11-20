/*
  ==============================================================================

    FMVoice.h
    Created: 25 Dec 2021 8:10:01am
    Author:  Benjamin Støier

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>


struct FMSound   : public juce::SynthesiserSound
{
public:
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};

//==============================================================================
struct FMVoice   : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity,
                    juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;

    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchEheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
    
    void setADSR(double sampleRate);
    
    //Updates all parameters from sliders
    void updateParameters(const double volumeToSet, const double modulatorRatioToSet, const double modulatorFeedbackToSet, const double fmIndexOneToSet, const double fmIndexTwoToSet, const double carrierTwoTuneToSet, const double carrierTwoAmpScaleToSet, const double carrierTwoIndexScaleToSet, const double filterCutoffToSet, const double filterResonanceToSet, const int filterMenuIdToSet, const int filterSlopeMenuIdToSet, const double ampAttackToSet, const double ampDecayToSet, const double ampSustainToSet, const double ampReleaseToSet, const double fmAttackToSet, const double fmDecayToSet, const double fmSustainToSet, const double fmReleaseToSet, const double filterAttackToSet, const double filterDecayToSet, const double filterSustainToSet, const double filterReleaseToSet, const double filterEnvelopeAmountToSet, const double pitchAttackToSet, const double pitchDecayToSet, const double pitchSustainToSet, const double pitchReleaseToSet, const double pitchEnvelopeAmountToSet, const double lfoRateToSet, const double lfoAmountToSet, const bool lfoMapCutoffToSet, const bool lfoMapPitchToSet, const bool lfoMapAmplitudeToSet, const bool lfoMapIndexToSet, const bool lfoMapModRatioToSet, const bool lfoMapLfoRateToSet);
    
    // Limiter to keep the output signal between -1 and 1. Arguments are: input value, minimum value and maximum value
    float limit (float val, float min, float max);
    
private:
    juce::ADSR ampAdsr;
    juce::ADSR::Parameters ampAdsrParams;
    
    juce::ADSR fmAdsr;
    juce::ADSR::Parameters fmAdsrParams;
    
    juce::ADSR filterAdsr;
    juce::ADSR::Parameters filterAdsrParams;
    
    juce::ADSR pitchAdsr;
    juce::ADSR::Parameters pitchAdsrParams;
    
    juce::Random random;
    
    int noteNumber =0;
    
    double fs = getSampleRate();

    double devOne = 0;
    double devTwo = 0;
    
    double volume = 0.5;
    double modulatorRatio = 2;
    double modulatorFeedback = 10;
    
    double carrierTwoTune = 2;
    double carrierTwoAmpScale = 0.5;
    double carrierTwoIndexScale = 0.5;
    
    double fmIndexOne = 0;
    double fmIndexTwo = 0;
    
    double filterCutoff = 20000;
    double filterResonance = 0.707;
    
    double ampAttack = 0.05;
    double ampDecay = 0.05;
    double ampSustain = 1;
    double ampRelease = 0.1;
    double fmAttack = 1;
    double fmDecay = 0.2;
    double fmSustain = 0.5;
    double fmRelease = 0.1;
    double filterAttack = 0.05;
    double filterDecay = 0.05;
    double filterSustain = 1;
    double filterRelease = 0.1;
    double filterEnvAmount = 0.0;
    int filterMenuId = 1;
    double b0=0, b1=0, b2=0;
    int filterSlopeMenuId = 1;
    double pitchAttack=50, pitchDecay=50, pitchSustain=1, pitchRelease=100;
    double pitchEnvAmount = 0;
    double lfoRate = 10;
    double lfoAmount = 0;
    bool lfoMapCutoff = false;
    bool lfoMapPitch = false;
    bool lfoMapAmplitude = false;
    bool lfoMapIndex = false;
    bool lfoMapModRatio = false;
    bool lfoMapLfoRate = false;
    double currentLfo = 0.0;
    double currentLfoFeedback = 0.0;
    
    bool isPrepared {false};
    
    double currentSample = 0.0;
    double currentAngleCarrier = 0.0;
    double angleDeltaCarrier = 0.0;
    double currentAngleModulator = 0.0;
    double angleDeltaModulator = 0.0;
    double currentModulatorFeedback = 1.0;
    double currentAngleCarrierTwo = 0.0;
    double angleDeltaCarrierTwo = 0.0;

    double currentAngleLfo = 0.0;
    double angleDeltaLfo = 0.0;
    
    double lastInSampleOne = 0.0;
    double lastInSampleTwo = 0.0;
    double lastOutSampleOne = 0.0;
    double lastOutSampleTwo = 0.0;
    
    double lastInSampleOne24db = 0.0;
    double lastInSampleTwo24db = 0.0;
    double lastOutSampleOne24db = 0.0;
    double lastOutSampleTwo24db = 0.0;
    
    double level = 0.0;
    double tailOff = 0.0;
};
