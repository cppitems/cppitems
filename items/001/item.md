1 // item status
# L1: Source code organization, compilation toolchain, distribution options, coding style, linting, sanitizers
This item briefly discusses important organizational options for a C++ project/library and steps through the phases of configuring and building a project using command line tools on a linux system and tools of the *LLVM* compiler infrastructure.

## Example library
This simplistic example is an interface to a `Grid` *structure* which holds data representing a 2D regular grid.
A `grid_init` function allocates the grid with the desired dimension and performs the initialization.
To access the grid at a coordinate, `grid_at` returns a *pointer* to the respective item in the grid. 
Finally, `grid_free` is used to *deallocate* the memory and reset the structure. So the *header* file looks like this: 

```pmans 
/* file: grid.h */
#pragma once 
struct /*f4*/ Grid {
  size_t nx;
  size_t ny;
  double *data;
};
typedef Grid /*f8*/ GridType;
void /*b9*/ grid_init(/*f9*/ GridType *grid);
void /*b9*/ grid_free(/*f9*/ GridType *grid);
double */*b7*/ grid_at(/*f9*/ GridType *grid, size_t x, size_t y);
```

and the associated *source* file which *defines* the interface functions looks like
```pmans
/* file: grid.cpp */ /* compile: clang++ -c grid.cpp */
#include <cstdlib> // std::calloc, std::free
#include "grid.h"
void /*b9*/ grid_init(/*f9*/ GridType *grid) {
  grid->nx = 3;
  grid->ny = 4;
  grid->data = (double *)std::calloc(grid->nx * grid->ny, sizeof(double));
}
void /*b9*/ grid_free(/*f9*/ GridType *grid) {
  std::free(grid->data);
  grid->nx = 0;
  grid->ny = 0;
}
double */*b7*/ grid_at(/*f9*/ GridType *grid, int x, int y) {
  return &grid->data[x + grid->nx * y];
}
```

 A usage of this interface can then look like

 ```pmans
/* file: main.cpp */ /* compile: clang++  grid.o main.cpp -o main */
#include "grid.h"
int main() {
  /*f8*/ GridType /*f4*/ grid = {0};
  /*b9*/ grid_init(&/*f4*/ grid, 3, 4);
  for (size_t x = 0; x != grid.nx; ++x) {
    for (size_t y = 0; y != grid.ny; ++y) {
      */*b7*/ grid_at(&/*f4*/ grid, x, y) = x + y;
    }
  }
  /*b9*/ grid_free(&/*f4*/ grid);
  return 0;
}
 ```
where a sweet over the grid is performed using a nested for-loop.
The `grid_at` function is used to write to the grid.

Note that beside the namespace operator `::` for the wrapped `calloc` and `free` this library only uses C language features.

> Which C++ language features are you missing? 


## File organization
The library source code is organized using *header* and *source* files.
It could also have been placed in a single (header) file.
Let's discuss some consequences triggered by these two options.

### Redundancy
If separated, a portion of the source code is redundant (the function declarations).
In a single file, functions definition are sufficient, removing this redundancy.

### Dependent projects
For a single file, the full implementation is included in a dependent project with the consequence that it is compiled together with the project in a single *compilation unit*. 
This can be advantageous during optimization but leads to longer compilation times (especially, as the compilation of a single compilation unit is hard to parallelize). 
A change in the library requires a recompilation of the dependent project.

> Why are most optimizations performed at the scope of a compilation unit?

If separated, the compilation of the library and the project happens in different compilation units, requiring a *linking* step after the compilation of the project.
A recompilation of the project is only required if the interface of the library changes. 
If project-wide *compiler flags* change, a recompilation of the library might be required, too.

> What is an example for a "project-wide compiler flag" which requires recompilation of dependent projects?

### Distribution
For a single file, in the simplest case, simply this file is deployed. If the library itself has specific compilation options or dependencies, additional configuration instructions are typically required.

If separated, the distribution of the compiled library together with the header files is possible. 
This requires a distribution of the compiled versions for all targeted platforms and configurations.
Additionally, the sources of the library together with build instructions can be distributed.

> Can different compilers/versions be used for building the distributed  libraries and later in the dependent projects?

### Template interfaces
If a library interface includes templates (or is a pure template library), the full implementation hast to be distributed (*header-only*).

> Why is *header-only* distribution required for template interfaces?

## Build toolchain
The compilation is performed using a *toolchain* which typically includes a *compiler*, *standard libraries*, *linker*, and *runtime libraries* for the targeted system.

> Examples for each of those?

### Invocation
The full set of flags when compiling our library can be revealed with
```
clang++ -### grid.cpp -c 
```
which will list the default settings assuming the host configuration as compilation target. Similar for the subsequent build command 
```
clang++ -### grid.o main.cpp 
```
which triggers a compilation step followed by a linker step.

> Some examples for compiler flags?

### Cross compilation
Targeting a specific system and architecture which is supported by the available toolchain can look like
```
clang++ -### --target=wasm32-unknown-wasi --sysroot=... grid.cpp -c
clang++ -### --target=wasm32-unknown-wasi --sysroot=... grid.o main.cpp 
```
which builds the application for the `wasm32` architecture with an `unkown` vendor assuming a `wasi` system.

## Compilation process

The compilation, which transforms the source files into the specified final format proceeds in phases. 
To reveal some details about the stages of compilation for our library one could use
```
clang++ -ccc-print-phases grid.cpp -c 
```
which will print something like
```
         +- 0: input, "grid.cpp", c++
      +- 1: preprocessor, {0}, c++-cpp-output
   +- 2: compiler, {1}, ir
+- 3: backend, {2}, assembler
4: assembler, {3}, object
```
which tells us that it starts with the `grid.cpp` file and final output will be an *object* file.

### Preprocessor
After the initial parsing of the individual sources (character mapping, comment stripping, line unwrapping, ... ) the preprocessor is the first 'real' action on the code: all actions defined for preprocessor *directives* like `#include` and `#define` are performed which *always* results in a *single* code document which can be revealed using
```
clang++ -E grid.cpp -c
```
which for our library results in
```pmans
/// ... // code from <cstlib> and nested #includes
extern void */*b6*/ calloc(...);
extern void /*b4*/ free(...);
/// ... // code from <cstlib> and nested #includes
struct Grid {
  int nx;
  int ny;
  double *data;
};
typedef struct Grid GridType;
void grid_init( GridType *grid);
void grid_free( GridType *grid);
double *grid_at( GridType *grid, int x, int y);
void grid_init( GridType *grid) {
  /// ...
}
void grid_free( GridType *grid) {
  /// ...
}
double *grid_at( GridType *grid, int x, int y) {
  /// ...
}
```
which is the starting point for the following translation.
Note that the order of `#include`s has consequences on the arrangement of this final code document.

> Possible errors during the preprocessing phase are?
> - "file not found" if includes are not found in the lookup mechanism of the toolchain
> - silent errors in macros, which manifest later during translation

### Translation
The translation process is performed according to the selected language standard. Using the C++17 standard for the translation looks like
```bash
clang++ -std=c++17 grid.cpp -c
clang++ -std=c++17 -stdlib=libc++ grid.cpp -c # select stdlib
```
As a intermediate (non-optimized) result of this translation, clang uses an intermediate representation for all C-like languages called *abstract syntax tree* (AST).
This AST can be inspected (and filtered) for our `grid_at` function with
```
clang-check -extra-arg=-std=c++17 -ast-dump --ast-dump-filter=grid_at grid.cpp --
```
which outputs a hierarchy of AST-nodes which closely resembles both the original source code but also the language standard. 
Each node represents an abstract language construct, for example the declaration of `grid_at` appears as
```
FunctionDecl 0x11c1b10 <srcloc>  grid_at 'double *(GridType *, int, int)'
|-ParmVarDecl 0x11c18e8 <srcloc> grid 'GridType *'
|-ParmVarDecl 0x11c1968 <srcloc> x 'int'
`-ParmVarDecl 0x11c19e8 <srcloc> y 'int'
```

which allows a direct mapping to names and positions in the original source files.

If the code documents satisfies the rules imposed by the language standard (i.e., a AST was successfully created), further stages of the translation, which are influenced by compiler flags produce the final output format. 
This final output object must comply to the language standard w.r.t. to the visibility of internal symbols to other objects, and referencing of external symbols used internally.

The symbols of the resulting object file (or library) can be inspected using
```
nm grid.o
```
which reveals information about the symbol table of the object:
```
0000000000000090 T _Z7grid_atP4Gridii
0000000000000050 T _Z9grid_freeP4Grid
0000000000000000 T _Z9grid_initP4Grid
                 U calloc
                 U free
```
We can see that the names of the functions are mangled using some scheme which involves a prefix `_` and additional prefixes indicating the number of characters of a function name `Z9grid_init` or a function parameter `P4Grid` and special names for fundamental types like `i` for `int`.
Note that the mangling schemes are not part of the C++ standard, but mostly the *Itanium C++ ABI* is used, enabling compatibility between objects created by different compilers.

> Examples for what else is defined in the ABI?

Demangling of the symbols is possible using
```
nm -C grid.o
```
which results in
```
0000000000000090 T grid_at(Grid*, int, int)
0000000000000050 T grid_free(Grid*)
0000000000000000 T grid_init(Grid*)
                 U calloc
                 U free
```

Extensive compiler options exist to granularly control the reporting of error and warnings.
Enabling all errors and also promote warnings to errors can be achieved with
```
clang++ -Werror -Weverything grid.cpp -c 
```
which is not a useful default, but is a good starting point as it reports the more granular flags responsible for the individual error groupings.

> Typical error during compilation?
>
> Which types of errors cannot be covered?

## Linking
Linking is performed after all required compilation units for an application are available in some form (object files, libraries). 

> Typical errors during linking are ?


The result of the linking step is a *dynamically* or *statically* linked application.
For example, 
```
clang++ grid.o main.cpp 
```
produces a dynamic executable `a.out` (default filename) which requires a suitable configuration (i.e., set of installed libraries of specific versions) at run time.
The `grid.o` object file is statically linked into the executable and (therefore does not need to be installed on the system). 
At run time, the dynamic resolution of linked libraries can be tested using 
```bash
ldd main
```
which shows something like
```
linux-vdso.so.1 (0x00007ffcaa5f0000)
libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007ffa17ec4000)
libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007ffa17b26000)
libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007ffa1790e000)
libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007ffa1751d000)
/lib64/ld-linux-x86-64.so.2 (0x00007ffa1824d000)
```
which reveals the resolved locations of the required libraries on the system.

To convert the `grid.o` object into a shared library and perform a dynamic linking one can use
```
clang++ -shared -fPIC grid.o -o libgridlib.so 
clang++ main.cpp -Wl,-rpath,/home/project/cppitems/items/001/grid libgridlib.so 
```
which is also reflected with an additional line in the output of `ldd`
```
...
libgridlib.so => /home/project/cppitems/items/001/grid/libgridlib.so (0x00007fa5f7a1c000)
...
```

## Build tools
Even for small projects the creation and maintenance of the instructions for the supported toolchains required to build the targets of project can be complex. 
The *CMake* tool is used for most openly available C++-projects as a cross-platform generator for project and dependency configurations.  

A minimal platform independent *CMakeLists.txt* (default filename) configuration for our library and application could look like:
```
cmake_minimum_required(VERSION 3.0)
project(001 LANGUAGES CXX)
add_compile_options("-std=c++17")
add_library(gridlib SHARED grid.cpp grid.h)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE gridlib)
```
and building the project could look like
```bash
# change to project 
mkdir build && cd build # prepare for 'out of source' build
cmake -L .. # create Makefiles and list variables
make VERBOSE=1 # use CMake generated Unix Makefiles
```

> Is a CMake configuration automatically portable?

## Coding Style
Formatting source code is only important if humans have to look at the code
(Even auto-generated code might be looked at by humans, e.g., to debug an error).
As many flavors of coding-styles and formatting exist, bigger projects restrict this freedom and settle with a set of rules for formatting and naming. 

> Examples for rules defined through a coding style?

### Formatting
To avoid manual code-rearrangement and to guarantee consistent style a very prominent tool is *clang-format*. 
To dump the default formatting settings for the *llvm* style this can be used
```bash
clang-format -style=llvm -dump-config > .clang-format
```
Applying the formatting to a file looks like
```bash
clang-format grid.h # prints formatted file to console
```
where the dominant adoptions are white space/newline arrangements.
Note that this is a lightweight standalone tool, e.g., it does not try to compile the code.

> Could clang-format also be used for refactoring tasks?

### Linting
To ensure a consistent coding style going further than bare-formatting, a 'linter' can be used.
Linting tries to capture constructs in the code which are 'not-wanted' for some reason, but happily compile, as they comply to the language standard. 
These reasons can be
- availability of a preferred alternative
- error-proneness of a construct
- custom guidelines (style or usage)


A popular linter based on clang is *clang-tidy* which is based on clang to analyze the source code. 
To dump the active configuration of checks which are performed one can use
```bash
clang-tidy -dump-config
``` 

For detailed linting, it is necessary that the linter has access to the exact compile commands associated with each source file.
CMake can be configured (`CMAKE_EXPORT_COMPILE_COMMANDS`) to output the compile commands to a `compile_commands.json` in the build directory.
To lint `grid.cpp` using clang-tidy on the command line can then be achieved with 
```bash
clang-tidy -p ./build grid.cpp 
``` 
which may output reports like
```
/grid.cpp:6:26: warning: do not manage memory manually; 
consider a container or a smart pointer [cppcoreguidelines-no-malloc]
  grid->data = (double *)std::calloc(grid->nx * grid->ny, sizeof(double));
```
if the check 'cppcoreguidelines-no-malloc' is enabled.
 
### IDE integration
Both tools clang-format and clang-tidy are integrated into the language server *clangd* which is available for many IDEs via a plugin.

> Examples for problems not detectable by the linter?

## Sanitizers
Many error-prone situations can be detected using linting. 
But many unwanted situations can only be detected during runtime, i.e., for a specific execution.
To capture such unwanted situations, clang (and gcc) offer *sanitizers* which can be enabled at compile time produce an *instrumented* binary which then reports problems and associated debug information during run time.
Enabling sanitizers typically leads to a slowdown and increased memory consumption.

> Major difference between compile time (linting) and run time (sanitizers) checks?

### Memory errors
The AddressSanitizer (ASAN) detects problems related to memory access like *out-of-bounds* access and also contains a
LeakSanitizer (LSAN) to detect memory leaks.
It can be enabled using the `-fsanitize=address` compiler flag.
For example this code
```pmans
#include "grid.h"
int main() {
  GridType grid;
  grid_init(&grid);
  for (int x = 0; x != grid.nx; ++x) {
    for (int y = 0; y != grid.ny /*b3*/ + 1; ++y) { // (1)
      *grid_at(&grid, x, y) = x + y;
    }
  }
  // grid_free(&grid); // (2)
  return 0;
}
```
will report something like 
```
==5420==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x608000000080 ...

WRITE of size 8 at 0x608000000080 thread T0
    #0 0x4c6181 in main .../main_asan.cpp:7:29
    ...
0x608000000080 is located 0 bytes to the right of 96-byte region ...
allocated by thread T0 here:
    #0 0x494082 in calloc (.../main_asan+0x494082)
    #1 0x7f182b0c0a9a in grid_init(Grid*) .../grid.cpp:6:26
    ...
```
for (1) and something like 
```
==5632==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 96 byte(s) in 1 object(s) allocated from:
    #0 0x494032 in calloc (.../main_asan+0x494032)
    #1 0x7f2fb1d11a9a in grid_init(Grid*) .../grid.cpp:6:26
    #2 0x4c5ff2 in main .../main_asan.cpp:4:3
```
for (2). 


### Reads of uninitialized values
If the assignment of an uninitialized value is directly visible for the compiler, a warning can be issued at compile time. 
This is the case at (1) in
```pmans
#include "grid.h"
int main() {
  GridType grid;
  grid_init(&grid);
  double /*b1*/ b;
  *grid_at(&grid, 0, 0) /*b3*/ = b; // (1) compiler warning
  auto /*f4*/ useb = [&grid, &b]() {
    *grid_at(&grid, 0, 0) /*b3*/ = b; // (2) no compiler warning
  };
  /*f4*/ useb();
  if(/*b*/ *grid_at(&grid, 0, 0) /*x*/) {} // (3) first 'read' of 'b'
  grid_free(&grid);
  return 0;
}
```
which outputs something like
```
...main_msan.cpp:6:27: error: variable 'b' is uninitialized ...
  *grid_at(&grid, 0, 0) = b; // (1) compiler warning
                          ^
```
In contrast, if the usage happens in a different context like at (2), the compiler cannot help.
MemorySanitizer (MSAN), which can be enabled using the `-fsanitize=memory` compiler flag can track the use of uninitialized values and warn if they are first *read*, which happens at (3) and means that the program behavior is influenced.
The output of MSAN might look like
```
==10149==WARNING: MemorySanitizer: use-of-uninitialized-value
    #0 0x497424 in main .../main_msan.cpp:11:7
    ...
  Uninitialized value was stored to memory at
    #0 0x4977b5 in main::$_0::operator()() const .../main_msan.cpp:8:27
    ...
  Uninitialized value was created by an allocation of 'b' in ... function 'main'
    #0 0x4971c0 in main .../main_msan.cpp:2
```
which also provides hints to where the uninitialized value originates from if `-fsanitize-memory-track-origins` is used.

### Data races
If more than one *thread* of execution is used, *data races* occur when
instructions from two different threads
- access the same memory location,
- at least one instruction is modifying value,
- and no synchronization rule is present.

For example in
```pmans
/// ...
  GridType grid;
  grid_init(&grid);
  auto /*f*/ write_grid /*x*/ = [&grid]() {
    /*b*/ *grid_at(&grid, 0, 0) /*x*/ = 5;
  };
  std::thread one(/*f*/ write_grid /*x*/);
  std::thread two(/*f*/ write_grid /*x*/);
  one.join();
  two.join();
  grid_free(&grid);
/// ...
```
the two threads access (load and store) the value at index `0,0` potentially at the same time, and no synchronization is defined.
Using the ThreadSanitizer (TSAN) `-fsanitize=thread` detects such data races during execution and may output something like
```
==================
WARNING: ThreadSanitizer: data race (pid=25119)
  Write of size 8 at 0x7b1800000000 by thread T2:
    #0 .../main_tsan.cpp:12:27 (main_tsan+0x4b6c34)
    #1 .../type_traits:3539:1 (main_tsan+0x4b6b20)
    #2 .../thread:273:5 (main_tsan+0x4b6a48)
    #3 .../thread:284:5 (main_tsan+0x4b6549)

  Previous write of size 8 at 0x7b1800000000 by thread T1:
    #0 .../main_tsan.cpp:12:27 (main_tsan+0x4b6c34)
    #1 .../type_traits:3539:1 (main_tsan+0x4b6b20)
    #2 .../thread:273:5 (main_tsan+0x4b6a48)
    #3 .../thread:284:5 (main_tsan+0x4b6549)
...
```

A similar situation when using *OpenMP* looks like
```pmans
/// ...
  omp_set_num_threads(3); 
#pragma omp parallel
  {
    /*b*/ *grid_at(&grid, 0,0) /*x*/ = 5; 
  }
  grid_free(&grid);
/// ...
```
which might log something like
```
==================
WARNING: ThreadSanitizer: data race (pid=24952)
  Write of size 8 at 0x7b1800000000 by thread T2:
    #0 .omp_outlined._debug__ .../main_tsan.cpp:24:28 (main_tsan+0x4b57de)
    #1 .omp_outlined. .../main_tsan.cpp:22:3 (main_tsan+0x4b5858)
    #2 __kmp_invoke_microtask <null> (libomp.so+0xab032)
    #3 main .../main_tsan.cpp:21:1 (main_tsan+0x4b573c)

  Previous write of size 8 at 0x7b1800000000 by thread T1:
    #0 .omp_outlined._debug__ .../main_tsan.cpp:24:28 (main_tsan+0x4b57de)
    #1 .omp_outlined. .../main_tsan.cpp:22:3 (main_tsan+0x4b5858)
    #2 __kmp_invoke_microtask <null> (libomp.so+0xab032)
    #3 main .../main_tsan.cpp:21:1 (main_tsan+0x4b573c)
...
```

## Links
[CMake](https://cmake.org/cmake/help/v3.0/)

[Clang](https://releases.llvm.org/10.0.0/tools/clang/docs/ReleaseNotes.html)

[LLVM](https://llvm.org/)

[Sanitizers](https://github.com/google/sanitizers)
