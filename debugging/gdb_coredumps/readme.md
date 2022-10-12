# Analysing the coredump with GDB

## Checking the symbols in binary

```shell
readelf -s game
```

```shell
Symbol table '.symtab' contains 83 entries:
```

## Extract debug symbols

```shell
objcopy --only-keep-debug game game.dbg
```

## Stip the symbols

```shell
strip --strip-debug --strip-unneeded game
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
