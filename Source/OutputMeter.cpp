/*
  ==============================================================================

    OutputMeter.cpp
    Created: 6 Jan 2024 7:22:00pm
    Author:  Marco

  ==============================================================================
*/

#include "OutputMeter.h"
#include "OutputMeter.h"

OutputMeter::OutputMeter()
    : level(0.0f)
{
    outputLabel.setText("OUT", juce::dontSendNotification);
    outputLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(outputLabel);
}

OutputMeter::~OutputMeter()
{
}

void OutputMeter::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    float meterWidth = 10.0f;
    auto meterArea = juce::Rectangle<float>(area.getWidth() / 2 - meterWidth / 2, area.getY(), meterWidth, area.getHeight() - 20);

    g.setColour(juce::Colours::black);
    g.fillRect(meterArea);

    auto meterHeight = meterArea.getHeight() * level;

    juce::Colour colour = (level <= 0.7f) ? juce::Colours::green : juce::Colours::red;
    g.setColour(colour);
    g.fillRect(meterArea.removeFromBottom(meterHeight));
}

void OutputMeter::resized()
{
    auto area = getLocalBounds();
    float meterWidth = 10.0f;
    int labelHeight = 20;

    auto meterArea = juce::Rectangle<int>(area.getWidth() / 2 - meterWidth / 2, area.getY(), meterWidth, area.getHeight() - labelHeight);
    outputLabel.setBounds(area.getX(), meterArea.getBottom(), area.getWidth(), labelHeight);
}

void OutputMeter::calculateOutRMS(const juce::AudioBuffer<float>& buffer)
{
    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    float rmsSum = 0.0f;

    // Calcola la somma dei quadrati dei campioni
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getReadPointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float sampleValue = channelData[sample];
            rmsSum += sampleValue * sampleValue;
        }
    }

    // Calcola la media dei quadrati e poi la radice quadrata
    float rms = std::sqrt(rmsSum / (numChannels * numSamples));

    // Imposta il livello RMS nel meter
    setLevel(rms);
}

void OutputMeter::setLevel(float newLevel)
{
    level = newLevel;
    triggerAsyncUpdate();
}

void OutputMeter::handleAsyncUpdate()
{
    repaint();
}
