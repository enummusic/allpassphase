// https://www.musicdsp.org/en/latest/Filters/266-4th-order-linkwitz-riley-filters.html
// © All credits go to the contributors at musicdsp.org

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <assert.h>


class LRCrossoverFilter { // LR4 crossover filter
private:

	// coefficient calculations are all changed to double
	// for filter stability at low frequencies

	struct filterCoefficents {
		double a0, a1, a2, a3, a4;
	} lpco, hpco;

	double b1co, b2co, b3co, b4co;

	struct {
		double xm1 = 0.0f;
		double xm2 = 0.0f;
		double xm3 = 0.0f;
		double xm4 = 0.0f;
		double ym1 = 0.0f, ym2 = 0.0f, ym3 = 0.0f, ym4 = 0.0f;
	} hptemp, lptemp;

	float coFreqRunningAv = 100.0f;
public:
	void setup(float crossoverFrequency, float sr);
	void copyCoefficientsFrom(LRCrossoverFilter filter);
	void process(float in, float * outHP, float * outLP);
	float getCrossover() {
		return coFreqRunningAv;
	}
};