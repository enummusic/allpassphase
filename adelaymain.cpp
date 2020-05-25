// not sure if filename can be changed without breaking the program

#ifndef __CrossoverPhase__
#include "CrossoverPhase.h"
#endif

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new CrossoverPhase (audioMaster);
}

