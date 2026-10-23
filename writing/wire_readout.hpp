// wire_readout: a waveform read from a single channel of a wire-based time projection chamber (TPC).

#ifndef RAW_WIREREADOUT_HPP
#define RAW_WIREREADOUT_HPP

#include <vector>
#include <cstdint>

namespace raw
{
  class wire_readout
  {
    public:
      using ADC_t = uint16_t;

      wire_readout(int channel, std::vector<ADC_t> const& waveform);
      wire_readout();

      int& channel();
      int channel() const;

      std::vector<ADC_t>& waveform();
      std::vector<ADC_t> const& waveform() const;

    private:
      int channel_;
      std::vector<ADC_t> waveform_;
  };
}

#endif //RAW_WIREREADOUT_HPP
