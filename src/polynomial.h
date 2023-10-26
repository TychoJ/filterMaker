#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <stdint.h>
#include <complex.h>

void analogNum(double complex *poles, uint16_t order, double complex *numerator);
void analogDen(double complex *poles, uint16_t order, double complex *denominator);

void digitalNum(double complex *poles, uint16_t order, double fSample, double complex *numerator);
void digitalDen(double complex *poles, uint16_t order, double fSample, double complex *denominator);

void polyMul(double complex *poly1, double complex *poly2, uint16_t degree, double complex *polyRes);
void examplePolyMul(void);

#endif // POLYNOMIAL_H