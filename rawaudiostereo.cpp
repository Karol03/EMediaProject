#include "rawaudiostereo.hpp"

RawAudioStereo::RawAudioStereo()
        : RawAudio(2)
        , channel{{Channel::Number::First, {}}, {Channel::Number::Second, {}}}
{}

RawAudioStereo::RawAudioStereo(const RawAudioStereo& audio)
    : RawAudio(2)
    , channel{{Channel::Number::First, {}}, {Channel::Number::Second, {}}}
{
    this->operator=(audio);
}

RawAudioStereo::RawAudioStereo(RawAudioStereo&& audio)
    : RawAudio(2)
    , channel{{Channel::Number::First, {}}, {Channel::Number::Second, {}}}
{
    this->operator=(std::move(audio));
}

RawAudioStereo& RawAudioStereo::operator=(const RawAudioStereo& audio)
{
    channel[0].bits = audio.channel[0].bits;
    channel[1].bits = audio.channel[1].bits;
    return *this;
}

RawAudioStereo& RawAudioStereo::operator=(RawAudioStereo&& audio)
{
    channel[0].bits = std::move(audio.channel[0].bits);
    channel[1].bits = std::move(audio.channel[1].bits);
    return *this;
}

RawAudioStereo::~RawAudioStereo() = default;

Channel& RawAudioStereo::operator()(Channel::Number chnumb)
{
    if (chnumb == Channel::Number::First)
        return channel[0];
    else
        return channel[1];
}

const Channel& RawAudioStereo::operator()(Channel::Number chnumb) const
{
    if (chnumb == Channel::Number::First)
        return channel[0];
    else
        return channel[1];
}

std::vector<DataType> RawAudioStereo::mergeSound() const
{
    auto length = channel[0].length();
    if (length and (length == channel[1].length()))
    {
        auto newLength = chNumb*length;
        std::vector<DataType> merged;
        merged.reserve(newLength);
        for (unsigned i=0; i<length; i++)
        {
            merged.push_back(static_cast<DataType>(channel[0][i]));
            merged.push_back(static_cast<DataType>(channel[1][i]));
        }
        return merged;
    }
    return {};
}

void RawAudioStereo::separateSound(const DataType* data, uint32_t length)
{
    if (data == nullptr or length == 0 or length%2)
        return;
    channel[0].bits.reserve(length/2);
    channel[1].bits.reserve(length/2);

    for (uint32_t i=0; i<length/2; i++)
    {
        channel[0].bits.push_back(data[2*i]);
        channel[1].bits.push_back(data[2*i+1]);
    }
}

void RawAudioStereo::for_each_channel(const std::function<void(Channel&)>& functor)
{
    functor(channel[0]);
    functor(channel[1]);
}

void RawAudioStereo::for_each_channel(const std::function<void(const Channel&)>& functor) const
{
    functor(channel[0]);
    functor(channel[1]);
}
