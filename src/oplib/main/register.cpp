#include "oplib/register.h"

#include "add.h"
#include "ahe.h"
#include "binmorph.h"
#include "carve.h"
#include "conv.h"
#include "dllconv.h"
#include "fftfilt.h"
#include "gauss.h"
#include "glmorph.h"
#include "globalhe.h"
#include "magsynth.h"
#include "marrhild.h"
#include "mask.h"
#include "median.h"
#include "nop.h"
#include "phsynth.h"
#include "plane.h"
#include "siahe.h"
#include "siahe2.h"
#include "sobel.h"
#include "spectrum.h"
#include "thresh.h"
#include "test.h"
#include "wmra.h"
#include "wvthrsh.h"
#include "zero.h"
#include <functional>

using namespace std;
using namespace ripl::oplib;

template<int(*executeFunc)(riplGreyMap&, const vector<string>&, const riplGreyMap&)>
static int convertNewOperator(unsigned argc, const char** argv, riplGreyMap* input, riplGreyMap* output)
{
    return executeFunc(*output, vector<string>(argv, argv + argc), *input);
}

void oplib_registerOps(Registrar& registrar)
{
    registrar.registerOp("add", "add images together pixel by pixel", convertNewOperator<addExecute>, addHelp);
    registrar.registerOp("ahe", "perform adaptive histogram equalization", aheExecute, aheHelp);
    registrar.registerOp("binmorph", "apply binary morphological operators", binmorphExecute, binmorphHelp);
    registrar.registerOp("carve", "carve image up", carveExecute, carveHelp);
    registrar.registerOp("conv", "perform 2D convolution", convExecute, convHelp);
    registrar.registerOp("dllconv", "perform 2D convolution (alternative syntax)", dllconvExecute, dllconvHelp);
    registrar.registerOp("fftfilt", "apply FFT-based filter", fftfiltExecute, fftfiltHelp);
    registrar.registerOp("gaussian", "perform Gaussian smoothing", gaussExecute, gaussHelp);
    registrar.registerOp("glmorph", "apply grey-level morphological operators", glmorphExecute, glmorphHelp);
    registrar.registerOp("globalhe", "apply global histogram equalization to image", globalHEExecute, globalHEHelp);
    registrar.registerOp("magsynth", "perform Fourier-magnitude-only synthesis", magsynthExecute, magsynthHelp);
    registrar.registerOp("marrhildreth", "perform Marr-Hildreth edge detection", marrhildExecute, marrhildHelp);
    registrar.registerOp("mask", "apply mask to image", maskExecute, maskHelp);
    registrar.registerOp("median", "apply median filter", medianExecute, medianHelp);
    registrar.registerOp("nop", "no-op filter", nopExecute, nopHelp);
    registrar.registerOp("phasesynth", "perform Fourier-phase-only synthesis", phasesynthExecute, phasesynthHelp);
    registrar.registerOp("plane", "extract bitplane from image", planeExecute, planeHelp);
    registrar.registerOp("siahe", "apply subimage-based AHE to image", siaheExecute, siaheHelp);
    registrar.registerOp("siahe2", "apply extended raised-cosine subimage AHE to image", siahe2Execute, siahe2Help);
    registrar.registerOp("sobel", "perform Sobel edge detection", sobelExecute, sobelHelp);
    registrar.registerOp("spectrum", "obtain normalized spectrum of image", spectrumExecute, spectrumHelp);
    registrar.registerOp("test", "test RConsole", testExecute, testHelp);
    registrar.registerOp("threshold", "threshold image", thresholdExecute, thresholdHelp);
    registrar.registerOp("wavethresh", "reconstruct after wavelet thresholding", wavethreshExecute, wavethreshHelp);
    registrar.registerOp("wmra", "wavelet-based multiresolution analysis", wmraExecute, wmraHelp);
    registrar.registerOp("zero", "apply zeroing operator to image", zeroExecute, zeroHelp);
}
