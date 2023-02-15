# Chapter 41

## Exercise 41-1

Program does nothing, only returns.

```sh
gcc exercise_41_01.c -o dynamic.out
gcc exercise_41_01.c -static -o static.out
ls -al *out
```

On my system, the dynamically linked executable is 17 KB while the statically linked executable is 852 KB.
