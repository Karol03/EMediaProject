#include <iostream>
#include <utility>

#include "detail.hpp"
#include "wavloader.hpp"

WavLoader::WavLoader(WAVFile& wav)
    : loaded(false)
    , sample(wav)
{}

void WavLoader::load(const char* fileName)
{
    std::ifstream file(fileName, std::ifstream::binary);

    if (not file)
    {
        std::cerr << "ERR: Cannot find the file" << std::endl;
        return;
    }

    if (not (loaded = isReadable(file)))
    {
        std::cerr << "ERR: File is unreadable" << std::endl;
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
    int length = sizeof(WAVHeader);
    char* buffer = new char[length];
    file.read(buffer, length);
    WAVHeader* wav = reinterpret_cast<WAVHeader*>(buffer);
    sample.header = std::make_shared<WAVHeader>(*wav);
    sample.samples_amount = detail::countSamplesAmount(*sample.header);
}

void WavLoader::readData(std::ifstream& file)
{
    auto size = detail::countSamplesSize(*sample.header, sample.samples_amount);
    char* buffer = new char[size];
    file.read(buffer, size);
    sample.data = reinterpret_cast<DataType*>(buffer);
}

void WavLoader::set(WAVFile& wav)
{
    sample = wav;
}

void WavLoader::save(const char* fileName)
{
    std::ofstream file(fileName, std::ofstream::binary);

    if (not file)
    {
        std::cerr << "ERR: Cannot find the file" << std::endl;
        return;
    }

    if (not sample.header)
    {
        std::cerr << "ERR: Cannot save file without header" << std::endl;
        file.close();
        return;
    }
    saveHeader(file);

    if (not sample.data)
    {
        std::cerr << "ERR: Cannot save file without uninitialized data pointer" << std::endl;
        file.close();
        return;
    }
    saveData(file);
    file.close();
    sample.data = nullptr;
}

void WavLoader::saveHeader(std::ofstream& file)
{
    WAVHeader header = *sample.header;
    char* buffer = reinterpret_cast<char*>(&header);
    file.write(buffer, sizeof(WAVHeader));
}

void WavLoader::saveData(std::ofstream& file)
{
    auto size = detail::countSamplesSize(*sample.header, sample.samples_amount);
    char* buffer = reinterpret_cast<char*>(&sample.data[0]);
    file.write(buffer, size);
}



