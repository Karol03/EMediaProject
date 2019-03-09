#include "detail.hpp"

namespace detail
{
uint32_t countSamplesAmount(const WAVHeader& header)
{
    const unsigned BYTE_SIZE = 8;
    return header.data_size/
            (header.bits_per_sample/BYTE_SIZE);
}

int countSamplesSize(const WAVHeader& header, uint32_t samples_amount)
{
    const unsigned BYTE_SIZE = 8;
    return static_cast<int>(samples_amount*(header.bits_per_sample/BYTE_SIZE));
}

RawAudioStereo getSampel(const RawAudioStereo& audio, uint32_t from, uint32_t to)
{
    RawAudioStereo newAudio = audio;
    if (from > to or to > audio(Channel::Number::First).length())
        return newAudio;

    newAudio.for_each_channel(
                [&from, &to](Channel& channel)
    {
        auto startIt = channel.bits.begin()+from;
        auto stopIt = channel.bits.begin()+to;
        Channel::Bits newChannelBits(startIt, stopIt);
        std::swap(newChannelBits, channel.bits);
    });
    return  newAudio;
}

}   // namespace detail
