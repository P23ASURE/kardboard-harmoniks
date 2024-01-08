/*
  ==============================================================================

    filters.h
    Created: 6 Jan 2024 11:39:20am
    Author:  Marco

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Filter {
public:
    Filter();

    void setSampleRate(double rate);
    void setHighPassFrequency(float frequency);
    void setLowPassFrequency(float frequency);
    void processBlock(juce::AudioBuffer<float>& buffer);

private:
    juce::dsp::IIR::Filter<float> highPassFilter, lowPassFilter;
    double sampleRate;
    float highPassFrequency;
    float lowPassFrequency;

    void updateFilters();
};

