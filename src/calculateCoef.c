//#include <stdio.h>
//
//int main(void) {
//    printf("Hello World!\n");
//    return 0;
//}

#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <stdint.h>

#include "calcPoles.h"
#include "polynomial.h"

#define NUM_OF_POLES 3

const char *argp_program_version =
  "cfc3 0.1";
const char *argp_program_bug_address =
  "<example@example.com>";

/* Program documentation. */
static char doc[] =
  "cfc3 stands for calculate digital filter coefficients 3 where 3 means three analog poles.";

/* A description of the arguments we accept. */
static char args_doc[] = "";

/* The options we understand. */
static struct argp_option options[] = {
    { 0, 0, 0, 0, "Configurations:", 1},
    {"output",     'o', "FILE", 0,
     "Save the digital filter coefficients in FILE"},
    {"poles",      'p', 0, 0, "Only calculate the poles"},
    {"order",      'n', "NUM", 0, "Filter order"},
    {"sampleFreq", 's', "NUM", OPTION_ARG_OPTIONAL, "Sample frequency"},
    {"fcutoff",    'f', "fcutoff", 0, "The cutoff frequency of the filter"},
    {"type",       'T', "NUM", 0, "The filter type\n0=chebyshev,1=?"},
    {"ripple",     'r', "NUM", OPTION_ARG_OPTIONAL, "ripple in dB"},
    { 0, 0, 0, 0, "Set level of details in the CLI:", 2},
    {"verbose",  'v', 0,      0,  "Produce verbose output"},
    {"quiet",    'q', 0,      0,  "Don't produce any output"},
    {0}
};

/* Used by main to communicate with parse_opt. */
struct arguments {
  int silent, verbose, poles, fType, fSample;
  double sampleFreq, fCutOff, ripple;
  char *output_file;
  uint16_t order;
};

enum filterType {
    CHEBYSHEV
};

char filterTypes[1][128] = {"chebyshev"};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state) {
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key) {
        case 'o':
            arguments->output_file = arg;
            break;
        case 'p':
            arguments->poles = 1;
            break;
        case 'n':
            arguments->order = atoi(arg);
            break;
        case 's':
            arguments->fSample = atof(arg);
            break;
        case 'f':
            arguments->fCutOff = atof(arg);
            break;
        case 'T':
            arguments->fType = atoi(arg);
            break;
        case 'r':
            if (arg == NULL) {
                printf("use -r=<value> or --ripple=<value>\n");
                argp_usage(state);
            }
            arguments->ripple = atof(arg);
            break;
        case 'q':
            arguments->silent = 1;
            break;
        case 'v':
            arguments->verbose = 1;
            break;
        
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv) {
    struct arguments arguments;

    /* Default values. */
    arguments.poles       = 0;
    arguments.order       = 0;
    arguments.fType       = 0;
    arguments.fSample     = 0;
    arguments.sampleFreq  = 0;
    arguments.fCutOff     = 0;
    arguments.ripple      = 0;
    arguments.silent      = 0;
    arguments.verbose     = 0;
    arguments.output_file = "-";

    /* Parse our arguments; every option seen by parse_opt will
       be reflected in arguments. */
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    if (!arguments.silent) {
        printf("Settings:\n\toutput: %s\n\tfilter type: %s\n\tcutoff: %.2f\n",
            arguments.output_file,
            filterTypes[arguments.fType],
            arguments.fCutOff);
    }

    // Create an array to hold all the pole locations
    double complex *poles = malloc(arguments.order * sizeof(double complex));
    if (poles == NULL) {
        printf("Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    switch (arguments.fType) {
        case CHEBYSHEV:
            chebyshev(arguments.fCutOff, arguments.order, arguments.ripple, poles);
            break;
        
        default:
            free(poles);
            printf("Not a valid filter type, see --help for more info.\n");
            exit(EXIT_FAILURE);
    }

    // Output all the poles
    if (!arguments.silent) {
        printf("Poles:\n");
        for (uint32_t i = 0; i < arguments.order; i++) {
            printf("\tp%d\t=\t%f+%fj\n",
                i+1,
                creal(poles[i]),
                cimag(poles[i]));
        }
    }

    double complex  num;
    double complex *den = malloc( (arguments.order + 1) * sizeof(double complex) );
    if (den == NULL) {
        printf("Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    analogNum(poles, arguments.order, &num);

    if (!arguments.silent) {
        printf("Numerator:%f + %fj\n",
            creal(num),
            cimag(num));
    }

    analogDen(poles, arguments.order, den);

    // Output all the polynomials
    if (!arguments.silent) {
        printf("Denominator: ");
        for (int32_t i = arguments.order; i >= 0; i--) {
            printf("(%f + %fj)s^%d%s",
                creal(den[i]),
                cimag(den[i]),
                i,
                i > 0 ? " + " : "");
        }
        printf("\n");
    }



    // Create polynomials

    // Multiply polynomials

    // return analog polynomials

    // calculate digital polynomials

    // return digital polynomial

    // return digital coef in c array

    // show succes full run

    free(poles);

    exit (EXIT_SUCCESS);
}

/*
    double complex poles[2];
    chebyshev(1, 2, 0.5, poles);
    printf("Poles:\n\tp1=wc[%.5f+%.5fj]\n\tp2=wc[%.5f+%.5fj]\n",
        creal(poles[0]),
        cimag(poles[0]),
        creal(poles[1]),
        cimag(poles[1]));

*/

/*
printf("Fsample: %d\nOutput: %s\nPOLES:\n",
            arguments.fSample,
            arguments.output_file);
            for (uint16_t i = 0; i < 3; i++) {
                printf("\tPole%d: %d\n",
                    i,
                    atoi(arguments.args[i]));
            }
*/

/*
    // Create array's to store H(z) denominator(z) and numerator(z)
    double complex numerator[NUM_OF_POLES * 2];
    double complex denominator[NUM_OF_POLES *2];

    // Calculate numerator
    double complex polyp1[] = {atof(arguments.args[0])*(1/arguments.fSample), atof(arguments.args[0])*(1/arguments.fSample)};
    double complex polyp2[] = {atof(arguments.args[1])*(1/arguments.fSample), atof(arguments.args[1])*(1/arguments.fSample)};
    double complex polyp3[] = {atof(arguments.args[2])*(1/arguments.fSample), atof(arguments.args[2])*(1/arguments.fSample), 0};

    double complex tmp[NUM_OF_POLES];
    polyMul(polyp1, polyp2, 1, tmp);
    polyMul(tmp, polyp3, 2, numerator);

    // Calculate denominator
    double complex denominatorPolyp1[] = {-(atof(arguments.args[0])*(1/arguments.fSample)+2), 2-atof(arguments.args[0])*(1/arguments.fSample)};
    double complex denominatorPolyp2[] = {-(atof(arguments.args[1])*(1/arguments.fSample)+2), 2-atof(arguments.args[1])*(1/arguments.fSample)};
    double complex denominatorPolyp3[] = {-(atof(arguments.args[2])*(1/arguments.fSample)+2), 2-atof(arguments.args[2])*(1/arguments.fSample), 0};
    printf("hello\n");

    polyMul(denominatorPolyp1, denominatorPolyp2, 1, tmp);
    polyMul(tmp, denominatorPolyp3, 2, denominator);

    printf("filter coefs:\n\ta:");
    for (int16_t i = NUM_OF_POLES; i >= 0; i--) {
        printf("%fz^%d%s", creal(denominator[i]), NUM_OF_POLES - i, i == 0 ? "": "+");
    }

    printf("\n\tb:");
    for (int16_t i = NUM_OF_POLES; i >= 0; i--) {
        printf("%fz^%d%s", creal(numerator[i]), NUM_OF_POLES - i, i == 0 ? "": "+");
    }

    printf("\n");
*/