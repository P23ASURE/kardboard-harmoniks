#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "InputMeter.h"
#include "OutputMeter.h"

#include "CustomLookAndFeel.h"
#include "CustomBackground.h"

//==============================================================================
/**
*/

class SaturatorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    
    enum RadioButtonsId
    {
        WaveFormButtons = 1001
    };
    
    SaturatorAudioProcessorEditor (SaturatorAudioProcessor&);
    ~SaturatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    //==============================================================================

    void updateInputMeter(const juce::AudioBuffer<float>& buffer);
    void updateOutputMeter(const juce::AudioBuffer<float>& buffer);
private:
    CustomLookAndFeel customLookAndFeel;
    CustomBackground customBackground;

    juce::Label functionLabel;
    juce::Label filtersLabel;
    juce::Label parametersLabel;
    juce::Label metersLabel;
 
    juce::Slider m_sliderInput, m_sliderDrive, m_sliderOutput, m_sliderMix;
    juce::Label driveLabel;
    juce::Label amountLabel;
    juce::Label mixLabel;

    juce::Label standardLabel;
    juce::Label softLabel;
    juce::Label hardLabel;
        
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mInputAttachment, mDriveAttachment, mDryWetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mMenuAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mWaveformAttachment1, mWaveformAttachment2, mWaveformAttachment3;
    
    juce::ComboBox funcMenu;
    
    juce::ToggleButton waveform1, waveform2, waveform3;

    juce::Slider m_sliderHPF;
    juce::Slider m_sliderLPF;
    juce::Label hpfLabel;
    juce::Label lpfLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_sliderHPFAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_sliderLPFAttachment;
    
    InputMeter inputMeter;
    OutputMeter outputMeter;

    juce::ToggleButton autoGainToggle;
    juce::Label autoGainLabel;

    SaturatorAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturatorAudioProcessorEditor)
};
