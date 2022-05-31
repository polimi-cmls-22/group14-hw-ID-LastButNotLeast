#include "MainComponent.h"

void OtherLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle,
                                        float rotaryEndAngle, juce::Slider& slider) {
    {
        float diameter = fmin(width, height) * 0.8;
        float radius = diameter / 2;
        float centerX = x + width / static_cast<float>(2);
        float centerY = y + height / static_cast<float>(2);
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        
        juce::Rectangle<float> dialArea(rx, ry, diameter, diameter);
        juce::Rectangle<int> dialArea1(rx, ry, diameter, diameter);
        juce::Path dialTick;
        juce::Path dialTickContour;
        juce::Path ellipse;
        juce::Path arc;
        juce::DropShadow shadow;
        
        g.setColour(juce::Colours::transparentBlack);
        
        arc.addArc(rx, ry + 8, diameter, diameter, juce::float_Pi * 0.5, juce::float_Pi * 1.5, true);
        g.fillPath(arc);
        
        
        shadow.drawForPath(g, arc);
        
        g.setColour(juce::Colours::dimgrey);
        g.fillEllipse(dialArea);
        
        g.setColour(juce::Colours::darkslategrey);
        ellipse.addEllipse((centerX - radius * 0.8), (centerY - radius * 0.8), diameter * 0.8, diameter * 0.8);
        g.fillPath(ellipse);
        
        
        g.setColour(juce::Colours::black);
        g.drawEllipse((centerX - radius * 0.8), (centerY - radius * 0.8), diameter * 0.8, diameter * 0.8, 2.5f);
        
        shadow.colour = juce::Colours::grey;
        shadow.drawForPath(g, ellipse);
        
        g.setColour(juce::Colours::aqua);
        dialTick.addRectangle(0, -radius, 6.0f, radius * 0.5);
        g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centerX, centerY));
        shadow.radius = 30;
        shadow.colour = juce::Colours::aqua;
        shadow.drawForPath(g, dialTick);
        
        g.setColour(juce::Colours::black);
        dialTickContour.addRectangle(0, -radius, 6.0f, radius * 0.5);
        g.strokePath(dialTickContour, juce::PathStrokeType(2.0, juce::PathStrokeType::JointStyle::beveled),
                     juce::AffineTransform::rotation(angle).translated(centerX, centerY));
        
        
        
        g.setColour(juce::Colours::black);
        g.drawEllipse(rx, ry, diameter, diameter, 2.5f);
    }
}

void OtherLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
                                        float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    
    drawLinearBackground(g, x, y, width, height, sliderPos, slider);
    drawLinearThumb(g, x, y, width, height, sliderPos,slider);
    
}

void OtherLookAndFeel::drawLinearThumb(juce::Graphics& g, float x, float y, int width, int height, float sliderPos, juce::Slider& slider)
{
    
    auto sliderRadius = (float)(getSliderThumbRadius(slider) - 2);
    auto outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;
    
    float kx, ky;
    kx = (float)x + (float)width * 0.5f;
    ky = sliderPos;
    
    drawThumb(g,
              kx - sliderRadius,
              ky - sliderRadius,
              sliderRadius * 2.0f,
              outlineThickness);
    
}

void OtherLookAndFeel::drawThumb(juce::Graphics& g, float x, float y, float diameter, float outlineThickness)
{

    auto halfThickness = outlineThickness * 0.5f;
    auto colour = juce::Colours::aqua;
    juce::DropShadow shadow;
    
    juce::Path thumb;
    
    
    thumb.addRectangle(x + halfThickness,
                       y - halfThickness,
                       diameter,
                       diameter);
    
    g.setColour(colour);
    g.fillPath(thumb);
    shadow.colour = juce::Colours::aqua;
    shadow.drawForPath(g, thumb);
    g.setColour(juce::Colours::black);
    g.strokePath(thumb, juce::PathStrokeType(2.0, juce::PathStrokeType::JointStyle::beveled));
    
    
}


void OtherLookAndFeel::drawLinearBackground(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, juce::Slider& slider)
{
    
    auto sliderRadius = getSliderThumbRadius(slider) - 5.0f;
    auto colour = juce::Colours::black;
    
    juce::Path on, off;
    
    auto ix = (float)x + (float)width * 0.5f - sliderRadius * 0.5f;
    juce::Rectangle<float> r(ix, (float)y + sliderRadius * 0.5f, sliderRadius, (float)height - sliderRadius);
    auto onH = r.getHeight() * ((float)slider.valueToProportionOfLength(slider.getValue()));
    
    on.addRectangle(r.removeFromBottom(onH));
    off.addRectangle(r);
    
    
    g.setColour(colour);
    g.fillPath(on);
    
    g.setColour(colour.brighter());
    g.fillPath(off);
    
}

void OtherLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) {
    auto buttonArea = button.getLocalBounds();

    buttonArea.reduce(button.getHeight()/4, 5);
    auto edge = 2;
    juce::DropShadow shadow;
  
    
    buttonArea.removeFromLeft (edge);
    buttonArea.removeFromTop (edge);
    
    
    auto offset = isButtonDown ? -edge / 2 : -edge;
    buttonArea.translate (offset, offset);
    
    juce::PixelRGB pixel;
    pixel.setARGB(255, 125, 78, 0);
    g.setColour(juce::Colour(pixel));
//    g.fillRect (buttonArea);
    g.fillRoundedRectangle(buttonArea.toFloat(), 100);
    if(button.getState() == juce::Button::ButtonState::buttonDown){
        g.setColour(juce::Colours::aqua);
    }
    else g.setColour(juce::Colours::black);

    g.drawRoundedRectangle(buttonArea.toFloat(), 100, 1);
    

}

void OtherLookAndFeel::drawButtonText (juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown){
    auto font = getTextButtonFont (button, button.getHeight());
    g.setFont (font);
    g.setColour (button.findColour (button.getToggleState() ? juce::TextButton::textColourOnId: juce::TextButton::textColourOffId).withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));
    
    auto yIndent = juce::jmin (4, button.proportionOfHeight (0.3f));
    auto cornerSize = juce::jmin (button.getHeight(), button.getWidth()) / 2;
    
    auto fontHeight = juce::roundToInt (font.getHeight() * 0.6f);
    auto leftIndent  = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft()  ? 4 : 2));
    auto rightIndent = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    auto textWidth = button.getWidth() - leftIndent - rightIndent;
    
    auto edge = 4;
    auto offset = isButtonDown ? edge / 2 : 0;
    
    if (textWidth > 0)
        g.drawFittedText (button.getButtonText(),
                          leftIndent + offset, yIndent + offset, textWidth, button.getHeight() - yIndent * 2 - edge,
                          juce::Justification::centred, 2);


}

void OtherLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY,
    int buttonW, int buttonH, juce::ComboBox& comboBox)

{
    juce::Rectangle<int> areaCombo = comboBox.getLocalBounds();
    juce::Point<float> point1;
    juce::Point<float> point2;
    juce::Point<float> point3;
    juce::Point<int> offs;
    juce::DropShadow shadow;
    juce::Path triangle;

    point1.setXY(buttonX, buttonY);
    point2.setXY(buttonX + 5.0f, buttonY);
    point3.setXY(buttonX + 2.5f, buttonY - 5.0f);

    comboBox.setPaintingIsUnclipped(true);
    offs.setXY(3, 4);
    shadow.offset = offs;
    shadow.drawForRectangle(g, areaCombo);

    g.setColour(juce::Colours::dimgrey);
    g.fillRect(areaCombo);

    g.setColour(juce::Colours::black);
    g.drawRect(areaCombo, 2.0);


    triangle.addTriangle(static_cast<float>(buttonX) - 2.0f + buttonW * 0.3, static_cast<float>(buttonY) + buttonH * 0.25,
        static_cast<float>(buttonX) + 12.0f + buttonW * 0.3, static_cast<float>(buttonY) + buttonH * 0.25,
        static_cast<float>(buttonX) + 5.0f + buttonW * 0.3, static_cast<float>(buttonY) + 5.0f + buttonH * 0.5);
    g.setColour(juce::Colours::aqua);
    g.fillPath(triangle);
    g.setColour(juce::Colours::black);
    g.strokePath(triangle, juce::PathStrokeType(2.0, juce::PathStrokeType::JointStyle::beveled));


}


//==============================================================================
MainComponent::MainComponent()
{
    sender.connect("127.0.0.1", 57120);
    ds.bindToPort(57109, "127.0.0.1");
    if (!connectToSocket(ds)) {
        DBG("ERROR while connecting to socket");
    }
    addListener(this, "/notes/noteOn");
    addListener(this, "/notes/noteOff");

    auto flute = juce::ImageCache::getFromMemory(BinaryData::flute1_png, BinaryData::flute1_pngSize);
        
        if(!flute.isNull()){
            fluteImage.setImage(flute, juce::RectanglePlacement::stretchToFit);
        }
        else{
            jassert(! flute.isNull());
        }
    
    addAndMakeVisible(fluteImage);
    
    auto logo = juce::ImageCache::getFromMemory(BinaryData::logo_png, BinaryData::logo_pngSize);
        
        if(!logo.isNull()){
            logoImage.setImage(logo, juce::RectanglePlacement::stretchToFit);
        }
        else{
            jassert(! logo.isNull());
        }
    
    addAndMakeVisible(logoImage);
    
    addAndMakeVisible(instrumentCombo);
    instrumentCombo.addItem("flute", 1);
    instrumentCombo.addItem("sax", 2);
    instrumentCombo.addItem("organ", 3);
    instrumentCombo.onChange = [this] {
        sender.send("/gui/synth", instrumentCombo.getSelectedId() - 1);
    };
    instrumentCombo.setSelectedId(1);
    
    instrumentCombo.setLookAndFeel(&otherLookAndFeel);

    volume.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volume.setRange(0, 100, 1);
    volume.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 45, 25);
    addAndMakeVisible(volume);
    volume.setValue(20);
    volume.setLookAndFeel(&otherLookAndFeel);
    volume.addListener(this);
    
    volumeLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(volumeLabel);
    
    pan.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pan.setRange(-1, 1, 0.05);
    pan.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 45, 25);
    addAndMakeVisible(pan);
    pan.setValue(0);
    pan.setLookAndFeel(&otherLookAndFeel);
    pan.addListener(this);
    
    panLabel.setText("Pan", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(panLabel);
    
    octave.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    octave.setRange(-2, 2, 1);
    octave.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 45, 25);
    addAndMakeVisible(octave);
    octave.setValue(0);
    octave.setLookAndFeel(&otherLookAndFeel);
    octave.addListener(this);
    
    octaveLabel.setText("Octave", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(octaveLabel);
     
    addAndMakeVisible(noteC);
    noteC.setLookAndFeel(&otherLookAndFeel);
    
    addAndMakeVisible(noteD);
    noteD.setLookAndFeel(&otherLookAndFeel);
    
    addAndMakeVisible(noteE);
    noteE.setLookAndFeel(&otherLookAndFeel);
    
    addAndMakeVisible(noteF);
    noteF.setLookAndFeel(&otherLookAndFeel);
    
    addAndMakeVisible(noteG);
    noteG.setLookAndFeel(&otherLookAndFeel);
    
    addAndMakeVisible(noteA);
    noteA.setLookAndFeel(&otherLookAndFeel);
    
    addAndMakeVisible(noteB);
    noteB.setLookAndFeel(&otherLookAndFeel);
    
    addAndMakeVisible(sharp);
    sharp.setLookAndFeel(&otherLookAndFeel);
    
    /*noteC.onClick = [this]{
        noteC.setState(juce::Button::ButtonState::buttonDown);
    };
    
    noteD.onClick = [this]{
        noteD.setState(juce::Button::ButtonState::buttonDown);
    };
    
    noteE.onClick = [this]{
        noteE.setState(juce::Button::ButtonState::buttonDown);
    };
    
    noteF.onClick = [this]{
        noteF.setState(juce::Button::ButtonState::buttonDown);
    };
    
    noteG.onClick = [this]{
        noteG.setState(juce::Button::ButtonState::buttonDown);
    };
    
    noteA.onClick = [this]{
        noteA.setState(juce::Button::ButtonState::buttonDown);
    };
    
    noteB.onClick = [this]{
        noteB.setState(juce::Button::ButtonState::buttonDown);
    };
    
    sharp.onClick = [this]{
        sharp.setState(juce::Button::ButtonState::buttonDown);
    };*/
    
    setSize (400, 500);
    
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkslategrey);

    g.setFont (juce::Font (16.0f));
    
}

void MainComponent::resized()
{
    setSize(400, 500);
    juce::Rectangle<int> window = getLocalBounds();
    fluteImage.setBounds(window.reduced(10, 50));
    juce::Rectangle<int> top = window.removeFromTop(130);
    top.removeFromLeft(top.getWidth()/3);
    top.removeFromRight(top.getWidth()/2);
    logoImage.setBounds(top.reduced(30));
    
    instrumentCombo.setBounds(30, 280, 100, 30);
    
    volume.setBounds(260, 130, 100, 150);
    volumeLabel.setBounds(280, 60, 100, 150);
    
    pan.setBounds(30, 370, 80, 100);
    panLabel.setBounds(50, 310, 80, 100);
    octave.setBounds(130, 370, 80, 100);
    octaveLabel.setBounds(140, 310, 80, 100);
    
    noteC.setBounds(132, 176, 30, 30);
    noteD.setBounds(155, 198, 30, 30);
    noteE.setBounds(174, 219, 30, 30);
    noteF.setBounds(199, 244, 30, 30);
    noteG.setBounds(229, 275, 30, 30);
    noteA.setBounds(250, 296, 30, 30);
    noteB.setBounds(272, 317, 30, 30);
    sharp.setBounds(313, 359, 30, 30);
    
    
}

void MainComponent::buttonClicked(juce::Button * button){
  
}

void MainComponent::changeNoteState(int buttonId, bool on) {
    if (on) {
        switch (buttonId) {
        case 0:
            noteC.setState(juce::Button::ButtonState::buttonDown);
            break;
        case 1:
            noteD.setState(juce::Button::ButtonState::buttonDown);
            break;
        case 2:
            noteE.setState(juce::Button::ButtonState::buttonDown);
            break;
        case 3:
            noteF.setState(juce::Button::ButtonState::buttonDown);
            break;
        case 4:
            noteG.setState(juce::Button::ButtonState::buttonDown);
            break;
        case 5:
            noteA.setState(juce::Button::ButtonState::buttonDown);
            break;
        case 6:
            noteB.setState(juce::Button::ButtonState::buttonDown);
            break;
        case 7:
            sharp.setState(juce::Button::ButtonState::buttonDown);
            break;
        default:
            DBG("NO CORRESPONDING NOTE (on)");
            break;
        }
    }
    else {
        switch (buttonId) {
        case 0:
            noteC.setState(juce::Button::ButtonState::buttonNormal);
            break;
        case 1:
            noteD.setState(juce::Button::ButtonState::buttonNormal);
            break;
        case 2:
            noteE.setState(juce::Button::ButtonState::buttonNormal);
            break;
        case 3:
            noteF.setState(juce::Button::ButtonState::buttonNormal);
            break;
        case 4:
            noteG.setState(juce::Button::ButtonState::buttonNormal);
            break;
        case 5:
            noteA.setState(juce::Button::ButtonState::buttonNormal);
            break;
        case 6:
            noteB.setState(juce::Button::ButtonState::buttonNormal);
            break;
        case 7:
            sharp.setState(juce::Button::ButtonState::buttonNormal);
            break;
        default:
            DBG("NO CORRESPONDING NOTE (off)");
            break;
        }
    }
};

void MainComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volume) {
        sender.send("/gui/volume", (float)slider->getValue() / 100.0f);
    }
    if (slider == &pan) {
        sender.send("/gui/pan", (float)slider->getValue());
    }
    if (slider == &octave) {
        sender.send("/gui/octave", (float)slider->getValue());
    }
}

void MainComponent::oscMessageReceived(const juce::OSCMessage& message) {
    if (message.getAddressPattern() == "/notes/noteOn") {
        changeNoteState(message[0].getInt32(), true);
    }
    if (message.getAddressPattern() == "/notes/noteOff") {
        changeNoteState(message[0].getInt32(), false);
    }
};



