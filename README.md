# C++ Demangler
Reading C++ disassembly tends to be specifically tedious because of [Name Mangling](https://en.wikipedia.org/wiki/Name_mangling). This
tool is my attempt at making reading C++ disassembly a little bit more tolerable.

# Dependencies
- A version of g++ that supports GNU++11.
- - MinGW for Windows.
- [nm](https://sourceware.org/binutils/docs/binutils/nm.html).
- [c++filt](https://sourceware.org/binutils/docs/binutils/c_002b_002bfilt.html).

# Build
## Windows
```
    g++ -std=gnu++11 cppdemangle.cpp -o cppdemangle.exe
```

## Linux
```
    g++ -std=gnu++11 cppdemangle.cpp -o cppdemangle.o
```

# Usage
Once the executable is in your PATH, demangling is very straightforward since the demangler only accepts two arguments so far.
- C++ standard to compile the target file with
- File name (relative to CurrentDirectory/PWD)

## Example usage
```bash
    ~/somedirectory/$ cppdemangle c++11 somecppfile.cpp
```
# Limitations
- Compiler flags are hardcoded and not user provided, so it is **very** limited in what it can actually compile. I might 
    parse compiler arguments in the future if I end up needing it. 
- Only supports `cout` as the output stream for the mangled -> demangled mapping. Will probably implement some fstream options later.
