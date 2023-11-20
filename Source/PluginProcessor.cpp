/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaturatorAudioProcessor::SaturatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), mValueTreeState(*this, nullptr, "PARAMETERS", createParameters())
#endif
{
    highPassFilter.reset();
    lowPassFilter.reset();
    currentCutoffFrequency = 200.0f; // O un altro valore iniziale
    targetCutoffFrequency = currentCutoffFrequency;
}


SaturatorAudioProcessor::~SaturatorAudioProcessor()
{
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

void SaturatorAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SaturatorAudioProcessor::getProgramName(int index)
{
    return {};
}

void SaturatorAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void SaturatorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
   
    // Prepare the filter with the specifications for each channel
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    highPassFilter.prepare(spec);
    float Q = 0.707f;
   *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 200.0f, Q);
        
    lowPassFilter.prepare(spec);
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 8000.0f, Q);

    upsampledBuffer.setSize(getTotalNumInputChannels(), samplesPerBlock * 2); // 2X Oversampling
    downsampledBuffer.setSize(getTotalNumInputChannels(), samplesPerBlock);
}


void SaturatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SaturatorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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


void SaturatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    targetCutoffFrequency = mValueTreeState.getRawParameterValue("highPassFreq")->load();
    // linear interpolation
    float interpolationStep = (targetCutoffFrequency - currentCutoffFrequency) * 0.01f;
    currentCutoffFrequency += interpolationStep;
    // non superare target
    if ((interpolationStep > 0 && currentCutoffFrequency > targetCutoffFrequency) ||
        (interpolationStep < 0 && currentCutoffFrequency < targetCutoffFrequency))
    {
        currentCutoffFrequency = targetCutoffFrequency;
    }

    saturatorInput = mValueTreeState.getRawParameterValue("INPUT")->load();
    saturatorDrive = mValueTreeState.getRawParameterValue("DRIVE")->load();
    saturatorMix = mValueTreeState.getRawParameterValue("DRYWET")->load();

    button1 = (int)mValueTreeState.getRawParameterValue("BUTTON1")->load();
    button2 = (int)mValueTreeState.getRawParameterValue("BUTTON2")->load();
    button3 = (int)mValueTreeState.getRawParameterValue("BUTTON3")->load();

    // Calculate RMS for each Channel
    float inputGain = 0.0f;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getReadPointer(channel);
        float rms = 0.0f;
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            rms += channelData[sample] * channelData[sample];
        }
        rms = std::sqrt(rms / buffer.getNumSamples());
        inputGain += rms;
    }
    inputGain /= totalNumInputChannels; //Averige RMS for channels

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    upsampledBuffer.clear();
    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
    {
        auto* readPointer = buffer.getReadPointer(channel);
        auto* writePointer = upsampledBuffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            writePointer[sample * 2] = readPointer[sample];
            writePointer[sample * 2 + 1] = readPointer[sample]; // Duplica il campione
        }
    }

    // Apply function
    if (button1) {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = upsampledBuffer.getWritePointer(channel);
            for (int sample = 0; sample < upsampledBuffer.getNumSamples(); ++sample)
            {
                float drySignal = channelData[sample];
                channelData[sample] *= saturatorInput * saturatorDrive;
                float wetSignal = std::tanh(channelData[sample]);
                channelData[sample] = (saturatorMix * wetSignal) + (1.0f - saturatorMix) * drySignal;
            }
        }
    }
    else if (button2) {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = upsampledBuffer.getWritePointer(channel);
            for (int sample = 0; sample < upsampledBuffer.getNumSamples(); ++sample)
            {
                float drySignal = channelData[sample];
                channelData[sample] *= saturatorInput * saturatorDrive;
                float wetSignal = (2.f / juce::MathConstants<float>::pi) * std::atan(channelData[sample]);
                channelData[sample] = (saturatorMix * wetSignal) + (1.0f - saturatorMix) * drySignal;
            }
        }
    }
    else {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = upsampledBuffer.getWritePointer(channel);
            for (int sample = 0; sample < upsampledBuffer.getNumSamples(); ++sample)
            {
                float drySignal = channelData[sample];
                channelData[sample] *= saturatorInput * saturatorDrive;
                float wetSignal = std::sin(channelData[sample]);
                channelData[sample] = (saturatorMix * wetSignal) + (1.0f - saturatorMix) * drySignal;
            }

        }
    }
    downsampledBuffer.clear();
    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
    {
        auto* readPointer = upsampledBuffer.getReadPointer(channel);
        auto* writePointer = downsampledBuffer.getWritePointer(channel);

        for (int sample = 0; sample < downsampledBuffer.getNumSamples(); ++sample)
        {
            writePointer[sample] = readPointer[sample * 2]; // Select every 2nd sample
        }
    }
    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
    {
        auto* readPointer = downsampledBuffer.getReadPointer(channel);
        auto* writePointer = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            writePointer[sample] = readPointer[sample];
        }
    }

    //Calculation of the RMS level for the output after saturation
    float outputGain = 0.0f;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        float rms = 0.0f;
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            rms += channelData[sample] * channelData[sample];
        }
        rms = std::sqrt(rms / buffer.getNumSamples());
        outputGain += rms;
    }
    outputGain /= totalNumInputChannels; // average RMS values of the channel

    // Calculate Gain factor
    float gainCorrection = (outputGain > 0.0f) ? inputGain / outputGain : 1.0f;

    // Gain Correction
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= gainCorrection;
        }
    }

    auto cutoffFrequency = mValueTreeState.getRawParameterValue("highPassFreq")->load();
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), currentCutoffFrequency, Q);

    // create audiblock for HPF
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    highPassFilter.process(context);
    lowPassFilter.process(context);

}

//==============================================================================
bool SaturatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SaturatorAudioProcessor::createEditor()
{
    return new SaturatorAudioProcessorEditor(*this);
}

//==============================================================================
void SaturatorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SaturatorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{

}

juce::AudioProcessorValueTreeState::ParameterLayout SaturatorAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    parameters.push_back(std::make_unique<juce::AudioParameterInt>("BUTTON1", "Waveform1", 0, 1, 1));
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("BUTTON2", "Waveform2", 0, 1, 0));
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("BUTTON3", "Waveform3", 0, 1, 0));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("INPUT", "Input", 0.25f, 10.0f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", 0.25f, 1.0f, 0.5f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DRYWET", "Mix", 0.0f, 1.0f, 0.5f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("highPassFreq", "High Pass Frequency", 20.0f, 800.0f, 200.0f));

    return { parameters.begin(), parameters.end() };

}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SaturatorAudioProcessor();
}
