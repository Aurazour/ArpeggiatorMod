/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class AMAudioProcessor  : public juce::AudioProcessor
{
public:
     float arpSpeed;
     float arpSpeed2;
     float arpSpeed3;
     int octaves;
     int direction;
     int speedcounter;
     int alternate;
     int evencounter;
     bool baseNote;
    //==============================================================================
    AMAudioProcessor();
    ~AMAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //================ stuff
    //note duation stuff for task 4
    //juce::Array<float> timestamps{ arpSpeed, arpSpeed2, arpSpeed3 }; //not used

private:
    //==============================================================================
    int currentNote, lastNoteValue;
    int time;
    float rate;
    juce::SortedSet<int> notes;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AMAudioProcessor)
};
