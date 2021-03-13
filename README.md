# Cloud Loader and ASsembler System (CLASS)
Welcome to CLASS! The assembler for ECE554 Spring 2021 at UW-Madison.

The Cloud Loader and Assembler System (CLASS) is a simple tool which allows, based on pRISCas,
that serves as a barebones assembler system for custom ISAs of designs loaded to FPGAs.

## Build Instructions
### UNIX and UNIX-like
Compilation on UNIX systems can be done using one of the many make utilities. To make just the Shell, typing `make` will suffice. After make is finished, just go to the `bin` directory (which will be created if it doesn't already exist) and run `class` or `cload`.
#### 
Other useful targets:
#### `all` - the default target, builds the `class` executable and `cload` executable with debugging symbols and no optimization (debug)
#### `clean` - clean the solution, remove executables and objects
#### `release` - build the project as a release (that is, without debugging symbols and with optimization on)
