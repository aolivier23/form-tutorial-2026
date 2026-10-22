#include "wire_readout.hpp"
#include <limits>

namespace raw
{
  wire_readout::wire_readout(int channel, std::vector<ADC_t> const& waveform): channel_(channel), waveform_(waveform)
  {
  }

  //Putting a wire_readout into a std::vector<> and using certain functions requires a default constructor
  wire_readout::wire_readout(): channel_(std::numeric_limits<decltype(channel_)>::max()), waveform_()
  {
  }

  int& wire_readout::channel() { return channel_; }
  int wire_readout::channel() const { return channel_; }

  std::vector<wire_readout::ADC_t>& wire_readout::waveform() { return waveform_; }
  std::vector<wire_readout::ADC_t> const& wire_readout::waveform() const { return waveform_; }
}
