/*
  ==============================================================================

    InputMeter.cpp
    Created: 6 Jan 2024 4:29:36pm
    Author:  Marco

  ==============================================================================
*/

#include "InputMeter.h"

InputMeter::InputMeter()
    : level(0.0f), gradientStartColour(juce::Colours::green), gradientEndColour(juce::Colours::red)
{
    inputLabel.setText("IN", juce::dontSendNotification);
    inputLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(inputLabel);
}

InputMeter::~InputMeter()
{
}
//===================================================================================
// UI
void InputMeter::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();

    // Larghezza del meter
    float meterWidth = 10.0f;

    // Calcola l'area del meter escludendo lo spazio per la label
    auto meterArea = juce::Rectangle<float>(area.getWidth() / 2 - meterWidth / 2, area.getY(), meterWidth, area.getHeight() - 20);

    // Sfondo del meter
    g.setColour(juce::Colours::black);
    g.fillRect(meterArea);

    auto meterHeight = meterArea.getHeight() * level;

    // Colore del meter
    juce::Colour colour = (level <= 0.7f) ? juce::Colours::green : juce::Colours::red;
    g.setColour(colour);
    g.fillRect(meterArea.removeFromBottom(meterHeight));
}

void InputMeter::resized()
{
    auto area = getLocalBounds();

    float meterWidth = 10.0f;
    int labelHeight = 20;

    // Calcola l'area del meter, lasciando spazio per la label sotto
    auto meterArea = juce::Rectangle<int>(area.getWidth() / 2 - meterWidth / 2, area.getY(), meterWidth, area.getHeight() - labelHeight);

    // Posiziona la label "IN" sotto l'area del meter
    inputLabel.setBounds(area.getX(), meterArea.getBottom(), area.getWidth(), labelHeight);
}

// ====================================================================================================================================================
// Logic

void InputMeter::calculateRMS(const juce::AudioBuffer<float>& buffer)
{
    auto numSamples = buffer.getNumSamples();
    float sum = 0.0f;

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            float sample = buffer.getSample(channel, i);
            sum += sample * sample;
        }
    }

    float rms = sqrt(sum / (numSamples * buffer.getNumChannels()));
    setLevel(rms);
}

void InputMeter::setLevel(float newLevel)
{
    level = newLevel;
    triggerAsyncUpdate();
}

void InputMeter::handleAsyncUpdate()
{
    repaint();  // Ridisegna l'interfaccia utente nel message thread
}