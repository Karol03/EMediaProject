#include <iostream>
#include <vector>

#include "gnuplot.hpp"
#include "fft.hpp"
#include "rawaudio.hpp"
#include "rsa.hpp"
#include "wavloader.hpp"

using namespace std;

template <typename T>
void printExampleVector(const std::vector<T>& vec)
{
    const unsigned limit = 100;
    cout << __func__ << " : [ ";
    for (auto i=0; i<limit and i<vec.size(); i++)
        cout << vec[i] << " ";
    if (vec.size() > limit)
        cout << "... ";
    cout << "]" << endl;
}

template <typename T, typename U>
void convert(const std::vector<T>& input,
             std::vector<U>& output)
{
    output.clear();
    output.reserve(input.size());
    for (const auto& i : input)
        output.push_back(static_cast<U>(i));
}

int main()
{
    WAVFile wav;
    WavLoader wavLoader(wav);
   // string fileName;

  //  cout << "Enter wav file name: \n";
   // cin >> fileName;
//    cout << "Searching file \'" << fileName << "\' \n";
    wavLoader.load("sampel.wav");
    wavLoader.print();

    cout << "\nFast Fourier Transform\n";
    RawAudioStereo audio;
    audio.separateSound(wav.data, wav.samples_amount);

    auto array = FFT::transform(audio);
    auto spectrum = FFT::getSpectrum(array);

    double duration =
            (double(wav.samples_amount/wav.header->byterate)/
            double(wav.header->channels)*60.0)/
            double(audio(Channel::Number::First).length());
    cout << "\n";
    cout << "\nDraw spectrum on GNUPlot\n";
//    std::vector<double> columnX(spectrum.size());

//    for(size_t i=0; columnX.size()!=spectrum[0].size(); i++)
//        columnX.push_back(duration*double(i));

    cout << "\n";
    cout << "\nDraw on GNUPlot\n";
//    GNUPlot::draw(columnX, spectrum[0]);
//    GNUPlot::draw(columnX, spectrum[1]);

    std::vector<double> columnX;
    std::vector<double> firstChannelBits;
    convert(audio(Channel::Number::First).bits, firstChannelBits);

    std::vector<double> secondChannelBits;
    convert(audio(Channel::Number::Second).bits, secondChannelBits);

//    columnX.reserve(firstChannelBits.size());
//    for(size_t i=0; columnX.size() != firstChannelBits.size(); i++)
//        columnX.push_back(duration*double(i));
//    GNUPlot::draw(columnX, firstChannelBits);

    using KeyDataType = int32_t;
    RSA<KeyDataType> rsa;
    KeyDataType min_ = 100;
    KeyDataType max_ = 200;

    std::vector<KeyDataType> convertedFirstChannelBits;
    std::vector<KeyDataType> convertedSecondChannelBits;

    convert(audio(Channel::Number::First).bits,
            convertedFirstChannelBits);
    convert(audio(Channel::Number::Second).bits,
            convertedSecondChannelBits);

    cout << "Creating keys with primes in <"
         << min_ << ", " << max_ << ">..." << endl;

    rsa.createKeys(min_, max_);
    cout << "Keys" << endl;
    cout << "Public: " << endl;
    cout << "{ " << rsa.getPublicKey().first << ", "
         <<  rsa.getPublicKey().second << " }" << endl;

    cout << "Private: " << endl;
    cout << "{ " << rsa.getPrivateKey().first << ", "
         <<  rsa.getPrivateKey().second << " }" << endl;

    cout << endl << "Original values" << endl;
    cout << "\tFirst channel: ";
    printExampleVector(convertedFirstChannelBits);
    cout << "\tSecond channel: ";
    printExampleVector(convertedSecondChannelBits);

    cout << endl << "Encrypted values" << endl;
    auto encryptedVec = rsa.encrypt(convertedFirstChannelBits);
    auto encryptedVec2 = rsa.encrypt(convertedSecondChannelBits);
    cout << "\tFirst channel: ";
    printExampleVector(encryptedVec);
    cout << "\tSecond channel: ";
    printExampleVector(encryptedVec2);


    cout << endl << "Decrypted values" << endl;
    auto decryptedVec = rsa.decrypt(encryptedVec);
    auto decryptedVec2 = rsa.decrypt(encryptedVec2);
    cout << "\tFirst channel: ";
    printExampleVector(decryptedVec);
    cout << "\tSecond channel: ";
    printExampleVector(decryptedVec2);

}
