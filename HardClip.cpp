#include "HardClip.h"

float HardClip::process(float in, float threshold) {
	if (in > threshold) {
		return threshold;
	}
	else if (in < -threshold) {
		return -threshold;
	}
	else {
		return in;
	}
}