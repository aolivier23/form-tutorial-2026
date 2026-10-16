// simulate_wires: generates a collection of wire_readouts on the same APA with noise with sparse Gaussian hits mixed in

#ifndef SIMULATE_WIRES_H
#define SIMULATE_WIRES_H

#include "wire_readout.hpp"

#include <vector>
#include <random>

namespace sim
{
  class simulate_wires
  {
    public:
      simulate_wires(size_t n_channels, size_t n_time_ticks, size_t n_noise_functions, size_t random_seed, double background_height);

      std::vector<raw::wire_readout> operator ()();

    private:
      size_t n_channels_;
      size_t n_time_ticks_;
      std::mt19937 generator_;
      std::vector<double> noise_frequencies_;
      std::exponential_distribution<double> signal_time_dist_;
      std::uniform_real_distribution<double> signal_width_dist_;
      std::uniform_real_distribution<double> signal_height_dist_;
      std::uniform_int_distribution<size_t> noise_offset_;
      double background_height_;

      std::vector<raw::wire_readout::ADC_t> simulate_signal();
  };
}

#endif //SIMULATE_WIRES_H
