# Analysing the coredump with GDB

Have you ever get a coredump from production to analyse? Opened the gdb to check the call stack to see just a lot of lines with `(??)`?

In this tutorial some background and hints about analysing the coredumps will be shared. 

## Compilation

In our example the Makefile takes care of compiling the code, in particular:

* `libbestiary.so` shared library
* `game` application

The binaries are compiled with debug information: `-g`.

```shell
make
```

## Checking the symbols in binary

To check the symbols in the binary, `readelf` can be used:

```shell
readelf -s game
```

The output shall present the symbol tables: `.dynsym` and `.symtab`.

The `.symtab` table contains every symbol that describe the ELF file. This table is typically non-allocable and therfore not available in the memory image of the process.

The `.dynsym` table contains a subset of the symbold from the `.symtab` that are needed to support dynamic linking. This table is allocable and available in the memory image of the process.

Memory image of the process is a copy of the process virtual memory stored in file. 

As our application is by default compiled with debug symbols, we shall see both tables available:

```shell
Symbol table '.dynsym' contains 50 entries:
...
Symbol table '.symtab' contains 83 entries:
```

Sizes of binaries are quite big due to debug version:

```shell
ls -lh game libbestiary.so 
-rwxrwxr-x 1 pstanisz pstanisz  80K paź 13 21:28 game
-rwxrwxr-x 1 pstanisz pstanisz 180K paź 13 21:28 libbestiary.so
```

## Extracting debug symbols

The `objcopy` tool can be used to extract the debug symbols from the binaries:

```shell
objcopy --only-keep-debug game game.dbg
```

## Stripping debug information

The `strip` tool can be used to remove the symbols from the binaries:

```shell
strip --strip-debug --strip-unneeded game
```

## Preparing dgb and stripped binaries

A special target named `dbg` was defined to combine extraction of the debug information and stipping it from binaries:

```shell
make dbg
```

As a result, the binaries shall no longer contain debug information. You can examine it using `readelf`. The sizes of binaries are also much smaller:

```shell
ls -lh game libbestiary.so 
-rwxrwxr-x 1 pstanisz pstanisz 19K paź 13 21:29 game
-rwxrwxr-x 1 pstanisz pstanisz 52K paź 13 21:29 libbestiary.so
```

The symbols are now available in separate files: `game.dbg` and `libbestiary.so.dbg`.

After this long introduction, we have the real-life use-case: optimized binaries without any debugging information are working in production environment.

## What is important?

For successfull analysis of the coredump, the most important thing is to find the binary that generated the coredump (or just an identical one) and the debug symbols to real the callstack.

Usually in well-organized projects there are builds per version persisted, among others containg the release binaries and debug information.

In less perfect cases, the only way to get proper binary and symbols might be to 're-create' the binary with debug information using the same codeline, environment, etc.

## Enabling core dumps

```shell
sudo service apport stop
ulimit -c unlimited
```

## Load the symbols of application

```shell
(gdb) symbol-file game.dbg
```

## Locate the shared library address

```shell
(gdb) info sharedlibrary 
From                To                  Syms Read   Shared Object Library
0x00007ff1415ff280  0x00007ff141601cee  Yes (*)     ./libbestiary.so
0x00007ff141461440  0x00007ff141568b02  Yes (*)     /lib/x86_64-linux-gnu/libstdc++.so.6
0x00007ff1413a2660  0x00007ff1413b8be5  Yes (*)     /lib/x86_64-linux-gnu/libgcc_s.so.1
0x00007ff14119f700  0x00007ff141331abd  Yes         /lib/x86_64-linux-gnu/libc.so.6
0x00007ff14109e3a0  0x00007ff1411198e8  Yes         /lib/x86_64-linux-gnu/libm.so.6
0x00007ff14160a090  0x00007ff141633335  Yes         /lib64/ld-linux-x86-64.so.2
```

## Load the symbols of shared library with the address from the shared library

```shell
(gdb) add-symbol-file libbestiary.so.dbg 0x00007ff1415ff280
```

## Enjoy the backtrace

```shell
(gdb) bt
#0  0x00007ff1415ffad8 in Bestiary::lucky_shot (result=Bestiary::Fight_result::Lost, aggr_luck=1 '\001', def_luck=0 '\000') at bestiary.cpp:99
#1  0x00007ff1415ffcad in Bestiary::fight (aggressor="troll", defender="goblin", attribute=Bestiary::Attribute::Charisma) at bestiary.cpp:137
#2  0x00005615dd41f609 in main () at game.cpp:43
```
