/*
 ==============================================================================
 
 This file is part of the ambix Ambisonic plug-in suite.
 Copyright (c) 2013/2014 - Matthias Kronlachner
 www.matthiaskronlachner.com
 
 Permission is granted to use this software under the terms of:
 the GPL v2 (or any later version)
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 ambix is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ==============================================================================
 */

#ifndef __PLUGINPROCESSOR_H_58CE358E__
#define __PLUGINPROCESSOR_H_58CE358E__

//////////////////////////////////////////////
// CHANGE AMBISONICS ORDER IN AppConfig.h !!!!
//////////////////////////////////////////////

#include "JuceHeader.h"

#include "AmbixEncoder.h"

#include "MyMeterDsp/MyMeterDsp.h"

#if WITH_OSC
    #include "lo/lo.h"
#endif

#define _2PI 6.2831853071795

//==============================================================================
/**
*/
class Ambix_encoderAudioProcessor  : public AudioProcessor,
#if WITH_OSC
                                    public Timer,
#endif
                                    public ChangeBroadcaster
{
public:
    //==============================================================================
    Ambix_encoderAudioProcessor();
    ~Ambix_encoderAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);
    const String getParameterLabel(int index);
    
    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

    void calcAzimuth();
    
#if WITH_ADVANCED_CONTROL
    void calcNewParameters(double SampleRate, int BufferLength);
#endif
    
    enum Parameters
	{
		AzimuthParam,
        ElevationParam,
        SizeParam,
#if INPUT_CHANNELS > 1
        WidthParam, // if multiple sources
#endif
        AzimuthSetParam, // for advanced control...
        AzimuthSetRelParam,
        AzimuthMvParam,
        ElevationSetParam,
        ElevationSetRelParam,
        ElevationMvParam,
        SpeedParam
	};
    
    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

    
    int m_id; // id of this instance
    
    static int s_ID; // global instance counter
    
#if WITH_OSC
    void timerCallback(); // call osc send in timer callback
    
    void sendOSC(); // send osc data
    
    bool oscOut(bool arg); // activate osc out
    bool oscIn(bool arg); // activate osc in
    
    void changeTimer(int time);
    
    // osc stuff
    bool osc_in;
	bool osc_out;
    int osc_interval;
    
	String osc_error;
    
	String osc_in_port, osc_out_ip, osc_out_port;
    
    lo_server_thread st;
	Array<lo_address> addresses;
#endif
    
    ApplicationProperties myProperties;
    
private:
    OwnedArray<AmbixEncoder> AmbiEnc;
    
    double SampleRate;
    
    unsigned int NumParameters;
    
    float azimuth_param; // for multiple inputs this is the center
    float elevation_param;
    float size_param;
    float width_param;  // arrange sources with equal angular distance
    
    // last osc value sent...
    float _azimuth_param; // for multiple inputs this is the center
    float _elevation_param;
    float _size_param;
    float _rms;
    float _dpk;
    
    // advanced parameter control
    float speed_param;
    float azimuth_set_param, azimuth_set_rel_param, azimuth_mv_param;
    float elevation_set_param, elevation_set_rel_param, elevation_mv_param;
    
    AudioSampleBuffer InputBuffer;
    
    MyMeterDsp _my_meter_dsp;
    
    float rms; // rms of W channel
    float dpk; // peak value of W channel
    /*
    void calcParams();
     
    Array<float> ambi_gain;
    Array<float> _ambi_gain; // buffer for gain ramp
    */
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Ambix_encoderAudioProcessor)
};

#endif  // __PLUGINPROCESSOR_H_58CE358E__
