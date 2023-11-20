/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class FMSynthMiniProjectAudioProcessorEditor  : public juce::AudioProcessorEditor, public Slider::Listener
{
public:
    FMSynthMiniProjectAudioProcessorEditor (FMSynthMiniProjectAudioProcessor&);
    ~FMSynthMiniProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (Slider* slider) override;
    
    void buttonStateChanged (Button* button);
    
    void setSliderAndLabelRotary(juce::Slider& slider, juce::Label& label, double minLim, double maxLim, double step, double defaultValue, const String &Text);
    
    void setSliderAndLabelVertical(juce::Slider& slider, juce::Label& label, double minLim, double maxLim, double step, double defaultValue, const String &Text);
    
    

private:
    
    Slider modulatorRatioSlider;
    Label modulatorRatioLabel;
    Slider modulatorFeedbackSlider;
    Label modulatorFeedbackLabel;
    Slider fmIndexOneSlider;
    Label fmIndexOneLabel;
    Slider fmIndexTwoSlider;
    Label fmIndexTwoLabel;
    
    Slider carrierTwoTuneSlider;
    Label carrierTwoTuneLabel;
    Slider carrierTwoScaleAmpSlider;
    Label carrierTwoScaleAmpLabel;
    Slider carrierTwoScaleIndexSlider;
    Label carrierTwoScaleIndexLabel;
    
    Slider filterCutoffSlider;
    Label filterCutoffLabel;
    Slider filterResonanceSlider;
    Label filterResonanceLabel;
    Slider filterEnvAmountSlider;
    Label filterEnvAmountLabel;
    
    juce::ComboBox filterMenu;
    juce::ComboBox filterSlopeMenu;
    
    Slider ampAttackSlider;
    Label ampAttackLabel;
    Slider ampDecaySlider;
    Label ampDecayLabel;
    Slider ampSustainSlider;
    Label ampSustainLabel;
    Slider ampReleaseSlider;
    Label ampReleaseLabel;
    
    Slider fmAttackSlider;
    Label fmAttackLabel;
    Slider fmDecaySlider;
    Label fmDecayLabel;
    Slider fmSustainSlider;
    Label fmSustainLabel;
    Slider fmReleaseSlider;
    Label fmReleaseLabel;
    
    Slider filterAttackSlider;
    Label filterAttackLabel;
    Slider filterDecaySlider;
    Label filterDecayLabel;
    Slider filterSustainSlider;
    Label filterSustainLabel;
    Slider filterReleaseSlider;
    Label filterReleaseLabel;
    
    Slider pitchAttackSlider;
    Label pitchAttackLabel;
    Slider pitchDecaySlider;
    Label pitchDecayLabel;
    Slider pitchSustainSlider;
    Label pitchSustainLabel;
    Slider pitchReleaseSlider;
    Label pitchReleaseLabel;
    
    Slider pitchEnvAmountSlider;
    Label pitchEnvAmountLabel;
    
    Slider lfoRateSlider;
    Label lfoRateLabel;
    Slider lfoAmountSlider;
    Label lfoAmountLabel;
    
    juce::TextButton lfoMapCutoff {"Cutoff"};
    juce::TextButton lfoMapPitch {"Pitch"};
    juce::TextButton lfoMapAmplitude {"Amp"};
    juce::TextButton lfoMapIndex {"Index"};
    juce::TextButton lfoMapModRatio {"Mod ratio"};
    juce::TextButton lfoMapLfoRate {"LFO rate"};
    
    Slider volumeSlider;
    Label volumeLabel;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FMSynthMiniProjectAudioProcessor& audioProcessor;
    
    //FMVoice& fmVoice;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSynthMiniProjectAudioProcessorEditor)
};

