/*
  ==============================================================================

    InputMeter.h
    Created: 6 Jan 2024 4:29:36pm
    Author:  Marco

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class InputMeter : public juce::Component, public juce::AsyncUpdater
{
public:
    InputMeter();
    ~InputMeter();

    void paint(juce::Graphics&) override;
    void resized() override;

    void calculateRMS(const juce::AudioBuffer<float>& buffer);
    void setLevel(float newLevel);

    void handleAsyncUpdate() override;
    float getRMSLevel() const { return level; }
private:
    float level;
    juce::Colour gradientStartColour;
    juce::Colour gradientEndColour;
    juce::Label inputLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputMeter)
};
