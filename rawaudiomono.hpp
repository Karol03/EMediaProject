#pragma once

#include "rawaudio.hpp"

class RawAudioMono : public RawAudio
{
public:
    RawAudioMono();
    virtual ~RawAudioMono();

    Channel& operator()(Channel::Number) override;
    const Channel& operator()(Channel::Number) const override;

    std::vector<DataType> mergeSound() const override;
    void separateSound(const DataType* data, uint32_t length) override;

    void for_each_channel(const std::function<void(Channel&)>& functor) override;
    void for_each_channel(const std::function<void(const Channel&)>& functor) const override;

private:
    Channel channel;
};
