//-------------------------------------------------------------------------------------------------------
// $Date: 2020/05/24 00:00:00 $
//
// Filename     : AllPassPhase.cpp
// Created by   : enummusic
// Description  : Crossover filter phase dispersion
//
// © 2020 enummusic
// VST SDK © 2005, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include <thread>

#ifndef __AllPassPhase__
#include "AllPassPhase.h"

#include "HardClip.h"
#endif

//----------------------------------------------------------------------------- 
AllPassPhaseProgram::AllPassPhaseProgram ()
{
	// default Program Values
	fFrequency = 0.5131f;
	fIterations = 0.5;
	fQ = 0.5;
	fOut = 0.5;
	fMix = 1;

	strcpy (name, "Init");
}

//-----------------------------------------------------------------------------
AllPassPhase::AllPassPhase (audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init
	
	size = 44100;
	buffer = new float[size];
	
	programs = new AllPassPhaseProgram[numPrograms];
	fFrequency = 0.3675f;
	fQ = 0.5;
	fIterations = 0.5;
	fOut = 0.5;
	fMix = 1;

	if (programs)
		setProgram (0);

	setNumInputs (2);
	setNumOutputs (2);

	setUniqueID ('aphs');

	curIterations = 25;
	freq = pow(200, fFrequency) * 100;
	q = fQ * sqrt(2);
	filterL[0].setup(freq, 44100.0f, q);
	filterR[0].setup(freq, 44100.0f, q);
	for (int i = 0; i < 50; i++) {
		filterL[i].copyCoefficientsFrom(filterL[0]);
		filterR[i].copyCoefficientsFrom(filterR[0]);
	}

	resume ();		// flush buffer
}

//------------------------------------------------------------------------
AllPassPhase::~AllPassPhase ()
{
	if (buffer)
		delete[] buffer;
	if (programs)
		delete[] programs;
}

//------------------------------------------------------------------------
void AllPassPhase::setProgram (long program)
{
	AllPassPhaseProgram* ap = &programs[program];

	curProgram = program;
	setParameter (kFrequency, ap->fFrequency);	
	setParameter (kIterations, ap->fIterations);
	//setParameter (kOut, ap->fOut);
}

//------------------------------------------------------------------------
void AllPassPhase::setProgramName (char *name)
{
	strcpy (programs[curProgram].name, name);
}

//------------------------------------------------------------------------
void AllPassPhase::getProgramName (char *name)
{
	if (!strcmp (programs[curProgram].name, "Init"))
		sprintf (name, "%s %d", programs[curProgram].name, curProgram + 1);
	else
		strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool AllPassPhase::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index < kNumPrograms)
	{
		strcpy (text, programs[index].name);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
void AllPassPhase::resume ()
{
	memset (buffer, 0, size * sizeof (float));
	AudioEffectX::resume ();
}

//------------------------------------------------------------------------
void AllPassPhase::setParameter (VstInt32 index, float value)
{
	AllPassPhaseProgram* ap = &programs[curProgram];

	switch (index)
	{
		case kFrequency:    
			fFrequency = ap->fFrequency = value;
			programs[curProgram].fFrequency = value;
			break;
		case kQ:		fQ = ap->fQ = value; break;
		case kIterations : fIterations = ap->fIterations = value; break;
		//case kOut :      fOut = ap->fOut = value;			break;
		case kClip:		fMix = ap->fMix = value;
	}
}

//------------------------------------------------------------------------
float AllPassPhase::getParameter (VstInt32 index)
{
	float v = 0;

	switch (index)
	{
		case kFrequency :    v = fFrequency;	break;
		case kQ : v = fQ; break;
		case kIterations : v = fIterations; break;
		//case kOut :      v = fOut;		break;
		case kClip: v = fMix;
	}
	return v;
}

//------------------------------------------------------------------------
void AllPassPhase::getParameterName (VstInt32 index, char *label)
{
	switch (index)
	{
		case kFrequency :    strcpy (label, "Frequency");		break;
		case kQ : strcpy (label, "Q");	break;
		case kIterations : strcpy (label, "Intensity");	break;
		//case kOut :      strcpy (label, "Volume");		break;
		case kClip:      strcpy(label, "Mix");		break;
	}
}

void AllPassPhase::mydB2string(float value, char* text, VstInt32 maxLen)
{
	if (value <= 0)
		vst_strncpy(text, "-oo", maxLen);
	else
		float2string((float)(20. * log10(value * 2)), text, maxLen);
}
int AllPassPhase::knobToFrequency(float x) {
	return floor(exp((16 + x * 100 * 1.20103)*log(1.059))*8.17742);
}

//------------------------------------------------------------------------
void AllPassPhase::getParameterDisplay (VstInt32 index, char *text)
{
	switch (index)
	{
		//case kDelay :    int2string (delay, text, kVstMaxParamStrLen);			break;
		case kFrequency:
			int2string(knobToFrequency(fFrequency), text, 5);
			break;
		case kQ:
			float2string(fQ * sqrt(2), text, 4);
			break;
		case kIterations:
			if (curIterations == 0) {
				strcpy(text, "BYPASS");
			}
			else {
				int2string(fIterations * 50, text, 5);
			}
			break;
		/*
		case kOut:
			mydB2string (fOut, text, 4);
			break;
		*/
		case kClip:
			if (fMix == 0) {
				strcpy(text, "0");
			}
			else {
				int2string(fMix * 100, text, 3);
			}
			break;
	}
}

//------------------------------------------------------------------------
void AllPassPhase::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
		case kFrequency :    strcpy (label, "Hz");	break;//samples
		case kQ : strcpy(label, " ");	break;//samples
		case kIterations : strcpy (label, "iterations");	break;//amount
		//case kOut :      strcpy (label, "dB");		break;
		case kClip:      strcpy(label, "%");		break;
		//case kClip:      float2string(dbginfo, label, kVstMaxParamStrLen);		break;
	}
}

//------------------------------------------------------------------------
bool AllPassPhase::getEffectName (char* name)
{
	strcpy (name, "AllPassPhase");
	return true;
}

//------------------------------------------------------------------------
bool AllPassPhase::getProductString (char* text)
{
	strcpy (text, "AllPassPhase");
	return true;
}

//------------------------------------------------------------------------
bool AllPassPhase::getVendorString (char* text)
{
	strcpy (text, "enummusic");
	return true;
}

//---------------------------------------------------------------------------
void AllPassPhase::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	// setup filter coefficients
	freq = knobToFrequency(fFrequency);
	q = fQ * sqrt(2);
	if (filterL[0].getFreq() != freq ||
		(int) (fIterations * 50) != curIterations ||
		filterL[0].getQ() != q) {
		filterL[0].setup(freq, 44100.0f, q);
		filterR[0].setup(freq, 44100.0f, q);
		for (int i = 0; i < fIterations * 50; i++)	{
			filterL[i].copyCoefficientsFrom(filterL[0]);
			filterR[i].copyCoefficientsFrom(filterR[0]);
		}
		curIterations = fIterations * 50;
	}

	float* in1 = inputs[0];
	float* in2 = inputs[1];   
	float* out1 = outputs[0];
	float* out2 = outputs[1];

	float *temp1{ new float[sampleFrames] {} };
	float *temp2{ new float[sampleFrames] {} };
	int samples = sampleFrames;

	// copy input to temp array
	while (--samples >= 0) {
		*temp1 = *in1++;
		*temp2 = *in2++;
		// checks the whole buffer
		// if it sees anything that isn't silence, reset the silence counter
		// ensures the entire buffer is processed
		if (abs(*temp1) >= noiseFloor || abs(*temp2) >= noiseFloor) {
			samplesSinceSilence = 0;
		}
		temp1++;
		temp2++;
	}

	// reset pointers
	temp1 -= sampleFrames;
	temp2 -= sampleFrames;
	samples = sampleFrames;
	
	// filter the audio
	if (samplesSinceSilence < deactivateAfterSamples && curIterations != 0 && fMix > 0) {
		// without curIterations != 0 the code sets temp1&2 without processing leftLp etc
		float *left{ new float[sampleFrames] {} };
		float *right{ new float[sampleFrames] {} };

		for (int i = 0; i < curIterations; i++) {
			filterL[i].processBlock(temp1, left, sampleFrames);
			filterR[i].processBlock(temp2, right, sampleFrames);
			while (--samples >= 0) {
				*temp1++ = *left++;
				*temp2++ = *right++;
			}
			samples = sampleFrames;
			temp1 -= sampleFrames;
			temp2 -= sampleFrames;
			left -= sampleFrames;
			right -= sampleFrames;
		}

		delete[] left;
		delete[] right;
	}

	in1 -= sampleFrames;
	in2 -= sampleFrames;

	while (--samples >= 0) {

		// if it sees anything that isn't silence, reset the silence counter
		if (abs(*temp1) >= noiseFloor || abs(*temp2) >= noiseFloor) {
			samplesSinceSilence = 0;
		}
		else if (samplesSinceSilence < 32768) { // int overflow protection
			samplesSinceSilence++;
		}

		*out1 = (*temp1 * fMix + *in1 * (1 - fMix)); // * fOut  * 2
		*out2 = (*temp2 * fMix + *in2 * (1 - fMix)); // * fOut  * 2

		in1++;
		in2++;
		out1++;
		out2++;
		temp1++;
		temp2++;
	}

	// always reset pointers before deleting the array
	temp1 -= sampleFrames;
	temp2 -= sampleFrames;
	delete[] temp1;
	delete[] temp2;
}
