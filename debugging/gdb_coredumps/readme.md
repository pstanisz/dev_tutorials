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

The `ulimit` command can be used to check or set the resource limit of the current user.

```shell
ulimit -a
real-time non-blocking time  (microseconds, -R) unlimited
core file size              (blocks, -c) 0
data seg size               (kbytes, -d) unlimited
scheduling priority                 (-e) 0
file size                   (blocks, -f) unlimited
pending signals                     (-i) 63730
max locked memory           (kbytes, -l) 2048192
max memory size             (kbytes, -m) unlimited
open files                          (-n) 1024
pipe size                (512 bytes, -p) 8
POSIX message queues         (bytes, -q) 819200
real-time priority                  (-r) 0
stack size                  (kbytes, -s) 8192
cpu time                   (seconds, -t) unlimited
max user processes                  (-u) 63730
virtual memory              (kbytes, -v) unlimited
file locks                          (-x) unlimited
```

The system resources are defined in a configuration file: `/etc/security/limits.conf`.
The "soft" limit is manageable by the user, while "hard" limit defines the physical limit the user can reach.

For this particular tutorial, the limit of core file size for the user is important.
Let's set `unlimited`:

```shell
ulimit -c unlimited
```

### Hint for Ubuntu

I noticed that in last LTS releases of Ubuntu the core dumps are not generated as expected. It seems that stopping the apport service helps: 

```shell
sudo service apport stop
```

## Running the test application

The time has come to run the `game` application:

```shell
LD_LIBRARY_PATH=. ./game
Bestiary game
...
Błąd w obliczeniach zmiennoprzecinkowych (zrzut pamięci)
```

Ups... We have a core dump!

```shell
ls | grep core*
core.2982
```

## Analysing the core dump

To analyse the core dump, let's use gdb with binary and generated core:

```shell
gdb game core.2982
```

Unfortunately, there are no symbols found, we shall not expect a nice output then:

```shell
Reading symbols from game...
(No debugging symbols found in game)
```

Anyway, the backtrace shows some useful information, we already know that the crash happened within `libbestiary.so`:

```shell
(gdb) bt
#0  0x00007f9f4b826ad8 in Bestiary::lucky_shot(Bestiary::Fight_result, unsigned char, unsigned char) () from ./libbestiary.so
#1  0x00007f9f4b826cad in Bestiary::fight(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, Bestiary::Attribute) () from ./libbestiary.so
#2  0x0000557323275609 in ?? ()
#3  0x00007f9f4b3c7d90 in __libc_start_call_main (main=main@entry=0x557323275489, argc=argc@entry=1, argv=argv@entry=0x7ffd3339b528) at ../sysdeps/nptl/libc_start_call_main.h:58
#4  0x00007f9f4b3c7e40 in __libc_start_main_impl (main=0x557323275489, argc=1, argv=0x7ffd3339b528, init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7ffd3339b518)
    at ../csu/libc-start.c:392
#5  0x00005573232753c5 in ?? ()
```

TBD

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
