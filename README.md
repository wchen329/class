# Cloud Loader and ASsembler System (CLASS)
Welcome to CLASS! The assembler for ECE554 Spring 2021 at UW-Madison.

The Cloud Loader and Assembler System (CLASS) is a simple tool which allows, based on pRISCas,
that serves as a barebones assembler system for custom ISAs of designs loaded to FPGAs.

## Build Instructions
### Windows
Only the assembler is supported in Windows.

To compile on Windows you need at least Visual Studio 2010 (10.0) and Windows XP (2002).
Windows 2000, VC 9.0/2008 and earlier are not supported, but may work.

To build, open `class\class.sln` and then press F7 (or Build Solution).

### UNIX and UNIX-like
Compilation on UNIX systems can be done using one of the many make utilities. There are three main applications that can be built from CLASS. These are CLASS (the assembler),
CLOAD (the loader), and CLOAD\_SIM (the loader for ASE).

To build each of these, make the following targets

#### `bin/class` - makes the assembler
#### `bin/cload` - makes the loader (with physical FPGA connectivity, for use on DevCloud)
#### `bin/cload_sim` - makes  the loader for simulation through Intel ASE

#### 
Other useful targets:
#### `all` - the default target, builds the `class` executable and `cload` executable with debugging symbols and no optimization (debug)
#### `clean` - clean the solution, remove executables and objects
#### `release` - build the project as a release (that is, without debugging symbols and with optimization on)

## Acknowledgments
To implement Intel DevCloud capability, this project also makes use of code from University of Florida intel-training-modules (GPLv3 licensed, in the contrib folder).
