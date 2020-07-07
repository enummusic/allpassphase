// not sure if filename can be changed without breaking the program

#ifndef __AllPassPhase__
#include "AllPassPhase.h"
#endif

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new AllPassPhase(audioMaster);
}

