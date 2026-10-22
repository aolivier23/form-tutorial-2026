# Writing a Data Product with FORM
This tutorial starts with:
- a toy physics algorithm: `simulate_wires`
- a toy data product produced by that algorithm: `wire_readout`
- phlex components to run that algorithm: `simulate_wires_provider` and `write_wires.jsonnet`
- a script to check the results: `plot_one_wire.py`
- a CMake build system to put it all together

When you complete this tutorial, you will be able to:
- configure phlex to write data products using FORM
- write data products to a file that FORM can read
- demonstrate that the data products in this FORM file contain plausible information

**Try running `write_wires.jsonnet` now.**  The job should complete, but it
won't produce an output file.

## Configuring FORM
`write_wires.jsonnet` drives phlex to run the `simulate_wires` algorithm.  But it doesn't save results to a file.  It can only pass data products to other algorithms in the same process.

Use FORM by scheduling a special algorithm in a phlex configuration file like this:
```
output: {
  cpp: 'form_module',
  outputs: {
    sim_wires: {
      output_file: 'form-tutorial-writing.root',
      technology: 'ROOT_RNTUPLE',
      products: ['sim',],
    },
  },
}
```

**Add this code block to `write_wires.jsonnet` now.**  It needs to be at the same scope as the `count_wires` algorithm.  

The `output` and `cpp: form_module` parts will always stay the same.  `sim_wires` defines a group of data products that phlex will write.  The name `sim_wires` does not need to match any other names and is only present for user convenience.  Each group of data products is written to the same `output_file`.  `technology` is the format in which those data products will be written.  The default is ROOT's `TTree` format.  This tutorial focuses on the new `RNTuple` format because it produces smaller files.

`products` is a list of phlex **suffixes** whose data products will be written to this file.  The suffixes FORM is configured for match the second argument to `output_product()` on line 41 of `simulate_wires_provider.cpp`.  Suffixes look a little different for most other kinds of phlex higher order nodes.  The writing exercise in this tutorial will demonstrate how to save data products from a transform node.

 **Run write_wires.jsonnet now: `phlex -c write_wires.jsonnet -j 1 --stage sim`**.  You should get output like this:
```
=== form_output_module::save_data_products ===
Creator: wires_provider:simulated_wires
Stage: sim
Data cell: [spill:0, apa:0]
Number of products: 1
  Product: simulate_wires/sim
root_rfield_write_container_imp::setup_write could not create column-wise storage for std::vector<raw::wire_readout, std::allocator<raw::wire_readout> >.  This class is probably using something obsolete like TLorentzVector.  Storing it in streamer mode to keep the application going.
Wrote 1 products to FORM
Simulated 1024 wires.

=== form_output_module::save_data_products ===
Creator: wires_provider:simulated_wires
Stage: sim
Data cell: [spill:0, apa:1]
Number of products: 1
  Product: simulate_wires/sim
Wrote 1 products to FORM
Simulated 1024 wires.
```

FORM is telling you that you're missing some ingredients for producing a readable file.  You can open `form-tutorial-writing.root` with `root -l form-tutorial-writing.root` and see that it contains something with `.ls`.  But `plot_one_wire.py` won't work yet, and the reading tutorial would crash if you ran it now.  You're missing a ROOT dictionary that tells FORM how to put data products back together from the contents of this file.

## ROOT Dictionary
When you use the `ROOT_TTree` or `ROOT_RNTuple` FORM technologies, you need to produce a ROOT dictionary for each data product that will be written to a file.

Most of ROOT dictionary generation happens when you compile your data product into a library.  This tutorial uses CMake to compile and link c++ libraries.  Open `CMakeLists.txt` in this directory and **add these code blocks now after `install(TARGETS wire_readout DESTINATION lib):**
```
reflex_generate_dictionary(wire_readout dict.h SELECTION classes_def.xml)
target_link_libraries(wire_readout PRIVATE ROOT::RIO)
set_target_properties(wire_readout PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wire_readout_rdict.pcm DESTINATION lib)
```

**Re-compile the tutorial like this:**
```
cd form-tutorial-2026-opt/build
make install
```

You'll have to do this each time you edit a file.  The jsonnet file is an exception to this rule.

**Run `write_wires.jsonnet` again.**  The build system will tell you you're missing a few more files:
- `classes_def.xml`: tells ROOT which classes to build dictionaries for
- `dict.h`: headers for the classes in `classes_def.xml`

**Create files that look like these:**
### `classes_def.xml`
```
<lcgdict>
  <class name="raw::wire_readout"/>
  <class name="std::vector<raw::wire_readout>"/>
</lcgdict>

```

### `dict.h`
```
#include "wire_readout.hpp"
#include <vector>
```

Once you add these two files, you'll need to **re-create the build directory**:
```
rm -rf opt/build
mkdir -p opt/build
cd opt/build
cmake ../../form-tutorial-2026 -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release
make install
```

Now **run `write_wires.jsonnet` again.**  The warning about streamer mode should be gone.  This should be a well-formed FORM file!  The reading tutorial will demonstrate how to use it.

This tutorial will use some expert knowledge of how FORM works to check that the results make sense.  `plot_one_wire.py` draws only one of the simulated wires as a histogram that kind of looks like the old LArSoft event display.  **Run `python plot_one_wire.py form-tutorial-writing.root 0 13 243`.**  This should produce an image file named `waveform_spill_0_apa_13.png`.  You can view it from an ipython terminal and verify that it looks like a noise background with maybe a handful of Gaussian hits superimposed on top.
