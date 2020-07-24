// code referenced:
// https://www.musicdsp.org/en/latest/Filters/197-rbj-audio-eq-cookbook.html
// https://www.musicdsp.org/en/latest/Filters/64-biquad-c-code.html
// https://www.musicdsp.org/en/latest/Filters/266-4th-order-linkwitz-riley-filters.html

#include <iostream>
#include <stdio.h>
#include <math.h>

#include "AllPassFilter.h"


void AllPassFilter::setup(float freq, float sr, float q) {

	// coefficient calculations need to be in double
	// for filter stability at low frequencies

	const double pi = 3.14159265358979323846;
	this->freq = freq;
	this->q = q;
	double b0, b1, b2, a0, a1, a2;
	double w0 = 2 * pi * freq / sr; // is this sr or sr/2 ?
	double cosw0 = cos(w0);
	double sinw0 = sin(w0);
	const double ln2 = 0.69314718055994530942;
	double alpha = sin(w0) * (2 * q);
	//double alpha = sin(w0) * sinh(ln2 / 2 * q * w0/sinw0);

	b0 = 1 - alpha;
	b1 = -2 * cosw0;
	b2 = 1 + alpha;
	a0 = 1 + alpha;
	a1 = -2 * cosw0;
	a2 = 1 - alpha;

	/*b0 = (1 - cosw0)/2;
	b1 = 1 - cosw0;
	b2 = (1 - cosw0) / 2;
	a0 = 1 + alpha;
	a1 = -2 * cosw0;
	a2 = 1 - alpha;*/
	
	co.c0 = b0 / a0;
	co.c1 = b1 / a0;
	co.c2 = b2 / a0;
	co.c3 = a1 / a0;
	co.c4 = a2 / a0;

	//zeroBuffers();
}

void AllPassFilter::copyCoefficientsFrom(AllPassFilter filter) {
	co.c0 = filter.co.c0;
	co.c1 = filter.co.c1;
	co.c2 = filter.co.c2;
	co.c3 = filter.co.c3;
	co.c4 = filter.co.c4;

	//zeroBuffers();
}

void AllPassFilter::zeroBuffers() {
	temp.xm1 = 0;
	temp.xm2 = 0;
	temp.ym1 = 0;
	temp.ym2 = 0;
}

void AllPassFilter::processBlock(float * in, float * out, int numSamples) {
	float tempx, tempy;
	for (int i = 0; i < numSamples; i++) {
		tempx = in[i];
		tempy = co.c0 * tempx +
			co.c1 * temp.xm1 +
			co.c2 * temp.xm2 -
			co.c3 * temp.ym1 -
			co.c4 * temp.ym2;

		temp.ym2 = temp.ym1;
		temp.ym1 = tempy;

		temp.xm2 = temp.xm1;
		temp.xm1 = tempx;

		out[i] = tempy;
	}
}