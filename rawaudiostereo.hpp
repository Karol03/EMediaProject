#pragma once

#include "rawaudio.hpp"

class RawAudioStereo : public RawAudio
{
public:
    RawAudioStereo();
    RawAudioStereo(const RawAudioStereo& audio);
    RawAudioStereo(RawAudioStereo&& audio);
    RawAudioStereo& operator=(const RawAudioStereo& audio);
    RawAudioStereo& operator=(RawAudioStereo&& audio);
    virtual ~RawAudioStereo();

    Channel& operator()(Channel::Number chnumb) override;
    const Channel& operator()(Channel::Number chnumb) const override;

    void separateSound(const DataType* data, uint32_t length) override;
    std::vector<DataType> mergeSound() const override;

    void for_each_channel(const std::function<void(Channel&)>& functor) override;
    void for_each_channel(const std::function<void(const Channel&)>& functor) const override;

private:
    Channel channel[2];
};
