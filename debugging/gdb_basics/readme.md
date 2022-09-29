# GDB basics

This tutorial purpose is to learn the basics of gdb (GNU Debugger) for debugging
the C++ applications. The application has an obvious issue with trying to write
to read-only memory.

## What is a debugger

The debugger is in general a tool which can be used to figure out why the program 
is not behaving as expected. It allows to step through the code, set breakpoints
or examine the value of variables.

## Compilation

In our example the Makefile takes care of compiling the code. Please note,
that `-g` flag is used to compile the programs with debugging information enabled.

```shell
make
```

## Execution

The program can be run with following command:

```shell
./gdb_basics
```

The output indicates an issue:

```
Program received signal SIGSEGV, Segmentation fault.
```

## Debugging

Let's start the application with debugger:

```shell
gdb ./gdb_basics
```

Then the application can be run using `r` or `run`:

```shell
Reading symbols from ./gdb_basics...
(gdb) r
```

The issue is observed and application aborted:

```shell
Starting program: /home/pstanisz/Dokumenty/workspace/cpp_tutorials/debugging/gdb_basics/gdb_basics 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
Basics of gdb
First sentence: Hello, World!
After replacing 'H' with 'Y': Yello, World!
Second sentence: Good bye, World!

Program received signal SIGSEGV, Segmentation fault.
0x0000555555555279 in replace_character (sentence=0x55555555604f "Good bye, World!", old_char=111 'o', new_char=79 'O') at gdb_basics.cpp:25
25	            sentence[idx] = new_char;

```

Probably the most useful command of gdb is `bt` (same as `backtrace` or `where`),
which prints the stack and helps to locate an issue

```shell
(gdb) bt
#0  0x0000555555555279 in replace_character (sentence=0x55555555604f "Good bye, World!", old_char=111 'o', new_char=79 'O') at gdb_basics.cpp:25
#1  0x00005555555553fc in main () at gdb_basics.cpp:46
```

Looks easy, debugger guides us to the `replace_character` function call with
`sentence2` as a parameter. Let's set a breakpoint at `gdb_basics.cpp` line 46:

```shell
(gdb) b gdb_basics.cpp:46
Breakpoint 1 at 0x13e6: file gdb_basics.cpp, line 46.
```

It is easy to preview surrounding code with `l`:

```shell
(gdb) l
17	
18	void replace_character(char *sentence, char old_char, char new_char)
19	{
20	    auto len = strlen(sentence);
21	    for (auto idx = 0U; idx < len; ++idx)
22	    {
23	        if (sentence[idx] == old_char)
24	        {
25	            sentence[idx] = new_char;
26	        }
```

Lets run the program again with `r`, it shall stop on a breakpoint:

```shell
Breakpoint 1, main () at gdb_basics.cpp:46
46	        replace_character((char*)sentence2, 'o', 'O');
```

Step into the function using `s` (or `step`):

```shell
(gdb) s
replace_character (sentence=0x55555555604f "Good bye, World!", old_char=111 'o', new_char=79 'O') at gdb_basics.cpp:20
20	    auto len = strlen(sentence);
```

To check the values of variables, `p` or `print` can be used:

```shell
(gdb) p len
$3 = 16
(gdb) p sentence
$4 = 0x55555555604f "Good bye, World!"
```

So far, so good... As we are not interested in getting into the `strlen`, let's jump
to the next source line in the current stack frame with `n` (or `next`):

```shell
(gdb) n
replace_character (sentence=0x55555555604f "Good bye, World!", old_char=111 'o', new_char=79 'O') at gdb_basics.cpp:21
21	    for (auto idx = 0U; idx < len; ++idx)
```

Nothing interesting, moving forward with `n` unless something happens:

```shell
21	    for (auto idx = 0U; idx < len; ++idx)
(gdb) n
23	        if (sentence[idx] == old_char)
(gdb) n
21	    for (auto idx = 0U; idx < len; ++idx)
(gdb) n
23	        if (sentence[idx] == old_char)
(gdb) n
25	            sentence[idx] = new_char;
(gdb) n

Program received signal SIGSEGV, Segmentation fault.
0x0000555555555279 in replace_character (sentence=0x55555555604f "Good bye, World!", old_char=111 'o', new_char=79 'O') at gdb_basics.cpp:25
25	            sentence[idx] = new_char;
```

Voilà! Apparently the program does not like replacing the character in sentence2.
But why it does not complain for first sentence? Let's compare them in next run:

```shell
Breakpoint 1, main () at gdb_basics.cpp:46
46	        replace_character((char*)sentence2, 'o', 'O');
(gdb) p sentence
$5 = "Yello, World!"
(gdb) p sentence2
$6 = 0x55555555604f "Good bye, World!"
```

Seems that sentences are of different types, therefore printed in a different
way. We can check the type by using `pt` (or `ptype`):

```shell
(gdb) pt sentence
type = char [14]
(gdb) pt sentence2
type = const char *
```

Wait, wait - sentence2 is a const char*, so it is a read-only memory, which we
try to modify! It is possible to confirm it by checking the address of variable
and look into the memory sections information using `maintenance info sections`:

```shell
(gdb) p sentence2
$9 = 0x55555555604f "Good bye, World!"
(gdb) maintenance info sections
Exec file: `/home/pstanisz/Dokumenty/workspace/cpp_tutorials/debugging/gdb_basics/gdb_basics', file type elf64-x86-64.
 ...
 [17]     0x555555556000->0x555555556097 at 0x00002000: .rodata ALLOC LOAD READONLY DATA HAS_CONTENTS
...
```

It is obvious now, that `0x55555555604f` address belongs to .rodata segment, so
it cannot be modified. How is it then possible, that the compiler did not complain?
An answer is in the code:

```cpp
replace_character((char*)sentence2, 'o', 'O');
```

Someone really clever probably solved an issue with non-compiling code by simply
casting the constant `sentence2` to `char*`. As s C-style cast can convert any
type to any other type, the result can be really unsafe. And the compiler cannot
complain, as it assumes that a developer knows what he/she do.

Let's remove the casting:

```cpp
replace_character(sentence2, 'o', 'O');
```

As expected, removal of suspected C-style cast reveals the issue already at compile time:

```shell
make
g++ -g -Wall -pedantic -o gdb_basics gdb_basics.cpp
gdb_basics.cpp: In function ‘int main()’:
gdb_basics.cpp:46:27: error: invalid conversion from ‘const char*’ to ‘char*’ [-fpermissive]
   46 |         replace_character(sentence2, 'o', 'O');
      |                           ^~~~~~~~~
      |                           |
      |                           const char*
gdb_basics.cpp:18:30: note:   initializing argument 1 of ‘void replace_character(char*, char, char)’
   18 | void replace_character(char *sentence, char old_char, char new_char)
      |                        ~~~~~~^~~~~~~~
```

## Summary

Issue identified, source code need to be fixed, very basic gdb usage demonstrated.
With next tutorials, we'll look at more ambitious use-cases for debugging!
