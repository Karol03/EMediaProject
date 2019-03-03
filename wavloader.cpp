#include <iostream>
#include <utility>

#include "wavloader.hpp"

WavLoader::WavLoader(WAVFile& wav)
    : loaded(false)
    , sample(wav)
{}

void WavLoader::load(const char* sampleName)
{
    std::ifstream file(sampleName, std::ifstream::binary);

    if (not file)
    {
        std::cout << "ERR: Cannot find the file\n";
        return;
    }

    if (not (loaded = isReadable(file)))
    {
        std::cout << "ERR: File is unreadable\n";
        file.close();
        return;
    }

    readHeader(file);
    readData(file);

    file.close();
}

bool WavLoader::isReadable(std::ifstream& file)
{
    file.seekg(0, file.end);
    long long length = file.tellg();
    file.seekg(0, file.beg);

    return length > sizeof(WAVHeader);
}

void WavLoader::readHeader(std::ifstream& file)
{
    size_t length = sizeof(WAVHeader);
    char* buffer = new char[length];
    file.read(buffer, length);
    WAVHeader* wav = reinterpret_cast<WAVHeader*>(buffer);
    sample.header = std::make_shared<WAVHeader>(*wav);
}

void WavLoader::readData(std::ifstream& file)
{
    sample.samples_amount =
            sample.header->data_size/(sample.header->bits_per_sample/sizeof(char));
    char* buffer = new char[sample.header->data_size];
    file.read(buffer, sample.samples_amount);
    sample.data = reinterpret_cast<WAVFile::DataType*>(buffer);
}

void WavLoader::print() const
{
    if (not loaded)
        return;
    std::cout << "\nWav file header\n";

    std::cout << "\tRiff: " << sample.header->riff[0]
                            << sample.header->riff[1]
                            << sample.header->riff[2]
                            << sample.header->riff[3]
              << "\n\tOverall_size: " << sample.header->overall_size
              << "\n\tWave: " <<  sample.header->wave[0]
                              << sample.header->wave[1]
                              << sample.header->wave[2]
                              << sample.header->wave[3]
              << "\n\tFmt_chunk_marker: " << sample.header->fmt_chunk_marker[0]
                                          << sample.header->fmt_chunk_marker[1]
                                          << sample.header->fmt_chunk_marker[2]
              << "\n\tLength_of_fmt: " <<  sample.header->length_of_fmt
              << "\n\tFormat_type: " <<  sample.header->format_type
              << "\n\tChannels: " <<  sample.header->channels
              << "\n\tSample_rate: " <<  sample.header->sample_rate
              << "\n\tByterate: " <<  sample.header->byterate
              << "\n\tBlock_align: " <<  sample.header->block_align
              << "\n\tBits_per_sample: " <<  sample.header->bits_per_sample
              << "\n\tData_chunk_header: " <<  sample.header->data_chunk_header[0]
                                           <<  sample.header->data_chunk_header[1]
                                           <<  sample.header->data_chunk_header[2]
                                           <<  sample.header->data_chunk_header[3]
              << "\n\tData_size: " <<  sample.header->data_size;

    std::cout << "\nData\n";
    for (int i=0; i < 1000; i++)
        std::cout << sample.data[i] << " ";
    std::cout << "\n";
}

const WAVFile& WavLoader::get() const
{
    return sample;
}
