{
  driver: {
    cpp: 'generate_layers',
    layers: {
      spill: { parent: 'job', total: 1, starting_number: 0, },
      apa: { parent: 'spill', total: 24, starting_number: 0, },
    },
  },
  sources: {
    fixed_pedestal: {
      cpp: 'window_hit_finder_alg',
    },
    form_wire_source: {
      cpp: 'form_source',
      stage: 'sim',
      input_file: 'form-tutorial-writing.root',
      algorithm: 'simulated_wires',
      plugin: 'wires_provider',
      products: ['sim'],
      creator: 'simulate_wires',
      technology: 'ROOT_RNTUPLE',
    },
  },
  modules: {
    window_hit_finder: {
     cpp: 'window_hit_finder_alg',
    },
    output: {
      cpp: 'form_module',
      outputs: {
        sim_wires: {
          output_file: 'form-tutorial-reading.root',
          technology: 'ROOT_RNTUPLE',
          products: ['sim', 'window_hits',],
        },
      },
    }
  },
}
