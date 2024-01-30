# Saturator
audio saturation and distrotion

## Basic Processing
* Signal Processing: The audio signal undergoes various stages of processing in the plugin. It starts with reading input buffers, followed by signal manipulation based on selected parameters, and finally, the modified signal is written to the output buffer.
* Auto-Gain: Applying Gain to Output: After calculating and smoothing the gain adjustment, the method applies this gain to the output buffer. (enabling / disabling)
* High-pass Filter: A high-pass filter (HPF) is calculated to remove unwanted low frequencies from the signal.
* Low-pass Filter : Remove low boomy, muddy, subby frequencies.
Plugin Parameters
* Drive: Amplifies the signal before the saturation effect is applied.
* Amount: Adjusts the signal's input level before saturation.
* Mix (Dry/Wet): Balances between the original (dry) and saturated (wet) signals.

## Saturation Functions
* Tanh (Hyperbolic Tangent): Used for creating soft saturation, "quite"-mimicking analog distortion characteristics.
* Atan (Arc Tangent): Produces a softer distortion characteristic compared to tanh.
* Sine: Generates a type of distortion based on the sine function, which can lead to a more aggressive and less predictable type of saturation.

## Implementation in the Code
* Saturation Application: The amplified signal is passed through one of the saturation functions (tanh, atan, sine) depending on the button pressed.
* Dry/Wet Mixing: The processed (wet) signal is mixed with the original (dry) signal based on the "Mix" parameter value.

## Improvement:
* Added Input & Output meters
* Added HPF & LPF
* Added Resized UI option
* Added Autogain

  ![Sautrator](https://github.com/P23ASURE/saturator/blob/main/saturator_screenshot.png)

