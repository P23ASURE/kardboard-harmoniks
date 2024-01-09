/*
  ==============================================================================

    OutputMeter.h
    Created: 6 Jan 2024 7:22:00pm
    Author:  Marco

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OutputMeter : public juce::Component, public juce::AsyncUpdater
{
public:
    OutputMeter();
    ~OutputMeter();

    void paint(juce::Graphics&) override;
    void resized() override;

    void calculateOutRMS(const juce::AudioBuffer<float>& buffer);
    void setLevel(float newLevel);

    void handleAsyncUpdate() override;

    float getOutpuRMSLevel() const { return level; }
private:
    float level;
    juce::Label outputLabel; // Label per "OUT"

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputMeter)
};

