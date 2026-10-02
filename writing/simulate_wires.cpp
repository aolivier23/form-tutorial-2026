#include "simulate_wires.hpp"
#include <algorithm>
#include <cmath>
#include <cassert>

double gaus_pdf(double where, double mean, double stddev)
{
  return exp(-(where - mean)*(where - mean)/2./stddev/stddev)/sqrt(2*stddev*stddev);
}

namespace sim
{
  simulate_wires::simulate_wires(size_t n_channels, size_t n_time_ticks, size_t n_noise_functions, size_t random_seed): n_channels_(n_channels), n_time_ticks_(n_time_ticks), generator_(random_seed), signal_time_dist_(n_time_ticks/3), signal_width_dist_(0., n_time_ticks/100), signal_height_dist_(0., 5.*n_noise_functions/2.)
  {
    std::uniform_real_distribution<double> noise_dist(0, 2*M_PI);
    for(size_t which_noise_function = 0; which_noise_function < n_noise_functions; ++which_noise_function)
    {
      noise_frequencies_.push_back(noise_dist(generator_));
    }
  }

  std::vector<raw::wire_readout> simulate_wires::operator ()()
  {
    std::vector<raw::wire_readout> wires;
    wires.reserve(n_channels_);
    for(size_t channel = 0; channel < n_channels_; ++channel)
    {
      wires.emplace_back(channel, simulate_signal());
    }

    return wires;
  }

  std::vector<raw::wire_readout::ADC_t> simulate_wires::simulate_signal()
  {
    std::vector<raw::wire_readout::ADC_t> result(n_time_ticks_, 0);
    for(size_t time_tick = 0; time_tick < n_time_ticks_; ++time_tick)
    {
      for(auto const freq: noise_frequencies_) result[time_tick] += sin(freq * time_tick);
    }

    for(int64_t time_tick = 0; time_tick < static_cast<int64_t>(n_time_ticks_); time_tick += signal_time_dist_(generator_))
    {
      double const stddev = signal_width_dist_(generator_);
      double const amp = signal_height_dist_(generator_);
      for(int64_t signal_tick = std::max(static_cast<int64_t>(time_tick - 3*stddev), 0l); signal_tick < std::min(static_cast<int64_t>(time_tick + 3*stddev), static_cast<int64_t>(n_time_ticks_)); ++signal_tick)
      {
        assert(signal_tick > 0);
        result[static_cast<size_t>(signal_tick)] += amp * gaus_pdf(signal_tick - time_tick, 0., stddev);
      }
    }

    return result;
  }
}
