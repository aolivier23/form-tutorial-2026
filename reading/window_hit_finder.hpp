#ifndef WINDOW_HIT_FINDER_H
#define WINDOW_HIT_FINDER_H

#include <vector>
#include <cstdint>

namespace reco
{
  class hit;
}

namespace raw
{
  class wire_readout;
}

std::vector<reco::hit> window_hit_finder(std::vector<raw::wire_readout> const& wires, std::uint16_t pedestal);

#endif //WINDOW_HIT_FINDER_H
