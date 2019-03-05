#pragma once

#include <cmath>
#include <complex>
#include <vector>

#include "wavloader.hpp"
#include "rawaudio.hpp"


class FFT final
{
public:
    using Complex = std::complex<double>;
    using SimpleArray = std::vector<double>;
    using ComplexArray = std::vector<Complex>;
    using ArraysSimple = std::vector<SimpleArray>;
    using ArraysComplex = std::vector<ComplexArray>;

    static ArraysComplex transform(const RawAudio& audio);

    static ArraysComplex transform(ArraysComplex& arrays);
    static ArraysComplex inverse(ArraysComplex& arrays);

    static ArraysSimple getSpectrum(const ArraysComplex& array);
private:
    static void transform(ComplexArray& array);
    static void inverse(ComplexArray& array);
};
