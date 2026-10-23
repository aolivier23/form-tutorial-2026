#include "window_hit_finder.hpp"
#include "hit.hpp"
#include "writing/wire_readout.hpp"

#include <cmath>
#include <algorithm>

std::vector<reco::hit> window_hit_finder(std::vector<raw::wire_readout> const& wires, std::uint16_t const pedestal)
{
  std::vector<reco::hit> hits;

  for(auto const& wire: wires)
  {
    auto const& waveform = wire.waveform();
    for(size_t time_tick = 0; time_tick < waveform.size(); ++time_tick)
    {
      if(waveform[time_tick] > pedestal)
      {
         size_t start_time_tick = time_tick;
         std::uint16_t peak_adc = 0, integral_adc = 0;
         float mean_time_tick = 0, rms_time_tick = 0;
         do
         {
           peak_adc = std::max(waveform[time_tick], peak_adc);
           integral_adc += waveform[time_tick];
           auto const weighted = (time_tick - start_time_tick)*waveform[time_tick];
           mean_time_tick += weighted;
           rms_time_tick += weighted*weighted;
         } while(++time_tick < waveform.size() && waveform[time_tick] > pedestal);
         hits.emplace_back(wire.channel(), start_time_tick, time_tick, peak_adc, integral_adc, mean_time_tick/integral_adc, std::sqrt(rms_time_tick/integral_adc/integral_adc));
      }
    }
  }

  return hits;
}
