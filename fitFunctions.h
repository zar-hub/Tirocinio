#ifndef FITFUNCTIONS_H
#define FITFUNCTIONS_H

#include <TROOT.h>

Double_t bigaus(Double_t *_x, Double_t *_par)
{
    Double_t x = _x[0];
    Double_t A = _par[0];
    Double_t mu = _par[1];
    Double_t sigL = _par[2];
    Double_t sigR = _par[3];

    if (x < mu)
        return A * (exp(-0.5 * (x - mu) * (x - mu) / (sigL * sigL)));
    else
        return A * (exp(-0.5 * (x - mu) * (x - mu) / (sigR * sigR)));
}

// Generate documentation
Double_t peak(Double_t *_x, Double_t *_par)
{
    Double_t x = _x[0];
    Double_t A = _par[0];
    Double_t mu = _par[1];
    Double_t sig = _par[2];

    if (abs(x - mu) < sig)
        return A;
    return 0;
}

Double_t peakBigausV0(Double_t *_x, Double_t *_par)
{
    // common
    Double_t mu = _par[0];
    // peak
    Double_t peakA = _par[1];
    Double_t sig = _par[2];
    // bigaus
    Double_t bigausA = _par[3];
    Double_t sigL = _par[4];
    Double_t sigR = _par[5];
    // split the parameters
    Double_t parBigaus[4] = {bigausA, mu, sigL, sigR};
    Double_t parPeak[3] = {peakA, mu, sig};

    return bigaus(_x, parBigaus) + peak(_x, parPeak);
}

Double_t peakBigausV1(Double_t *_x, Double_t *_par)
{
    // peak
    Double_t peakMu = _par[0];
    Double_t peakA = _par[1];
    Double_t peakSig = _par[2];
    // bigaus
    Double_t bigausMu = _par[3];
    Double_t bigausA = _par[4];
    Double_t bigausSigL = _par[5];
    Double_t bigausSigR = _par[6];

    // split the parameters
    Double_t parBigaus[4] = {bigausA, bigausMu, bigausSigL, bigausSigR};
    Double_t parPeak[3] = {peakA, peakMu, peakSig};

    return bigaus(_x, parBigaus) + peak(_x, parPeak);
}

Double_t exponentialFit(Double_t *_x, Double_t *_par)
{
    Double_t x = _x[0];
    Double_t x0 = _par[0];
    Double_t A = _par[1];
    Double_t k = _par[2];

    return A * exp(-k * (x - x0));
}

Double_t expBigausV0(Double_t *_x, Double_t *_par)
{
    // exp
    Double_t x0 = _par[0];
    Double_t A = _par[1];
    Double_t k = _par[2];
    // bigaus
    Double_t bigausMu = _par[3];
    Double_t bigausA = _par[4];
    Double_t bigausSigL = _par[5];
    Double_t bigausSigR = _par[6];

    // split the parameters
    Double_t parExp[3] = {x0, A, k};
    Double_t parBigaus[4] = {bigausA, bigausMu, bigausSigL, bigausSigR};

    return bigaus(_x, parBigaus) + exponentialFit(_x, parExp);
}

Double_t biExpFit(Double_t *_x, Double_t *_par)
{
    Double_t x = _x[0];
    Double_t x0 = _par[0];
    Double_t A = _par[1];
    Double_t kL = _par[2];
    Double_t kR = _par[3];

    if(x<x0)
        return A * exp(kL * (x - x0));
    else
        return A * exp(-kR * (x - x0));
}

#endif // FITFUNCTIONS_H