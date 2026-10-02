# Writing a Data Product with FORM
Particle physics reconstruction software in phlex is organized into *algorithms*.  One algorithm represents a single step in DUNE's multi-hour reconstruction procedure.  How algorithms communicate with each other is defined by the phlex framework.  phlex limits algorithms to communicating through only their python or c++ *return values* to give us benefits like:
- simpler development and debugging
- control of memory usage
- saving algorithm results to a file

A *data product* is the data type of the return value of an algorithm.  Particle physicists usually want to associate numbers, lists, etc. together in the output of an algorithm.  When working with phlex in c++ and python, we use classes to do this.  An example of a data product class is a `wire_readout`.  A single `wire_readout` represents one pixel in DUNE's horizontal drift far detector.  It has a channel number and a recorded waveform.  It's recorded in a particular anode plane assembly (APA) that we will later reconstruct all at once.  Take a look at `wire_readout.hpp` to see what a data product looks like in action!

To work with `wire_readout`s in phlex, we need an algorithm that produces them.  Since we're starting with an empty project and are saving file reading for later, we need an algorithm that acts as a phlex *provider* node.  A provider is a phlex higher-order function that takes no data products as input and returns data products as output.  It can still read from phlex configuration.  A provider just can't receive communication from other algorithms.  The way phlex organizes algorithms within a job is an acyclic graph, which can look a little like a tree when there's only one provider node.  In this example, the `simulate_wires` algorithm is like the root of that tree.

`simulate_wires.cpp` defines an algorithm for creating wires from random numbers.  But it does *not* define a phlex higher order function yet.  One of the strengths of phlex is that physics logic is completely separate from communication with the framework.  `simulate_wires` could be re-used in a dedicated program to study systematic uncertainties completely outside of phlex, and nothing about this tutorial would need to change.  Take a look at `simulate_wires.hpp` and `simulate_wires.cpp` to see the physics logic for this exercise.

`simulate_wires_provider.cpp` actually ties the `simulate_wires` library in with the phlex framework.  It defines a higher order function, a provider, through which `simulate_wires` interacts with the framework.  `simulate_wires_provider.cpp` is also our chance to query the user for initialization information through the framework.  Finally, this file labels the wires produced by the `simulate_wires_provider` node so other algorithms can search for them in future exercises.

phlex needs a configuration file to describe what job to run.  A phlex configuration file lists algorithms, in no particular order, and communicates initialization information like how many wires to simulate to that algorithm.  We're going to use a jsonnet file to configure phlex.  Jsonnet blends the simplicity of JSON with the human-friendliness features like comments, references, and limited arithmetic.  A phlex jsonnet configuration needs a *driver* block and a *modules* block.  The modules block is our chance to register the `simulate_wires_provider` node with the framework.  It takes a mandatory `cpp` or `python` argument that points phlex to a library.  Modules that use phlex's configuration system also list configuration parameters here.  Configuration parameters can include maps, lists, and nested data structures.

In addition to `simulate_wires_provider`, we also schedule an `output` module.  This is how phlex communicates with files.  The `cpp` algorithm is always `form_module` at the time of writing.  `form_module` requires a `products` list of dictionaries where each product can define an `output_file`.  `form_module` identifies data products by their `suffix` which must go in a list named `products` for each file.  We also specify in this jsonnet file that we want to use the new `ROOT_RNTUPLE` file format `technology` to store these products instead of the default `ROOT_TTREE`.  `ROOT_RNTUPLE` files are often 10-30\% smaller than `ROOT_TTREE` files.

Run a phlex job with this command:

```
phlex -c form_writing_tutorial.jsonnet -j 1
```

It won't work!  The problem is that FORM needs to figure out how to convert a `wire_readout` into a binary representation in a file.  The ROOT toolkit's `RNTuple` component does almost all of the hard work for us.  We just have to create a *dictionary* any time we want to store a new data product in a file.  `classes_def.xml` configures ROOT to discover the data members of `wire_readout.cpp`.  We use it with a few special commands in `CMakeLists.txt`:

```
TODO: CMakeLists.txt for ROOT dictionary generation
```

ROOT also needs a list of headers to process: `dict.h`.  Finally, we need a special phlex module to register `wire_readout` with FORM: `register_wire_readout.cpp`.  There aren't many options to configure `register_wire_readout`, so you can copy this module around as you write new data products.

Now try running the same phlex job again.  That should produce a `.root` file in your current directory.  `.root` files confuse the versioning system and use up our most expensive tier of storage at Fermilab rapidly.  So we'll try to only produce ROOT files in `/exp/dune/data/users/${USER}/form_tutorial`.  Move this file there, then let's look at what's inside!

You can open a `.root` file with the ROOT toolkit's interpreter executable: `root -l form_writing_tutorial.root`.  The ROOT interpretter works a little like a combination of your command line shell and the command `python`.  But it interprets c++ instead of the python language!  You can use `root`'s shell like the bash shell to see what objects a ROOT file contains with `.ls`.  This should produce a list of objects as follows:

```
TODO
```

Most `ROOT::RNTuple`s in the file correspond roughly to the data products produced by a single phlex node.  There's also a `TODO` RNTuple for the internal workings of FORM that we're going to ignore today.  An RNTuple is structured as a tree of *fields*.  Most top-level fields hold a single data product each.  Each RNTuple holds many *entries* in this tree of fields format.  So an RNTuple does a job similar to a Pandas dataframe in python but more focused on storing classes instead of collections of integers and floating point numbers.  We can list the fields in an RNTuple like this:

```
auto reader = ROOT::RNTupleReader::Open(*_file0, "TODO");
reader->PrintInfo();
```

We can also print the contents of individual entries in an RNTuple.  This can be helpful when debugging a new algorithm.

```
reader->Show(0);
```

Let's histogram the `waveforms` values in this RNTuple.  It's convenient to visualize an RNTuple's contents using another tool from ROOT: `RDataFrame`.  `RDataFrame` takes that analogy to Pandas a few steps further.  We can write cuts and plot combinations of fields in a more "functional" style than would be possibly just writing a loop over the RNTuple ourselves.  Try running this code:

```
TODO: RDataFrame
```

An interactive window should pop up with a histogram.  In this histogram, you can see what `simulate_wires` is doing.  It starts with a fake noise floor constructed from a few different frequencies of sine functions.  Then it layers gaussians on top of that floor randomly to simulate physics hits in a toy particle detector.  It's a very poor simulation for lots of reasons: it doesn't simulate any of the physics of the readout electronics that massively reduce noise, and it doesn't simulate charges moving past a conductor.  But it gets faux waveforms on screen with just enough features to be analyzed in the next exercise.
