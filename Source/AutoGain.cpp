#include "AutoGain.h"
#include "InputMeter.h"  
#include "OutputMeter.h" 

AutoGain::AutoGain()
        : autoGainEnabled(false),
         responseTimeMs(0.1f),
         smoothingAmount(0.1f),
         currentGain(1.0f), // Initialize with a default value
         inputLevelRMS(0.0f), // Initialize with a default value
         outputLevelRMS(0.0f) // Initialize with a default value
    {
    // 
    }

void AutoGain::process(const juce::AudioBuffer<float>& inputBuffer, juce::AudioBuffer<float>& outputBuffer)
    {
    if (!autoGainEnabled) return;

    // Create instances of InputMeter and OutputMeter
    InputMeter inputMeter;
    OutputMeter outputMeter;

    // Use inputBuffer for input RMS and outputBuffer for output RMS
    inputMeter.calculateRMS(inputBuffer);
    outputMeter.calculateOutRMS(outputBuffer);

    float inputRMS = inputMeter.getRMSLevel(); 
    float outputRMS = outputMeter.getOutpuRMSLevel(); 

    float gainAdjustment = calculateGainAdjustment(inputRMS, outputRMS);
    currentGain = applySmoothing(currentGain);
    outputBuffer.applyGain(currentGain);
}

    void AutoGain::setAutoGainEnabled(bool isEnabled)
    {
    autoGainEnabled = isEnabled;
}

    void AutoGain::setResponseTime(float timeMs)
    {
    responseTimeMs = timeMs;
}

    float AutoGain::calculateGainAdjustment(float inputLevel, float outputLevel)
    {
        // Calcolare la differenza in decibel
        float gainChangeDb = 20.0f * log10(inputLevel / outputLevel);

        // Calcolare il fattore di guadagno lineare da questa differenza
        float gainAdjustment = pow(10.0f, gainChangeDb / 20.0f);

        return gainAdjustment;
}

    float AutoGain::applySmoothing(float newGain) {
        // Interpolazione lineare tra il guadagno corrente e il nuovo guadagno
        currentGain = currentGain + smoothingAmount * (newGain - currentGain);

        // guadagno rimane entro limiti accettabili (?)
        currentGain = juce::jlimit(0.0f, 1.0f, currentGain);

        return currentGain;
    }

