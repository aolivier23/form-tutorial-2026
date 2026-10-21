// tells phlex how to add a provider node driven by simulate_wires

#include "simulate_wires.hpp"

#include "phlex/source.hpp"
#include "phlex/module.hpp"

#include <iostream>

//Wrapper to keep simulate_wires independent of phlex::data_cell_index
class do_simulate_wires
{
  public:
    do_simulate_wires(size_t n_channels, size_t n_time_ticks, size_t n_noise_functions, size_t seed, double background_height): simulate_wires_(n_channels, n_time_ticks, n_noise_functions, seed, background_height)
    {
    }

    auto operator ()([[maybe_unused]] phlex::data_cell_index const& idx)
    {
      return simulate_wires_();
    }

  private:
    sim::simulate_wires simulate_wires_;
};

PHLEX_REGISTER_PROVIDERS(graph, config)
{
  //Configuration options from user
  auto const seed = config.get<size_t>("seed", static_cast<size_t>(std::chrono::system_clock::now().time_since_epoch().count()));
  auto const n_channels = config.get<size_t>("n_channels_per_apa", 1024);
  auto const n_time_ticks = config.get<size_t>("n_time_ticks", 1024);
  auto const n_noise_functions = config.get<size_t>("n_noise_functions", 10);
  auto const background_height = config.get<double>("background_height", 5);

  //Register provider node with phlex
  graph.make<do_simulate_wires>(n_channels, n_time_ticks, n_noise_functions, seed, background_height)
    .provide("simulated_wires", &do_simulate_wires::operator())
    .output_product(
      phlex::experimental::algorithm_name("simulate_wires"),
      phlex::experimental::identifier("sim"),
      phlex::experimental::identifier("apa"));
}

PHLEX_REGISTER_ALGORITHMS(graph, config)
{
  //Add a simple observer to make sure provider runs
  graph.observe("count_wires", [](std::vector<raw::wire_readout> const& wires){ std::cout << "Simulated " << wires.size() << " wires." << std::endl; })
    .input_family(phlex::product_selector("simulate_wires", "apa"));
}
