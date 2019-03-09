#include "rawaudiomono.hpp"

RawAudioMono::RawAudioMono()
    : RawAudio(1)
    , channel{Channel::Number::First, {}}
{}

RawAudioMono::~RawAudioMono() = default;

std::vector<DataType> RawAudioMono::mergeSound() const
{
    if (channel.length())
    {
        std::vector<DataType> merged;
        merged.reserve(channel.length());
        for (const auto& bit : channel.bits)
            merged.push_back(static_cast<DataType>(bit));
        return merged;
    }
    return {};
}

Channel& RawAudioMono::operator()(Channel::Number)
{
    return channel;
}

const Channel& RawAudioMono::operator()(Channel::Number) const
{
    return channel;
}

void RawAudioMono::separateSound(const DataType* data, uint32_t length)
{
    if (data == nullptr or length == 0)
        return;
    channel.bits.reserve(length);
    for (uint32_t i=0; i<length; i++)
        channel.bits.push_back(data[i]);
}

void RawAudioMono::for_each_channel(const std::function<void(Channel&)>& functor)
{
    functor(channel);
}

void RawAudioMono::for_each_channel(const std::function<void(const Channel&)>& functor) const
{
    functor(channel);
}
