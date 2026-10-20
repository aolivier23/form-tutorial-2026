#include "hit.hpp"

namespace reco
{
  hit::hit() = default;

  hit::hit(int channel, size_t start_time, size_t end_time, uint16_t peak, uint16_t integral, float mean, float rms): channel_(channel), start_time_tick_(start_time), end_time_tick_(end_time), peak_adc_(peak), integral_adc_(integral), mean_time_tick_(mean), rms_time_tick_(rms)
  {
  }

  int hit::get_channel() const { return channel_; }
  std::size_t hit::get_start_time_tick() const { return start_time_tick_; }
  std::size_t hit::get_end_time_tick() const { return end_time_tick_; }
  uint16_t hit::get_peak_adc() const { return peak_adc_; }
  uint16_t hit::get_integral_adc() const { return integral_adc_; }
  float hit::get_mean_time_tick() const { return mean_time_tick_; }
  float hit::get_rms_time_tick() const { return rms_time_tick_; }
}
