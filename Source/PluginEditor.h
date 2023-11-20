/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class SliderStyle : public juce::LookAndFeel_V4
{
public:
        
    SliderStyle()
    {

        setColour(juce::Slider::thumbColourId, juce::Colours::white);
        knob = juce::ImageCache::getFromMemory(BinaryData::stitchedJuceKnob_png, BinaryData::stitchedJuceKnob_pngSize);

    }
    
    // Metodo per disegnare i toggle buttons
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {

        // disegna effettivamente il bottone
        LookAndFeel_V4::drawToggleButton(g, button, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        // Calcoliamo il rettangolo dove disegnare l'immagine
        auto bounds = button.getLocalBounds().toFloat();
        auto source = button.getToggleState() ? juce::Rectangle<int>(0, 32, 32, 32) // La parte superiore dello sprite sheet
            : juce::Rectangle<int>(0, 0, 32, 32); // La parte inferiore dello sprite sheet
        
        // Carica l'immagine dallo sprite sheet
        juce::Image spriteSheet = juce::ImageCache::getFromMemory(BinaryData::button1_png, BinaryData::button1_pngSize);
        
        // Se l'immagine è valida, disegna la parte corretta dello sprite sheet
        if (spriteSheet.isValid())
        {
            g.drawImage(spriteSheet,
                
                (int)bounds.getX(), (int)bounds.getY(), (int)bounds.getWidth(), (int)bounds.getHeight(),
               
                source.getX(), source.getY(), source.getWidth(), source.getHeight());
        }
        else
        {
            
            g.setColour(button.getToggleState() ? juce::Colours::green : juce::Colours::red);
            g.fillRect(bounds);
        }
    }


    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {     
        
        if (knob.isValid())
            {
                const double rotation = (slider.getValue()
                    - slider.getMinimum())
                    / (slider.getMaximum()
                        - slider.getMinimum());

                const int frames = 100;
                const int frameId = (int)ceil(rotation * ((double)frames - 1.0));

                int imgWidth = knob.getWidth();
                int imgHeight = knob.getHeight() / frames;
                g.drawImage(knob, 0, 0, imgWidth, imgHeight, 0, frameId * imgHeight, imgWidth, imgHeight);
            }
            else
            {
                static const float textPpercent = 0.35f;
                juce::Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f, 0.5f * height, width * textPpercent, 0.5f * height);

                g.setColour(juce::Colours::white);

                g.drawFittedText(juce::String("No Image"), text_bounds.getSmallestIntegerContainer(), juce::Justification::horizontallyCentred | juce::Justification::centred, 1);
            }
             
    };
private:
    juce::Image knob;
    
};


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

private:
    juce::Slider highPassFreqSlider; 
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highPassFreqSliderAttachment; 
    juce::Label highPassFreqLabel;
    
    SliderStyle customLookAndFeel;
    SliderStyle customLook;
    juce::Slider m_sliderInput, m_sliderDrive, m_sliderOutput, m_sliderMix;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mInputAttachment, mDriveAttachment, mDryWetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mMenuAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mWaveformAttachment1, mWaveformAttachment2, mWaveformAttachment3;
    
    juce::ComboBox funcMenu;
    
    juce::ToggleButton waveform1, waveform2, waveform3;
    
    SaturatorAudioProcessor& audioProcessor;

    juce::Image backgroundImage;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturatorAudioProcessorEditor)
};
