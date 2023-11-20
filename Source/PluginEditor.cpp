/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FMVoice.h"

//==============================================================================
FMSynthMiniProjectAudioProcessorEditor::FMSynthMiniProjectAudioProcessorEditor (FMSynthMiniProjectAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    
    setSliderAndLabelRotary(modulatorRatioSlider, modulatorRatioLabel, 0.01, 24, 0.01, audioProcessor.modulatorRatio, "Ratio");
    addAndMakeVisible (modulatorRatioSlider);
    addAndMakeVisible(modulatorRatioLabel);
    
    setSliderAndLabelRotary(modulatorFeedbackSlider, modulatorFeedbackLabel, 0, 100, 1, audioProcessor.modulatorFeedback, "Feedback");
    addAndMakeVisible (modulatorFeedbackSlider);
    modulatorFeedbackSlider.setTextValueSuffix(" %");
    addAndMakeVisible(modulatorFeedbackLabel);
    
    setSliderAndLabelRotary(fmIndexOneSlider, fmIndexOneLabel, 0, 50, 0.1, audioProcessor.fmIndexOne, "Index 1");
    addAndMakeVisible (fmIndexOneSlider);
    addAndMakeVisible(fmIndexOneLabel);
    
    setSliderAndLabelRotary(fmIndexTwoSlider, fmIndexTwoLabel, 0, 50, 0.1, audioProcessor.fmIndexTwo, "Index 2");
    addAndMakeVisible (fmIndexTwoSlider);
    addAndMakeVisible(fmIndexTwoLabel);
    
    setSliderAndLabelRotary(carrierTwoTuneSlider, carrierTwoTuneLabel, -48, 48, 1,  audioProcessor.carrierTwoTune, "Tune");
    carrierTwoTuneSlider.setTextValueSuffix(" st");
    addAndMakeVisible(carrierTwoTuneSlider);
    addAndMakeVisible(carrierTwoTuneLabel);
    
    setSliderAndLabelRotary(carrierTwoScaleAmpSlider, carrierTwoScaleAmpLabel, 0, 4, 0.01, audioProcessor.carrierTwoAmpScale, "Amp scale");
    addAndMakeVisible(carrierTwoScaleAmpSlider);
    addAndMakeVisible(carrierTwoScaleAmpLabel);
    
    setSliderAndLabelRotary(carrierTwoScaleIndexSlider, carrierTwoScaleIndexLabel, 0, 10, 0.01, audioProcessor.carrierTwoIndexScale, "Index scale");
    addAndMakeVisible(carrierTwoScaleIndexSlider);
    addAndMakeVisible(carrierTwoScaleIndexLabel);
    
    setSliderAndLabelRotary(filterCutoffSlider, filterCutoffLabel, 20, 20000, 1, audioProcessor.filterCutoff, "Cutoff");
    filterCutoffSlider.setTextValueSuffix(" Hz");
    filterCutoffSlider.setSkewFactor(0.40);
    addAndMakeVisible(filterCutoffSlider);
    addAndMakeVisible(filterCutoffLabel);
    
    setSliderAndLabelRotary(filterResonanceSlider, filterResonanceLabel, 0.01, 10, 0.01, audioProcessor.filterResonance, "Resonance");
    addAndMakeVisible(filterResonanceSlider);
    addAndMakeVisible(filterResonanceLabel);
    
    setSliderAndLabelRotary(filterEnvAmountSlider, filterEnvAmountLabel, -100, 100, 1, audioProcessor.filterEnvelopeAmount, "Env Amount");
    filterEnvAmountSlider.setTextValueSuffix(" %");
    addAndMakeVisible(filterEnvAmountSlider);
    addAndMakeVisible(filterEnvAmountLabel);
    
    filterMenu.addItem("Lowpass", 1);
    filterMenu.addItem("Highpass", 2);
    filterMenu.addItem("Bandpass", 3);
    filterMenu.setSelectedId (audioProcessor.filterMenuId);
    filterMenu.onChange = [this]
    {
        switch (filterMenu.getSelectedId())
        {
            case 1: audioProcessor.filterMenuId = 1; break;
            case 2: audioProcessor.filterMenuId = 2; break;
            case 3: audioProcessor.filterMenuId = 3; break;
            default: break;
        };
    };
    addAndMakeVisible(filterMenu);
    
    filterSlopeMenu.addItem("12 dB/oct", 1);
    filterSlopeMenu.addItem("24 dB/oct", 2);
    filterSlopeMenu.setSelectedId (audioProcessor.filterMenuSlopeId);
    filterSlopeMenu.onChange = [this]
    {
        switch (filterSlopeMenu.getSelectedId())
        {
            case 1: audioProcessor.filterMenuSlopeId = 1; break;
            case 2: audioProcessor.filterMenuSlopeId = 2; break;
            default: break;
        };
    };
    addAndMakeVisible(filterSlopeMenu);
    
    setSliderAndLabelRotary(volumeSlider, volumeLabel, 0, 1, 0.01, audioProcessor.volume, "Volume");
    addAndMakeVisible (volumeSlider);
    addAndMakeVisible(volumeLabel);
    
    setSliderAndLabelVertical(ampAttackSlider, ampAttackLabel, 1, 5000, 1, audioProcessor.ampAttack, "Attack");
    ampAttackSlider.setTextValueSuffix(" ms");
    ampAttackSlider.setSkewFactor(0.4);
    addAndMakeVisible(ampAttackSlider);
    addAndMakeVisible(ampAttackLabel);
    
    setSliderAndLabelVertical(ampDecaySlider, ampDecayLabel, 1, 5000, 1, audioProcessor.ampDecay, "Decay");
    ampDecaySlider.setTextValueSuffix(" ms");
    ampDecaySlider.setSkewFactor(0.4);
    addAndMakeVisible(ampDecaySlider);
    addAndMakeVisible(ampDecayLabel);
    
    setSliderAndLabelVertical(ampSustainSlider, ampSustainLabel, 0, 1, 0.01, audioProcessor.ampSustain, "Sustain");
    addAndMakeVisible(ampSustainSlider);
    addAndMakeVisible(ampSustainLabel);
    
    setSliderAndLabelVertical(ampReleaseSlider, ampReleaseLabel, 1, 5000, 1, audioProcessor.ampRelease, "Release");
    ampReleaseSlider.setTextValueSuffix(" ms");
    ampReleaseSlider.setSkewFactor(0.4);
    addAndMakeVisible(ampReleaseSlider);
    addAndMakeVisible(ampReleaseLabel);
    
    setSliderAndLabelVertical(fmAttackSlider, fmAttackLabel, 1, 5000, 1, audioProcessor.fmAttack, "Attack");
    fmAttackSlider.setTextValueSuffix(" ms");
    fmAttackSlider.setSkewFactor(0.4);
    addAndMakeVisible(fmAttackSlider);
    addAndMakeVisible(fmAttackLabel);
    
    setSliderAndLabelVertical(fmDecaySlider, fmDecayLabel, 1, 5000, 1, audioProcessor.fmDecay, "Decay");
    fmDecaySlider.setTextValueSuffix(" ms");
    fmDecaySlider.setSkewFactor(0.4);
    addAndMakeVisible(fmDecaySlider);
    addAndMakeVisible(fmDecayLabel);
    
    setSliderAndLabelVertical(fmSustainSlider, fmSustainLabel, 0, 1, 0.01, audioProcessor.fmSustain, "Sustain");
    addAndMakeVisible(fmSustainSlider);
    addAndMakeVisible(fmSustainLabel);
    
    setSliderAndLabelVertical(fmReleaseSlider, fmReleaseLabel, 1, 5000, 1, audioProcessor.fmRelease, "Release");
    fmReleaseSlider.setTextValueSuffix(" ms");
    fmReleaseSlider.setSkewFactor(0.4);
    addAndMakeVisible(fmReleaseSlider);
    addAndMakeVisible(fmReleaseLabel);
    
    setSliderAndLabelVertical(filterAttackSlider, filterAttackLabel, 1, 5000, 1, audioProcessor.filterAttack, "Attack");
    filterAttackSlider.setTextValueSuffix(" ms");
    filterAttackSlider.setSkewFactor(0.4);
    addAndMakeVisible(filterAttackSlider);
    addAndMakeVisible(filterAttackLabel);
    
    setSliderAndLabelVertical(filterDecaySlider, filterDecayLabel, 1, 5000, 1, audioProcessor.filterDecay, "Decay");
    filterDecaySlider.setTextValueSuffix(" ms");
    filterDecaySlider.setSkewFactor(0.4);
    addAndMakeVisible(filterDecaySlider);
    addAndMakeVisible(filterDecayLabel);
    
    setSliderAndLabelVertical(filterSustainSlider, filterSustainLabel, 0, 1, 0.01, audioProcessor.filterSustain, "Sustain");
    addAndMakeVisible(filterSustainSlider);
    addAndMakeVisible(filterSustainLabel);
    
    setSliderAndLabelVertical(filterReleaseSlider, filterReleaseLabel, 1, 5000, 1, audioProcessor.filterRelease, "Release");
    filterReleaseSlider.setTextValueSuffix(" ms");
    filterReleaseSlider.setSkewFactor(0.4);
    addAndMakeVisible(filterReleaseSlider);
    addAndMakeVisible(fmReleaseLabel);
    
    setSliderAndLabelVertical(pitchAttackSlider, pitchAttackLabel, 1, 5000, 1, audioProcessor.pitchAttack, "Attack");
    pitchAttackSlider.setTextValueSuffix(" ms");
    pitchAttackSlider.setSkewFactor(0.4);
    addAndMakeVisible(pitchAttackSlider);
    addAndMakeVisible(pitchAttackLabel);
    
    setSliderAndLabelVertical(pitchDecaySlider, pitchDecayLabel, 1, 5000, 1, audioProcessor.pitchDecay, "Decay");
    pitchDecaySlider.setTextValueSuffix(" ms");
    pitchDecaySlider.setSkewFactor(0.4);
    addAndMakeVisible(pitchDecaySlider);
    addAndMakeVisible(pitchDecayLabel);
    
    setSliderAndLabelVertical(pitchSustainSlider, pitchSustainLabel, 0, 1, 0.01, audioProcessor.pitchSustain, "Sustain");
    addAndMakeVisible(pitchSustainSlider);
    addAndMakeVisible(pitchSustainLabel);
    
    setSliderAndLabelVertical(pitchReleaseSlider, pitchReleaseLabel, 1, 5000, 1, audioProcessor.pitchRelease, "Release");
    pitchReleaseSlider.setTextValueSuffix(" ms");
    pitchReleaseSlider.setSkewFactor(0.4);
    addAndMakeVisible(pitchReleaseSlider);
    addAndMakeVisible(pitchReleaseLabel);
    
    setSliderAndLabelRotary(pitchEnvAmountSlider, pitchEnvAmountLabel, -48, 48, 1,  audioProcessor.pitchEnvelopeAmount, "Amount");
    pitchEnvAmountSlider.setTextValueSuffix(" st");
    addAndMakeVisible(pitchEnvAmountSlider);
    addAndMakeVisible(pitchEnvAmountSlider);
    
    setSliderAndLabelRotary(lfoRateSlider, lfoRateLabel, 0.1, 50, 0.1, audioProcessor.lfoRate, "Rate");
    lfoRateSlider.setTextValueSuffix(" Hz");
    lfoRateSlider.setSkewFactor(0.50);
    addAndMakeVisible(lfoRateSlider);
    addAndMakeVisible(lfoRateLabel);
    
    setSliderAndLabelRotary(lfoAmountSlider, lfoAmountLabel, 0, 100, 1, audioProcessor.lfoAmount, "Amount");
    lfoAmountSlider.setTextValueSuffix(" %");
    addAndMakeVisible(lfoAmountSlider);
    addAndMakeVisible(lfoAmountLabel);
    
    lfoMapCutoff.setClickingTogglesState(true);
    lfoMapCutoff.setColour(TextButton::ColourIds::buttonOnColourId, juce::Colours::purple);
    lfoMapCutoff.onClick = [&]
    {
        audioProcessor.setLfoMapCutoff(lfoMapCutoff.getToggleState());
    };
    addAndMakeVisible(lfoMapCutoff);
    
    
    lfoMapPitch.setClickingTogglesState(true);
    lfoMapPitch.setColour(TextButton::ColourIds::buttonOnColourId, juce::Colours::purple);
    lfoMapPitch.onClick = [&]
    {
        audioProcessor.setLfoMapPitch(lfoMapPitch.getToggleState());
    };
    addAndMakeVisible(lfoMapPitch);
    
    lfoMapAmplitude.setClickingTogglesState(true);
    lfoMapAmplitude.setColour(TextButton::ColourIds::buttonOnColourId, juce::Colours::purple);
    lfoMapAmplitude.onClick = [&]
    {
        audioProcessor.setLfoMapAmplitude(lfoMapAmplitude.getToggleState());
    };
    addAndMakeVisible(lfoMapAmplitude);
    
    lfoMapIndex.setClickingTogglesState(true);
    lfoMapIndex.setColour(TextButton::ColourIds::buttonOnColourId, juce::Colours::purple);
    lfoMapIndex.onClick = [&]
    {
        audioProcessor.setLfoMapIndex(lfoMapIndex.getToggleState());
    };
    addAndMakeVisible(lfoMapIndex);
    
    lfoMapModRatio.setClickingTogglesState(true);
    lfoMapModRatio.setColour(TextButton::ColourIds::buttonOnColourId, juce::Colours::purple);
    lfoMapModRatio.onClick = [&]
    {
        audioProcessor.setLfoMapModRatio(lfoMapModRatio.getToggleState());
    };
    addAndMakeVisible(lfoMapModRatio);
    
    lfoMapLfoRate.setClickingTogglesState(true);
    lfoMapLfoRate.setColour(TextButton::ColourIds::buttonOnColourId, juce::Colours::purple);
    lfoMapLfoRate.onClick = [&]
    {
        audioProcessor.setLfoMapLfoRate(lfoMapLfoRate.getToggleState());
    };
    addAndMakeVisible(lfoMapLfoRate);

    

    
    modulatorRatioSlider.addListener (this);
    modulatorFeedbackSlider.addListener (this);
    fmIndexOneSlider.addListener (this);
    fmIndexTwoSlider.addListener (this);
    carrierTwoTuneSlider.addListener(this);
    carrierTwoScaleAmpSlider.addListener(this);
    carrierTwoScaleIndexSlider.addListener(this);
    filterCutoffSlider.addListener(this);
    filterResonanceSlider.addListener(this);
    filterEnvAmountSlider.addListener(this);
    ampAttackSlider.addListener(this);
    ampDecaySlider.addListener(this);
    ampSustainSlider.addListener(this);
    ampReleaseSlider.addListener(this);
    fmAttackSlider.addListener(this);
    fmDecaySlider.addListener(this);
    fmSustainSlider.addListener(this);
    fmReleaseSlider.addListener(this);
    filterAttackSlider.addListener(this);
    filterDecaySlider.addListener(this);
    filterSustainSlider.addListener(this);
    filterReleaseSlider.addListener(this);
    pitchAttackSlider.addListener(this);
    pitchDecaySlider.addListener(this);
    pitchSustainSlider.addListener(this);
    pitchReleaseSlider.addListener(this);
    pitchEnvAmountSlider.addListener(this);
    lfoRateSlider.addListener(this);
    lfoAmountSlider.addListener(this);
    volumeSlider.addListener (this);
    
    //setResizable(true, true);
    setSize (900, 660);
    //centreWithSize (getWidth(), getHeight());
    
    
}

void FMSynthMiniProjectAudioProcessorEditor::setSliderAndLabelRotary(juce::Slider& slider, juce::Label& label, double minLim, double maxLim, double step, double defaultValue, const String &Text)
{
    slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    //slider.setColour(0x1001300, juce::Colours::aliceblue);
    slider.setColour(0x1001311, juce::Colours::purple);
    slider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    slider.setRange (minLim, maxLim, step);
    slider.setValue (defaultValue);
    label.setText(Text, dontSendNotification);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    label.attachToComponent(&slider , false);
}

void FMSynthMiniProjectAudioProcessorEditor::setSliderAndLabelVertical(juce::Slider& slider, juce::Label& label, double minLim, double maxLim, double step, double defaultValue, const String &Text)
{
    slider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slider.setColour(0x1001310, juce::Colours::purple);
    slider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    slider.setRange (minLim, maxLim, step);
    slider.setValue (defaultValue);
    label.setText(Text, dontSendNotification);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    label.attachToComponent(&slider , false);
}



FMSynthMiniProjectAudioProcessorEditor::~FMSynthMiniProjectAudioProcessorEditor()
{
}


//==============================================================================
void FMSynthMiniProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    //g.drawText("FM synth by Benjamin Stoier", 10, 350, 200, 40, juce::Justification::centred);
    g.drawRoundedRectangle(10, 30, 420, 150, 10.0, 4.0);
    g.drawRoundedRectangle(450, 30, 320, 150, 10.0, 4.0);
    g.drawRoundedRectangle(10, 220 , 260, 195, 10.0, 4.0);
    g.drawRoundedRectangle(290, 220 , 260, 195, 10.0, 4.0);
    g.drawRoundedRectangle(570, 220, 320, 195, 10.0, 4.0);
    g.drawRoundedRectangle(10, 455 , 360, 195, 10.0, 4.0);
    g.drawRoundedRectangle(390, 455 , 260, 195, 10.0, 4.0);
    g.drawRoundedRectangle(670, 455 , 220, 195, 10.0, 4.0);
    g.setFont(20.f);
    g.drawText("Modulator", 20, 5, 420, 20, Justification::left);
    g.drawText("2nd Carrier", 460, 5, 320, 20, Justification::left);
    g.drawText("Modulator Envelope", 20, 195, 260, 20, Justification::left);
    g.drawText("Filter Envelope", 300, 195, 260, 20, Justification::left);
    g.drawText("Filter", 580, 195, 320, 20, Justification::left);
    g.drawText("Pitch Envelope", 20, 430, 360, 20, Justification::left);
    g.drawText("Amplitude Envelope", 400, 430, 260, 20, Justification::left);
    g.drawText("LFO", 680, 430, 230, 20, Justification::left);
}

void FMSynthMiniProjectAudioProcessorEditor::resized()
{

    
    
    modulatorRatioSlider.setBounds(20,60,100,100);
    modulatorFeedbackSlider.setBounds(120, 60, 100, 100);
    fmIndexOneSlider.setBounds(220,60,100,100);
    fmIndexTwoSlider.setBounds(320,60,100,100);
    carrierTwoTuneSlider.setBounds(460,60,100,100);
    carrierTwoScaleAmpSlider.setBounds(560,60,100,100);
    carrierTwoScaleIndexSlider.setBounds(660,60,100,100);
    volumeSlider.setBounds(780, 60, 100, 100);
    fmAttackSlider.setBounds(20, 250, 60, 150);
    fmDecaySlider.setBounds(80, 250, 60, 150);
    fmSustainSlider.setBounds(140, 250, 60, 150);
    fmReleaseSlider.setBounds(200, 250, 60, 150);
    filterAttackSlider.setBounds(300, 250, 60, 150);
    filterDecaySlider.setBounds(360, 250, 60, 150);
    filterSustainSlider.setBounds(420, 250, 60, 150);
    filterReleaseSlider.setBounds(480, 250, 60, 150);
    filterCutoffSlider.setBounds(580, 300, 100, 100);
    filterResonanceSlider.setBounds(680, 300, 100, 100);
    filterEnvAmountSlider.setBounds(780, 300, 100, 100);
    filterMenu.setBounds(590, 235, 150, 30);
    filterSlopeMenu.setBounds(760, 235, 110, 30);
    pitchAttackSlider.setBounds(20, 485, 60, 150);
    pitchDecaySlider.setBounds(80, 485, 60, 150);
    pitchSustainSlider.setBounds(140, 485, 60, 150);
    pitchReleaseSlider.setBounds(200, 485, 60, 150);
    pitchEnvAmountSlider.setBounds(260, 485, 100, 150);
    ampAttackSlider.setBounds(400, 485, 60, 150);
    ampDecaySlider.setBounds(460, 485, 60, 150);
    ampSustainSlider.setBounds(520, 485, 60, 150);
    ampReleaseSlider.setBounds(580, 485, 60, 150);
    lfoRateSlider.setBounds(680, 485, 100, 100);
    lfoAmountSlider.setBounds(780, 485, 100, 100);
    lfoMapCutoff.setBounds(680,595, 60, 20);
    lfoMapPitch.setBounds(750,595, 60, 20);
    lfoMapAmplitude.setBounds(820,595, 60, 20);
    lfoMapIndex.setBounds(680,620, 60, 20);
    lfoMapModRatio.setBounds(750,620, 60, 20);
    lfoMapLfoRate.setBounds(820,620, 60, 20);
}


void FMSynthMiniProjectAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (slider == &volumeSlider)
    {
        audioProcessor.setVolume(volumeSlider.getValue());
    }
    if (slider == &modulatorRatioSlider)
    {
        audioProcessor.setModulatorRatio(modulatorRatioSlider.getValue());
    }
    if (slider == &modulatorFeedbackSlider)
    {
        audioProcessor.setModulatorFeedback(modulatorFeedbackSlider.getValue());
    }
    if (slider == &fmIndexOneSlider)
    {
        audioProcessor.setFmIndexOne(fmIndexOneSlider.getValue());
    }
    if (slider == &fmIndexTwoSlider)
    {
        audioProcessor.setFmIndexTwo(fmIndexTwoSlider.getValue());
    }
    if (slider == &carrierTwoTuneSlider)
    {
        audioProcessor.setCarrierTwoTune(carrierTwoTuneSlider.getValue());
    }
    if (slider == &carrierTwoScaleAmpSlider)
    {
        audioProcessor.setCarrierTwoAmpScale(carrierTwoScaleAmpSlider.getValue());
    }
    if (slider == &carrierTwoScaleIndexSlider)
    {
        audioProcessor.setCarrierTwoIndexScale(carrierTwoScaleIndexSlider.getValue());
    }
    if (slider == &filterCutoffSlider)
    {
        audioProcessor.setFilterCutoff(filterCutoffSlider.getValue());
    }
    if (slider == &filterResonanceSlider)
    {
        audioProcessor.setFilterResonance(filterResonanceSlider.getValue());
    }
    if (slider == &ampAttackSlider)
    {
        audioProcessor.setAmpAttack(ampAttackSlider.getValue());
    }
    if (slider == &ampDecaySlider)
    {
        audioProcessor.setAmpDecay(ampDecaySlider.getValue());
    }
    if (slider == &ampSustainSlider)
    {
        audioProcessor.setAmpSustain(ampSustainSlider.getValue());
    }
    if (slider == &ampReleaseSlider)
    {
        audioProcessor.setAmpRelease(ampReleaseSlider.getValue());
    }
    if (slider == &fmAttackSlider)
    {
        audioProcessor.setFmAttack(fmAttackSlider.getValue());
    }
    if (slider == &fmDecaySlider)
    {
        audioProcessor.setFmDecay(fmDecaySlider.getValue());
    }
    if (slider == &fmSustainSlider)
    {
        audioProcessor.setFmSustain(fmSustainSlider.getValue());
    }
    if (slider == &fmReleaseSlider)
    {
        audioProcessor.setFmRelease(fmReleaseSlider.getValue());
    }
    if (slider == &filterAttackSlider)
    {
        audioProcessor.setFilterAttack(filterAttackSlider.getValue());
    }
    if (slider == &filterDecaySlider)
    {
        audioProcessor.setFilterDecay(filterDecaySlider.getValue());
    }
    if (slider == &filterSustainSlider)
    {
        audioProcessor.setFilterSustain(filterSustainSlider.getValue());
    }
    if (slider == &filterReleaseSlider)
    {
        audioProcessor.setFilterRelease(filterReleaseSlider.getValue());
    }
    if (slider == &filterEnvAmountSlider)
    {
        audioProcessor.setFilterEnvelopeAmount(filterEnvAmountSlider.getValue());
    }
    if (slider == &pitchAttackSlider)
    {
        audioProcessor.setPitchAttack(pitchAttackSlider.getValue());
    }
    if (slider == &pitchDecaySlider)
    {
        audioProcessor.setPitchDecay(pitchDecaySlider.getValue());
    }
    if (slider == &pitchSustainSlider)
    {
        audioProcessor.setPitchSustain(pitchSustainSlider.getValue());
    }
    if (slider == &pitchReleaseSlider)
    {
        audioProcessor.setPitchRelease(pitchReleaseSlider.getValue());
    }
    if (slider == &pitchEnvAmountSlider)
    {
        audioProcessor.setPitchEnvelopeAmount(pitchEnvAmountSlider.getValue());
    }
    if (slider == &lfoRateSlider)
    {
        audioProcessor.setLfoRate(lfoRateSlider.getValue());
    }
    if (slider == &lfoAmountSlider)
    {
        audioProcessor.setLfoAmount(lfoAmountSlider.getValue());
    }
}

