#pragma once

#include <inttypes.h>

#include "wavloader.hpp"

namespace detail
{
uint32_t countSamplesAmount(const WAVHeader& header);
int countSamplesSize(const WAVHeader& header, uint32_t samples_amount);
}   // namespace detail
