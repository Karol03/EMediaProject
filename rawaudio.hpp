#pragma once
#include <functional>
#include <inttypes.h>
#include <stdexcept>

#include "wavloader.hpp"

struct Channel
{
    using BitType = double;
        // MOZE ZASSTOSOWAC WEKTOR ZAMIAST DOUBLE* BO BEDZIE
        // BEZPIECZNIEJ DLA PAMIECI I OGOLNIE GIT bo git jest git
    enum class Number
    {
        First,
        Second
    };

    Channel(Number number,
            uint32_t length,
            double* bits)
        : number(number)
        , length(length)
        , bits(bits)
    {}

    virtual ~Channel()
    {
        if (bits != nullptr)
            delete [] bits;
    }

    void create(uint32_t length_, BitType* bits_)
    {
        if (length_ == 0 or bits_ == 0)
            return;
        length = length_;
        bits = bits_;
    }

    BitType operator[](uint32_t i)
    {
        if (i<length)
            return bits[i];
        throw std::out_of_range("Out of channel range");
    }

    BitType operator[](uint32_t i) const
    {
        if (i<length)
            return bits[i];
        throw std::out_of_range("Out of channel range");
    }

public:
    const Number number;
    uint32_t length;
    BitType* bits;
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
        , channel{Channel::Number::First, 0, nullptr}
    {}

    Channel& operator()(Channel::Number) override
    { return channel; }

    const Channel& operator()(Channel::Number) const override
    { return channel; }

    void separateSound(const DataType* data, uint32_t length) override
    {
        if (data == nullptr or length == 0)
            return;
        channel.length = length;
        channel.bits = new Channel::BitType[length];
        for (uint32_t i=0; i<length; i++)
            channel.bits[i] = data[i];
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
        , channel{{Channel::Number::First, 0, nullptr}, {Channel::Number::Second, 0, nullptr}}
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

        channel[0].bits = new Channel::BitType[length/2];
        channel[1].bits = new Channel::BitType[length/2];
        channel[0].length = length/2;
        channel[1].length = length/2;

        for (uint32_t i=0; i<length/2; i++)
        {
            channel[0].bits[i] = data[2*i];
            channel[1].bits[i] = data[2*i+1];
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
