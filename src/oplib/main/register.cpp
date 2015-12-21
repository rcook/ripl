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
#include "riplregistry/Registry.h"
#include <functional>

using namespace std;
using namespace ripl::oplib;

template<int(*executeFunc)(riplGreyMap&, const vector<string>&, const riplGreyMap&)>
static int convertNewOperator(unsigned argc, const char** argv, riplGreyMap* input, riplGreyMap* output)
{
    return executeFunc(*output, vector<string>(argv, argv + argc), *input);
}

RIPL_REGISTER_PLUGIN_OPS(registry)
{
    registry.registerOp("add", "add images together pixel by pixel", convertNewOperator<addExecute>, addHelp);
    registry.registerOp("ahe", "perform adaptive histogram equalization", aheExecute, aheHelp);
    registry.registerOp("binmorph", "apply binary morphological operators", binmorphExecute, binmorphHelp);
    registry.registerOp("carve", "carve image up", carveExecute, carveHelp);
    registry.registerOp("conv", "perform 2D convolution", convExecute, convHelp);
    registry.registerOp("dllconv", "perform 2D convolution (alternative syntax)", dllconvExecute, dllconvHelp);
    registry.registerOp("fftfilt", "apply FFT-based filter", fftfiltExecute, fftfiltHelp);
    registry.registerOp("gaussian", "perform Gaussian smoothing", gaussExecute, gaussHelp);
    registry.registerOp("glmorph", "apply grey-level morphological operators", glmorphExecute, glmorphHelp);
    registry.registerOp("globalhe", "apply global histogram equalization to image", globalHEExecute, globalHEHelp);
    registry.registerOp("magsynth", "perform Fourier-magnitude-only synthesis", magsynthExecute, magsynthHelp);
    registry.registerOp("marrhildreth", "perform Marr-Hildreth edge detection", marrhildExecute, marrhildHelp);
    registry.registerOp("mask", "apply mask to image", maskExecute, maskHelp);
    registry.registerOp("median", "apply median filter", medianExecute, medianHelp);
    registry.registerOp("nop", "no-op filter", nopExecute, nopHelp);
    registry.registerOp("phasesynth", "perform Fourier-phase-only synthesis", phasesynthExecute, phasesynthHelp);
    registry.registerOp("plane", "extract bitplane from image", planeExecute, planeHelp);
    registry.registerOp("siahe", "apply subimage-based AHE to image", siaheExecute, siaheHelp);
    registry.registerOp("siahe2", "apply extended raised-cosine subimage AHE to image", siahe2Execute, siahe2Help);
    registry.registerOp("sobel", "perform Sobel edge detection", sobelExecute, sobelHelp);
    registry.registerOp("spectrum", "obtain normalized spectrum of image", spectrumExecute, spectrumHelp);
    registry.registerOp("test", "test RConsole", testExecute, testHelp);
    registry.registerOp("threshold", "threshold image", thresholdExecute, thresholdHelp);
    registry.registerOp("wavethresh", "reconstruct after wavelet thresholding", wavethreshExecute, wavethreshHelp);
    registry.registerOp("wmra", "wavelet-based multiresolution analysis", wmraExecute, wmraHelp);
    registry.registerOp("zero", "apply zeroing operator to image", zeroExecute, zeroHelp);
}
