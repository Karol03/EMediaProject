#include <iostream>

#include "gnuplot.hpp"
#include "fft.hpp"
#include "rawaudio.hpp"
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

    RawAudioStereo audio;
    audio.separateSound(wav.data, wav.samples_amount);

    auto array = FFT::transform(audio);
    auto spectrum = FFT::getSpectrum(array);

    double bps = (wav.header->data_size/wav.header->byterate)/
            wav.samples_amount;
    double duration = 1.0/bps;

    cout << "\n";
    cout << "\nDraw on GNUPlot\n";
    for(size_t i=0; i<spectrum.size(); i++)
    {
        cout << "\tCHANNEL " << i << endl;
        GNUPlot::draw(duration, spectrum[i]);
    }

}
