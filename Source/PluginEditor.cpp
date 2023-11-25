/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LevelMeter.h"

//==============================================================================
SaturatorAudioProcessorEditor::SaturatorAudioProcessorEditor(SaturatorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    waveform1.setRadioGroupId(WaveFormButtons);
    waveform2.setRadioGroupId(WaveFormButtons);
    waveform3.setRadioGroupId(WaveFormButtons);

    addAndMakeVisible(&waveform1);
    addAndMakeVisible(&waveform2);
    addAndMakeVisible(&waveform3);
    waveform1.setClickingTogglesState(true);
    waveform2.setClickingTogglesState(true);
    waveform3.setClickingTogglesState(true);

    waveform1.setLookAndFeel(&customLook);
    waveform2.setLookAndFeel(&customLook);
    waveform3.setLookAndFeel(&customLook);


    mWaveformAttachment1 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getValueTreeState(), "BUTTON1", waveform1);
    mWaveformAttachment2 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getValueTreeState(), "BUTTON2", waveform2);
    mWaveformAttachment3 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getValueTreeState(), "BUTTON3", waveform3);


    m_sliderInput.setLookAndFeel(&customLook);
    m_sliderInput.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mInputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getValueTreeState(), "INPUT", m_sliderInput);
    addAndMakeVisible(&m_sliderInput);

    m_sliderDrive.setLookAndFeel(&customLook);
    m_sliderDrive.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mDriveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getValueTreeState(), "DRIVE", m_sliderDrive);
    addAndMakeVisible(&m_sliderDrive);

    m_sliderMix.setLookAndFeel(&customLook);
    m_sliderMix.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mDryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getValueTreeState(), "DRYWET", m_sliderMix);
    addAndMakeVisible(&m_sliderMix);

    
    highPassFreqSlider.setRange(20.0, 800.0, 1.0); 
    highPassFreqSlider.setSliderStyle(juce::Slider::LinearHorizontal); 
    highPassFreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 20); 
    addAndMakeVisible(highPassFreqSlider); 
    highPassFreqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getValueTreeState(), "highPassFreq", highPassFreqSlider);
    

    highPassFreqLabel.setJustificationType(juce::Justification::centred);
    highPassFreqLabel.setEditable(false, true, false);
    addAndMakeVisible(highPassFreqLabel);
    highPassFreqLabel.setText(juce::String(highPassFreqSlider.getValue()), juce::dontSendNotification); 

    highPassFreqSlider.onValueChange = [this]() {
        highPassFreqLabel.setText(juce::String(highPassFreqSlider.getValue()), juce::dontSendNotification);
        };
        
    lowPassFreqSlider.setRange(1000.0, 20000.0, 1.0);  
    lowPassFreqSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    lowPassFreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 20);
    addAndMakeVisible(&lowPassFreqSlider);
    lowPassFreqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getValueTreeState(), "lowPassFreq", lowPassFreqSlider);

    lowPassFreqLabel.setJustificationType(juce::Justification::centred);
    lowPassFreqLabel.setEditable(false, false, false);
    addAndMakeVisible(&lowPassFreqLabel);
    lowPassFreqLabel.setText(juce::String(lowPassFreqSlider.getValue()), juce::dontSendNotification);

    lowPassFreqSlider.onValueChange = [this]() {
        lowPassFreqLabel.setText(juce::String(lowPassFreqSlider.getValue()), juce::dontSendNotification);
        };

    setSize(500, 250);
}

SaturatorAudioProcessorEditor::~SaturatorAudioProcessorEditor()
{
    
    waveform1.setLookAndFeel(nullptr);
    waveform2.setLookAndFeel(nullptr);
    waveform3.setLookAndFeel(nullptr);

    m_sliderInput.setLookAndFeel(nullptr);
    m_sliderDrive.setLookAndFeel(nullptr);
    m_sliderMix.setLookAndFeel(nullptr);

    highPassFreqSlider.setLookAndFeel(nullptr);
    lowPassFreqSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void SaturatorAudioProcessorEditor::paint (juce::Graphics& g)
{
   
    g.drawImageAt(backgroundImage, 0, 0);
    
    auto& outputMeter = processor.getOutputLevelMeter();
    outputMeter.setBounds(10, 30, 15, getHeight() - 60); // Regola le dimensioni come necessario
    outputMeter.repaint(); // Chiama repaint invece di paint direttamente
}


void SaturatorAudioProcessorEditor::resized()
{
    
  
    highPassFreqSlider.setLookAndFeel(&customLookAndFeel);
    highPassFreqSlider.setBounds(getWidth() / 5 * 1 - 50, getHeight() / 2 +50, 100, 100);
    int labelYOffset = 35;
    highPassFreqLabel.setBounds(highPassFreqSlider.getRight() + 10, highPassFreqSlider.getY() + labelYOffset, 50, 20);

    lowPassFreqSlider.setLookAndFeel(&customLookAndFeel);
    lowPassFreqSlider.setBounds(getWidth() / 5 * 4 - 50, getHeight() / 2 + 50, 100, 100);

    lowPassFreqLabel.setBounds(lowPassFreqSlider.getRight() + 10, lowPassFreqSlider.getY() + labelYOffset, 50, 20);


    waveform1.setBounds(getWidth()/5 * 1 - 85, getHeight()/2 - 75, 30, 30);
    waveform2.setBounds(getWidth()/5 * 1 - 85, getHeight()/2 - 35 , 30, 30);
    waveform3.setBounds(getWidth()/5 * 1 - 85, getHeight()/2 + 5, 30, 30);
        
    m_sliderInput.setBounds(getWidth()/5 * 2- 50, getHeight()/2 - 65, 100, 100);
    m_sliderInput.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    m_sliderDrive.setBounds(getWidth()/5 * 3 - 50, getHeight()/2 - 65, 100, 100);
    m_sliderDrive.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    m_sliderMix.setBounds(getWidth()/5 * 4 - 50, getHeight()/2 - 65, 100, 100);
    m_sliderMix.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);

}
