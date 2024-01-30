#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaturatorAudioProcessor::SaturatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), mValueTreeState(*this, nullptr, "PARAMETERS", createParameters())
#endif
{

}

SaturatorAudioProcessor::~SaturatorAudioProcessor()
{
}

//==============================================================================
void SaturatorAudioProcessor::updateFilterParameters() {
    
    if (auto* hpParam = mValueTreeState.getRawParameterValue("hpFrequency")) {
        float hpFreq = hpParam->load();
        if (hpFreq > 0 && hpFreq < getSampleRate() / 2) {
            highPassFilter.setHighPassFrequency(hpFreq);
        }
    }

    if (auto* lpParam = mValueTreeState.getRawParameterValue("lpFrequency")) {
        float lpFreq = lpParam->load();
        if (lpFreq > 0 && lpFreq < getSampleRate() / 2) {
            lowPassFilter.setLowPassFrequency(lpFreq);
        }
    }
}

//==============================================================================
const juce::String SaturatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SaturatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SaturatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SaturatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SaturatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SaturatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SaturatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SaturatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SaturatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void SaturatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SaturatorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    highPassFilter.setSampleRate(sampleRate);
    highPassFilter.setHighPassFrequency(20.0f); 
    lowPassFilter.setSampleRate(sampleRate);
    lowPassFilter.setLowPassFrequency(20000.0f); 
}


void SaturatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SaturatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void SaturatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    updateFilterParameters();
    
    saturatorInput = mValueTreeState.getRawParameterValue("INPUT")->load();
    saturatorDrive = mValueTreeState.getRawParameterValue("DRIVE")->load();
    saturatorMix = mValueTreeState.getRawParameterValue("DRYWET")->load();

    button1 = (int)mValueTreeState.getRawParameterValue("BUTTON1")->load();
    button2 = (int)mValueTreeState.getRawParameterValue("BUTTON2")->load();
    button3 = (int)mValueTreeState.getRawParameterValue("BUTTON3")->load();
    
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    if(button1){

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);

                for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
                {
                    
                    float drySignal = channelData[sample];
                    channelData[sample] *= saturatorInput * saturatorDrive;
                    float wetSignal = std::tanh(channelData[sample]);
                    channelData[sample] = (saturatorMix * wetSignal) + (1.0f - saturatorMix) * drySignal;
                }
        }
        
    } else if (button2){
        

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);

                for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
                {
                    
                    float drySignal = channelData[sample];
                    channelData[sample] *= saturatorInput * saturatorDrive;
                    float wetSignal = (2.f / juce::MathConstants<float>::pi) * std::atan(channelData[sample]);
                    channelData[sample] = (saturatorMix * wetSignal) + (1.0f - saturatorMix) * drySignal;
                }
        }
        
    } else {
        
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);

                for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
                {
                    
                    float drySignal = channelData[sample];
                    channelData[sample] *= saturatorInput * saturatorDrive;
                    float wetSignal = std::sin(channelData[sample]);
                    channelData[sample] = (saturatorMix * wetSignal) + (1.0f - saturatorMix) * drySignal;
                }
        }
        
    }

    // Process signals with filters
    highPassFilter.processBlock(buffer);
    lowPassFilter.processBlock(buffer);

    // Update input meter
    if (auto* editor = dynamic_cast<SaturatorAudioProcessorEditor*>(getActiveEditor()))
    {
        editor->updateInputMeter(buffer);
    }

    // Update output meter
    if (auto* editor = dynamic_cast<SaturatorAudioProcessorEditor*>(getActiveEditor()))
    {
        editor->updateOutputMeter(buffer);
    }

    // Apply auto-gain processing
    autoGain.process(buffer, buffer);
        
}

//==============================================================================
bool SaturatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SaturatorAudioProcessor::createEditor()
{
    return new SaturatorAudioProcessorEditor (*this);
}

//=============ll=================================================================
void SaturatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SaturatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
 
}

juce::AudioProcessorValueTreeState::ParameterLayout SaturatorAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("BUTTON1", "Waveform1", 0, 1, 1));
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("BUTTON2", "Waveform2", 0, 1, 0));
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("BUTTON3", "Waveform3", 0, 1, 0));
    
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("INPUT", "Input", 1.0f, 10.0f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", 0.25f, 1.0f, 0.5f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DRYWET", "Mix", 0.0f, 1.0f, 0.5f));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("hpFrequency", "High Pass Frequency", 20.0f, 900.0f, 200.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("lpFrequency", "Low Pass Frequency", 1000.0f, 20000.0f, 5000.0f));

    parameters.push_back(std::make_unique<juce::AudioParameterBool>("autoGainParam", "Auto Gain", false));

    return {parameters.begin(), parameters.end()};
    
 }


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SaturatorAudioProcessor();
}
