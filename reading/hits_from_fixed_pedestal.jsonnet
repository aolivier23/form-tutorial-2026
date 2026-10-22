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
  },
  modules: {
    window_hit_finder: {
     cpp: 'window_hit_finder_alg',
    },
  },
}
