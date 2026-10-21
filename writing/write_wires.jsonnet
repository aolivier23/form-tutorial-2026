{
  driver: {
    cpp: 'generate_layers',
    layers: {
      spill: { parent: 'job', total: 1, starting_number: 0, },
      apa: { parent: 'spill', total: 24, starting_number: 0, },
    },
  },
  sources: {
    wires_provider: {
      cpp: 'simulate_wires_provider',
      n_time_ticks: 1024,
    },
  },
  modules: {
    count_wires: {
     cpp: 'simulate_wires_provider',
    },
  },
}
