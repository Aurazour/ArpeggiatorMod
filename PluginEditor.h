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
class AMAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    AMAudioProcessorEditor (AMAudioProcessor&);
    ~AMAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged (juce::Slider* slider) override;
    void updateToggleState(juce::Button* button, juce::String name)
    {
        auto state = button->getToggleState();
        juce::String stateString = state ? "ON" : "OFF";
        juce::String selectedString = state ? " (selected)" : "";

        juce::Logger::outputDebugString(name + " Button changed to " + stateString);
        button->setButtonText(name + selectedString);
    }
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AMAudioProcessor& audioProcessor;

    juce::Slider arpSlider; // [1]
    juce::Slider octaveSlider;
    juce::Label  octaveLabel;
    juce::TextButton one;
    juce::TextButton two;
    juce::TextButton three;
    juce::TextButton four;
    juce::Slider arpSlider2;
    juce::Slider arpSlider3;
    juce::Label  arplabel;
    juce::Label  arp2label;
    juce::Label  arp3label;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AMAudioProcessorEditor)
};
