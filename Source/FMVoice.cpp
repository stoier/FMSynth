/*
  ==============================================================================

    FMVoice.cpp
    Created: 25 Dec 2021 8:10:01am
    Author:  Benjamin Støier

  ==============================================================================
*/

#include "FMVoice.h"
#include "PluginEditor.h"

bool FMVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<FMSound*> (sound) != nullptr;
}

//Called every time a new voice is played
void FMVoice::startNote (int midiNoteNumber, float velocity,
                juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    //Apply note velocity to output level of the voice
    level = velocity * 0.1;
    
    //Get midi note number
    noteNumber = midiNoteNumber;
    
    //Get sample rate
    fs = getSampleRate();
    
    //Start envelopes
    ampAdsr.noteOn();
    fmAdsr.noteOn();
    filterAdsr.noteOn();
    pitchAdsr.noteOn();
}

void FMVoice::stopNote (float velocity, bool allowTailOff)
{
    //Stop envelopes
    ampAdsr.noteOff();
    fmAdsr.noteOff();
    filterAdsr.noteOff();
    pitchAdsr.noteOff();
    
    //Clear note if amplitude envelope is not active
    if (! ampAdsr.isActive())
    {
        clearCurrentNote();
    }
}
void FMVoice::pitchWheelMoved (int newPitchEheelValue)
{
    
}
void FMVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void FMVoice::setADSR(double sampleRate)
{
    ampAdsr.setSampleRate(sampleRate);
    ampAdsrParams.attack = ampAttack;
    ampAdsrParams.decay = ampDecay;
    ampAdsrParams.sustain = ampSustain;
    ampAdsrParams.release = ampRelease;
    ampAdsr.setParameters(ampAdsrParams);
    
    fmAdsr.setSampleRate(sampleRate);
    fmAdsrParams.attack = fmAttack;
    fmAdsrParams.decay = fmDecay;
    fmAdsrParams.sustain = fmSustain;
    fmAdsrParams.release = fmRelease;
    fmAdsr.setParameters(fmAdsrParams);
    
    filterAdsr.setSampleRate(sampleRate);
    filterAdsrParams.attack = filterAttack;
    filterAdsrParams.decay = filterDecay;
    filterAdsrParams.sustain = filterSustain;
    filterAdsrParams.release = filterRelease;
    filterAdsr.setParameters(filterAdsrParams);
    
    pitchAdsr.setSampleRate(sampleRate);
    pitchAdsrParams.attack = pitchAttack;
    pitchAdsrParams.decay = pitchDecay;
    pitchAdsrParams.sustain = pitchSustain;
    pitchAdsrParams.release = pitchRelease;
    pitchAdsr.setParameters(pitchAdsrParams);
}


void FMVoice::updateParameters(const double volumeToSet, const double modulatorRatioToSet, const double modulatorFeedbackToSet, const double fmIndexOneToSet, const double fmIndexTwoToSet, const double carrierTwoTuneToSet, const double carrierTwoAmpScaleToSet, const double carrierTwoIndexScaleToSet, const double filterCutoffToSet, const double filterResonanceToSet, const int filterMenuIdToSet, const int filterSlopeMenuIdToSet, const double ampAttackToSet, const double ampDecayToSet, const double ampSustainToSet, const double ampReleaseToSet, const double fmAttackToSet, const double fmDecayToSet, const double fmSustainToSet, const double fmReleaseToSet, const double filterAttackToSet, const double filterDecayToSet, const double filterSustainToSet, const double filterReleaseToSet, const double filterEnvelopeAmountToSet, const double pitchAttackToSet, const double pitchDecayToSet, const double pitchSustainToSet, const double pitchReleaseToSet, const double pitchEnvelopeAmountToSet, const double lfoRateToSet, const double lfoAmountToSet, const bool lfoMapCutoffToSet, const bool lfoMapPitchToSet, const bool lfoMapAmplitudeToSet, const bool lfoMapIndexToSet, const bool lfoMapModRatioToSet, const bool lfoMapLfoRateToSet)
{
    volume = volumeToSet;
    modulatorRatio = modulatorRatioToSet;
    modulatorFeedback = modulatorFeedbackToSet/100;
    fmIndexOne = fmIndexOneToSet;
    fmIndexTwo = fmIndexTwoToSet;
    carrierTwoTune = carrierTwoTuneToSet;
    carrierTwoAmpScale = carrierTwoAmpScaleToSet;
    carrierTwoIndexScale = carrierTwoIndexScaleToSet;
    filterCutoff = filterCutoffToSet;
    filterResonance = filterResonanceToSet;
    filterMenuId = filterMenuIdToSet;
    filterSlopeMenuId = filterSlopeMenuIdToSet;
    ampAttack = ampAttackToSet/1000;
    ampDecay = ampDecayToSet/1000;
    ampSustain = ampSustainToSet;
    ampRelease = ampReleaseToSet/1000;
    fmAttack = fmAttackToSet/1000;
    fmDecay = fmDecayToSet/1000;
    fmSustain = fmSustainToSet;
    fmRelease = fmReleaseToSet/1000;
    filterAttack = filterAttackToSet/1000;
    filterDecay = filterDecayToSet/1000;
    filterSustain = filterSustainToSet;
    filterRelease = filterReleaseToSet/1000;
    filterEnvAmount = filterEnvelopeAmountToSet;
    pitchAttack = pitchAttackToSet/1000;
    pitchDecay = pitchDecayToSet/1000;
    pitchSustain = pitchSustainToSet;
    pitchRelease = pitchReleaseToSet/1000;
    pitchEnvAmount = pitchEnvelopeAmountToSet;
    lfoRate = lfoRateToSet;
    lfoAmount = lfoAmountToSet/100;
    lfoMapCutoff = lfoMapCutoffToSet;
    lfoMapPitch = lfoMapPitchToSet;
    lfoMapAmplitude = lfoMapAmplitudeToSet;
    lfoMapIndex = lfoMapIndexToSet;
    lfoMapModRatio = lfoMapModRatioToSet;
    lfoMapLfoRate = lfoMapLfoRateToSet;
    
    //Convert filter envelope amount percentage to frequency
    if (0 < filterEnvAmount)
        filterEnvAmount = (20000 - filterCutoff) * filterEnvAmount*0.01;
    if (filterEnvAmount < 0)
        filterEnvAmount = (filterCutoff - 20) * filterEnvAmount * 0.01;
}

// Implementation of the limiter
float FMVoice::limit (float val, float min, float max)
{
    if (val < min)
        return min;
    else if (val > max)
        return max;
    else
        return val;
}

void FMVoice::renderNextBlock(juce::AudioSampleBuffer &outputBuffer, int startSample, int numSamples)
{
    if (noteNumber != 0)
    {
        while (--numSamples >= 0)
        {
            //Get number of cycles pr. sample the number of cycles pr. sample for the 1. carrier
            auto cyclesPerSampleCarrier = juce::MidiMessage::getMidiNoteInHertz(noteNumber) / fs;
            //Get number of cycles pr. sample the number of cycles pr. sample for the 2. carrier
            auto cyclesPerSampleCarrierTwo = juce::MidiMessage::getMidiNoteInHertz(noteNumber + carrierTwoTune) / fs;
            
            //Get number of cycles pr. sample the number of cycles pr. sample for the LFO
            auto cyclesPerSampleLfo= lfoRate / fs;
            
            //Update the amount that the phase angle of the LFO needs to increment for each sample
            angleDeltaLfo = cyclesPerSampleLfo * 2.0 * juce::MathConstants<double>::pi;
            
            if(lfoMapLfoRate == false)
            {
                currentLfo = std::sin(currentAngleLfo)*lfoAmount;
            }
            else
            {
                currentLfo = std::sin(currentAngleLfo+currentLfoFeedback)*lfoAmount;
            }
            
            //If pitch envelope is applied or if LFO is mapped to pitch
            if (pitchEnvAmount != 0 || lfoMapPitch == true)
            {
                //convert midi notes (midi input, pitch envelope, lfo) to frequency for both carrier 1 and carrier 2
                auto pitchModCarrier = 440 *std::powf(2, ((noteNumber+pitchEnvAmount*pitchAdsr.getNextSample()+(currentLfo*12)) - 69) / 12);
                auto pitchModCarrierTwo = 440 *std::powf(2, ((noteNumber + carrierTwoTune+pitchEnvAmount*pitchAdsr.getNextSample()+(currentLfo*12)) - 69) / 12);
                
                if (pitchEnvAmount == 0)
                {
                    pitchModCarrier = 440 *std::powf(2, ((noteNumber+(currentLfo*12)) - 69) / 12);
                    pitchModCarrierTwo = 440 *std::powf(2, ((noteNumber + carrierTwoTune) - 69) / 12);
                }
                if (lfoMapPitch == false)
                {
                    pitchModCarrier = 440 *std::powf(2, ((noteNumber+pitchEnvAmount*pitchAdsr.getNextSample()) - 69) / 12);
                    pitchModCarrierTwo = 440 *std::powf(2, ((noteNumber + carrierTwoTune+pitchEnvAmount*pitchAdsr.getNextSample()) - 69) / 12);
                }
                //Get number of cycles pr. sample
                cyclesPerSampleCarrier = pitchModCarrier / fs;
                cyclesPerSampleCarrierTwo = pitchModCarrierTwo / fs;
            }
            
            //Update the amount that the phase angle of the 1. carrier needs to increment for each sample
            angleDeltaCarrier = cyclesPerSampleCarrier * 2.0 * juce::MathConstants<double>::pi;
            //Update the amount that the phase angle of the 2. carrier needs to increment for each sample
            angleDeltaCarrierTwo = cyclesPerSampleCarrierTwo * 2.0 * juce::MathConstants<double>::pi;
            
            if(lfoMapModRatio == true)
            {
                modulatorRatio = modulatorRatio + (currentLfo/10 * modulatorRatio);
            }
            //Update the amount that the phase angle of the modulator needs to increment for each sample
            angleDeltaModulator = cyclesPerSampleCarrier * modulatorRatio * 2.0 * juce::MathConstants<double>::pi;
            
            auto fmIndex = fmIndexOne + (fmIndexTwo-fmIndexOne)*fmAdsr.getNextSample();
            
            if (lfoMapIndex== true)
            {
                fmIndex = fmIndex + (currentLfo/10 * fmIndex);
            }
            
            //Calculate current sample of the modulator oscillator
            auto currentFm = std::sin(currentAngleModulator+currentModulatorFeedback)*fmIndex;
            //Calculate current sample of the first carrier osclillator
            auto currentCarrier = std::sin (currentAngleCarrier + currentFm) * level * ampAdsr.getNextSample();
            //Calculate current sample of the second carrier osclillator
            auto currentCarrierTwo = std::sin (currentAngleCarrierTwo + currentFm * carrierTwoIndexScale) * level * ampAdsr.getNextSample()*carrierTwoAmpScale;
            
            if(lfoMapAmplitude == true)
            {
                volume = volume * (1 + currentLfo*volume/10);
            }
            
            //If lfo is mapped to the cutoff frequency
            if(lfoMapCutoff == true)
            {
                filterCutoff = filterCutoff + (currentLfo * filterCutoff / 20);
                if (filterCutoff > 20000)
                    filterCutoff = 20000;
                if (filterCutoff < 20)
                    filterCutoff = 20;
            }
            
            // Calculate angular centerfrequency divided by sample rate
            auto w0 = (2.0*juce::MathConstants<double>::pi*(filterCutoff + filterAdsr.getNextSample()*filterEnvAmount))/ fs;
            
            //Calculate alpha and a coefficients (same for all filter types)
            auto alpha = std::sin(w0)/(2.0*filterResonance);
            auto a0 = 1.0 + alpha;
            auto a1 = (-2.0*std::cos(w0))/a0;
            auto a2= (1.0-alpha)/a0;
            
            if (filterMenuId == 1)
            {
                //Calculate filter coefficients for biquad lowpass filter
                b0 = ((1.0-std::cos(w0))/2.0)/a0;
                b1 = (1.0-std::cos(w0))/a0;
                b2 = ((1.0-std::cos(w0))/2.0)/a0;
            }
            //If filter is highpass
            if  (filterMenuId == 2)
            {
                //Calculate filter coefficients for biquad highpass filter
                b0 = ((1.0+std::cos(w0))/2.0)/a0;
                b1 = -(1.0+std::cos(w0))/a0;
                b2 = ((1.0+std::cos(w0))/2.0)/a0;
            }
            //If filter is bandpass
            if (filterMenuId == 3)
            {
                //Calculate filter coefficients for biquad bandpass filter
                b0 = filterResonance*alpha;
                b1 = 0;
                b2 = -filterResonance*alpha;
            }
            
            
            
            auto filterIn = currentCarrier+currentCarrierTwo;
            auto filterOut= b0*filterIn+b1*lastInSampleOne+b2*lastInSampleTwo-a1*lastOutSampleOne-a2*lastOutSampleTwo;
            
            // get x[n-2]
            lastInSampleTwo = lastInSampleOne;
            // get x[n-1]
            lastInSampleOne = filterIn;
            // get y[n-2]
            lastOutSampleTwo = lastOutSampleOne;
            // get y[n-1]
            lastOutSampleOne = filterOut;
            
            //if 24db/octave slope is selected. Cascade the biquad filter, with a biquad second filter with the same coefficients
            if (filterSlopeMenuId == 2)
            {
                auto filterOut24dB = b0*filterOut+b1*lastInSampleOne24db+b2*lastInSampleTwo24db-a1*lastOutSampleOne24db-a2*lastOutSampleTwo24db;
                
                lastInSampleTwo24db = lastInSampleOne24db;
            
                lastInSampleOne24db = filterOut;
            
                lastOutSampleTwo24db = lastOutSampleOne24db;
            
                lastOutSampleOne24db = filterOut24dB;
                
                currentSample = limit((float) filterOut24dB*volume, -0.125, 0.125);
            }
            //if 12db/octave slope is selected
            else
            {
                currentSample = limit((float) filterOut*volume, -0.125, 0.125);
            }

            
            for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample (i, startSample, currentSample);
            
            currentModulatorFeedback = currentFm*modulatorFeedback;
            currentAngleCarrier += angleDeltaCarrier;
            currentAngleModulator += angleDeltaModulator;
            currentAngleCarrierTwo += angleDeltaCarrierTwo;
            currentLfoFeedback = std::sin(currentAngleLfo)*lfoAmount*10;
            currentAngleLfo += angleDeltaLfo;

            
            ++startSample;
        }
        
    }
}
