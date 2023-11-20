/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMSynthMiniProjectAudioProcessor::FMSynthMiniProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    //Add voices to synthesizer (8 voices of polyphony)
    fmSynth.clearVoices();
    for (int i = 0; i < 8; i++)
    {
        fmSynth.addVoice(new FMVoice());
    }
    fmSynth.clearSounds();
    fmSynth.addSound(new FMSound());
}

FMSynthMiniProjectAudioProcessor::~FMSynthMiniProjectAudioProcessor()
{
}

//==============================================================================
const juce::String FMSynthMiniProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FMSynthMiniProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FMSynthMiniProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FMSynthMiniProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FMSynthMiniProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FMSynthMiniProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FMSynthMiniProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FMSynthMiniProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FMSynthMiniProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void FMSynthMiniProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
//void FMSynthMiniProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
void FMSynthMiniProjectAudioProcessor::prepareToPlay (double sampleRate, int)
{
    //Read sample rate into synthesizer class
    fs = sampleRate;
    fmSynth.setCurrentPlaybackSampleRate(fs);
}

void FMSynthMiniProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FMSynthMiniProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FMSynthMiniProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //For every voice
    for (int i = 0; i < fmSynth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<FMVoice*>(fmSynth.getVoice(i)))
        {
            //Update parameters
            voice -> updateParameters(volume, modulatorRatio, modulatorFeedback, fmIndexOne, fmIndexTwo, carrierTwoTune, carrierTwoAmpScale, carrierTwoIndexScale, filterCutoff, filterResonance, filterMenuId, filterMenuSlopeId, ampAttack, ampDecay, ampSustain, ampRelease, fmAttack, fmDecay, fmSustain, fmRelease, filterAttack, filterDecay, filterSustain, filterRelease, filterEnvelopeAmount, pitchAttack, pitchDecay, pitchSustain, pitchRelease, pitchEnvelopeAmount, lfoRate, lfoAmount, lfoMapCutoff, lfoMapPitch, lfoMapAmplitude, lfoMapIndex, lfoMapModRatio, lfoMapLfoRate);
            //Update ADSR
            voice -> setADSR(fs);
            
        }
    }
    //Read MIDI buffer into the synthesizer and render all the synth voices
    fmSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool FMSynthMiniProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FMSynthMiniProjectAudioProcessor::createEditor()
{
    return new FMSynthMiniProjectAudioProcessorEditor (*this);
}

//==============================================================================

//Save parameters
void FMSynthMiniProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    std::unique_ptr<juce::XmlElement> xml (new juce::XmlElement ("SynthParameters"));
    xml->setAttribute ("modulatorRatio", (double) modulatorRatio);
    xml->setAttribute ("modulatorFeedback", (double) modulatorFeedback);
    xml->setAttribute ("fmIndexOne", (double) fmIndexOne);
    xml->setAttribute ("fmIndexTwo", (double) fmIndexTwo);
    xml->setAttribute ("carrierTwoTune", (double) carrierTwoTune);
    xml->setAttribute ("carrierTwoAmpScale", (double) carrierTwoAmpScale);
    xml->setAttribute ("carrierTwoIndexScale", (double) carrierTwoIndexScale);
    xml->setAttribute ("filterCutoff", (double) filterCutoff);
    xml->setAttribute ("filterResonance", (double) filterResonance);
    xml->setAttribute ("filterMenuId", (int) filterMenuId);
    xml->setAttribute ("filterMenuSlopeId", (int) filterMenuSlopeId);
    xml->setAttribute ("ampAttack", (double) ampAttack);
    xml->setAttribute ("ampDecay", (double) ampDecay);
    xml->setAttribute ("ampSustain", (double) ampSustain);
    xml->setAttribute ("ampRelease", (double) ampRelease);
    xml->setAttribute ("fmAttack", (double) fmAttack);
    xml->setAttribute ("fmDecay", (double) fmDecay);
    xml->setAttribute ("fmSustain", (double) fmSustain);
    xml->setAttribute ("fmRelease", (double) fmRelease);
    xml->setAttribute ("filterAttack", (double) filterAttack);
    xml->setAttribute ("filterDecay", (double) filterDecay);
    xml->setAttribute ("filterSustain", (double) filterSustain);
    xml->setAttribute ("filterRelease", (double) filterRelease);
    xml->setAttribute ("filterEnvelopeAmount", (double) filterEnvelopeAmount);
    xml->setAttribute ("pitchAttack", (double) pitchAttack);
    xml->setAttribute ("pitchDecay", (double) pitchDecay);
    xml->setAttribute ("pitchSustain", (double) pitchSustain);
    xml->setAttribute ("pitchRelease", (double) pitchRelease);
    xml->setAttribute ("pitchEnvelopeAmount", (double) pitchEnvelopeAmount);
    xml->setAttribute ("volume", (double) volume);
    xml->setAttribute ("lfoRate", (double) lfoRate);
    xml->setAttribute ("lfoAmount", (double) lfoAmount);
    //xml->setAttribute ("lfoMapCutoff", (bool) lfoMapCutoff);
    //xml->setAttribute ("lfoMapPitch", (bool) lfoMapPitch);
    //xml->setAttribute ("lfoMapAmplitude", (bool) lfoMapAmplitude);
    //xml->setAttribute ("lfoMapIndex", (bool) lfoMapIndex);
    //xml->setAttribute ("lfoMapModRatio", (bool) lfoMapModRatio);
    //xml->setAttribute ("lfoMapLfoRate", (bool) lfoMapLfoRate);
    copyXmlToBinary (*xml, destData);
}

//Load parameters
void FMSynthMiniProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
            {
                if (xmlState->hasTagName ("SynthParameters"))
                {
                    modulatorRatio = (double) xmlState->getDoubleAttribute ("modulatorRatio");
                    modulatorFeedback = (double) xmlState->getDoubleAttribute ("modulatorFeedback");
                    fmIndexOne = (double) xmlState->getDoubleAttribute ("fmIndexOne");
                    fmIndexTwo = (double) xmlState->getDoubleAttribute ("fmIndexTwo");
                    carrierTwoTune = (double) xmlState->getDoubleAttribute ("carrierTwoTune");
                    carrierTwoAmpScale = (double) xmlState->getDoubleAttribute ("carrierTwoAmpScale");
                    carrierTwoIndexScale = (double) xmlState->getDoubleAttribute ("carrierTwoIndexScale");
                    filterCutoff = (double) xmlState->getDoubleAttribute ("filterCutoff");
                    filterResonance = (double) xmlState->getDoubleAttribute ("filterResonance");
                    filterMenuId = (int) xmlState->getDoubleAttribute ("filterMenuId");
                    filterMenuSlopeId = (int) xmlState->getDoubleAttribute ("filterMenuSlopeId");
                    ampAttack = (double) xmlState->getDoubleAttribute ("ampAttack");
                    ampDecay = (double) xmlState->getDoubleAttribute ("ampDecay");
                    ampSustain = (double) xmlState->getDoubleAttribute ("ampSustain");
                    ampRelease = (double) xmlState->getDoubleAttribute ("ampRelease");
                    fmAttack = (double) xmlState->getDoubleAttribute ("fmAttack");
                    fmDecay = (double) xmlState->getDoubleAttribute ("fmDecay");
                    fmSustain = (double) xmlState->getDoubleAttribute ("fmSustain");
                    fmRelease = (double) xmlState->getDoubleAttribute ("fmRelease");
                    filterAttack = (double) xmlState->getDoubleAttribute ("filterAttack");
                    filterDecay = (double) xmlState->getDoubleAttribute ("filterDecay");
                    filterSustain = (double) xmlState->getDoubleAttribute ("filterSustain");
                    filterRelease = (double) xmlState->getDoubleAttribute ("filterRelease");
                    filterEnvelopeAmount = (double) xmlState->getDoubleAttribute ("filterEnvelopeAmount");
                    pitchAttack = (double) xmlState->getDoubleAttribute ("pitchAttack");
                    pitchDecay = (double) xmlState->getDoubleAttribute ("pitchDecay");
                    pitchSustain = (double) xmlState->getDoubleAttribute ("pitchSustain");
                    pitchRelease = (double) xmlState->getDoubleAttribute ("pitchRelease");
                    pitchEnvelopeAmount = (double) xmlState->getDoubleAttribute ("pitchEnvelopeAmount");
                    volume = (double) xmlState->getDoubleAttribute ("volume");
                    lfoRate = (double) xmlState->getDoubleAttribute ("lfoRate");
                    lfoAmount = (double) xmlState->getDoubleAttribute ("lfoAmount");
                    //lfoMapCutoff = (bool) xmlState->getDoubleAttribute ("lfoMapCutoff");
                    //lfoMapPitch = (bool) xmlState->getDoubleAttribute ("lfoMapPitch");
                    //lfoMapAmplitude = (bool) xmlState->getDoubleAttribute ("lfoMapAmplitude");
                    //lfoMapIndex = (bool) xmlState->getDoubleAttribute ("lfoMapIndex");
                    //lfoMapModRatio = (bool) xmlState->getDoubleAttribute ("lfoMapModRatio");
                    //lfoMapLfoRate = (bool) xmlState->getDoubleAttribute ("lfoMapLfoRate");
                }
            }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FMSynthMiniProjectAudioProcessor();
}


