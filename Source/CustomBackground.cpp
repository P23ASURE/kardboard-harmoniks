/*
  ==============================================================================

    CustomBackground.cpp
    Created: 20 Jan 2024 9:57:27am
    Author:  Marco

  ==============================================================================
*/

#include "CustomBackground.h"

CustomBackground::CustomBackground()
{
      auto* svgData = BinaryData::Background_svg; 
    const int svgDataSize = BinaryData::Background_svgSize; 

    if (svgData != nullptr && svgDataSize > 0)
    {
        std::unique_ptr<juce::XmlElement> svg(juce::XmlDocument::parse(svgData));
        if (svg != nullptr)
        {
            backgroundDrawable = std::unique_ptr<juce::Drawable>(juce::Drawable::createFromSVG(*svg));
        }
    }

    if (backgroundDrawable != nullptr)
    {
        addAndMakeVisible(backgroundDrawable.get());
        backgroundDrawable->setBounds(getLocalBounds());
    }
}


CustomBackground::~CustomBackground()
{
    //
}


void CustomBackground::paint(juce::Graphics& g)
{
    if (backgroundDrawable != nullptr)
    {
       backgroundDrawable->drawWithin(g, getLocalBounds().toFloat(), juce::RectanglePlacement::centred, 0.5f);
    }
}

void CustomBackground::resized()
{

}


