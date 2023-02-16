# Chapter 42

## Exercise 42-1

This demo requires two shared libraries. libbar depends on libfoo.

```sh
# compile library sources
gcc -g -c -fPIC -Wall foo.c
gcc -g -c -fPIC -Wall bar.c

# create shared libraries
gcc -g -shared -o libfoo.so foo.o
gcc -g -shared -o libbar.so bar.o -L. -lfoo
```

This program loads the shared libs, calls a function from foo, closes foo, then calls the same function again.

```sh
# build program
gcc -g -Wall -L. -o exercise_42_01.out exercise_42_01.c -lfoo -lbar -ldl

# run it
LD_LIBRARY_PATH=. ./exercise_42_01.out
```

## Exercise 42-2

This was run from the cmake build directory in the project root. It requires libfoo from the first exercise.

```sh
./exercise_42_02 ../chapter_42/libfoo.so say_hello
```
