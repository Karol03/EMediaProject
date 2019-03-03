#pragma once

#include <cmath>
#include <complex>
#include <valarray>
#include <vector>

#include "wavloader.hpp"

using Complex = std::complex<double>;
using ComplexArray = std::valarray<Complex>;

class FFT final
{
public:
    static ComplexArray transform(const WAVFile& wav);
    static ComplexArray inverse(WAVFile& wav);

    static double* getSpectrum(const ComplexArray& array);
private:
    static void transform(std::valarray<Complex>& array);
};
