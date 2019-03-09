#pragma once

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
