1 // item status
# Hands-on session: C++ compilation process and tools

- C++ is compiled
- Compilation means translating C++ code to machine code (processor language)

## Example

A short piece of C++ code:

```C++
int square(int a) {
  return a * a;
}
```

The same code in the assembler language (human readable machine code):

```C++
square:
      push    rbp 
      mov     rbp, rsp
      mov     DWORD PTR [rbp-4], edi
      mov     eax, DWORD PTR [rbp-4]
      imul    eax, eax          # multiplication
      pop     rbp
      ret
```

The same code in actual machine code:

```C++
\x55\x48\x89\xE5\x89\x7D\xFC\x8B\x45\xFC\x0F\xAF\xC0\x5D\xC3
```

In order to execute this code, our OS points the processor to the start of the machine code.
The processor will then execute these commands.

Conclusions:
- It is hard to write machine code as a human
- We want to write code in C++
- C++ code needs to be translated to our processor
- The entire process to get an executable is called **building**

## Building steps

Building a C++ program consists of two main steps:

- Compiling: check syntax, types and translate to machine code
- Linking: put machine code in correct format and connect with OS libraries

## Executing a program

The machine code is executed using specific inputs (files, user inputs, OS).

## Compile time vs. Runtime

- Every part of our program that must be defined during compilation must be defined during **compile time**
- All parts of our program that change depending on the specific input during execution are set during **runtime**

This difference is extremely important. Make sure you are always aware of:
- the parts of your program that must be fixed at **compile time** 
- the parts that are only known during **runtime**

## Questions

> What is building? What steps are there?
  - transformation from source code and project configuration to final executables/libraries
  - main steps are: compiling and linking
> What is the output of a compilation step?
  - each compilation unit results in an object file
> What is the output of a linking step?
  - linking several objects results in an executable or library
> How can C++ code be split across several files? Why would we do that?
  - different functionality is typically organized in separate files (which are then included, i.e., copy-pasted by the preprocessor) 
  - the included files are typically headers which might contain:
    - an interface only (no definitions)
    - the full implementation (including definitions)
  - separating interfaces from definitions allows to use individual object files
  - only outdated object files have to be recompiled before final linking
> How can a Makefile help in the build process?
  - Makefiles allows to define dependencies between generated files
  - Makefiles help to re-compile only the required objects
> Why do we use CMake to generate Makefiles?
  - Makefiles tend to be very verbose for larger projects
  - CMake allows to define dependencies on a higher level; finally, CMake generates Makefiles according ti these dependencies 

# The source edited during the session can be found here:
https://github.com/cppitems/cppitems/tree/master/items/003
