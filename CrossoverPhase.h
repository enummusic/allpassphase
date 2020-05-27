//-------------------------------------------------------------------------------------------------------
// $Date: 2020/05/24 00:00:00 $
//
// Filename     : CrossoverPhase.h
// Created by   : enummusic
// Description  : Crossover filter phase dispersion
//
// © 2020 enummusic
// VST SDK © 2005, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __CrossoverPhase__
#define __CrossoverPhase__

#include <string>

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "LRCrossoverFilter.h"

//class LRCrossoverFilter;

enum
{
	// Global
	kNumPrograms = 16,

	// Parameters Tags
	kFrequency = 0,
	kIterations,
	kOut,
	kClip,

	kNumParams
};

class CrossoverPhase;

//------------------------------------------------------------------------
class CrossoverPhaseProgram
{
friend class CrossoverPhase;
public:
	CrossoverPhaseProgram ();
	~CrossoverPhaseProgram () {}

private:	
	float fFrequency;
	float fIterations;
	float fOut;
	float fClip;
	char name[24];
};

//------------------------------------------------------------------------
class CrossoverPhase : public AudioEffectX
{
public:
	CrossoverPhase (audioMasterCallback audioMaster);
	~CrossoverPhase ();

	virtual void processReplacing (float **inputs, float **outputs, VstInt32 sampleFrames);

	virtual void setProgram (long program);
	virtual void setProgramName (char *name);
	virtual void getProgramName (char *name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char *label);
	virtual void getParameterDisplay (VstInt32 index, char *text);
	virtual void getParameterName (VstInt32 index, char *text);

	void mydB2string(float value, char * text, VstInt32 maxLen);

	virtual void resume ();

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }

protected:
	void setDelay (float delay);

	CrossoverPhaseProgram *programs;
	
	float *buffer;
	float fFrequency, fIterations, fOut, fClip;
	
	long delay;
	long size;
	long cursor;

	LRCrossoverFilter filter1;
	LRCrossoverFilter filter2;
	LRCrossoverFilter filterL[50];
	LRCrossoverFilter filterR[50];
	int crossover;
	int curIterations;

	int samplesSinceSilence = 1;
	const int deactivateAfterSamples = 16384;
	const float noiseFloor = 0.000007; // -113dB ish, idk it just works lol

	//float dbginfo = 0;
};

#endif
