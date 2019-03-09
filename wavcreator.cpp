#include "detail.hpp"

#include "wavcreator.hpp"

WavCreator::WavCreator() = default;

WavCreator& WavCreator::create(const WAVHeader& header)
{
    if (wav.header)
        wav.header.reset();
    wav.header = std::make_shared<WAVHeader>(header);
    wav.samples_amount = detail::countSamplesAmount(header);
    return *this;
}

WavCreator& WavCreator::create(const WAVHeader& header,
                               const WAVDataType data)
{
    create(header);
    if (wav.data)
        delete [] wav.data;
    wav.data = new DataType[wav.samples_amount];
    for (uint32_t i=0; i<wav.samples_amount; i++)
        wav.data[i] = data[i];
    return *this;
}

WavCreator& WavCreator::set(const WAVHeader& header)
{
    create(header);
    return *this;
}

WavCreator& WavCreator::set(const WAVDataType data)
{
    if (wav.header)
    {
        if (wav.data)
            delete [] wav.data;
        wav.data = new DataType[wav.samples_amount];

        for (uint32_t i=0; i<wav.samples_amount; i++)
            wav.data[i] = data[i];
    }
    return *this;
}

WavCreator& WavCreator::set(const RawAudio& audio)
{
    auto bits = audio.mergeSound();
    if (wav.data)
        delete [] wav.data;
    wav.data = new DataType[bits.size()];
    wav.samples_amount = bits.size();
    for (unsigned i=0; i<bits.size(); i++)
        wav.data[i] = bits[i];
    return *this;
}

void WavCreator::change(const HeaderChanger& headerChanger)
{
    if (wav.header)
        wav.header.reset();
    wav.header = std::make_shared<WAVHeader>();
    headerChanger(*wav.header);
}

void WavCreator::change(const DataChanger& dataChanger)
{
    dataChanger(wav.data);
}


const WAVFile& WavCreator::get() const
{
    return wav;
}

WAVFile& WavCreator::get()
{
    return wav;
}
