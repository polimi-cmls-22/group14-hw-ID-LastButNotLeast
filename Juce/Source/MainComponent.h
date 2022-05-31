#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class OtherLookAndFeel : public juce::LookAndFeel_V4,private juce::Component
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider& slider) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
        float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    void drawLinearThumb(juce::Graphics& g, float x, float y, int width, int height, float sliderPos, juce::Slider& slider);

    void drawLinearBackground(juce::Graphics& g, int x, int y, int width, int height,float sliderPos, juce::Slider& slider);
    
    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;
    void drawButtonText (juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override;

    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY,
        int buttonW, int buttonH, juce::ComboBox& comboBox) override;

private:

    void drawThumb(juce::Graphics& g, float x, float y, float diameter, float outlineThickness);



};



class MainComponent  : public juce::Component, private juce::Button::Listener, private juce::Slider::Listener, private juce::OSCReceiver, 
    private juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::MessageLoopCallback>
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Connection
    juce::OSCSender sender;
    juce::DatagramSocket ds;
    void changeNoteState(int buttonId, bool on);
    void oscMessageReceived(const juce::OSCMessage& message) override;
    
    // Images
    juce::ImageComponent fluteImage;
    juce::ImageComponent logoImage;
    
    // ComboBox
    juce::ComboBox instrumentCombo;
    
    // Look and feel
    OtherLookAndFeel otherLookAndFeel;
    
    // Knobs
    juce::Slider volume;
    juce::Slider pan;
    juce::Slider octave;
    void sliderValueChanged(juce::Slider* slider) override;
    
    // Buttons
    juce::TextButton noteC;
    juce::TextButton noteD;
    juce::TextButton noteE;
    juce::TextButton noteF;
    juce::TextButton noteG;
    juce::TextButton noteA;
    juce::TextButton noteB;
    juce::TextButton sharp;
    
    
    // Function definition
    void buttonClicked (juce::Button * button) override;
    
    // Label
    juce::Label volumeLabel;
    juce::Label panLabel;
    juce::Label octaveLabel;

    
    
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
