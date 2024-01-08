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

    standardLabel.setText("Standard", juce::dontSendNotification);
    standardLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(standardLabel);

    softLabel.setText("Soft", juce::dontSendNotification);
    softLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(softLabel);

    hardLabel.setText("Hard", juce::dontSendNotification);
    hardLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(hardLabel);
    
    hpfLabel.setText("HPF", juce::dontSendNotification);
    hpfLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(hpfLabel);

    lpfLabel.setText("LPF", juce::dontSendNotification);
    lpfLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lpfLabel);

    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(driveLabel);

    amountLabel.setText("Amount", juce::dontSendNotification);
    amountLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(amountLabel);

    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mixLabel);
    
    m_sliderInput.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_sliderInput.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mInputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getValueTreeState(), "INPUT", m_sliderInput);
    addAndMakeVisible(&m_sliderInput);

    m_sliderDrive.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_sliderDrive.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mDriveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getValueTreeState(), "DRIVE", m_sliderDrive);
    addAndMakeVisible(&m_sliderDrive);

    m_sliderMix.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_sliderMix.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mDryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getValueTreeState(), "DRYWET", m_sliderMix);
    addAndMakeVisible(&m_sliderMix);

    m_sliderHPF.setSliderStyle(juce::Slider::LinearHorizontal);
    m_sliderHPF.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    m_sliderHPFAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getValueTreeState(), "hpFrequency", m_sliderHPF);
    addAndMakeVisible(&m_sliderHPF);

    m_sliderLPF.setSliderStyle(juce::Slider::LinearHorizontal);
    m_sliderLPF.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20); 
    m_sliderLPFAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getValueTreeState(), "lpFrequency", m_sliderLPF);
    addAndMakeVisible(&m_sliderLPF);

    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);

    functionLabel.setText("TYPE:", juce::dontSendNotification);
    functionLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(functionLabel);

    parametersLabel.setText("PARAMETERS:", juce::dontSendNotification);
    parametersLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(parametersLabel);

    filtersLabel.setText("FILTERS:", juce::dontSendNotification);
    filtersLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(filtersLabel);

    metersLabel.setText("METERS:", juce::dontSendNotification);
    metersLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(metersLabel);

    setSize (500, 800);
}

SaturatorAudioProcessorEditor::~SaturatorAudioProcessorEditor()
{
}
//==============================================================================
void SaturatorAudioProcessorEditor::updateInputMeter(const juce::AudioBuffer<float>& buffer)
{
    inputMeter.calculateRMS(buffer);
}

void SaturatorAudioProcessorEditor::updateOutputMeter(const juce::AudioBuffer<float>& buffer)
{
    outputMeter.calculateOutRMS(buffer);
}


//==============================================================================
void SaturatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void SaturatorAudioProcessorEditor::resized() {


    // Top Section: Waveform Buttons
    juce::FlexBox waveformFlexBox;
    waveformFlexBox.flexDirection = juce::FlexBox::Direction::row;
    waveformFlexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    waveformFlexBox.alignItems = juce::FlexBox::AlignItems::center;
    // Section label:Parmeters
    waveformFlexBox.items.add(juce::FlexItem(functionLabel).withMinHeight(20).withMinWidth(100));

    waveformFlexBox.items.add(juce::FlexItem(waveform1).withMinHeight(30).withMinWidth(30));
    waveformFlexBox.items.add(juce::FlexItem(waveform2).withMinHeight(30).withMinWidth(30));
    waveformFlexBox.items.add(juce::FlexItem(waveform3).withMinHeight(30).withMinWidth(30));

    // Applica il layout del FlexBox
    waveformFlexBox.performLayout(getLocalBounds().toFloat());

    // Middle Section: Parameters
    juce::FlexBox parametersFlexBox;
    parametersFlexBox.flexDirection = juce::FlexBox::Direction::row;
    parametersFlexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    parametersFlexBox.alignItems = juce::FlexBox::AlignItems::center;

    // Section label:Parmeters
    parametersFlexBox.items.add(juce::FlexItem(parametersLabel).withMinHeight(20).withMinWidth(100));

    //  - Drive Slider and Label
    juce::FlexBox driveFlexBox;
    driveFlexBox.flexDirection = juce::FlexBox::Direction::column;
    driveFlexBox.items.add(juce::FlexItem(m_sliderDrive).withMinHeight(100).withMinWidth(100));
    driveFlexBox.items.add(juce::FlexItem(driveLabel).withMinHeight(20).withMinWidth(100));
    parametersFlexBox.items.add(juce::FlexItem(driveFlexBox).withFlex(1));

    // - Amount Slider and Label (replace with your amount slider and label)
    juce::FlexBox amountFlexBox;
    amountFlexBox.flexDirection = juce::FlexBox::Direction::column;
    amountFlexBox.items.add(juce::FlexItem(m_sliderInput).withMinHeight(100).withMinWidth(100));
    amountFlexBox.items.add(juce::FlexItem(amountLabel).withMinHeight(20).withMinWidth(100));
    parametersFlexBox.items.add(juce::FlexItem(amountFlexBox).withFlex(1));

    // - Mix Slider and Label
    juce::FlexBox mixFlexBox;
    mixFlexBox.flexDirection = juce::FlexBox::Direction::column;
    mixFlexBox.items.add(juce::FlexItem(m_sliderMix).withMinHeight(100).withMinWidth(100));
    mixFlexBox.items.add(juce::FlexItem(mixLabel).withMinHeight(20).withMinWidth(100));
    parametersFlexBox.items.add(juce::FlexItem(mixFlexBox).withFlex(1));

    // Bottom Section: Filter
    juce::FlexBox filtersFlexBox;
    filtersFlexBox.flexDirection = juce::FlexBox::Direction::row;
    filtersFlexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    filtersFlexBox.alignItems = juce::FlexBox::AlignItems::center;

    // Section Label: Filters
    filtersFlexBox.items.add(juce::FlexItem(filtersLabel).withMinHeight(20).withMinWidth(100));


    // - HPF Slider and Label
    juce::FlexBox hpfFlexBox;
    hpfFlexBox.flexDirection = juce::FlexBox::Direction::column;
    hpfFlexBox.items.add(juce::FlexItem(hpfLabel).withMinHeight(20).withMinWidth(50));
    hpfFlexBox.items.add(juce::FlexItem(m_sliderHPF).withMinHeight(100).withMinWidth(100));
    filtersFlexBox.items.add(juce::FlexItem(hpfFlexBox).withFlex(1));

    // - LPF Slider and Label
    juce::FlexBox lpfFlexBox;
    lpfFlexBox.flexDirection = juce::FlexBox::Direction::column;
    lpfFlexBox.items.add(juce::FlexItem(lpfLabel).withMinHeight(20).withMinWidth(50));
    lpfFlexBox.items.add(juce::FlexItem(m_sliderLPF).withMinHeight(100).withMinWidth(100));
    filtersFlexBox.items.add(juce::FlexItem(lpfFlexBox).withFlex(1));


    // FlexBox per input e output meters
    juce::FlexBox meterFlexBox;
    meterFlexBox.flexDirection = juce::FlexBox::Direction::row;
    meterFlexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    meterFlexBox.alignItems = juce::FlexBox::AlignItems::center;

    // - Section Label: Meters
    meterFlexBox.items.add(juce::FlexItem(metersLabel).withMinHeight(20).withMinWidth(100));

    meterFlexBox.items.add(juce::FlexItem(inputMeter).withFlex(2).withMinHeight(100).withMinWidth(100));
    meterFlexBox.items.add(juce::FlexItem(outputMeter).withFlex(2).withMinHeight(100).withMinWidth(100));

    // - Main FlexBox for overall layout
    juce::FlexBox mainFlexBox;
    mainFlexBox.flexDirection = juce::FlexBox::Direction::column;
    mainFlexBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    mainFlexBox.alignItems = juce::FlexBox::AlignItems::stretch;
    mainFlexBox.items.add(juce::FlexItem(waveformFlexBox).withFlex(8));
    mainFlexBox.items.add(juce::FlexItem(parametersFlexBox).withFlex(10)); 
    mainFlexBox.items.add(juce::FlexItem(filtersFlexBox).withFlex(15)); 
    mainFlexBox.items.add(juce::FlexItem(meterFlexBox).withFlex(15)); 
    
    // Apply the main FlexBox layout
    mainFlexBox.performLayout(getLocalBounds().toFloat());

    // Calcola e posiziona le etichette in base alla posizione attuale dei radio button
    int labelHeight = 20;
    int marginLabel = 10; // Margine aggiuntivo per centrare le etichette

    // Aggiusta le dimensioni e la posizione delle etichette
    standardLabel.setBounds(waveform1.getX() - marginLabel, waveform1.getBottom() + 5, waveform1.getWidth() + 2 * marginLabel, labelHeight);
    softLabel.setBounds(waveform2.getX() - marginLabel, waveform2.getBottom() + 5, waveform2.getWidth() + 2 * marginLabel, labelHeight);
    hardLabel.setBounds(waveform3.getX() - marginLabel, waveform3.getBottom() + 5, waveform3.getWidth() + 2 * marginLabel, labelHeight);
}
//==============================================================================

