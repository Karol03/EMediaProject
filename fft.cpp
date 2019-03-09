#include "fft.hpp"

#include <utility>

FFT::ArraysComplex FFT::transform(const RawAudio& audio)
{
    ArraysComplex arrays;

    audio.for_each_channel(
        [&arrays](const auto& channel)
        {
            auto length = channel.length();
            ComplexArray array;
            array.reserve(length);
            for (uint32_t i=0; i<length; i++)
                array.push_back(channel[i]);
            arrays.emplace_back(std::move(array));
        });
    return transform(arrays);
}

FFT::ArraysComplex FFT::transform(ArraysComplex& arrays)
{
    for (auto& array : arrays)
        transform(array);
    return arrays;
}

FFT::ArraysSimple FFT::getSpectrum(const ArraysComplex& arrays)
{
    if (arrays.empty())
        return {};

    ArraysSimple simpleArrays;
    for (const auto& array : arrays)
    {
        SimpleArray newArray;
        newArray.reserve(array.size()/2);
        for (unsigned i=0; i<array.size(); i++)
            newArray.push_back(std::abs(array[i]));
        simpleArrays.emplace_back(std::move(newArray));
    }

    return simpleArrays;
}

/* Cooley–Tukey FFT algorithm */
void FFT::transform(ComplexArray& array)
{
    const size_t N = array.size();
    if (N <= 1)
        return;

    ComplexArray even(N/2);
    ComplexArray odd(N/2);

    for (unsigned i=0; i<N/2; i++)
    {
        even[i] = array[i*2];
        odd[i] = array[i*2+1];
    }

    transform(even);
    transform(odd);

    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0, -2.0 * M_PI * k / N) * odd[k];
        array[k] = even[k] + t;
        array[k+N/2] = even[k] - t;
    }
}

FFT::ArraysComplex FFT::inverse(ArraysComplex& arrays)
{
    for (auto& array : arrays)
        inverse(array);
    return arrays;
}

void FFT::inverse(ComplexArray& array)
{
    for (auto& complex : array)
        std::conj(complex);
    transform(array);

    for (auto& complex : array)
        std::conj(complex);

    for (auto& complex : array)
        complex /= array.size();
}
