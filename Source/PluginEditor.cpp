/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define PLUGIN_VERSION JucePlugin_VersionString

int textBoxWidth = 80;
int textBoxHeight = 20;

//==============================================================================
Hot_PotatoAudioProcessorEditor::Hot_PotatoAudioProcessorEditor (Hot_PotatoAudioProcessor& p) :
		AudioProcessorEditor (&p),
		audioProcessor (p),
		presetPanel(p.getPresetManager())
{

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

	
	addAndMakeVisible(presetPanel);
	
	
    setSize (400, 600);

	gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
	freqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FREQ", freqSlider);
	clipAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CLIP", clipSlider);
	powerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "POWER", powerSlider);
	outGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OUTGAIN", outGain);
	inGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INGAIN", inGain);
	bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "BYPASS", bypassButton);

	
}

Hot_PotatoAudioProcessorEditor::~Hot_PotatoAudioProcessorEditor()
{
}

//==============================================================================
void Hot_PotatoAudioProcessorEditor::paint (juce::Graphics& g)
{

	auto backgroundImage = juce::ImageCache::getFromMemory(BinaryData::HotPotatoBG_png, BinaryData::HotPotatoBG_pngSize);
	
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

	// draw bg image
	g.drawImage(backgroundImage, getLocalBounds().toFloat());

	gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
	gainSlider.setTextValueSuffix(" dB");
	addAndMakeVisible(gainSlider);

	outGain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	outGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
	outGain.setTextValueSuffix(" dB");
	addAndMakeVisible(outGain);

	inGain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	inGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
	inGain.setTextValueSuffix(" dB");
	addAndMakeVisible(inGain);

	freqSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	freqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
	freqSlider.setTextValueSuffix(" dB");
	addAndMakeVisible(freqSlider);

	clipSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	clipSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
	clipSlider.setTextValueSuffix(" dB");
	addAndMakeVisible(clipSlider);

	powerSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	powerSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
	powerSlider.setTextValueSuffix(" %");
	addAndMakeVisible(powerSlider);

	bypassButton.setButtonText("Bypass");
	bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
	bypassButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
	addAndMakeVisible(bypassButton);

	versionLabel.setText("Version " PLUGIN_VERSION, juce::dontSendNotification);
	addAndMakeVisible(versionLabel);

}

void Hot_PotatoAudioProcessorEditor::resized()
{
	presetPanel.setBounds(getLocalBounds().removeFromTop(proportionOfHeight(0.1f)));
	
	//window is 400 x 600
	
	int border = 20; // between knobs
	int padding = 50; // knob and edge
	
	int ppOffset = presetPanel.getHeight() + padding + 45;
	
	auto row1 = Rectangle<int>(padding, ppOffset, 300, 110);

	Grid g1;
	using Track = Grid::TrackInfo;
	g1.templateRows = { Track(1_fr) };
	g1.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr) };
	g1.columnGap = Grid::Px(border);
	g1.items = { 
		GridItem(gainSlider),
		GridItem(freqSlider),
		GridItem(clipSlider),
	};
	g1.performLayout(row1);

	auto row2 = Rectangle<int>(padding, ppOffset + 110 + border, 300, 110);
	
	Grid g2;
	g2.templateRows = { Track(1_fr) };
	g2.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr) };
	g2.columnGap = Grid::Px(border);
	g2.items = {
		GridItem(powerSlider),
		GridItem(inGain),
		GridItem(outGain),
	};
	g2.performLayout(row2);
	
	// bypass button
	int bypSize = 75;

	bypassButton.setBounds(getWidth() / 2 - (bypSize / 2), 450, bypSize, bypSize);

	versionLabel.setBounds(getBounds().removeFromBottom(20).removeFromLeft(100));
	
}
