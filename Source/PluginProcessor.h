/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LevelMeter.h"

//==============================================================================
/**
*/
class SaturatorAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    SaturatorAudioProcessor();
    ~SaturatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    LevelMeter& getOutputLevelMeter() { return outputLevelMeter; }

    juce::AudioProcessorValueTreeState& getValueTreeState()
    {
        return mValueTreeState;
    }


  
private:
    LevelMeter outputLevelMeter;

    float currentLowPassCutoffFrequency;
    float targetLowPassCutoffFrequency;

    float lowPassFreq;

    float currentCutoffFrequency;
    float targetCutoffFrequency;
    float Q = 0.707f;

    std::deque<float> rmsValues; 
    float smoothedRms = 0.0f;    
    const size_t smoothSize = 10; 

    juce::AudioBuffer<float> upsampledBuffer;
    juce::AudioBuffer<float> downsampledBuffer;

    using Filter = juce::dsp::IIR::Filter<float>;
    using FilterCoefs = juce::dsp::IIR::Coefficients<float>;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highPassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowPassFilter;


    float saturatorInput, saturatorDrive, saturatorMix;
    int button1, button2, button3;

    juce::AudioProcessorValueTreeState mValueTreeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SaturatorAudioProcessor)
};