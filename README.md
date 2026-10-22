# FORM Tutorial
A tutorial on how to use the phlex framework's I/O system.  Demonstrates registering a data product with FORM, writing data products from phlex, reading data products into phlex, and eventually how FORM handles phlex's data cell hierarchies.  This tutorial develops a handful of toy phlex algorithms to generate data products that FORM can read and write.  These toy algorithms are not intended to have any physics content.

# Installation
`form-tutorial-2026` is provided as a CMake project that depends on phlex and ROOT.  Both packages are already provided in the container for the 2026 IISc tutorial.  `form-tutorial-2026` was tested as an out-of-source build.

Instructions to install:
```
git clone https://github.com/aolivier23/form-tutorial-2026.git
mkdir -p form-tutorial-2026-opt/build
cd form-tutorial-2026-opt/build
cmake ../../form-tutorial-2026 -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release
make install
```

Once you've installed the tutorial, you have to set it up:
- the first time you install
- each time you log in to a new shell

Setup instructions: `source form-tutorial-2026-opt/bin/setup.sh`

# Exercises
A series of walkthroughs explaining how to use phlex with FORM.  The `main` branch is an answer key.  One branch will be provided as the starting point for each exercise with a name like `exercise_reading`.  Feel free to peak at the answer key if you get stuck with `git diff main -- <file_name>` for a specific file or just `git diff main -- *` for a whole directory.

## Exercise A
Demonstrates how to write a data product with FORM.

## Exercise B
Demonstrates how to read a data product from a FORM file into phlex and generate a different data product.

## Exercise C
Demonstrates how FORM handles data cell hierarchies.
