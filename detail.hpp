#pragma once

#include <inttypes.h>

#include "rawaudiostereo.hpp"
#include "wavloader.hpp"

namespace detail
{
uint32_t countSamplesAmount(const WAVHeader& header);
int countSamplesSize(const WAVHeader& header, uint32_t samples_amount);
RawAudioStereo getSampel(const RawAudioStereo& audio, uint32_t from, uint32_t to);
}   // namespace detail
