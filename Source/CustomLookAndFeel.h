#pragma once
#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel() override;

    void drawRotarySlider(juce::Graphics&,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider&) override;
private:
    std::unique_ptr<juce::Drawable> buttonOnImage;
    std::unique_ptr<juce::Drawable> buttonOffImage;
    std::unique_ptr<juce::Drawable> autoGainOnImage;
    std::unique_ptr<juce::Drawable> autoGainOffImage;
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool isMouseOverButton, bool isButtonDown) override;

    juce::Colour getThumbColour(float sliderPosProportional);
};
