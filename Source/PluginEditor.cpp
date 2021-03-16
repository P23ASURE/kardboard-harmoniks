/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaturatorAudioProcessorEditor::SaturatorAudioProcessorEditor (SaturatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    waveform1.setRadioGroupId(WaveFormButtons);
    waveform2.setRadioGroupId(WaveFormButtons);
    waveform3.setRadioGroupId(WaveFormButtons);
    
    addAndMakeVisible(&waveform1);
    addAndMakeVisible(&waveform2);
    addAndMakeVisible(&waveform3);
    waveform1.setClickingTogglesState(true);
    waveform2.setClickingTogglesState(true);
    waveform3.setClickingTogglesState(true);
    
    
    mWaveformAttachment1 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.getValueTreeState(), "BUTTON1", waveform1);
    mWaveformAttachment2 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.getValueTreeState(), "BUTTON2", waveform2);
    mWaveformAttachment3 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.getValueTreeState(), "BUTTON3", waveform3);
    
    
    m_sliderInput.setLookAndFeel(&customLook);
    m_sliderInput.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mInputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getValueTreeState(), "INPUT", m_sliderInput);
    addAndMakeVisible(&m_sliderInput);

    m_sliderDrive.setLookAndFeel(&customLook);
    m_sliderDrive.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mDriveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getValueTreeState(), "DRIVE", m_sliderDrive);
    addAndMakeVisible(&m_sliderDrive);

    m_sliderMix.setLookAndFeel(&customLook);
    m_sliderMix.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mDryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getValueTreeState(), "DRYWET", m_sliderMix);
    addAndMakeVisible(&m_sliderMix);
    
    
    
    setSize (500, 200);
}

SaturatorAudioProcessorEditor::~SaturatorAudioProcessorEditor()
{
}

//==============================================================================
void SaturatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.drawText("Drive", getWidth()/5 * 2 - 50, getHeight()/2 + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("Amount", getWidth()/5 * 3 - 50, getHeight()/2 + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("Mix", getWidth()/5 * 4 - 50, getHeight()/2 + 5, 100, 100, juce::Justification::centred, false);
    
    g.drawText("Normal", getWidth()/5 * 1 - 15, getHeight()/2 - 60, 90, 90, juce::Justification::left, false);
    g.drawText("Soft", getWidth()/5 * 1 - 15, getHeight()/2 - 40, 90, 90, juce::Justification::left, false);
    g.drawText("Heavy", getWidth()/5 * 1 - 15, getHeight()/2 - 20, 90, 90, juce::Justification::left, false);
}

void SaturatorAudioProcessorEditor::resized()
{
    
    waveform1.setBounds(getWidth()/5 * 1 - 45, getHeight()/2 - 30, 30, 30);
    waveform2.setBounds(getWidth()/5 * 1 - 45, getHeight()/2 - 10 , 30, 30);
    waveform3.setBounds(getWidth()/5 * 1 - 45, getHeight()/2 + 10, 30, 30);
    
    
    m_sliderInput.setBounds(getWidth()/5 * 2- 50, getHeight()/2 - 50, 100, 100);
    m_sliderInput.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    m_sliderDrive.setBounds(getWidth()/5 * 3 - 50, getHeight()/2 - 50, 100, 100);
    m_sliderDrive.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    m_sliderMix.setBounds(getWidth()/5 * 4 - 50, getHeight()/2 - 50, 100, 100);
    m_sliderMix.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);

    
}
