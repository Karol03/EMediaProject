#pragma once

#include <functional>
#include <inttypes.h>
#include <vector>
#include <stdexcept>

#include "channel.hpp"
#include "wavloader.hpp"

class RawAudio
{
public:
    RawAudio(uint8_t chNumb) : chNumb(chNumb) {}
    virtual ~RawAudio() = default;

    virtual Channel& operator()(Channel::Number) = 0;
    virtual const Channel& operator()(Channel::Number) const = 0;

    virtual void separateSound(const DataType*, uint32_t) = 0;
    virtual std::vector<DataType> mergeSound() const = 0;

    virtual void for_each_channel(const std::function<void(Channel&)>&) = 0;
    virtual void for_each_channel(const std::function<void(const Channel&)>&) const = 0;

    const uint8_t chNumb;
};

