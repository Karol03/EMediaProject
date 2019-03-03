#pragma once

#include <vector>
#include <fstream>
#include <memory>
#include <inttypes.h>


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

struct WAVFile final
{
    using Header = std::shared_ptr<WAVHeader>;
    using DataType = int16_t;

    WAVFile() : data(nullptr)
    {}
    ~WAVFile()
    {
        if (data)
            delete [] data;
    }

    Header header;
    long samples_amount;
    DataType* data;
};


class WavLoader
{
public:
    explicit WavLoader(WAVFile& wav);
    const WAVFile& get() const;
    void load(const char* sampleName);
    void print() const;

private:
    void readHeader(std::ifstream& file);
    void readData(std::ifstream& file);
    bool isReadable(std::ifstream& file);

    bool loaded;
    WAVFile& sample;
};
