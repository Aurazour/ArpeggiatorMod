/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AMAudioProcessorEditor::AMAudioProcessorEditor (AMAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 500); 
    octaveSlider.setValue(1); // [5]
    
    // these define the parameters of our slider object
    arpSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    arpSlider.setRange (0.0, 1.0, 0.05);
    arpSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    arpSlider.setPopupDisplayEnabled (true, false, this);
    arpSlider.setTextValueSuffix (" Speed");
    arpSlider.setValue(0.5);
 
    // this function adds the slider to the editor
    addAndMakeVisible (&arpSlider);
    arpSlider.addListener(this);

    //speed
    arpSlider2.setSliderStyle(juce::Slider::LinearBarVertical);
    arpSlider2.setRange(0.0, 1.0, 0.05);
    arpSlider2.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    arpSlider2.setPopupDisplayEnabled(true, false, this);
    arpSlider2.setTextValueSuffix(" Speed");
    arpSlider2.setValue(0.5);
    addAndMakeVisible(&arpSlider2);
    arpSlider2.addListener(this);

    arpSlider3.setSliderStyle(juce::Slider::LinearBarVertical);
    arpSlider3.setRange(0.0, 1.0, 0.05);
    arpSlider3.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    arpSlider3.setPopupDisplayEnabled(true, false, this);
    arpSlider3.setTextValueSuffix(" Speed");
    arpSlider3.setValue(0.5);
    addAndMakeVisible(&arpSlider3);
    arpSlider3.addListener(this);

    //octave
    addAndMakeVisible(octaveSlider);
    octaveSlider.setRange(1, 7, 1);
    octaveSlider.addListener(this);
    addAndMakeVisible(octaveLabel);
    octaveLabel.setText("Octaves", juce::dontSendNotification);
    octaveLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    octaveLabel.attachToComponent(&octaveSlider, true);
    octaveSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);

    addAndMakeVisible(one);
    addAndMakeVisible(two);
    addAndMakeVisible(three);
    addAndMakeVisible(four);
    one.setButtonText("Ascending (selected)");
    two.setButtonText("Descending");
    three.setButtonText("First Note Repetition");
    four.setButtonText("Rotating Double Note");
    one.onClick = [this] { updateToggleState(&one, "Ascending (selected)"); updateToggleState(&two, "Descending"); updateToggleState(&three, "First Note Repetition"); updateToggleState(&four, "Rotating Double Note"); audioProcessor.direction = 0; };
    two.onClick = [this] { updateToggleState(&two, "Descending (selected)"); updateToggleState(&one, "Ascending"); updateToggleState(&three, "First Note Repetition"); updateToggleState(&four, "Rotating Double Note"); audioProcessor.direction = 1; };
    three.onClick = [this] { updateToggleState(&three, "First Note Repetition (selected)"); updateToggleState(&two, "Descending"); updateToggleState(&one, "Ascending"); updateToggleState(&four, "Rotating Double Note"); audioProcessor.direction = 2; };
    four.onClick = [this] { updateToggleState(&four, "Rotating Double Note (selected)"); updateToggleState(&two, "Descending"); updateToggleState(&one, "Ascending"); updateToggleState(&three, "First Note Repetition"); audioProcessor.direction = 3; };

}

enum RadioButtonIds
{
    GenderButtons = 1001
};





AMAudioProcessorEditor::~AMAudioProcessorEditor()
{
}

//==============================================================================
void AMAudioProcessorEditor::paint (juce::Graphics& g)
{
   // fill the whole window white
    g.fillAll (juce::Colours::cyan);
 
    // set the current drawing colour to black
    g.setColour (juce::Colours::black);
 
    // set the font size and draw text to the screen
    g.setFont (15.0f);
 
    g.drawFittedText ("Arpeggiator", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
    g.drawFittedText("Speed\n1", 22, getHeight() - 120, 50, 50, juce::Justification::centred, 1);
    g.drawFittedText("Speed\n2", 66, getHeight() - 120, 50, 50, juce::Justification::centred, 1);
    g.drawFittedText("Speed\n3", 108, getHeight() - 120, 50, 50, juce::Justification::centred, 1);

}

void AMAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // sets the position and size of the slider with arguments (x, y, width, height)
    arpSlider.setBounds(40, 135, 20, getHeight() - 250); 
    arpSlider2.setBounds(80, 135, 20, getHeight() - 250);
    arpSlider3.setBounds(120, 135, 20, getHeight() - 250);

    //agfjaf
    auto sliderLeft = 120;
    octaveSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    one.setBounds(50, 60, (getWidth() -100), 20); 
    two.setBounds(50, 90, (getWidth() -100), 20);
    three.setBounds(190, 135, (getWidth() -250), 20); 
    four.setBounds(190, 165, (getWidth() - 250), 20); 
}

void AMAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.arpSpeed = arpSlider.getValue();
    audioProcessor.octaves = octaveSlider.getValue();
    audioProcessor.arpSpeed2 = arpSlider2.getValue();
    audioProcessor.arpSpeed3 = arpSlider3.getValue();

}
