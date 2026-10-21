#include "window_hit_finder.hpp"
#include "hit.hpp"
#include "writing/wire_readout.hpp"

#include "phlex/form/form_source_type_registry.hpp"
#include "phlex/source.hpp"
#include "phlex/module.hpp"

PHLEX_REGISTER_PROVIDERS(graph, config)
{
  auto const pedestal = config.get<uint16_t>("pedestal", 60);
  graph.provide("fixed_pedestal", [pedestal]([[maybe_unused]] phlex::data_cell_index const& idx){ return pedestal; })
    .output_product(phlex::experimental::algorithm_name("fixed_pedestal"),
                    phlex::experimental::identifier("user_input"),
                    phlex::experimental::identifier("spill"));
}

PHLEX_REGISTER_ALGORITHMS(graph, config)
{
  form::experimental::register_form_vector_product_type<raw::wire_readout>("raw::wire_readout");

  graph.transform("window_hit_finder", window_hit_finder)
    .input_family(phlex::product_selector(config.get<std::string>("waveform_alg", "simulate_wires"), "apa"), phlex::product_selector(config.get<std::string>("pedestal_alg", "fixed_pedestal"), "spill"))
    .output_product_suffixes("window_hits");
}
