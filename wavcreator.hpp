#pragma once

#include <functional>
#include "rawaudio.hpp"
#include "wavloader.hpp"

class WavCreator
{
    using WAVDataType = DataType*;
    using HeaderChanger = std::function<void(WAVHeader&)>;
    using DataChanger = std::function<void(WAVDataType)>;
public:
    WavCreator();

    WavCreator& create(const WAVHeader&);
    WavCreator& create(const WAVHeader&, const WAVDataType);

    WavCreator& set(const WAVHeader&);
    WavCreator& set(const WAVDataType);
    WavCreator& set(const RawAudio&);

    void change(const HeaderChanger&);
    void change(const DataChanger&);

    const WAVFile& get() const;
    WAVFile& get();

private:
    WAVFile wav;
};
