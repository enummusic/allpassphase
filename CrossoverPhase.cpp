//-------------------------------------------------------------------------------------------------------
// $Date: 2020/05/24 00:00:00 $
//
// Filename     : CrossoverPhase.cpp
// Created by   : enummusic
// Description  : Crossover filter phase dispersion
//
// © 2020 enummusic
// VST SDK © 2005, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include <thread>

#ifndef __CrossoverPhase__
#include "CrossoverPhase.h"

#include "HardClip.h"
#endif

//----------------------------------------------------------------------------- 
CrossoverPhaseProgram::CrossoverPhaseProgram ()
{
	// default Program Values
	fFrequency = 0.36f;
	fIterations = 0.5;
	fOut = 0.5;
	fClip = 0;

	strcpy (name, "Init");
}

//-----------------------------------------------------------------------------
CrossoverPhase::CrossoverPhase (audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init
	
	size = 44100;
	cursor = 0;
	delay = 0;
	buffer = new float[size];
	
	programs = new CrossoverPhaseProgram[numPrograms];
	//fFrequency = fIterations = fOut = 0;
	fFrequency = 0.36f;
	fIterations = 0.5;
	fOut = 0.5;
	fClip = 0;

	if (programs)
		setProgram (0);

	setNumInputs (2);
	setNumOutputs (2);

	setUniqueID ('Xphs');

	//
	curIterations = 25;
	crossover = pow(200, fFrequency) * 100;
	filterL[0].setup(crossover, 44100.0f);
	filterR[0].setup(crossover, 44100.0f);
	for (int i = 0; i < 50; i++) {
		filterL[i].copyCoefficientsFrom(filterL[0]);
		filterR[i].copyCoefficientsFrom(filterR[0]);
	}
	//

	resume ();		// flush buffer
}

//------------------------------------------------------------------------
CrossoverPhase::~CrossoverPhase ()
{
	if (buffer)
		delete[] buffer;
	if (programs)
		delete[] programs;
}

//------------------------------------------------------------------------
void CrossoverPhase::setProgram (long program)
{
	CrossoverPhaseProgram* ap = &programs[program];

	curProgram = program;
	setParameter (kFrequency, ap->fFrequency);	
	setParameter (kIterations, ap->fIterations);
	setParameter (kOut, ap->fOut);
}

//------------------------------------------------------------------------
void CrossoverPhase::setDelay (float fdelay)
{
	fFrequency = fdelay;
	programs[curProgram].fFrequency = fdelay;
	cursor = 0;
	delay = (long)(fdelay * (float)(size - 1));
}

//------------------------------------------------------------------------
void CrossoverPhase::setProgramName (char *name)
{
	strcpy (programs[curProgram].name, name);
}

//------------------------------------------------------------------------
void CrossoverPhase::getProgramName (char *name)
{
	if (!strcmp (programs[curProgram].name, "Init"))
		sprintf (name, "%s %d", programs[curProgram].name, curProgram + 1);
	else
		strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool CrossoverPhase::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index < kNumPrograms)
	{
		strcpy (text, programs[index].name);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
void CrossoverPhase::resume ()
{
	memset (buffer, 0, size * sizeof (float));
	AudioEffectX::resume ();
}

//------------------------------------------------------------------------
void CrossoverPhase::setParameter (VstInt32 index, float value)
{
	CrossoverPhaseProgram* ap = &programs[curProgram];

	switch (index)
	{
		case kFrequency:    
			fFrequency = ap->fFrequency = value;
			programs[curProgram].fFrequency = value;
			break;
		case kIterations : fIterations = ap->fIterations = value; break;
		case kOut :      fOut = ap->fOut = value;			break;
		case kClip:		fClip = ap->fClip = value;
	}
}

//------------------------------------------------------------------------
float CrossoverPhase::getParameter (VstInt32 index)
{
	float v = 0;

	switch (index)
	{
		case kFrequency :    v = fFrequency;	break;
		case kIterations : v = fIterations; break;
		case kOut :      v = fOut;		break;
		case kClip: v = fClip;
	}
	return v;
}

//------------------------------------------------------------------------
void CrossoverPhase::getParameterName (VstInt32 index, char *label)
{
	switch (index)
	{
		case kFrequency :    strcpy (label, "Frequency");		break;
		case kIterations : strcpy (label, "Intensity");	break;
		case kOut :      strcpy (label, "Volume");		break;
		case kClip:      strcpy(label, "Hard clip");		break;
	}
}

void CrossoverPhase::mydB2string(float value, char* text, VstInt32 maxLen)
{
	if (value <= 0)
		vst_strncpy(text, "-oo", maxLen);
	else
		float2string((float)(20. * log10(value * 2)), text, maxLen);
}

//------------------------------------------------------------------------
void CrossoverPhase::getParameterDisplay (VstInt32 index, char *text)
{
	switch (index)
	{
		//case kDelay :    int2string (delay, text, kVstMaxParamStrLen);			break;
		case kFrequency:
			int2string(crossover, text, 5);
			break;
		case kIterations:
			if (curIterations == 0) {
				strcpy(text, "BYPASS");
			}
			else {
				int2string(curIterations, text, 5);
			}
			break;
		case kOut:
			mydB2string (fOut, text, kVstMaxParamStrLen);
			break;
		case kClip:
			if (fClip >= 0.5) {
				strcpy(text, "ON");
			}
			else {
				strcpy(text, "OFF");
			}
	}
}

//------------------------------------------------------------------------
void CrossoverPhase::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
		case kFrequency :    strcpy (label, "Hz");	break;//samples
		case kIterations : strcpy (label, "iterations");	break;//amount
		case kOut :      strcpy (label, "dB");		break;
		case kClip:      strcpy(label, " ");		break;
		//case kClip:      float2string(dbginfo, label, kVstMaxParamStrLen);		break;
	}
}

//------------------------------------------------------------------------
bool CrossoverPhase::getEffectName (char* name)
{
	strcpy (name, "CrossoverPhase");
	return true;
}

//------------------------------------------------------------------------
bool CrossoverPhase::getProductString (char* text)
{
	strcpy (text, "CrossoverPhase");
	return true;
}

//------------------------------------------------------------------------
bool CrossoverPhase::getVendorString (char* text)
{
	strcpy (text, "enummusic");
	return true;
}

//---------------------------------------------------------------------------
// typedef int VstInt32;
void CrossoverPhase::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	// setup filter coefficients
	crossover = pow(200, fFrequency) * 100;
	if (filterL[0].getCrossover() != crossover || (int) (fIterations * 50) != curIterations) {
		filterL[0].setup(crossover, 44100.0f);
		filterR[0].setup(crossover, 44100.0f);
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
	if (samplesSinceSilence < deactivateAfterSamples && curIterations != 0) {
		// without curIterations != 0 the code sets temp1&2 without processing leftLp etc
		float *leftLp{ new float[sampleFrames] {} };
		float *leftHp{ new float[sampleFrames] {} };
		float *rightLp{ new float[sampleFrames] {} };
		float *rightHp{ new float[sampleFrames] {} };

		for (int i = 0; i < curIterations; i++) {
			filterL[i].processBlock(temp1, leftHp, leftLp, sampleFrames);
			filterR[i].processBlock(temp2, rightHp, rightLp, sampleFrames);
			while (--samples >= 0) {
				*temp1++ = (*leftLp++ + *leftHp++);
				*temp2++ = (*rightLp++ + *rightHp++);
			}
			samples = sampleFrames;
			temp1 -= sampleFrames;
			temp2 -= sampleFrames;
			leftLp -= sampleFrames;
			leftHp -= sampleFrames;
			rightLp -= sampleFrames;
			rightHp -= sampleFrames;
		}

		delete[] leftLp;
		delete[] leftHp;
		delete[] rightLp;
		delete[] rightHp;
	}

	while (--samples >= 0) {
		// apply gain
		float l = *temp1 * fOut * 2;
		float r = *temp2 * fOut * 2;

		if (fClip >= 0.5 && samplesSinceSilence < deactivateAfterSamples) {
			*out1 = HardClip::process(l, 1);
			*out2 = HardClip::process(r, 1);
		}
		else {
			*out1 = l;
			*out2 = r;
		}
		
		// if it sees anything that isn't silence, reset the silence counter
		if (abs(*temp1) >= noiseFloor || abs(*temp2) >= noiseFloor) {
			samplesSinceSilence = 0;
		}
		else if (samplesSinceSilence < 32768) { // int overflow protection
			samplesSinceSilence++;
		}

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
