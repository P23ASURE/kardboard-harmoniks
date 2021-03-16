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
        
//        auto fill = juce::Colours::lightgrey;
//        auto outline    = slider.findColour (juce::Slider::rotarySliderFillColourId);
//
//        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);
//
//        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
//        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
//        auto lineW = juce::jmin (6.0f, radius * 0.5f);
//        auto arcRadius = radius - lineW * 0.5f;
//
//        juce::Path backgroundArc;
//        backgroundArc.addCentredArc (bounds.getCentreX(),
//                                     bounds.getCentreY(),
//                                     arcRadius,
//                                     arcRadius,
//                                     0.0f,
//                                     rotaryStartAngle,
//                                     rotaryEndAngle,
//                                     true);
//
//        g.setColour (outline);
//        g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
//
//        if (slider.isEnabled())
//        {
//            juce::Path valueArc;
//            valueArc.addCentredArc (bounds.getCentreX(),
//                                    bounds.getCentreY(),
//                                    arcRadius,
//                                    arcRadius,
//                                    0.0f,
//                                    rotaryStartAngle,
//                                    toAngle,
//                                    true);
//
//            g.setColour (fill);
//            g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
//        }
//
//        auto thumbWidth = lineW * 1.5f;
//        juce::Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - juce::MathConstants<float>::halfPi),
//                                 bounds.getCentreY() + arcRadius * std::sin (toAngle - juce::MathConstants<float>::halfPi));
//
//        g.setColour (slider.findColour (juce::Slider::thumbColourId));
//        g.fillEllipse (juce::Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint));
        
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
    
    SliderStyle customLook;
    juce::Slider m_sliderInput, m_sliderDrive, m_sliderOutput, m_sliderMix;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mInputAttachment, mDriveAttachment, mDryWetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> mMenuAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mWaveformAttachment1, mWaveformAttachment2, mWaveformAttachment3;
    
    juce::ComboBox funcMenu;
    
    juce::ToggleButton waveform1, waveform2, waveform3;
    
    SaturatorAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturatorAudioProcessorEditor)
};
