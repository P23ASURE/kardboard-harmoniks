/*
  ==============================================================================
    filters.cpp
    Corrected: 6 Jan 2024
    Author:  Marco (P23ASURE)
  ==============================================================================
*/

#include "filters.h"

Filter::Filter() : sampleRate(44100.0), highPassFrequency(20.0f), lowPassFrequency(20000.0f) {
    updateFilters();
}

void Filter::setSampleRate(double rate) {
    sampleRate = rate;
    updateFilters();
}

void Filter::setHighPassFrequency(float frequency) {
    highPassFrequency = frequency;
    updateFilters();
}

void Filter::setLowPassFrequency(float frequency) {
    lowPassFrequency = frequency;
    updateFilters();
}

void Filter::processBlock(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        auto singleChannelBlock = block.getSingleChannelBlock(channel);
        juce::dsp::ProcessContextReplacing<float> context(singleChannelBlock);
        highPassFilter.process(context);
        lowPassFilter.process(context);
    }
}

void Filter::updateFilters() {
    auto hpCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, highPassFrequency);
    auto lpCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, lowPassFrequency);
    highPassFilter.coefficients = *hpCoefficients;
    lowPassFilter.coefficients = *lpCoefficients;
}
