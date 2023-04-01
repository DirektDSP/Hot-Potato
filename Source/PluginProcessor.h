/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PresetManager.h"

//==============================================================================
/**
*/
class Hot_PotatoAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    Hot_PotatoAudioProcessor();
    ~Hot_PotatoAudioProcessor() override;

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

    Service::PresetManager& getPresetManager() { return *presetManager; }

    AudioProcessorValueTreeState apvts;
    
	static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
	{
		std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

		params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Boost", 1.0f, 3.14f, 1.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>("FREQ", "Tone", 1.0f, 10.0f, 1.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>("CLIP", "Clip Amount", 0.1f, 1.0f, 1.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>("POWER", "Power", 0.0f, 100.0f, 0.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>("OUTGAIN", "Out Gain", 0.0f, 1.5f, 1.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>("INGAIN", "In Gain", 0.0f, 1.5f, 1.0f));
        
		params.push_back(std::make_unique<juce::AudioParameterBool>("BYPASS", "Bypass", false));

        
		return { params.begin(), params.end() };
	}
    
private:
    //==============================================================================
    //AudioProcessorValueTreeState apvts;
    std::unique_ptr<Service::PresetManager> presetManager;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Hot_PotatoAudioProcessor)
};
