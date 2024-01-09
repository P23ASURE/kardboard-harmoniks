/*
  ==============================================================================

    AutoGain.h
    Created: 8 Jan 2024 4:01:38pm
    Author:  Marco

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "OutputMeter.h"


class AutoGain
{
public:
    AutoGain();

    void process(const juce::AudioBuffer<float>& inputBuffer, juce::AudioBuffer<float>& outputBuffer);

    void setAutoGainEnabled(bool isEnabled);
    void setResponseTime(float timeMs);
    

private:
    float calculateGainAdjustment(float inputLevel, float outputLevel);
    float applySmoothing(float currentGain);

    bool autoGainEnabled;
    float inputLevelRMS;
    float outputLevelRMS;
    float currentGain;
    float responseTimeMs;
    float smoothingAmount;
};
