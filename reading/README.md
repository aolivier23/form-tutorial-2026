# Reading a Data Product with FORM
In the "Writing a Data Product with FORM" tutorial, you:
- started with a phlex algorithm that produces a data product from only a pseudo-random number generator
- configured phlex to write that data product to a file using FORM
- wrote a ROOT dictionary so other programs can read that data product

This tutorial introduces a second phlex algorithm, `window_hit_finder`, that reads `wire_readout`s from the file at the end of the `writing` tutorial and writes a new data product, `hit`.  It also consumes a "pedestal" data product whose value we will hard-code for now.  You could run both algorithms in the same phlex job and skip file input (this might be interesting to try on your own).  Instead, this tutorial will walk you through reading `wire_readout`s from the previous tutorial's file and writing the new `hit` data product to a second file.

**Start with the ROOT file `form-tutorial-writing.root` from the writing tutorial in _this_ directory.**  You can either move it from where you ran the writing tutorial's phlex commands or re-run the now-complete writing tutorial like this: `phlex -c ../writing/write_wires.jsonnet -j 1 --stage sim`

## Configuring phlex

**Try running this exercise's new configuration, `hits_from_fixed_pedestal.jsonnet`, now:**
`phlex -c hits_from_fixed_pedestal.jsonnet -j 1 --stage hit_reco`

phlex should tell you that it needs a source of a data product from layer "apa":
```
Using configuration file: hits_from_fixed_pedestal.jsonnet
[2026-10-22 12:19:37.743] [info] Number of worker threads: 1
[2026-10-22 12:19:37.906] [error] No provider found for the following required products:
  - Node 'window_hit_finder:window_hit_finder' requires product '<suffix [ANY] (of type [UNSET TYPE]) by creator simulate_wires (of stage [ANY]) in layer apa>'

[2026-10-22 12:19:37.906] [info] 
[2026-10-22 12:19:37.906] [info] CPU time: 0.01446s  Real time: 0.17840s  CPU efficiency:   8.11%
[2026-10-22 12:19:37.906] [info] Max. RSS: 21.672 MB
No provider found for the following required products:
  - Node 'window_hit_finder:window_hit_finder' requires product '<suffix [ANY] (of type [UNSET TYPE]) by creator simulate_wires (of stage [ANY]) in layer apa>'
```

FORM can provide source plugins that read data products from an existing file.  In the `sources` block of `hits_from_fixed_pedestal.jsonnet`, **add these lines:**
```
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
```

The `cpp: form_source` will always stay the same.  You can change `input_file` to point to any file on the current file system.  It should also support xrootd URIs, but that wasn't tested for this tutorial.  `technology` has to match the `technology` in `writing/write_wires.jsonnet` for now.  `algorithm`, `stage`, `creator`, `products`, and `plugin` must match the phlex job that produced the data product you want to read:
- `algorithm` matches the first argument to `provide()` on line 38 of `writing/simulate_wires_provider.cpp`
- `stage` matches the `--stage` flag from the command line that ran `write_wires.jsonnet`
- `creator` matches the first argument to `output_product()` on line 40 of `writing/simulate_wires_provider.cpp`
- `products` matches the suffixes of data products which were specified in the second argument to `output_product` on line 41
- `plugin` matches the jsonnet key in `writing/write_wires.jsonnet` that scheduled the algorithm producing this data product.  It's on line 10 in this case. 

**Run `hits_from_fixed_pedestal.jsonnet` again with this new block.**  FORM will print a new error message this time:
```
Using configuration file: hits_from_fixed_pedestal.jsonnet
[2026-10-22 12:34:31.528] [info] Number of worker threads: 1
Registering FORM input source...
FORM input source registered successfully
[2026-10-22 12:34:31.762] [error] void form::detail::experimental::root_rfield_read_container_imp::create_view(const std::type_info&): type std::vector<bool, std::allocator<bool> > requested for a field named sim does not match the type in the file: std::vector<raw::wire_readout> because:
in-memory field sim._0 of type bool is incompatible with on-disk field _0: type version 0 vs. 4294967295; structural role Plain vs. Record;
In-memory field/type hierarchy:
sim [std::vector<bool>] (id: 0)
  _0 [bool] (id: 1)
On-disk field/type hierarchy:
sim [std::vector<raw::wire_readout>] (id: 0)
  _0 [raw::wire_readout, type version: 4294967295, type checksum: 2300998981] (id: 1)

At:
  ROOT::RResult<void> ROOT::RFieldBase::EnsureMatchingOnDiskField(const ROOT::RNTupleDescriptor&, uint32_t) const [/exp/dune/app/users/aolivier/phlex_develop_sep2026/spack/var/spack/stage/spack-stage-root-6.40.02-b66mkt67ehridfbrcmc6itl4ku37mrqb/spack-src/tree/ntuple/src/RFieldBase.cxx:1070]
```

The type `bool` is just the default type FORM's RNTuple backend assumes for a data product when it isn't given one.  When you see this error message, it really means that you need to register a data product with FORM before you can read it.

## Registering a Data Product
FORM provides a special function template to register a data product for reading: `form::experimental::register_form_vector_product_type<>()`.  Use it by filling in the type of your data product both as a template argument and as a string like this:
`form::experimental::register_form_vector_product_type<raw::wire_readout>("raw::wire_readout");`

`register_form_vector_product_type<>()` has to be called from inside a phlex plugin registration macro.  You can write a dedicated phlex plugin that only registers a data product.  This is how many of the FORM CI tests work.  In this tutorial, **include it with the `window_hit_finder` plugin on line 19 of `register_window_hit_finder.cpp`.  Also `#include` the header it comes from: `phlex/form/form_source_type_registry.hpp`.  Now rebuild the tutorial.  Try running `hits_from_fixed_pedestal.jsonnet` again.**  You should get an error message from phlex about a missing library symbol that looks something like this:
```
Using configuration file: hits_from_fixed_pedestal.jsonnet
[2026-10-22 12:46:22.257] [info] Number of worker threads: 1
phlex: symbol lookup error: /exp/dune/app/users/aolivier/phlex_develop_sep2026/form-tutorial-opt/lib/libwindow_hit_finder_alg.so: undefined symbol: _ZN4form12experimental26register_form_product_typeENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN5phlex12experimental7type_idERKSt9type_infoSt8functionIFSt10unique_ptrINS8_12product_baseESt14default_deleteISF_EEPKvRKS6_SM_EE
```

Any phlex plugin that registers a data product needs to link against the `phlex::form` library.  **Edit line 18 of CMakeLists.txt to link against `phlex::form`.**  You can add it after `phlex::module`.  **Run `hits_from_fixed_pedestal.jsonnet`.**  Your phlex job should finish quickly!  But you don't have any way to check that it produced reasonable output.  You could add an `observer` plugin to print out information from the `hit`s you're producing.  But this is a great opportunity to practice registering a new data product for output.

## Outputting Hits and Verifying Results
First, **create a dictionary for the `hit` data product**.  The process should be very similar to what you learned in the `writing` tutorial.  You'll need a `classes_def.xml`, a `dict.h`, and some `CMakeLists.txt` changes to get it to work.  Peek at `git diff origin/main` if you need some help.  You'll have to **re-compile the tutorial** after making these changes.

Next, **add FORM output to `hits_from_fixed_pedestal.jsonnet`** like you did in the `writing` tutorial.  Try writing out both `wire_readout`s and `hit`s at suffixes `sim` and `window_hits`.  Write them to a file named `form-tutorial-reading.root`.

Now, **run `hits_from_fixed_pedestal.jsonnet`** one more time.  It should produce an output ROOT file.  **Run `plot_hit_heights.py form-tutorial-reading.root`** to produce a histogram of peak hit charges.  The stage name on the command line for `hits_from_fixed_pedestal.jsonnet` must still be `hit_reco` for this plotting script to work.  It has a "skewed Gaussian" distribution that starts at the hard-coded pedestal cut of 60 ADC. 
