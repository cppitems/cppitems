5 // item status
# Basic Example
## Example library: 2D Grid

This item introduces a simplistic interface to work with a `Grid` *structure* which holds data representing a 2D regular grid.
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
typedef struct Grid /*f8*/ GridType;
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
      */*b7*/ grid_at(&/*f4*/ grid, x, y) = (double)x + y;
    }
  }
  /*b9*/ grid_free(&/*f4*/ grid);
  return 0;
}
 ```
where a sweet over the grid is performed using a nested for-loop.
The `grid_at` function is used to write to the grid.

Note that beside the namespace operator `::` for the wrapped `calloc` and `free` this library only uses C language features. This is on purpose, as we will introduce C++ language features step-by-step in later items.

## File organization
The library source code is organized using *header* and *source* files.
It could also have been placed in a single header file.
Let's discuss some consequences triggered by these to options.

### Redundancy
If separated, a portion of the sourcecode is redundant (the function declarations).
In a single file, functions defintions are sufficient, removing this redundancy.

### Dependent projects
For a single file, the full implementation is included in the dependent project with the consequence that it is compiled togehther with project in a single *compilation unit*. 
This can be advantegeous during *optimization* but leads to a longer compilation times (especially, as the compilation of a single compilation unit is hard to parallelize). 
A change in the library requires a recompilation of the project.

If separated, the compilation of the library and the project happens in different compilation units, requiring a *linking* step after the compilation of the project.
A recompilation of the project is only required if the interface of the library changes. 
If project-wide *compiler flags* change, a recompilation of the library is required.

### Distribution
For a single file, in the simplest case, simply this file is deployed. If the library itself has specific compilation options or dependencies, additional configuration instructions are typically required.

If separated, the distribution of the compiled library togehter with the header files is possible. This requires a distribution of compiled verions for all targeted platforms and configurations.
Additionally, the sources of the library together with build instructions can be distributed.

## Build toolchain
The compilation is performed using a *toolchain* which typically includes a *compiler*, *standard libraries*, *linker*, and *runtime libraries* for the targeted system.

### Invokation
The full set of flags when compiling our library can be revealed with
```
clang++ -### grid.cpp -c 
```
which will list the default settings assuming the host configuration as compilation target. Similar for the subsequent build command 
```
clang++ -###  grid.o main.cpp 
```
which triggers a compilation step followed by a linker step.

### Cross compilation
To target a specific system and architecture which is supported by the available toolchain could look like
```
clang -### --target=wasm32-unknown-wasi --sysroot=... grid.cpp -c
clang -### --target=wasm32-unknown-wasi --sysroot=... grid.o main.cpp 
```
which builds the application for the `wasm32` architecture with an `unkown` vendor assuming a `wasi` system.

## Compilation process

The compilation, which transformes the source files into the specified final format proceeds in phases. 
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
After the initial parsing of the individual sources (charater mapping, comment stripping, line unwrapping, ... ) the preprocessor is the first 'real' action on the code: all actions defined for preprocessor *directives* like `#include` and `#define` are performed which *always* results in a *single* code document which can be revealed using
```
lang++ -E grid.cpp -c
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
Note that the order of `#include`s has consequences on the arragement of this final code document.

Possible errors during the preprocessing phase are 
- "file not found" if includes are not found in the lookup mechanism of the toolchain
- 'silent' errors in macros, which manifest later during translation

### Translation
The translation process is performed according to the defined language standard. Using the C++17 standard for the translation looks like
```
clang++ -std=c++17 grid.c -c
```
As a intermediate (non-optimized) result of this translation, clang uses an intermediate represenation for all C-like languages called *abstract syntax tree* (AST).
This AST can be inspected (and filtered) for our `grid_at` function with
```
clang-check -extra-arg=-std=c++17 -ast-dump --ast-dump-filter=grid_at grid.cpp --
```
which outputs a hierarchy of nodes which closely resembles both the 'original code' but also the language standard. 
Each node represents an abstract language construct, for example the declaration appears as
```
FunctionDecl 0x11c1b10 <srcloc>  grid_at 'double *(GridType *, int, int)'
|-ParmVarDecl 0x11c18e8 <srcloc> grid 'GridType *'
|-ParmVarDecl 0x11c1968 <srcloc> x 'int'
`-ParmVarDecl 0x11c19e8 <srcloc> y 'int'
```
for the `grid_at` function allowing a direct mapping to names and positions in the original source files.

If the code documents satifies the rules imposed by the language standard (i.e., a AST was successfully created), further stages of the translation, which influenced by compiler flags produce the final output format. 
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
We can see that the names of the functions are mangled using some scheme which involves a prefix `_` and additional prefixes indicating the number of characters of a function name `Z9grid_init` or a function paramter `P4Grid` and special names for fundamental types like `i` for `int`.
Note that the mangling schemes are not part of the C++ standard, but mostly the *Itanium C++ ABI* is used, enabling compatibility between objects created by different compilers.

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

Typical errors during translation are essentially all possible errors  excluding errors aparent already during preprocessing or which are only detectable during linking.
Extensive compiler options exist to granularly control the reporting of error and warnings.
Enabling all errors and also promote warnings to errors can be achieved with
```
clang++ -Werror -Weverything grid.cpp -c 
```
which is not a useful default, but is a good starting point as it reports the more granular flags responsible for the individual errors.

## Linking
For dynamic applications, linking is performed after all required compilation units for an application are available in some form (object files, libraries). 

Typical errors during linking are 
- unresolved symbols, i.e., referenced symbols are not found in any of the objects participating in linking
- multiple definitions of symbols, i.e., a symbol cannot be unabiguously resolved 
- type mismatch


## Run time

## Linting
clangd, clang-tidy, custom linting rules


## Build tools

## Formatting

