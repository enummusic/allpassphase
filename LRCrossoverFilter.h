#include <iostream>
#include <stdio.h>
#include <math.h>
#include <assert.h>


class LRCrossoverFilter { // LR4 crossover filter
private:
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
	void processBlock(float * in, float * outHP, float * outLP, int numSamples);
	void dumpCoefficents(struct filterCoefficents x) {
		std::cout << "a0: " << x.a0 << "\n";
		std::cout << "a1: " << x.a1 << "\n";
		std::cout << "a2: " << x.a2 << "\n";
		std::cout << "a3: " << x.a3 << "\n";
		std::cout << "a4: " << x.a4 << "\n";
	}
	void dumpInformation() {
		std::cout << "-----\nfrequency: " << coFreqRunningAv << "\n";
		std::cout << "lpco:\n";
		dumpCoefficents(lpco);
		std::cout << "hpco:\n";
		dumpCoefficents(hpco);
		std::cout << "bco:\nb1: ";
		std::cout << b1co << "\nb2: " << b2co << "\nb3: " << b3co << "\nb4: " << b4co << "\n";
	}

	float getCrossover() {
		return coFreqRunningAv;
	}
};