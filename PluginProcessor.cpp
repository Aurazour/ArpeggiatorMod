#include <iostream> //TODO remove this line after we're done print debugging :3

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AMAudioProcessor::AMAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

AMAudioProcessor::~AMAudioProcessor()
{
}

//==============================================================================
const juce::String AMAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AMAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AMAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AMAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AMAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AMAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AMAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AMAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AMAudioProcessor::getProgramName (int index)
{
    return {};
}

void AMAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AMAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    notes.clear();                          // [1]
    currentNote = 0;                        // [2]
    lastNoteValue = -1;                     // [3]
    time = 0;                               // [4]
    rate = static_cast<float> (sampleRate); // [5]
    arpSpeed = 0.5;
    std::cout << arpSpeed;
    octaves = 1; 
    arpSpeed2 = 0.5; 
    arpSpeed3 = 0.5;
    direction = 0; //controls which of the 4 patterns is selected
    speedcounter = 0; 
    alternate = 0; 
    evencounter = 1;
    baseNote = false;
}

void AMAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AMAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AMAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
 
    // We use the audio buffer to get timing information
    auto numSamples = buffer.getNumSamples();                                                       // [7]

    auto noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f - arpSpeed))));

    if (speedcounter == 0) {
        noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f - arpSpeed))));
    }
    else if (speedcounter == 1) {
        noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f - arpSpeed2))));
    }
    else if (speedcounter == 2) {
        noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f - arpSpeed3))));
    }
    
    //originally that was arpspeed
    //i think i want the values of timestampms to be the values of the arpspeeds
    //ok somehow. We need to have arpspeeds as the array and then for each note we loop through arpspeeds????
    

    //the notes we're adding is gonna change based on octave
    int counter = 1; 
    for (const auto metadata : midi)                                                                // [9]
    {
        const auto msg = metadata.getMessage();
        
        if (octaves <= 1) {
            if (msg.isNoteOn())  notes.add(msg.getNoteNumber());
            else if (msg.isNoteOff()) notes.removeValue(msg.getNoteNumber());
        }
        else {
            if (msg.isNoteOn()) {
                notes.add(msg.getNoteNumber());
                while(counter<octaves){
                    notes.add(msg.getNoteNumber() + (12 * counter));
                    counter++;
                }
                counter = 1;

                
            }
            else if (msg.isNoteOff()) {
                notes.removeValue(msg.getNoteNumber());
                while (counter < octaves) {
                    notes.removeValue(msg.getNoteNumber() + (12 * counter));
                    counter++;
                }
                counter = 1;
            } 
        }
    }

    midi.clear();                                                                                   // [10]


    //0 is asending 1 is descending 2 is play base note 4 is staggered pattern
    if (direction == 0) {//pattern: ABCDABCD
        if ((time + numSamples) >= noteDuration)                                                        // [11]
        {
            auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));     // [12]

            if (lastNoteValue > 0)                                                                      // [13]
            {
                midi.addEvent(juce::MidiMessage::noteOff(1, lastNoteValue), offset);
                lastNoteValue = -1;
            }

            if (notes.size() > 0)                                                                       // [14]
            {
                currentNote = (currentNote + 1) % notes.size();
                lastNoteValue = notes[currentNote];
                midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8)127), offset);
                if (speedcounter >= 2) { speedcounter = 0; }
                else { speedcounter++;}
            }

        }
    }
    else if (direction == 1) {//pattern: DCBADCBA
        if ((time + numSamples) >= noteDuration)                                                        // [11]
        {
            auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));     // [12]

            if (lastNoteValue > 0)                                                                      // [13]
            {
                midi.addEvent(juce::MidiMessage::noteOff(1, lastNoteValue), offset);
                lastNoteValue = -1;
            }

            if (notes.size() > 0)                                                                       // [14]
            {
                currentNote = ((currentNote - 1) + notes.size()) % notes.size();
                lastNoteValue = notes[currentNote];
                midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8)127), offset);
                if (speedcounter >= 2) { speedcounter = 0; }
                else { speedcounter++; }
            }

        }
    }
    else if (direction == 2) {//pattern: ABACADAA
        
        if ((time + numSamples) >= noteDuration)                                                        // [11]
        {
            auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));     // [12]

            if (lastNoteValue > 0)                                                                      // [13]
            {
                midi.addEvent(juce::MidiMessage::noteOff(1, lastNoteValue), offset);
                lastNoteValue = -1;
            }

            if (notes.size() > 0)                                                                       // [14]
            {
                

                if (baseNote) { //this is the note that goes inbetween the other note
                    //we check if we have played a note already in the sequence and tack in on infront of it
                    lastNoteValue = notes[0]; //first note
                    midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8)127), offset);
                    baseNote = false;
                }
                else {
                    lastNoteValue = notes[currentNote]; //first note
                    midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8)127), offset);
                        if (lastNoteValue != notes[0]) {
                            baseNote = true;
                        }
                        currentNote = (currentNote + 1) % notes.size();
                }
                if (speedcounter >= 2) { speedcounter = 0; }
                else { speedcounter++; }
            }

        }
    }
    else if (direction == 3) { //pattern: rotating double note (ex: CBADBBADDBC for 4 notes or CBBACCBAA for 3 notes)
        
        if ((time + numSamples) >= noteDuration)                                                        // [11]
        {
            auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));     // [12]

            if (lastNoteValue > 0)                                                                      // [13]
            {
                midi.addEvent(juce::MidiMessage::noteOff(1, lastNoteValue), offset);
                lastNoteValue = -1;
            }

            if (notes.size() > 0)                                                                       // [14]
            {
                if (alternate == 0) { currentNote = (currentNote + notes.size()) % notes.size(); }
                else if (alternate == 1) { currentNote = (currentNote + 2 + notes.size()) % notes.size(); }
                else if (alternate == 2) { currentNote = (currentNote - 1 + notes.size()) % notes.size(); }
                lastNoteValue = notes[currentNote];
                midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8)127), offset);
                if (speedcounter >= 2) { speedcounter = 0; }
                else { speedcounter++; }
                if (alternate == 2) { alternate = 0; }
                else { alternate++; }
            }

        }
    }

    time = (time + numSamples) % noteDuration;                                                      // [15]

    
    }

//==============================================================================
bool AMAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AMAudioProcessor::createEditor()
{
    return new AMAudioProcessorEditor (*this);
}

//==============================================================================
void AMAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AMAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AMAudioProcessor();
}
