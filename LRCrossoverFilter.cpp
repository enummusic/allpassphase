// https://www.musicdsp.org/en/latest/Filters/266-4th-order-linkwitz-riley-filters.html
// © All credits go to the contributors at musicdsp.org

#include <iostream>
#include <stdio.h>
#include <math.h>

#include "LRCrossoverFilter.h"


void LRCrossoverFilter::setup(float crossoverFrequency, float sr) {

	// coefficient calculations are all changed to double
	// for filter stability at low frequencies

	const double pi = 3.141592654f;

	coFreqRunningAv = crossoverFrequency;

	double cowc = 2 * pi*coFreqRunningAv;
	double cowc2 = cowc * cowc;
	double cowc3 = cowc2 * cowc;
	double cowc4 = cowc2 * cowc2;

	double cok = cowc / tan(pi*coFreqRunningAv / sr);
	double cok2 = cok * cok;
	double cok3 = cok2 * cok;
	double cok4 = cok2 * cok2;
	double sqrt2 = sqrt(2);
	double sq_tmp1 = sqrt2 * cowc3 * cok;
	double sq_tmp2 = sqrt2 * cowc * cok3;
	double a_tmp = 4 * cowc2*cok2 + 2 * sq_tmp1 + cok4 + 2 * sq_tmp2 + cowc4;

	b1co = (4 * (cowc4 + sq_tmp1 - cok4 - sq_tmp2)) / a_tmp;
	b2co = (6 * cowc4 - 8 * cowc2*cok2 + 6 * cok4) / a_tmp;
	b3co = (4 * (cowc4 - sq_tmp1 + sq_tmp2 - cok4)) / a_tmp;
	b4co = (cok4 - 2 * sq_tmp1 + cowc4 - 2 * sq_tmp2 + 4 * cowc2*cok2) / a_tmp;



	//================================================
	// low-pass
	//================================================
	lpco.a0 = cowc4 / a_tmp;
	lpco.a1 = 4 * cowc4 / a_tmp;
	lpco.a2 = 6 * cowc4 / a_tmp;
	lpco.a3 = lpco.a1;
	lpco.a4 = lpco.a0;

	//=====================================================
	// high-pass
	//=====================================================
	hpco.a0 = cok4 / a_tmp;
	hpco.a1 = -4 * cok4 / a_tmp;
	hpco.a2 = 6 * cok4 / a_tmp;
	hpco.a3 = hpco.a1;
	hpco.a4 = hpco.a0;

	zeroBuffers();
}

void LRCrossoverFilter::copyCoefficientsFrom(LRCrossoverFilter filter) {
	lpco.a0 = filter.lpco.a0;
	lpco.a1 = filter.lpco.a1;
	lpco.a2 = filter.lpco.a2;
	lpco.a3 = filter.lpco.a3;
	lpco.a4 = filter.lpco.a4;

	hpco.a0 = filter.hpco.a0;
	hpco.a1 = filter.hpco.a1;
	hpco.a2 = filter.hpco.a2;
	hpco.a3 = filter.hpco.a3;
	hpco.a4 = filter.hpco.a4;

	b1co = filter.b1co;
	b2co = filter.b2co;
	b3co = filter.b3co;
	b4co = filter.b4co;

	zeroBuffers();
}

void LRCrossoverFilter::zeroBuffers() {
	hptemp.ym1 = 0;
	hptemp.ym2 = 0;
	hptemp.ym3 = 0;
	hptemp.ym4 = 0;
	lptemp.ym1 = 0;
	lptemp.ym2 = 0;
	lptemp.ym3 = 0;
	lptemp.ym4 = 0;
	temp.xm1 = 0;
	temp.xm2 = 0;
	temp.xm3 = 0;
	temp.xm4 = 0;
}

void LRCrossoverFilter::processBlock(float * in, float * outHP, float * outLP, int numSamples) {
	float tempx, tempy;
	for (int i = 0; i < numSamples; i++) {
		tempx = in[i];

		// High pass
		tempyH = hpco.a0*tempx +
			hpco.a1*temp.xm1 +
			hpco.a2*temp.xm2 +
			hpco.a3*temp.xm3 +
			hpco.a4*temp.xm4 -
			b1co * hptemp.ym1 -
			b2co * hptemp.ym2 -
			b3co * hptemp.ym3 -
			b4co * hptemp.ym4;

		hptemp.ym4 = hptemp.ym3;
		hptemp.ym3 = hptemp.ym2;
		hptemp.ym2 = hptemp.ym1;
		hptemp.ym1 = tempyH;

		outHP[i] = tempyH;

		// Low pass
		tempyL = lpco.a0*tempx +
			lpco.a1*temp.xm1 +
			lpco.a2*temp.xm2 +
			lpco.a3*temp.xm3 +
			lpco.a4*temp.xm4 -
			b1co * lptemp.ym1 -
			b2co * lptemp.ym2 -
			b3co * lptemp.ym3 -
			b4co * lptemp.ym4;

		lptemp.ym4 = lptemp.ym3;
		lptemp.ym3 = lptemp.ym2;
		lptemp.ym2 = lptemp.ym1;
		lptemp.ym1 = tempyL;

		temp.xm4 = temp.xm3;
		temp.xm3 = temp.xm2;
		temp.xm2 = temp.xm1;
		temp.xm1 = tempx;

		outLP[i] = tempyL;
	}
}