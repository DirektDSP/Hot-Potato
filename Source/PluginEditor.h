#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "StyleSheet.h"
#include "PresetPanel.h"

#define PLUGIN_VERSION JucePlugin_VersionString

//==============================================================================
/**
*/
class Hot_PotatoAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Hot_PotatoAudioProcessorEditor (Hot_PotatoAudioProcessor&);
    ~Hot_PotatoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Hot_PotatoAudioProcessor& audioProcessor;

    Gui::PresetPanel presetPanel;

    ExtLabel versionLabel;

    RasterKnob gainSlider;
    RasterKnob outGain;
    RasterKnob inGain;
    RasterKnob freqSlider;
	RasterKnob clipSlider;
    RasterKnob powerSlider;

    BypassButton bypassButton;
    
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outGainAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inGainAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> clipAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> powerAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Hot_PotatoAudioProcessorEditor)
};
