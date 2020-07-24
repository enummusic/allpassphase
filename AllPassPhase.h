//-------------------------------------------------------------------------------------------------------
// $Date: 2020/05/24 00:00:00 $
//
// Filename     : AllPassPhase.h
// Created by   : enummusic
// Description  : Crossover filter phase dispersion
//
// © 2020 enummusic
// VST SDK © 2005, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __AllPassPhase__
#define __AllPassPhase__

#include <string>

#include "public.sdk/source/vst2.x/audioeffectx.h"
//#include "LRCrossoverFilter.h"
#include "AllPassFilter.h"

//class LRCrossoverFilter;

enum
{
	// Global
	kNumPrograms = 16,

	// Parameters Tags
	kFrequency = 0,
	kQ,
	kIterations,
	//kOut,
	kMix,

	kNumParams
};

class AllPassPhase;

//------------------------------------------------------------------------
class AllPassPhaseProgram
{
friend class AllPassPhase;
public:
	AllPassPhaseProgram ();
	~AllPassPhaseProgram () {}

private:	
	float fFrequency;
	float fQ; // get it?? lmao
	float fIterations;
	float fOut;
	float fMix;
	char name[24];
};

//------------------------------------------------------------------------
class AllPassPhase : public AudioEffectX
{
public:
	AllPassPhase (audioMasterCallback audioMaster);
	~AllPassPhase ();

	virtual void processReplacing (float **inputs, float **outputs, VstInt32 sampleFrames);

	virtual void setProgram (long program);
	virtual void setProgramName (char *name);
	virtual void getProgramName (char *name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	
	virtual void setParameter (VstInt32 index, float value);
	void setupFilters();
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char *label);
	virtual void getParameterDisplay (VstInt32 index, char *text);
	virtual void getParameterName (VstInt32 index, char *text);

	void mydB2string(float value, char * text, VstInt32 maxLen);

	int knobToFrequency(float x);

	virtual void resume ();

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }

protected:
	AllPassPhaseProgram *programs;
	
	float *buffer;
	float fFrequency, fIterations, fQ, fOut, fMix;

	long size;

	AllPassFilter filterL[50];
	AllPassFilter filterR[50];
	int freq;
	int curIterations;
	float q;

	int samplesSinceSilence = 1;
	const int deactivateAfterSamples = 16384;
	const float noiseFloor = 0.000007; // -113dB ish, idk it just works lol
	float lastfFreq = 0;
	//float dbginfo = 0;
};

#endif
