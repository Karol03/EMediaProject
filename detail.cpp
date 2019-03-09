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
}   // namespace detail
