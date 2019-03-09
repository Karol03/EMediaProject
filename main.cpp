#include <iostream>
#include <vector>

#include "fft.hpp"
#include "gnuplot.hpp"
#include "rawaudiostereo.hpp"
#include "rsa.hpp"
#include "wavcreator.hpp"
#include "wavloader.hpp"

using namespace std;


void print(const WAVFile& sample)
{
    if (not sample.header)
    {
        cerr << "ERR: " << __func__ << "Nothing to print" << endl;
        return;
    }
    std::cout << "\tWav file header" << endl;
    std::cout << "\t\tRiff: " << sample.header->riff[0]
                            << sample.header->riff[1]
                            << sample.header->riff[2]
                            << sample.header->riff[3] << endl
              << "\t\tOverall_size: " << sample.header->overall_size << endl
              << "\t\tWave: " <<  sample.header->wave[0]
                              << sample.header->wave[1]
                              << sample.header->wave[2]
                              << sample.header->wave[3] << endl
              << "\t\tFmt_chunk_marker: " << sample.header->fmt_chunk_marker[0]
                                          << sample.header->fmt_chunk_marker[1]
                                          << sample.header->fmt_chunk_marker[2] << endl
              << "\t\tLength_of_fmt: " <<  sample.header->length_of_fmt << endl
              << "\t\tFormat_type: " <<  sample.header->format_type << endl
              << "\t\tChannels: " <<  sample.header->channels << endl
              << "\t\tSample_rate: " <<  sample.header->sample_rate << endl
              << "\t\tByterate: " <<  sample.header->byterate << endl
              << "\t\tBlock_align: " <<  sample.header->block_align << endl
              << "\t\tBits_per_sample: " <<  sample.header->bits_per_sample << endl
              << "\t\tData_chunk_header: " <<  sample.header->data_chunk_header[0]
                                           <<  sample.header->data_chunk_header[1]
                                           <<  sample.header->data_chunk_header[2]
                                           <<  sample.header->data_chunk_header[3] << endl
              << "\t\tData_size: " <<  sample.header->data_size << endl;
    std::cout << "\tWav samples amount: " << sample.samples_amount << endl;
}


template <typename T>
void printExampleVector(const std::vector<T>& vec)
{
    const unsigned limit = 30;
    cout << __func__ << " : [ ";
    for (unsigned i=0; i<limit and i<vec.size(); i++)
        cout << vec[i] << " ";
    if (vec.size() > limit)
        cout << " and " << vec.size()-limit << " others ";
    cout << "]" << endl;
}

template <typename T>
void printExampleRawPointer(T* rawPointer, unsigned size)
{
    const unsigned limit = 30;
    cout << __func__ << " : [ ";
    for (unsigned i=0; i<limit and i<size; i++)
        cout << rawPointer[i] << " ";
    if (size > limit)
        cout << " and " << size-limit << " others ";
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
    cout << "Application start" << endl;
    cout << "Load *.wav file from file" << endl;
    WAVFile wav;
    WavLoader wavLoader(wav);
    string fileName;
    cout << "Enter wav file name: \n";
    cin >> fileName;
    cout << "Load file \'" << fileName << "\' \n";
    wavLoader.load(fileName.c_str());
    print(wav);
    printExampleRawPointer(wav.data, wav.samples_amount);

    cout << "Separate audio bits from *.wav format" << endl;
    cout << "\tCreating raw audio" << endl;
    RawAudioStereo audio;
    audio.separateSound(wav.data, wav.samples_amount);

    cout << endl << "Fast Fourier Transform" << endl;
    {   // START FFT
//    cout << "\tGet complex vector from raw audio" << endl;
//    auto array = FFT::transform(audio);
//    cout << "\tGet spectrum array from complex numbers" << endl;
//    auto spectrum = FFT::getSpectrum(array);

//    cout << endl << "Draw spectrum on GNUPlot" << endl;
//    cout << "\tGet duration time to plot" << endl;
//    double duration =
//            (double(wav.header->data_size/wav.header->byterate)/
//            double(audio(Channel::Number::First).length());
//    std::vector<double> columnX(spectrum.size());

//    for(size_t i=0; columnX.size()!=spectrum[0].size(); i++)
//        columnX.push_back(duration*double(i));

//    cout << endl << "\tDraw on GNUPlot" << endl;
//    GNUPlot::draw(columnX, spectrum[0]);
//    GNUPlot::draw(columnX, spectrum[1]);

//    std::vector<double> columnX;
//    std::vector<double> firstChannelBits;
//    convert(audio(Channel::Number::First).bits, firstChannelBits);

//    std::vector<double> secondChannelBits;
//    convert(audio(Channel::Number::Second).bits, secondChannelBits);

//    columnX.reserve(firstChannelBits.size());
//    for(size_t i=0; columnX.size() != firstChannelBits.size(); i++)
//        columnX.push_back(duration*double(i));
//    GNUPlot::draw(columnX, firstChannelBits);

    }   // END OF FFT

    cout << endl << "Encrypt data by RSA algorithm" << endl;
    {   // START OF ENCRYPTING
    using KeyDataType = int32_t;
    RSA<KeyDataType> rsa;
    KeyDataType min_ = 100;
    KeyDataType max_ = 200;

    cout << endl << "\tConvert data to proper container type" << endl;
    std::vector<KeyDataType> convertedFirstChannelBits;
    std::vector<KeyDataType> convertedSecondChannelBits;

    convert(audio(Channel::Number::First).bits,
            convertedFirstChannelBits);
    convert(audio(Channel::Number::Second).bits,
            convertedSecondChannelBits);

    cout << "\tCreating keys with primes in <"
         << min_ << ", " << max_ << ">..." << endl;

    rsa.createKeys(min_, max_);
    cout << "\tCreated keys" << endl;
    cout << "\t\tPublic: " << endl;
    cout << "{ " << rsa.getPublicKey().first << ", "
         <<  rsa.getPublicKey().second << " }" << endl;

    cout << "\t\tPrivate: " << endl;
    cout << "{ " << rsa.getPrivateKey().first << ", "
         <<  rsa.getPrivateKey().second << " }" << endl;

    cout << "\tShow original values" << endl;
    cout << "\t\tFirst channel: ";
    printExampleVector(convertedFirstChannelBits);
    cout << "\t\tSecond channel: ";
    printExampleVector(convertedSecondChannelBits);

    cout << endl << "\tShow encrypted values" << endl;
    auto encryptedVec = rsa.encrypt(convertedFirstChannelBits);
    auto encryptedVec2 = rsa.encrypt(convertedSecondChannelBits);
    cout << "\t\tFirst channel: ";
    printExampleVector(encryptedVec);
    cout << "\t\tSecond channel: ";
    printExampleVector(encryptedVec2);


    cout << endl << "\tShow decrypted values" << endl;
    auto decryptedVec = rsa.decrypt(encryptedVec);
    auto decryptedVec2 = rsa.decrypt(encryptedVec2);
    cout << "\t\tFirst channel: ";
    printExampleVector(decryptedVec);
    cout << "\t\tSecond channel: ";
    printExampleVector(decryptedVec2);

    cout << "\tCreating new raw audio with encrypted data" << endl;
    RawAudioStereo encrypted;
    convert(encryptedVec, encrypted(Channel::Number::First).bits);
    convert(encryptedVec2, encrypted(Channel::Number::Second).bits);
    std::swap(encrypted, audio);
    }   // END OF ENCRYPTING

    cout << endl << endl << "Save to file" << endl;
    { // CREATE NEW WAV
    cout << "\tCreating new *.wav object" << endl;
    WavCreator creator;
    cout << "\tSet header from old file" << endl;
    creator.create(*wav.header);
    cout << "\tFill by encrypted data" << endl;
    creator.set(audio);
    WAVFile newWav = creator.get();
    wavLoader.set(newWav);

    print(newWav);
    printExampleRawPointer(newWav.data, newWav.samples_amount);
    cout << "\tEnter new file name: " << endl;
    cin >> fileName;
    cout << "\tSave data to file \'" << fileName << "\'" << endl;
    wavLoader.save(fileName.c_str());
    } // END OF CREATE NEW WAV
    cout << "[Done]" << endl;
}
