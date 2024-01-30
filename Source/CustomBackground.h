/*
  ==============================================================================

    CustomBackground.h
    Created: 20 Jan 2024 9:49:23am
    Author:  Marco

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class CustomBackground : public juce::Component
{
public:
    
    CustomBackground();

    
    ~CustomBackground() override;

    void paint(juce::Graphics&) override;

    void resized() override;

private:
    std::unique_ptr<juce::Drawable> backgroundDrawable;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomBackground)
};

