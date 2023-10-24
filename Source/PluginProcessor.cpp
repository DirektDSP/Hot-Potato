/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "StyleSheet.h"

//==============================================================================
Hot_PotatoAudioProcessor::Hot_PotatoAudioProcessor()

#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                    ),
#endif
            apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    apvts.state.setProperty(Service::PresetManager::presetNameProperty, "", nullptr);
    apvts.state.setProperty("version", ProjectInfo::versionString, nullptr);

    presetManager = std::make_unique<Service::PresetManager>(apvts);
};

Hot_PotatoAudioProcessor::~Hot_PotatoAudioProcessor()
{
    
}


//==============================================================================
const juce::String Hot_PotatoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Hot_PotatoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Hot_PotatoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Hot_PotatoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Hot_PotatoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Hot_PotatoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Hot_PotatoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Hot_PotatoAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Hot_PotatoAudioProcessor::getProgramName (int index)
{
    return {};
}

void Hot_PotatoAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Hot_PotatoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Hot_PotatoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Hot_PotatoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

/*
float distort(float input, float gain, float freq, float clipAmt, float power) {
    float output = tanh(gain * input);
	float pi = 3.14159265358979323846;
    output = (sin(pi * output * freq) + sinh(pi * input )) / 2;
    output = tanh(output * power);
    output = output * 0.6f;
    
    return juce::jlimit<float>(-clipAmt, clipAmt, output);
}
*/

float dist(float x, float g, float f, float p)
{
    float pi = 3.14159265358979323846;
    float output = tanh(g * x);
    output = (sin(pi * output * f) + tanh(pi * x)) / 2;
    output = (tanh((output / pi) * p) + tanh((x * pi) / (pi * pi))) * (4 / (p + 1)) * (1 + (p / 10));
    return output * 0.8f;
}

void Hot_PotatoAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.


    // get parameter values
	auto g = apvts.getRawParameterValue("GAIN")->load();
    auto f = apvts.getRawParameterValue("FREQ")->load();
	auto p = apvts.getRawParameterValue("POWER")->load();
    p = p / 10; // Changes the 0->100 to 0->10
	auto c = apvts.getRawParameterValue("CLIP")->load();
	auto g2 = apvts.getRawParameterValue("OUTGAIN")->load();
	auto i = apvts.getRawParameterValue("INGAIN")->load();
	bool bp = apvts.getRawParameterValue("BYPASS")->load();
    
    

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
    }
    
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
            // clip the signal to the amp value
		    for (int channel = 0; channel < totalNumInputChannels; ++channel)
		    {
                auto* channelData = buffer.getWritePointer(channel);
                float input = channelData[sample];

                if (bp == false)
                {
                    // distort
                    input = input * i;
                    float output = dist(input, g, f, p);
                    output = juce::jlimit<float>(-c, c, output);
                    output *= 1 / c;
                    output *= g2;
                    channelData[sample] = output;
                }
                else {
                    channelData[sample] = input;
                }
            }
    }
};

//==============================================================================
bool Hot_PotatoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Hot_PotatoAudioProcessor::createEditor()
{
    return new Hot_PotatoAudioProcessorEditor (*this);
}

//==============================================================================
void Hot_PotatoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void Hot_PotatoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Hot_PotatoAudioProcessor();
}
