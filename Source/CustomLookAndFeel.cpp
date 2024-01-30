#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel() {
    autoGainOffImage = juce::Drawable::createFromImageData(BinaryData::AutoGainOFF_svg, BinaryData::AutoGainOFF_svgSize);
    autoGainOnImage = juce::Drawable::createFromImageData(BinaryData::AutoGainON_svg, BinaryData::AutoGainON_svgSize);
    buttonOnImage = juce::Drawable::createFromImageData(BinaryData::ButtonON_svg, BinaryData::ButtonON_svgSize);
    buttonOffImage = juce::Drawable::createFromImageData(BinaryData::ButtonOFF_svg, BinaryData::ButtonOFF_svgSize);
}

CustomLookAndFeel::~CustomLookAndFeel() {
    // Pulizia risorse (se necessario)
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPosProportional,
    float rotaryStartAngle,
    float rotaryEndAngle,
    juce::Slider& slider) {
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // Fill
    g.setColour(juce::Colours::white.withAlpha(0.8f));
    g.fillEllipse(rx, ry, rw, rw);

    // Outline
    g.setColour(juce::Colours::grey);
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 5.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // Thumb color
    juce::Colour thumbColour = getThumbColour(sliderPosProportional);
    g.setColour(thumbColour);
    g.fillPath(p);

}

juce::Colour CustomLookAndFeel::getThumbColour(float sliderPosProportional) {
    // Calcola il colore del thumb in base alla posizione dello slider
    // Qui puoi definire la tua logica per il gradiente
    // Esempio: interpolazione lineare tra due colori
    auto colourStart = juce::Colour::fromString("FF29FFA5"); 
    auto colourEnd = juce::Colour::fromString("FF43A17A");
    return colourStart.interpolatedWith(colourEnd, sliderPosProportional);
}

void CustomLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
    auto bounds = button.getLocalBounds().toFloat();
    auto name = button.getName();

    if (name == "autoGain") {
        // Disegna il pulsante Auto-Gain con le sue immagini personalizzate
        if (button.getToggleState()) {
            if (autoGainOnImage != nullptr) {
                autoGainOnImage->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.0f);
            }
        }
        else {
            if (autoGainOffImage != nullptr) {
                autoGainOffImage->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.0f);
            }
        }
    } else if (name == "waveform") {
        // Disegna i RadioButton con le loro immagini personalizzate
        if (button.getToggleState()) {
            if (buttonOnImage != nullptr) {
                buttonOnImage->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.0f);
            }
        } else {
            if (buttonOffImage != nullptr) {
                buttonOffImage->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.0f);
            }
        }
    }
    // Puoi aggiungere altri 'else if' per altri tipi di pulsanti se necessario
}
