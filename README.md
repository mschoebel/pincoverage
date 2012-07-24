# Pin tool: instruction coverage

[Pin](http://www.pintool.org/) is an extendable tool for dynamic instrumentation of programs.
`coverage.cpp` implements a Pin module to determine instruction coverage of (almost) arbitrary programs.

## Build instructions

* Obviously, you need [Pin](http://www.pintool.org/).  
  The tool was tested as `ia32` tool for `pin-2.11-49306-msvc9-ia32_intel64-windows` and for the `msvc10` version.

* Copy the directory `coverage` to `<Pin dir>/source/tools/`

* Build the tool using one of the provided VisualStudio solution files `coverage_vs9.sln` or `coverage_vs10.sln`.  
  For other platforms or different compilers, please see the instructions in the Pin manual.

* The result should be a file named `coverage.dll` in the tools directory


## Usage

* You need a test application **with** debug information (`.pdb` file).  
  If you have no other choice, use the `testapp` from this repository.

* Go to the directory `<Pin dir>/ia32/bin/` (if you have built the coverage tool for `ia32`).

* Execute Pin using the coverage tool:

  `pin -t coverage.dll [-p <source code dir>] [-n <name>] -- <test program>`

  With `-t coverage.dll` the extension module is selected.
  Using `-p <source code dir>` the source code location of the test program is specified.
  This path is used to reduce the amount of instrumentation: only instructions are instrumented that are located in a subdirectory of the specified source directory. The location is determined based on the debug information. The name of the application is specified with `-n <name>` and is used to name the resulting `.csv` files.

  If `-p` is not specified all instructions are instrumented. If `-n` is not specified "app" is used as program name.

* The Pin tool creates two `.csv` files with coverage information.

  `app_files.csv` contains coverage information on the file level. Result for the provided test application:

  ```text
  File;Instructions SUM;Instructions EXEC;Coverage
  testapp\main.cpp;4;4;100.0
  testapp\test.cpp;114;102;89.47
  ```

  `app_routines.csv` contains coverage information on the file level. Result for the provided test application:

  ```text
  File;Routine;Instructions SUM;Instructions EXEC;Coverage
  testapp\test.cpp;`dynamic initializer for 'FizzBuzz::BUZZ'';7;7;100.0
  testapp\test.cpp;`dynamic initializer for 'FizzBuzz::FIZZ'';7;7;100.0
  testapp\test.cpp;FizzBuzz::sayFizz;32;32;100.0
  testapp\test.cpp;FizzBuzz::sayFizzBuzz;68;56;82.35
  testapp\main.cpp;main;4;4;100.0
  ```

## Further information

* Remarks, hints, suggestions, pull requests are welcome.

* There is a binary release for Windows (VS9) available [here](https://github.com/downloads/mschoebel/pincoverage/coverage.zip). Use at your own risk.

* See `license.txt` for license information - in short: Simplified BSD License.

* See this [blog post](http://mschoebel.info/coverage_using_pin.html) for additional information.
