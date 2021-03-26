# C++ Demangler
Reading C++ disassembly tends to be specifically tedious because of [Name Mangling](https://en.wikipedia.org/wiki/Name_mangling). This
tool is my attempt at making reading C++ disassembly a little bit more tolerable.

# Dependencies
- A version of g++ that supports GNU++11.
- - MinGW for Windows.
- [nm](https://sourceware.org/binutils/docs/binutils/nm.html) (shipped with gcc).
- [c++filt](https://sourceware.org/binutils/docs/binutils/c_002b_002bfilt.html) (shipped with gcc).

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
- Doesn't support dictating output directory. All demangled assembly files will be written in the same directory as source.

# Full example
Assume the given C++ file is of interest for disassembly.
```cpp
// main.cpp
#include <cstdio>

class Point
{
    public:    
    Point(int x_, int y_) : x(x_), y(y_) { }
    ~Point()
    {
        puts("hello");
    }

    static int someStaticFunc() { return z++;}
    virtual int someVirtualFunc() { return y++;}
    
    private:
    int x;
    int y;
    static int z;
};

int Point::z = 0;

int main()
{
    Point pt(20, 30);
    Point::someStaticFunc();
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

        .file	"main.cpp"
        .section	.text$_ZN5PointC1Eii,"x"
        .linkonce discard
        .align 2
        .globl	__ZN5PointC1Eii
        .def	__ZN5PointC1Eii;	.scl	2;	.type	32;	.endef
    __ZN5PointC1Eii:
    LFB14:
        .cfi_startproc
        pushl	%ebp	 #
        .cfi_def_cfa_offset 8
        .cfi_offset 5, -8
        movl	%esp, %ebp	 #,
        .cfi_def_cfa_register 5
        subl	$4, %esp	 #,
        movl	%ecx, -4(%ebp)	 # this, this
        movl	$__ZTV5Point+8, %edx	 #, _4
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
    LFE14:
        .section .rdata,"dr"
    LC0:
        .ascii "hello\0"
        .section	.text$_ZN5PointD1Ev,"x"
        .linkonce discard
        .align 2
        .globl	__ZN5PointD1Ev
        .def	__ZN5PointD1Ev;	.scl	2;	.type	32;	.endef
    __ZN5PointD1Ev:
    LFB17:
        .cfi_startproc
        pushl	%ebp	 #
        .cfi_def_cfa_offset 8
        .cfi_offset 5, -8
        movl	%esp, %ebp	 #,
        .cfi_def_cfa_register 5
        subl	$40, %esp	 #,
        movl	%ecx, -12(%ebp)	 # this, this
        movl	$__ZTV5Point+8, %edx	 #, _1
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
    LFE17:
        .section	.text$_ZN5Point14someStaticFuncEv,"x"
        .linkonce discard
        .globl	__ZN5Point14someStaticFuncEv
        .def	__ZN5Point14someStaticFuncEv;	.scl	2;	.type	32;	.endef
    __ZN5Point14someStaticFuncEv:
    LFB18:
        .cfi_startproc
        pushl	%ebp	 #
        .cfi_def_cfa_offset 8
        .cfi_offset 5, -8
        movl	%esp, %ebp	 #,
        .cfi_def_cfa_register 5
        movl	__ZN5Point1zE, %eax	 # z, z.0_2
        leal	1(%eax), %edx	 #, _4
        movl	%edx, __ZN5Point1zE	 # _4, z
        popl	%ebp	 #
        .cfi_restore 5
        .cfi_def_cfa 4, 4
        ret
        .cfi_endproc
    LFE18:
        .section	.text$_ZN5Point15someVirtualFuncEv,"x"
        .linkonce discard
        .align 2
        .globl	__ZN5Point15someVirtualFuncEv
        .def	__ZN5Point15someVirtualFuncEv;	.scl	2;	.type	32;	.endef
    __ZN5Point15someVirtualFuncEv:
    LFB19:
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
    LFE19:
        .globl	__ZN5Point1zE
        .bss
        .align 4
    __ZN5Point1zE:
        .space 4
        .def	___main;	.scl	2;	.type	32;	.endef
        .text
        .globl	_main
        .def	_main;	.scl	2;	.type	32;	.endef
    _main:
    LFB20:
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
        call	__ZN5PointC1Eii	 #
        subl	$8, %esp	 #,
        call	__ZN5Point14someStaticFuncEv	 #
        leal	-20(%ebp), %eax	 #, tmp90
        movl	%eax, %ecx	 # tmp90,
        call	__ZN5PointD1Ev	 #
        movl	$0, %eax	 #, _6
        movl	-4(%ebp), %ecx	 #,
        .cfi_def_cfa 1, 0
        leave
        .cfi_restore 5
        leal	-4(%ecx), %esp	 #,
        .cfi_def_cfa 4, 4
        ret
        .cfi_endproc
    LFE20:
        .globl	__ZTV5Point
        .section	.rdata$_ZTV5Point,"dr"
        .linkonce same_size
        .align 4
    __ZTV5Point:
        .long	0
        .long	__ZTI5Point
        .long	__ZN5Point15someVirtualFuncEv
        .globl	__ZTI5Point
        .section	.rdata$_ZTI5Point,"dr"
        .linkonce same_size
        .align 4
    __ZTI5Point:
    # <anonymous>:
    # <anonymous>:
        .long	__ZTVN10__cxxabiv117__class_type_infoE+8
    # <anonymous>:
        .long	__ZTS5Point
        .globl	__ZTS5Point
        .section	.rdata$_ZTS5Point,"dr"
        .linkonce same_size
        .align 4
    __ZTS5Point:
        .ascii "5Point\0"
        .ident	"GCC: (MinGW.org GCC-6.3.0-1) 6.3.0"
        .def	_puts;	.scl	2;	.type	32;	.endef

```
</details>

What's immediately obvious is that the all of the member functions names, including the constructors, aren't present in readable form. This is because they have been mangled by the compiler into the following:
```
    __ZN5Point14someStaticFuncEv
    __ZN5PointIiE15someVirtualFuncEv
    __ZN5Point1zE
    __ZN5PointIiEC1Eii
    __ZN5PointIiED1Ev
    __ZTI5PointIiE
    __ZTS5PointIiE
    __ZTV5PointIiE
```
The words "Point" and "someVirtualFunc" are recognizable, but not much else. Using the demangler on the same cpp file, it outputs much more recognizable names:
```
__ZN5Point14someStaticFuncEv -> Point::someStaticFunc()
__ZN5Point15someVirtualFuncEv -> Point::someVirtualFunc()
__ZN5Point1zE -> Point::z
__ZN5PointC1Eii -> Point::Point(int, int)
__ZN5PointD1Ev -> Point::~Point()
__ZTI5Point -> typeinfo for Point
__ZTS5Point -> typeinfo name for Point
__ZTV5Point -> vtable for Point
```
