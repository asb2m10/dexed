/*
  ==============================================================================

    PluginFx.h
    Created: 26 Dec 2013 7:13:29pm
    Author:  Pascal Gauthier

  ==============================================================================
*/

#ifndef PLUGINFX_H_INCLUDED
#define PLUGINFX_H_INCLUDED

class PluginFx {
    /**
     * Used for the 4pole LFP
     */
    // process and ui values
    float pReso;
    float pCutoff;
    float p, Q;
    int srate;
    float state[4];
    float output;
public:
    
    float uiCutoff;
    float uiReso;
    
    void init(int sampleRate);
    void process(float *work, int sampleSize);
};

#endif  // PLUGINFX_H_INCLUDED
