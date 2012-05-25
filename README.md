# Pin tool: instruction coverage

[Pin](http://www.pintool.org/) is an extendable tool for dynamic instrumentation of programs.
`coverage.cpp` implements a Pin module to determine instruction coverage of (almost) arbitrary programs.

## Build instructions

* Obviously, you need [Pin](http://www.pintool.org/).  
  The tool was tested as `ia32` tool for `pin-2.11-49306-msvc9-ia32_intel64-windows`.

* Copy the directory `coverage` to `<Pin dir>/source/tools/`

* Build the tool using the provided VisualStudio 2008 solution file `coverage.sln`.  
  For other platforms or different compilers, please see the instructions in the Pin manual.

* The result should be a file named `coverage.dll` in the tools directory


## Usage

* You need a test application **with** debug information (`.pcb` file).  
  If you have no other choice, use the `testapp` from this repository.


## Further information

..
