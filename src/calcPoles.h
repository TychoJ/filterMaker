#ifndef CALC_POLES_H
#define CALC_POLES_H

#include <stdint.h>
#include <complex.h>

void chebyshev(double cutOffFreq, uint16_t order, double rimple, double complex *poles);

#endif // CALC_POLES_H