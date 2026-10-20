//A hit is a region of physics-driven charge deposit in a waveform.  It records some statistics about the region of charge deposit for later analysis.
#ifndef RECO_HIT_HPP
#define RECO_HIT_HPP

#include <cstdint>
#include <cstddef>

namespace reco
{
  class hit
  {
    public:
      hit();
      hit(int channel, size_t start, size_t end, std::uint16_t peak, std::uint16_t integral, float mean, float rms);

      int get_channel() const;
      size_t get_start_time_tick() const;
      size_t get_end_time_tick() const;
      std::uint16_t get_peak_adc() const;
      std::uint16_t get_integral_adc() const;
      float get_mean_time_tick() const;
      float get_rms_time_tick() const;

    private:
      int channel_;
      size_t start_time_tick_;
      size_t end_time_tick_;
      std::uint16_t peak_adc_;
      std::uint16_t integral_adc_;
      float mean_time_tick_;
      float rms_time_tick_;
  };
}

#endif //RECO_HIT_HPP
