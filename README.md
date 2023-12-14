# kardboard:harmoniks
audio saturation and distrotion

## Basic Processing
* Signal Processing: The audio signal undergoes various stages of processing in the plugin. It starts with reading input buffers, followed by signal manipulation based on selected parameters, and finally, the modified signal is written to the output buffer.
* High-pass Filter: A high-pass filter (HPF) is calculated to remove unwanted low frequencies from the signal.
* Low-pass Filter : Remove low boomy, muddy, subby frequencies.
Plugin Parameters
* Drive: Amplifies the signal before the saturation effect is applied.
* Input: Adjusts the signal's input level before saturation.
* Mix (Dry/Wet): Balances between the original (dry) and saturated (wet) signals.

## Saturation Functions
* Tanh (Hyperbolic Tangent): Used for creating soft saturation, "quite"-mimicking analog distortion characteristics.
* Atan (Arc Tangent): Produces a softer distortion characteristic compared to tanh.
* Sine: Generates a type of distortion based on the sine function, which can lead to a more aggressive and less predictable type of saturation.

## Implementation in the Code
* Upsampling: The signal is upsampled (increased in sampling rate) before the saturation effect is applied.
* Saturation Application: The amplified signal is passed through one of the saturation functions (tanh, atan, sine) depending on the button pressed.
* Dry/Wet Mixing: The processed (wet) signal is mixed with the original (dry) signal based on the "Mix" parameter value.
* Downsampling: After saturation is applied, the signal is downsampled back to its original sampling rate.
* Gain Correction: A gain correction factor is calculated and applied to maintain a consistent signal level.

![kardboard:harmoniks](https://github.com/P23ASURE/kardboard-harmoniks/blob/main/saturator_screenshot.png)
