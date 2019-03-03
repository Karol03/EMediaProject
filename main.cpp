#include <iostream>
#include "gnuplot.hpp"
#include "fft.hpp"
#include "wavloader.hpp"

using namespace std;

int main()
{
    WAVFile wav;
    WavLoader wavLoader(wav);
   // string fileName;

  //  cout << "Enter wav file name: \n";
   // cin >> fileName;
//    cout << "Looking for file \'" << fileName << "\' \n";
    wavLoader.load("sampel.wav");
    wavLoader.print();

    cout << "\nFast Fourier Transform\n";
    auto array = FFT::transform(wav);
    auto spectrum = FFT::getSpectrum(array);

    double sample_freq = double(wav.header->sample_rate);
    double duration = 1.0/sample_freq;

    for(int i=0; i<100; i++)
        cout << spectrum[i] << " ";
    cout << "\n";
    cout << "\nDraw on GNUPlot\n";
    GNUPlot::draw(duration, spectrum, wav.header->data_size);
}
