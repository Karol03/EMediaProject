#include "fft.hpp"

ComplexArray FFT::transform(const WAVFile& wav)
{
    long size = wav.samples_amount;
    Complex* array = new Complex[size];
    for (long i=0; i<size; i++)
        array[i] = wav.data[i];

    std::valarray<Complex> complexArray(array, size);
    transform(complexArray);

    return complexArray;
}

double* FFT::getSpectrum(const ComplexArray& array)
{
    if (array.size() == 0)
        return nullptr;

    double* newArray;
    newArray = new double[array.size()];
    for (unsigned i=0; i<array.size(); i++)
        newArray[i] = std::abs(array[i]);
    return newArray;
}

void FFT::transform(std::valarray<Complex>& array)
{
    const size_t N = array.size();
    if (N <= 1)
        return;

    ComplexArray even = array[std::slice(0, N/2, 2)];
    ComplexArray odd = array[std::slice(1, N/2, 2)];

    transform(even);
    transform(odd);

    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
        array[k] = even[k] + t;
        array[k+N/2] = even[k] - t;
    }
}

ComplexArray FFT::inverse(WAVFile&)
{
    return ComplexArray{};
}
