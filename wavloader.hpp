#pragma once

#include <vector>
#include <fstream>
#include <memory>
#include <inttypes.h>

using DataType = int16_t;

struct WAVHeader final
{
    unsigned char riff[4];
    uint32_t overall_size;
    unsigned char wave[4];
    unsigned char fmt_chunk_marker[4];
    uint32_t length_of_fmt;
    uint16_t format_type;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byterate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    unsigned char data_chunk_header[4];
    uint32_t data_size;
};

struct WAVFile
{
    using Header = std::shared_ptr<WAVHeader>;

    WAVFile() : data(nullptr) {}
    WAVFile(const WAVFile& wav) : data(nullptr)
    {
        this->operator=(wav);
    }

    WAVFile& operator=(const WAVFile& wav)
    {
        header = wav.header;
        samples_amount = wav.samples_amount;
        if (data)
            delete [] data;
        if (samples_amount == 0)
        {
            data = nullptr;
            return *this;
        }
        data = new DataType[samples_amount];
        for (uint32_t i=0; i<samples_amount; i++)
            data[i] = wav.data[i];
        return *this;
    }

    virtual ~WAVFile()
    {
        if (data)
            delete [] data;
        data = nullptr;
    }

    Header header;
    uint32_t samples_amount;
    DataType* data;
};

class WavLoader
{
public:
    explicit WavLoader(WAVFile& wav);
    void set(WAVFile& wav);

    void save(const char* fileName);
    void load(const char* fileName);

private:
    void readHeader(std::ifstream& file);
    void readData(std::ifstream& file);
    bool isReadable(std::ifstream& file);

    void saveHeader(std::ofstream& file);
    void saveData(std::ofstream& file);

    bool loaded;
    WAVFile& sample;
};
