#pragma once
#include <functional>
#include <inttypes.h>
#include <stdexcept>

#include "wavloader.hpp"

struct Channel
{
    using BitType = double;
    using Bits = std::vector<BitType>;
    enum class Number
    {
        First,
        Second
    };

    Channel(Number number,
            Bits bits)
        : number(number)
        , bits(bits)
    {}

    virtual ~Channel() = default;

    void create(Bits& bits_)
    {
        if (bits_.empty())
            return;
        std::swap(bits_, bits);
    }

    BitType operator[](uint32_t i)
    {   return bits[i];   }

    BitType operator[](uint32_t i) const
    {   return bits[i];   }

    uint32_t length() const
    {   return bits.size();   }


public:
    const Number number;
    Bits bits;
};


class RawAudio
{
public:
    RawAudio(uint8_t chNumb) : chNumb(chNumb) {}
    virtual ~RawAudio() = default;
    virtual Channel& operator()(Channel::Number) = 0;
    virtual const Channel& operator()(Channel::Number) const = 0;
    virtual void separateSound(const DataType*, uint32_t) = 0;
    virtual void for_each_channel(const std::function<void(Channel&)>&) = 0;
    virtual void for_each_channel(const std::function<void(const Channel&)>&) const = 0;

    const uint8_t chNumb;
};


class RawAudioMono : public RawAudio
{
public:
    RawAudioMono()
        : RawAudio(1)
        , channel{Channel::Number::First, {}}
    {}

    Channel& operator()(Channel::Number) override
    { return channel; }

    const Channel& operator()(Channel::Number) const override
    { return channel; }

    void separateSound(const DataType* data, uint32_t length) override
    {
        if (data == nullptr or length == 0)
            return;
        channel.bits.reserve(length);
        for (uint32_t i=0; i<length; i++)
            channel.bits.push_back(data[i]);
    }

    void for_each_channel(const std::function<void(Channel&)>& functor) override
    {   functor(channel);   }

    void for_each_channel(const std::function<void(const Channel&)>& functor) const override
    {   functor(channel);   }

private:
    Channel channel;
};


class RawAudioStereo : public RawAudio
{
public:
    RawAudioStereo()
        : RawAudio(2)
        , channel{{Channel::Number::First, {}}, {Channel::Number::Second, {}}}
    {}

    Channel& operator()(Channel::Number chnumb) override
    {
        if (chnumb == Channel::Number::First)
            return channel[0];
        else
            return channel[1];
    }

    const Channel& operator()(Channel::Number chnumb) const override
    {
        if (chnumb == Channel::Number::First)
            return channel[0];
        else
            return channel[1];
    }

    void separateSound(const DataType* data, uint32_t length) override
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

    void for_each_channel(const std::function<void(Channel&)>& functor) override
    {
        functor(channel[0]);
        functor(channel[1]);
    }

    void for_each_channel(const std::function<void(const Channel&)>& functor) const override
    {
        functor(channel[0]);
        functor(channel[1]);
    }

private:
    Channel channel[2];
};
