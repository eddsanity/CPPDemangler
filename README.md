# C++ Demangler
Reading C++ disassembly tends to be specifically tedious because of [Name Mangling](https://en.wikipedia.org/wiki/Name_mangling). This
tool is my attempt at making reading C++ disassembly a little bit more tolerable.

# Dependencies
- A version of g++ that supports GNU++11.
- - MinGW for Windows.
- [nm](https://sourceware.org/binutils/docs/binutils/nm.html) (is shipped with gcc).
- [c++filt](https://sourceware.org/binutils/docs/binutils/c_002b_002bfilt.html) (is shipped with gcc).

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

# Full example
Assume the given C++ file is of interest for disassembly.
```cpp
// main.cpp
#include <cstdio>

template <class type>
class Point
{
    public:    
    Point(type x_, type y_) : x(x_), y(y_) { }
    ~Point()
    {
        puts("hello");
    }

    static int someStaticFunc() { return z++;}
    virtual int someVirtualFunc() { return y++;}

    private:
    type x;
    type y;
    static type z;
};


int main()
{
    Point<int> pt(20, 30);
}
```
Compiling with gcc, but stopping before the assembler is run to view the assembly
```
    g++ main.cpp -O0 -S -o main.s
```
### main.s
<details>
    <summary>click to expand code</summary>

    ```assembly

    	.file	"test.cpp"

	.def	___main;	.scl	2;	.type	32;	.endef
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
    _main:
    LFB16:
        .cfi_startproc
        leal	4(%esp), %ecx	 #,
        .cfi_def_cfa 1, 0
        andl	$-16, %esp	 #,
        pushl	-4(%ecx)	 #
        pushl	%ebp	 #
        .cfi_escape 0x10,0x5,0x2,0x75,0
        movl	%esp, %ebp	 #,
        pushl	%ecx	 #
        .cfi_escape 0xf,0x3,0x75,0x7c,0x6
        subl	$36, %esp	 #,
        call	___main	 #
        leal	-20(%ebp), %eax	 #, tmp89
        movl	$30, 4(%esp)	 #,
        movl	$20, (%esp)	 #,
        movl	%eax, %ecx	 # tmp89,
        call	__ZN5PointIiEC1Eii	 #
        subl	$8, %esp	 #,
        leal	-20(%ebp), %eax	 #, tmp90
        movl	%eax, %ecx	 # tmp90,
        call	__ZN5PointIiED1Ev	 #
        movl	$0, %eax	 #, _5
        movl	-4(%ebp), %ecx	 #,
        .cfi_def_cfa 1, 0
        leave
        .cfi_restore 5
        leal	-4(%ecx), %esp	 #,
        .cfi_def_cfa 4, 4
        ret
        .cfi_endproc
    LFE16:
        .section	.text$_ZN5PointIiEC1Eii,"x"
        .linkonce discard
        .align 2
        .globl	__ZN5PointIiEC1Eii
        .def	__ZN5PointIiEC1Eii;	.scl	2;	.type	32;	.endef
    __ZN5PointIiEC1Eii:
    LFB19:
        .cfi_startproc
        pushl	%ebp	 #
        .cfi_def_cfa_offset 8
        .cfi_offset 5, -8
        movl	%esp, %ebp	 #,
        .cfi_def_cfa_register 5
        subl	$4, %esp	 #,
        movl	%ecx, -4(%ebp)	 # this, this
        movl	$__ZTV5PointIiE+8, %edx	 #, _4
        movl	-4(%ebp), %eax	 # this, tmp88
        movl	%edx, (%eax)	 # _4, this_2(D)->_vptr.Point
        movl	-4(%ebp), %eax	 # this, tmp89
        movl	8(%ebp), %edx	 # x_, tmp90
        movl	%edx, 4(%eax)	 # tmp90, this_2(D)->x
        movl	-4(%ebp), %eax	 # this, tmp91
        movl	12(%ebp), %edx	 # y_, tmp92
        movl	%edx, 8(%eax)	 # tmp92, this_2(D)->y
        nop
        leave
        .cfi_restore 5
        .cfi_def_cfa 4, 4
        ret	$8	 #
        .cfi_endproc
    LFE19:
        .section .rdata,"dr"
    LC0:
        .ascii "hello\0"
        .section	.text$_ZN5PointIiED1Ev,"x"
        .linkonce discard
        .align 2
        .globl	__ZN5PointIiED1Ev
        .def	__ZN5PointIiED1Ev;	.scl	2;	.type	32;	.endef
    __ZN5PointIiED1Ev:
    LFB22:
        .cfi_startproc
        pushl	%ebp	 #
        .cfi_def_cfa_offset 8
        .cfi_offset 5, -8
        movl	%esp, %ebp	 #,
        .cfi_def_cfa_register 5
        subl	$40, %esp	 #,
        movl	%ecx, -12(%ebp)	 # this, this
        movl	$__ZTV5PointIiE+8, %edx	 #, _1
        movl	-12(%ebp), %eax	 # this, tmp88
        movl	%edx, (%eax)	 # _1, this_3(D)->_vptr.Point
        movl	$LC0, (%esp)	 #,
        call	_puts	 #
        nop
        leave
        .cfi_restore 5
        .cfi_def_cfa 4, 4
        ret
        .cfi_endproc
    LFE22:
        .globl	__ZTV5PointIiE
        .section	.rdata$_ZTV5PointIiE,"dr"
        .linkonce same_size
        .align 4
    __ZTV5PointIiE:
        .long	0
        .long	__ZTI5PointIiE
        .long	__ZN5PointIiE15someVirtualFuncEv
        .globl	__ZTI5PointIiE
        .section	.rdata$_ZTI5PointIiE,"dr"
        .linkonce same_size
        .align 4
    __ZTI5PointIiE:
    # <anonymous>:
    # <anonymous>:
        .long	__ZTVN10__cxxabiv117__class_type_infoE+8
    # <anonymous>:
        .long	__ZTS5PointIiE
        .globl	__ZTS5PointIiE
        .section	.rdata$_ZTS5PointIiE,"dr"
        .linkonce same_size
        .align 4
    __ZTS5PointIiE:
        .ascii "5PointIiE\0"
        .section	.text$_ZN5PointIiE15someVirtualFuncEv,"x"
        .linkonce discard
        .align 2
        .globl	__ZN5PointIiE15someVirtualFuncEv
        .def	__ZN5PointIiE15someVirtualFuncEv;	.scl	2;	.type	32;	.endef
    __ZN5PointIiE15someVirtualFuncEv:
    LFB23:
        .cfi_startproc
        pushl	%ebp	 #
        .cfi_def_cfa_offset 8
        .cfi_offset 5, -8
        movl	%esp, %ebp	 #,
        .cfi_def_cfa_register 5
        subl	$4, %esp	 #,
        movl	%ecx, -4(%ebp)	 # this, this
        movl	-4(%ebp), %eax	 # this, tmp90
        movl	8(%eax), %eax	 # this_2(D)->y, _3
        leal	1(%eax), %ecx	 #, _5
        movl	-4(%ebp), %edx	 # this, tmp91
        movl	%ecx, 8(%edx)	 # _5, this_2(D)->y
        leave
        .cfi_restore 5
        .cfi_def_cfa 4, 4
        ret
        .cfi_endproc
    LFE23:
        .ident	"GCC: (MinGW.org GCC-6.3.0-1) 6.3.0"
        .def	_puts;	.scl	2;	.type	32;	.endef

    ```
</details>

What's immediately obvious is that the all of the member functions names, including the constructors, aren't present in readable form. This is because they have been mangled by the compiler into the following:
```
    __ZN5PointIiE15someVirtualFuncEv
    __ZN5PointIiEC1Eii
    __ZN5PointIiED1Ev
    __ZTI5PointIiE
    __ZTS5PointIiE
    __ZTV5PointIiE
```
The words "Point" and "someVirtualFunc" are recognizable, but not much else. Using the demangler on the same cpp file, it outputs much more recognizable names:
```
    __ZN5PointIiE15someVirtualFuncEv -> Point<int>::someVirtualFunc()
    __ZN5PointIiEC1Eii -> Point<int>::Point(int, int)
    __ZN5PointIiED1Ev -> Point<int>::~Point()
    __ZTI5PointIiE -> typeinfo for Point<int>
    __ZTS5PointIiE -> typeinfo name for Point<int>
    __ZTV5PointIiE -> vtable for Point<int>
```
